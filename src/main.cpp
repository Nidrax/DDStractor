#include "dds_file.hpp"
#include <iostream>

int main(const int argc, const char * const argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [FILE]" << std::endl;
        return EXIT_FAILURE;
    }
    const std::string fileName(argv[1]);
    try {
        const DDS::File dds(fileName);
        dds.Print();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}