#pragma once

#include <cinttypes>

// 48 8B 3D ?? ?? ?? ?? 48 8B 5C 24 60 
constexpr std::uint64_t GWORLD = 0x7D2A0D0;
// 48 8D 0D ?? ?? ?? ?? 8B FA 75 0F
constexpr std::uint64_t GNAMES = 0x7B1C600;
// 48 8B 05 ?? ?? ?? ?? 48 8B 0C C8 4C 8D 04 D1 EB 03 45 33 C0 41 8B 40 08 0F BA E0 1E 72 1B 
constexpr std::uint64_t GOBJECTS = 0x7BBBF90;