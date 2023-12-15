#pragma once

#include <cinttypes>

// 48 8B 3D ?? ?? ?? ?? 48 8B 5C 24 60 
// "\x48\x8B\x3D\xCC\xCC\xCC\xCC\x48\x8B\x5C\x24\x60", "xxx????xxxxx"
constexpr std::uint64_t GWORLD = 0x7D692D0;

// 48 8D 0D ?? ?? ?? ?? 8B FA 75 0F
// "\x48\x8D\x0D\xCC\xCC\xCC\xCC\x8B\xFA\x75\x0F", "xxx????xxxx"
constexpr std::uint64_t GNAMES = 0x7B5B800;

// 48 8B 05 ?? ?? ?? ?? 48 8B 0C C8 4C 8D 04 D1 EB 03 45 33 C0 41 8B 40 08 0F BA E0 1E 72 1B 
// "\x48\x8B\x05\xCC\xCC\xCC\xCC\x48\x8B\x0C\xC8\x4C\x8D\x04\xD1\xEB\x03\x45\x33\xC0\x41\x8B\x40\x08\x0F\xBA\xE0\x1E\x72\x1B", "xxx????xxxxxxxxxxxxxxxxxxxxxxx"
constexpr std::uint64_t GOBJECTS = 0x7BFB190;