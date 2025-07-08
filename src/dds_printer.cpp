#include "dds_printer.hpp"
#include "dxgifmt.hpp"
#include <iomanip>
#include <ostream>

namespace DDS {

using kulerd::Color;

void Printer::PrintHeader(const Header& header) const
{
    m_output << c("DDS Header:", Color::Cyan, Color::Default) << std::endl;
    m_output << t("  Width:       ") << c(std::to_string(header.width), Color::Blue) << std::endl;
    m_output << t("  Height:      ") << c(std::to_string(header.height), Color::Blue) << std::endl;
    m_output << t("  MipMaps:     ") << c(std::to_string(header.mipMapCount), Color::Yellow) << std::endl;
    m_output << t("  Pitch:       ") << header.pitch << std::endl;
    if (header.depth > 0) { m_output << t("  VTexDepth:   ") << header.depth << std::endl; }
    m_output << t("  Flags:       ") << hf_to_str(header.flags) << std::endl;
    if (header.caps1 > 0) { m_output << t("  Caps1:       ") << caps_to_str(header.caps1) << std::endl; }
    if (header.caps2 > 0) { m_output << t("  Caps2:       ") << caps2_to_str(header.caps2) << std::endl; }
}

void Printer::PrintHeaderDX10(const HeaderDX10& headerDX10) const
{
    m_output << c("DDS Header DX10:", Color::Cyan) << std::endl;
    m_output << t("  Format:      ") << format_to_str(headerDX10.dxgiFormat) << std::endl;
    m_output << t("  Dimension:   ") << rd_to_str(headerDX10.resourceDimension) << std::endl;
    m_output << t("  ArraySize:   ") << headerDX10.arraySize << std::endl;
    if (headerDX10.miscFlag & 0x4) { m_output << t("  MiscFlag1:   ") << "TextureCube" << std::endl; }
    m_output << t("  AlphaMode:   ") << misc_to_str(headerDX10.miscFlags2) << std::endl;
}

void Printer::PrintPixelFormat(const PixelFormat& pixelFormat) const
{
    if (pixelFormat.fourCC != 0) { m_output << t("  FourCC:    ") << c(CCToStr(pixelFormat.fourCC), Color::Magenta) << std::endl; }
    m_output << t("  PixelFFlags: ") << pff_to_str(pixelFormat.flags) << std::endl;
    m_output << t("  RGBBits:     ") << pixelFormat.rgbBitCount << std::endl;
    m_output << t("  RedMask:     ") << c(h(pixelFormat.rBitMask), Color::White, Color::Red) << std::endl;
    m_output << t("  GreenMask:   ") << c(h(pixelFormat.gBitMask), Color::White, Color::Green) << std::endl;
    m_output << t("  BlueMask:    ") << c(h(pixelFormat.bBitMask), Color::White, Color::Blue) << std::endl;
    m_output << t("  AlphaMask:   ") << c(h(pixelFormat.aBitMask), Color::White, Color::BrightBlack) << std::endl;
}

std::string Printer::c(const std::string& s, const Color fg, const Color bg)
{
    return kulerd::c(s, fg, bg);
}

std::string Printer::h(const uint32_t n)
{
    std::stringstream ss;
    ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << n;
    return ss.str();
}

std::string Printer::t(const std::string& s)
{
    return c(s, Color::BrightBlack, Color::Default);
}

std::string Printer::caps_to_str(const uint32_t caps)
{
    std::string result;
    if (caps & static_cast<uint32_t>(Caps::Complex)) { result += "Complex; "; }
    if (caps & static_cast<uint32_t>(Caps::Texture)) { result += "Texture; "; }
    if (caps & static_cast<uint32_t>(Caps::MipMap))  { result += "Mipmap; "; }
    return result;
}

std::string Printer::caps2_to_str(const uint32_t caps)
{
    std::string result;
    if (caps & static_cast<uint32_t>(Caps2::CubeMap))     { result += "Cube Map; "; }
    if (caps & static_cast<uint32_t>(Caps2::CubeMapPosX)) { result += "Positive X; "; }
    if (caps & static_cast<uint32_t>(Caps2::CubeMapNegX)) { result += "Negative X; "; }
    if (caps & static_cast<uint32_t>(Caps2::CubeMapPosY)) { result += "Positive Y; "; }
    if (caps & static_cast<uint32_t>(Caps2::CubeMapNegY)) { result += "Negative Y; "; }
    if (caps & static_cast<uint32_t>(Caps2::CubeMapPosZ)) { result += "Positive Z; "; }
    if (caps & static_cast<uint32_t>(Caps2::CubeMapNegZ)) { result += "Negative Z; "; }
    if (caps & static_cast<uint32_t>(Caps2::Volume))      { result += "Volume; "; }
    return result;
}

std::string Printer::rd_to_str(const ResDimension rd)
{
    switch(rd)
    {
        case ResDimension::Texture1D:   return "Texture1D";
        case ResDimension::Texture2D:   return "Texture2D";
        case ResDimension::Texture3D:   return "Texture3D";
        case ResDimension::TextureCube: return "TextureCube";
        case ResDimension::Buffer:      return "Buffer";
        default:                        return c("Unknown", Color::Red);
    }
}

std::string Printer::pff_to_str(PFFlags flags)
{
    std::string result;
    const auto cint = static_cast<uint32_t>(flags);
    if (cint & static_cast<uint32_t>(PFFlags::AlphaPixels)) { result += "AlphaPixels; "; }
    if (cint & static_cast<uint32_t>(PFFlags::FourCC))      { result += "FourCC; "; }
    if (cint & static_cast<uint32_t>(PFFlags::RGB))         { result += "RGB; "; }
    if (cint & static_cast<uint32_t>(PFFlags::YUV))         { result += "YUV; "; }
    if (cint & static_cast<uint32_t>(PFFlags::Luminance))   { result += "Luminance; "; }
    if (result.empty())                                     { result = "None"; }
    return result;
}

std::string Printer::hf_to_str(HFlags flags)
{
    std::string result;
    const auto cint = static_cast<uint32_t>(flags);
    if (cint & static_cast<uint32_t>(HFlags::Caps))              { result += "Caps; "; }
    if (cint & static_cast<uint32_t>(HFlags::Height))            { result += "Height; "; }
    if (cint & static_cast<uint32_t>(HFlags::Width))             { result += "Width; "; }
    if (cint & static_cast<uint32_t>(HFlags::PitchOrLinearSize)) { result += "PitchOrLinearSize; "; }
    if (cint & static_cast<uint32_t>(HFlags::PixelFormat))       { result += "PixelFormat; "; }
    if (cint & static_cast<uint32_t>(HFlags::Texture))           { result += "Texture; "; }
    if (cint & static_cast<uint32_t>(HFlags::MipMapCount))       { result += "MipMapCount; "; }
    if (cint & static_cast<uint32_t>(HFlags::LinearSize))        { result += "LinearSize; "; }
    if (cint & static_cast<uint32_t>(HFlags::Depth))             { result += "Depth; "; }
    return result;
}

std::string Printer::format_to_str(const uint32_t format)
{
    return 191 < format || 0 == format
        ? c("Unknown", Color::Red)
        : c(DXGI::Format[format], DXGI::Format[format] == "[Unused]"
            ? Color::Red
            : Color::Blue);
}

std::string Printer::misc_to_str(const uint32_t flags)
{
    switch (flags & 0x7)
    {
        case 1:  return "Straight";
        case 2:  return "Premultiplied";
        case 3:  return "Opaque";
        case 4:  return "Custom";
        default: return c("Unknown", Color::Red);
    }
}

} // DDS