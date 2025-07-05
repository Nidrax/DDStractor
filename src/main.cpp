#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "dds.hpp"
#include "printing.hpp"

namespace fs = std::filesystem;
typedef std::vector<std::byte> byte_v;

byte_v read_all_bytes(const std::string& fileName)
{
    const fs::path path(fileName);
    if (!fs::exists(path)) {
        throw std::runtime_error("File does not exist: " + fileName);
    }
    if (!fs::is_regular_file(path)) {
        throw std::runtime_error("Path is not a regular file: " + fileName);
    }

    const auto size = fs::file_size(path);
    if (size == 0) {
        return {};
    }

    byte_v data(size);
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }
    file.read(
        reinterpret_cast<char*>(data.data()),
        static_cast<std::streamsize>(size));
    if (!file) {
        throw std::runtime_error("Failed to read file: " + fileName);
    }
    file.close();
    return data;
}

int main(const int argc, const char * const argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [FILE]" << std::endl;
        return EXIT_FAILURE;
    }
    const std::string fileName(argv[1]);
    const byte_v data = read_all_bytes(fileName);

    if (data.size() < 4 + sizeof(DDS::Header)) {
        std::cerr << "File is too small to be a valid DDS file." << std::endl;
        return EXIT_FAILURE;
    }

    uint32_t magic = 0;
    std::memcpy(&magic, data.data(), sizeof(uint32_t));
    if (magic != DDS::MagicNumber) {
        std::cerr << "File is not a valid DDS file." << std::endl;
        return EXIT_FAILURE;
    }

    DDS::Header header{};
    DDS::HeaderDX10 headerDX10{};
    std::memcpy(&header, data.data() + 4, sizeof(DDS::Header));

    const bool isDX10 = header.pixelFormat.flags == DDS::PixelFormatFlags::FourCC
        && header.pixelFormat.fourCC == DDS::str_to_cc("DX10");

    if (isDX10)
    {
        std::memcpy(&headerDX10, data.data() + 4 + sizeof(DDS::Header), sizeof(DDS::HeaderDX10));
    }

    DDS::print_header(header, headerDX10, isDX10);

    return EXIT_SUCCESS;
}