#ifndef PRINTING_HPP
#define PRINTING_HPP

#include <iomanip>
#include <iostream>
#include <cstdint>

#include "dds.hpp"
#include <kulerd/kulerd.hpp>

namespace DDS
{
using namespace kulerd;
static std::string c(const uint32_t n, const Color fg = Color::Default, const Color bg = Color::Default)
{
    return kulerd::c(std::to_string(n), fg, bg);
}

static std::string h(const uint32_t n)
{
    std::stringstream ss;
    ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << n;
    return ss.str();
}

static std::string t(const std::string& s)
{
    return c(s, Color::BrightBlack, Color::Default);
}

static uint32_t str_to_cc(const char* str)
{
    if (std::strlen(str) > 4) {
        throw std::invalid_argument("FourCC string must be 4 characters or less");
    }
    return static_cast<uint32_t>(str[0]) |
           static_cast<uint32_t>(str[1]) << 8 |
           static_cast<uint32_t>(str[2]) << 16 |
           static_cast<uint32_t>(str[3]) << 24;
}

static char* cc_to_str(const uint32_t fourCC)
{
    static char str[5] = {0};
    str[0] = static_cast<char>(fourCC & 0xFF);
    str[1] = static_cast<char>(fourCC >> 8 & 0xFF);
    str[2] = static_cast<char>(fourCC >> 16 & 0xFF);
    str[3] = static_cast<char>(fourCC >> 24 & 0xFF);
    return str;
}

static std::string caps_to_str(const uint32_t caps)
{
    std::string result;
    if (caps & 0x8) { result += "Complex; "; }
    if (caps & 0x1000) { result += "Texture; "; }
    if (caps & 0x400000) { result += "Mipmap; "; }
    return result;
}

static std::string caps2_to_str(const uint32_t caps)
{
    std::string result;
    if (caps & 0x200) { result += "Cubemap; "; }
    if (caps & 0x400) { result += "Positive X; "; }
    if (caps & 0x800) { result += "Negative X; "; }
    if (caps & 0x1000) { result += "Positive Y; "; }
    if (caps & 0x2000) { result += "Negative Y; "; }
    if (caps & 0x4000) { result += "Positive Z; "; }
    if (caps & 0x8000) { result += "Negative Z; "; }
    if (caps & 0x200000) { result += "Volume; "; }
    return result;
}

static std::string to_str(const ResourceDimension rd)
{
    switch(rd)
    {
        case ResourceDimension::Texture1D:
            return "Texture1D";
        case ResourceDimension::Texture2D:
            return "Texture2D";
        case ResourceDimension::Texture3D:
            return "Texture3D";
        case ResourceDimension::TextureCube:
            return "TextureCube";
        case ResourceDimension::Buffer:
            return "Buffer";
        default:
            return c("Unknown", Color::Red);
    }
}

static std::string to_str(const PixelFormatFlags flags)
{
    std::string result;
    const auto cint = static_cast<uint32_t>(flags);
    if (cint & static_cast<uint32_t>(PixelFormatFlags::AlphaPixels)) { result += "AlphaPixels; "; }
    if (cint & static_cast<uint32_t>(PixelFormatFlags::FourCC)) { result += "FourCC; "; }
    if (cint & static_cast<uint32_t>(PixelFormatFlags::RGB)) { result += "RGB; "; }
    if (cint & static_cast<uint32_t>(PixelFormatFlags::YUV)) { result += "YUV; "; }
    if (cint & static_cast<uint32_t>(PixelFormatFlags::Luminance)) { result += "Luminance; "; }
    if (result.empty()) { result = "None"; }
    return result;
}

static std::string to_str(const HeaderFlags flags)
{
    std::string result;
    const auto cint = static_cast<uint32_t>(flags);
    if (cint & static_cast<uint32_t>(HeaderFlags::Caps)) { result += "Caps; "; }
    if (cint & static_cast<uint32_t>(HeaderFlags::Height)) { result += "Height; "; }
    if (cint & static_cast<uint32_t>(HeaderFlags::Width)) { result += "Width; "; }
    if (cint & static_cast<uint32_t>(HeaderFlags::PitchOrLinearSize)) { result += "PitchOrLinearSize; "; }
    if (cint & static_cast<uint32_t>(HeaderFlags::PixelFormat)) { result += "PixelFormat; "; }
    if (cint & static_cast<uint32_t>(HeaderFlags::Texture)) { result += "Texture; "; }
    if (cint & static_cast<uint32_t>(HeaderFlags::MipMapCount)) { result += "MipMapCount; "; }
    if (cint & static_cast<uint32_t>(HeaderFlags::LinearSize)) { result += "LinearSize; "; }
    if (cint & static_cast<uint32_t>(HeaderFlags::Depth)) { result += "Depth; "; }
    return result;
}

static std::string format_to_str(const uint32_t format)
{
    if (191 < format || 0 == format) {
        return c("Unknown", Color::Red);
    }
    return c(DXGI::Format[format], DXGI::Format[format] == "[Unused]" ? Color::Red : Color::Blue);
}

static std::string misc_to_str(const uint32_t flags)
{
    switch (flags & 0x7) {
        case 1: return "Straight";
        case 2: return "Premultiplied";
        case 3: return "Opaque";
        case 4: return "Custom";
        default: return c("Unknown", Color::Red);
    }
}

inline void print_header(const Header& header, const HeaderDX10& headerDX10 = {}, const bool isDX10 = false)
{
    std::cout << c("DDS Header:", Color::Cyan, Color::Default) << std::endl;
    std::cout << t("  Width:     ") << c(header.width, Color::Blue) << std::endl;
    std::cout << t("  Height:    ") << c(header.height, Color::Blue) << std::endl;
    std::cout << t("  MipMaps:   ") << c(header.mipMapCount, Color::Yellow) << std::endl;
    std::cout << t("  Pitch:     ") << header.pitch << std::endl;
    if (header.depth > 0) { std::cout << t("  VTexDepth: ") << header.depth << std::endl; }
    std::cout << t("  Flags:     ") << to_str(header.flags) << std::endl;
    if (header.caps1 > 0) { std::cout << t("  Caps1:     ") << caps_to_str(header.caps1) << std::endl; }
    if (header.caps2 > 0) { std::cout << t("  Caps2:     ") << caps2_to_str(header.caps2) << std::endl; }

    if (isDX10) {
        std::cout << c("DDS Header DX10:", Color::Cyan) << std::endl;
        std::cout << t("  Format:    ") << format_to_str(headerDX10.dxgiFormat) << std::endl;
        std::cout << t("  Dimension: ") << to_str(headerDX10.resourceDimension) << std::endl;
        std::cout << t("  ArraySize: ") << headerDX10.arraySize << std::endl;
        if (headerDX10.miscFlag & 0x4) { std::cout << t("  MiscFlag1: ") << "TextureCube" << std::endl; }
        std::cout << t("  AlphaMode: ") << misc_to_str(headerDX10.miscFlags2) << std::endl;
    } else {
        if (header.pixelFormat.fourCC != 0) { std::cout << t("  FourCC:    ") << c(cc_to_str(header.pixelFormat.fourCC), Color::Magenta) << std::endl; }
        std::cout << t("  RGBBits:   ") << header.pixelFormat.rgbBitCount << std::endl;
        std::cout << t("  RedMask:   ") << c(h(header.pixelFormat.rBitMask), Color::White, Color::Red) << std::endl;
        std::cout << t("  GreenMask: ") << c(h(header.pixelFormat.gBitMask), Color::White, Color::Green) << std::endl;
        std::cout << t("  BlueMask:  ") << c(h(header.pixelFormat.bBitMask), Color::White, Color::Blue) << std::endl;
        std::cout << t("  AlphaMask: ") << c(h(header.pixelFormat.aBitMask), Color::White, Color::BrightBlack) << std::endl;
    }
}
}

#endif //PRINTING_HPP
