#ifndef DDS_DEFS_HPP
#define DDS_DEFS_HPP

#include <cstdint>
#include <stdexcept>
#include <string>

namespace DDS
{

enum class HFlags : uint32_t
{
    Caps = 0x1,
    Height = 0x2,
    Width = 0x4,
    PitchOrLinearSize = 0x8,
    PixelFormat = 0x1000,
    Texture     = 0x1007, //Bitwise-OR combination of Caps, Height, Width, and PixelFormat
    MipMapCount = 0x20000,
    LinearSize = 0x80000,
    Depth = 0x800000
};

enum class PFFlags : uint32_t
{
    AlphaPixels = 0x1,
    FourCC = 0x4,
    RGB = 0x40,
    YUV = 0x200,
    Luminance = 0x20000
};

enum class ResDimension : uint32_t
{
    Unknown = 0,
    Buffer = 1,
    Texture1D = 2,
    Texture2D = 3,
    Texture3D = 4,
    TextureCube = 5
};

enum class Caps : uint32_t
{
    Complex = 0x8,
    Texture = 0x1000,
    MipMap  = 0x400000
};

enum class Caps2 : uint32_t
{
    CubeMap     = 0x200,
    CubeMapPosX = 0x400,
    CubeMapNegX = 0x800,
    CubeMapPosY = 0x1000,
    CubeMapNegY = 0x2000,
    CubeMapPosZ = 0x4000,
    CubeMapNegZ = 0x8000,
    Volume      = 0x200000
};

struct PixelFormat
{
    uint32_t size;           // Size of the pixel format structure (32 bytes)
    PFFlags  flags;          // Flags indicating pixel format properties
    uint32_t fourCC;         // FourCC code for compressed formats, 0 for uncompressed
    uint32_t rgbBitCount;    // Number of bits per pixel for uncompressed formats
    uint32_t rBitMask;       // Bit mask for red channel
    uint32_t gBitMask;       // Bit mask for green channel
    uint32_t bBitMask;       // Bit mask for blue channel
    uint32_t aBitMask;       // Bit mask for alpha channel
};

struct Header
{
    uint32_t    size;           // Size of the header (124 bytes)
    HFlags      flags;          // Flags indicating which fields are valid
    uint32_t    height;         // Height of the texture
    uint32_t    width;          // Width of the texture
    uint32_t    pitch;          // Pitch or linear size of the texture
    uint32_t    depth;          // Depth of the texture (1 for 2D textures)
    uint32_t    mipMapCount;    // Number of mipmap levels
    uint32_t    _reserved1[11];  // Reserved fields, must be zero
    PixelFormat pixelFormat;
    uint32_t    caps1;          // Caps1 flags indicating texture type and usage
    uint32_t    caps2;          // Caps2 flags indicating additional properties
    uint32_t    _reserved2[3];   // Reserved fields, must be zero
};

struct HeaderDX10
{
    uint32_t     dxgiFormat;         // DXGI format of the texture
    ResDimension resourceDimension;  // Resource dimension (1D, 2D, 3D, etc.)
    uint32_t     miscFlag;           // Miscellaneous flags
    uint32_t     arraySize;          // Number of textures in an array
    uint32_t     miscFlags2;         // Additional metadata.
    // The lower 3 bits indicate the alpha mode of the associated resource.
    // The upper 29 bits are reserved and are typically 0
};

static uint32_t StrToCC(const std::string& str)
{

    if (str.length() > 4)
    {
        throw std::invalid_argument("FourCC string must be 4 characters or less");
    }
    return static_cast<uint32_t>(str[0]) |
           static_cast<uint32_t>(str[1]) << 8 |
           static_cast<uint32_t>(str[2]) << 16 |
           static_cast<uint32_t>(str[3]) << 24;
}

static std::string CCToStr(const uint32_t fourCC)
{
    static char str[5] = {};
    str[0] = static_cast<char>(fourCC & 0xFF);
    str[1] = static_cast<char>(fourCC >> 8 & 0xFF);
    str[2] = static_cast<char>(fourCC >> 16 & 0xFF);
    str[3] = static_cast<char>(fourCC >> 24 & 0xFF);
    return {str};
}

}

#endif //DDS_DEFS_HPP
