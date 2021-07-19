#include <iostream>

#include <boost/lexical_cast.hpp>

#include "signature_maker.hpp"

int main(int argc, char *argv[])
{
    try
    {
        std::size_t block_size_mb = 1;
        std::string_view src_file_path, dst_file_path;

        switch(argc)
        {
        case 4:
            block_size_mb = boost::lexical_cast<size_t>(argv[3]);
        case 3:
            src_file_path = argv[1];
            dst_file_path = argv[2];
            break;
        default:
            throw std::logic_error("Wrong number of arguments: " + std::to_string(argc));
        }

        signature_maker s_maker(src_file_path, dst_file_path, block_size_mb);
        s_maker.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
