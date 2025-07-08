#ifndef DDS_PRINTER_HPP
#define DDS_PRINTER_HPP

#include "dds_defs.hpp"
#include <kulerd/kulerd.hpp>
#include <iosfwd>

namespace DDS {

using kulerd::Color;

class Printer {
public:
    explicit Printer(std::ostream& output) : m_output(output) {}
    ~Printer() = default;

    void PrintHeader(const Header& header) const;
    void PrintHeaderDX10(const HeaderDX10& headerDX10) const;
    void PrintPixelFormat(const PixelFormat& pixelFormat) const;
private:
    std::ostream& m_output;

    static std::string c(const std::string& s, Color fg = Color::Default, Color bg = Color::Default);
    static std::string h(uint32_t n);
    static std::string t(const std::string& s);
    static std::string caps_to_str(uint32_t caps);
    static std::string caps2_to_str(uint32_t caps);
    static std::string rd_to_str(ResDimension rd);
    static std::string pff_to_str(PFFlags flags);
    static std::string hf_to_str(HFlags flags);
    static std::string format_to_str(uint32_t format);
    static std::string misc_to_str(uint32_t flags);
};

} // DDS

#endif //DDS_PRINTER_HPP
