#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "dds.hpp"

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

void print_header(const DDS::Header& header, const DDS::HeaderDX10& headerDX10 = {}, const bool isDX10 = false)
{
    std::cout << "DDS Header:" << std::endl;
    std::cout << "  Size: " << header.size << std::endl;
    std::cout << "  Flags: " << static_cast<uint32_t>(header.flags) << std::endl;
    std::cout << "  Height: " << header.height << std::endl;
    std::cout << "  Width: " << header.width << std::endl;
    std::cout << "  Pitch: " << header.pitch << std::endl;
    std::cout << "  Depth: " << header.depth << std::endl;
    std::cout << "  MipMapCount: " << header.mipMapCount << std::endl;
    std::cout << "  Caps1: " << static_cast<uint32_t>(header.caps1) << std::endl;
    std::cout << "  Caps2: " << static_cast<uint32_t>(header.caps2) << std::endl;
    if (isDX10) {
        std::cout << "DDS Header DX10:" << std::endl;
        std::cout << "  DXGI Format: " << static_cast<uint32_t>(headerDX10.dxgiFormat) << std::endl;
        std::cout << "  ResourceDimension: " << static_cast<uint32_t>(headerDX10.resourceDimension) << std::endl;
        std::cout << "  Format: " << static_cast<uint32_t>(headerDX10.dxgiFormat) << std::endl;
        std::cout << "  MiscFlag: " << static_cast<uint32_t>(headerDX10.miscFlag) << std::endl;
        std::cout << "  ArraySize: " << headerDX10.arraySize << std::endl;
        std::cout << "  MiscFlags2: " << static_cast<uint32_t>(headerDX10.miscFlags2) << std::endl;
    } else {
        std::cout << "  PixelFormat: RGBBitCount = " << header.pixelFormat.rgbBitCount << std::endl;
        std::cout << "  RedMask: 0x" << std::setfill ('0') << std::setw(8) << std::hex << header.pixelFormat.rBitMask << std::endl;
        std::cout << "  GreenMask: 0x" << std::setfill ('0') << std::setw(8) << std::hex << header.pixelFormat.gBitMask << std::endl;
        std::cout << "  BlueMask: 0x" << std::setfill ('0') << std::setw(8) << std::hex << header.pixelFormat.bBitMask << std::endl;
        std::cout << "  AlphaMask: 0x" << std::setfill ('0') << std::setw(8) << std::hex << header.pixelFormat.aBitMask << std::endl;
    }
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
        && header.pixelFormat.fourCC == DDS::StrToFourCC("DX10");

    if (isDX10)
    {
        std::memcpy(&headerDX10, data.data() + 4 + sizeof(DDS::Header), sizeof(DDS::HeaderDX10));
    }

    print_header(header, headerDX10, isDX10);

    return EXIT_SUCCESS;
}