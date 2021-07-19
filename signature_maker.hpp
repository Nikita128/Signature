#ifndef SIGNATURE_MAKER_HPP
#define SIGNATURE_MAKER_HPP

#include <map>
#include <mutex>

#include <boost/uuid/detail/md5.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#include "thread_pool.hpp"

class signature_maker
{
    size_t block_size;
    size_t max_map_size = 1000 << 20;

    std::string src_file_path, dst_file_path;

    using hash_value = boost::uuids::detail::md5::digest_type;
    std::map<int, hash_value> block_signatures;

    std::mutex m, hash_m;

public:
    signature_maker(std::string_view src_file_path, std::string_view dst_file_path, size_t block_size_mb = 1);

    void run();

private:
    void convert_file_to_block_signatures();
    void write_block_signatures_to_file();

    void create_signatures_from_block_sectors(thread_pool &pool, const char *&data_ptr, size_t block_count, size_t blocks_per_task, size_t& block_sector_id);
    void create_signatures_task(const char* ptr, size_t blocks_count, size_t sector_id);
    void create_block_signature(const char *begin, std::size_t size, hash_value& buffer);
};

#endif // SIGNATURE_MAKER_HPP
