#include "signature_maker.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

signature_maker::signature_maker(std::string_view src_file_path, std::string_view dst_file_path, size_t block_size_mb) :
    block_size(block_size_mb << 20),
    src_file_path(src_file_path),
    dst_file_path(dst_file_path)
{
    max_map_size -= max_map_size % block_size;
}

void signature_maker::run()
{
    try
    {
        convert_file_to_block_signatures();
        write_block_signatures_to_file();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw std::exception();
    }
}

void signature_maker::convert_file_to_block_signatures()
{
    std::filesystem::path src_path(src_file_path);

    size_t bytes_left = std::filesystem::file_size(src_path);

    using namespace boost::iostreams;

    thread_pool pool;

    mapped_file_params map_params(src_file_path);
    map_params.offset = 0;

    size_t block_sector_id = 0;

    while(bytes_left)
    {
        map_params.length = (bytes_left < max_map_size)? bytes_left : max_map_size;

        mapped_file_source mapped_src_file(map_params);
        if (!mapped_src_file.is_open())
        {
            throw std::runtime_error("Couldn't open a file");
        }

        auto data_ptr = mapped_src_file.data();
        auto block_count = map_params.length / block_size;
        auto blocks_per_task = (block_count < pool.size())? 1 : block_count / pool.size();

        create_signatures_from_block_sectors(pool, data_ptr, block_count, blocks_per_task, block_sector_id);

        if (auto blocks_per_task_remainder = block_count % pool.size();
                blocks_per_task > 1 && blocks_per_task_remainder)
        {
            create_signatures_from_block_sectors(pool, data_ptr, blocks_per_task_remainder, 1, block_sector_id);
        }

        pool.wait();

        if (auto block_remainder = map_params.length % block_size)
        {
             create_block_signature(data_ptr, block_remainder, block_signatures[block_sector_id]);
        }

        std::cout << "Current block id: " << block_sector_id << std::endl;

        mapped_src_file.close();
        map_params.offset += map_params.length;
        bytes_left -= map_params.length;
    }
}

void signature_maker::write_block_signatures_to_file()
{
    std::ofstream signature_file(dst_file_path, std::ios::binary);

    for (auto& node : block_signatures)
    {
        auto& block_signature = node.second;

        signature_file.write(reinterpret_cast<const char*>(block_signature), sizeof (hash_value));
    }

    signature_file.flush();
    signature_file.close();
}

void signature_maker::create_signatures_from_block_sectors(thread_pool& pool,
                                                           const char * & data_ptr,
                                                           size_t block_count,
                                                           size_t blocks_per_task,
                                                           size_t &block_sector_id)
{
    auto sector_size = block_size * blocks_per_task;

    for (size_t i = 0, end = block_count - blocks_per_task + 1; i < end; i += blocks_per_task)
    {
        pool.execute_task(std::bind(&signature_maker::create_signatures_task, this, data_ptr, blocks_per_task, block_sector_id));

        block_sector_id += blocks_per_task;
        data_ptr += sector_size;
    }
}

void signature_maker::create_signatures_task(const char *ptr, size_t blocks_count, size_t sector_id)
{
    std::vector<std::pair<size_t, hash_value>> result_hash_values;
    result_hash_values.reserve(blocks_count);

    for (size_t i = 0; i < blocks_count; ++i)
    {
        auto& [id, hash_buffer] = result_hash_values.emplace_back();
        id = sector_id;
        create_block_signature(ptr, block_size, hash_buffer);

        ++sector_id;
        ptr += block_size;
    }

    std::unique_lock lock(m);
    for (auto& [id, hash_buffer] : result_hash_values)
    {
        std::copy(std::begin(hash_buffer), std::end(hash_buffer), std::begin(block_signatures[id]));
    }
}

void signature_maker::create_block_signature(const char *begin, std::size_t size, hash_value& buffer)
{
    using namespace boost::uuids::detail;

    md5 hash_function;

    hash_function.process_bytes(begin, size);
    hash_function.get_digest(buffer);
}
