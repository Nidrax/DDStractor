#ifndef DDS_FILE_HPP
#define DDS_FILE_HPP

#include "dds_defs.hpp"
#include <iostream>
#include <vector>

namespace DDS
{

class File
{
public:
    explicit File(const std::string&filename);
    ~File() = default;

    void Print(std::ostream& os = std::cout) const;
    bool IsDX10() const
    {
        return m_header.pixelFormat.flags == PFFlags::FourCC &&
               m_header.pixelFormat.fourCC == StrToCC("DX10");
    }

private:
    static constexpr uint32_t MagicNumber = 0x20534444;
    Header m_header;
    HeaderDX10 m_headerDX10;

    bool m_isDX10 = false; // Flag to indicate if the DDS file uses the DX10 header format

    static std::vector<std::byte> ReadAllBytes(const std::string&filename);
};
}

#endif //DDS_FILE_HPP
