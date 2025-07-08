#include "dds_file.hpp"
#include "dds_printer.hpp"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>

namespace DDS
{

File::File(const std::string&filename)
{
    const std::vector<std::byte> data = ReadAllBytes(filename);

    if (data.size() < 4 + sizeof(m_header))
    {
        throw std::runtime_error("File is too small to be a valid DDS file");
    }

    uint32_t magic = 0;
    std::memcpy(&magic, data.data(), sizeof(uint32_t));

    if (MagicNumber != magic)
    {
        throw std::runtime_error("File is not a valid DDS file");
    }

    std::memcpy(&m_header, data.data() + 4, sizeof(m_header));
    if (IsDX10())
    {
        std::memcpy(&m_headerDX10, data.data() + 4 + sizeof(m_header), sizeof(m_headerDX10));
    }
}

void File::Print(std::ostream& os) const
{
    const Printer printer(os);

    printer.PrintHeader(m_header);

    IsDX10()
        ? printer.PrintHeaderDX10(m_headerDX10)
        : printer.PrintPixelFormat(m_header.pixelFormat);
}

std::vector<std::byte> File::ReadAllBytes(const std::string& filename)
{
    namespace fs = std::filesystem;

    const fs::path path(filename);
    if (!fs::exists(path))
    {
        throw std::runtime_error("File does not exist: " + filename);
    }
    if (!fs::is_regular_file(path))
    {
        throw std::runtime_error("Path is not a regular file: " + filename);
    }

    const auto size = fs::file_size(path);
    if (size == 0)
    {
        return {};
    }

    std::vector<std::byte> data(size);
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    file.read(
        reinterpret_cast<char*>(data.data()),
        static_cast<std::streamsize>(size));
    if (!file)
    {
        throw std::runtime_error("Failed to read file: " + filename);
    }
    file.close();
    return data;
}

} // namespace DDS
