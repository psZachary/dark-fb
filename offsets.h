#pragma once

#include <cinttypes>

// https://www.youtube.com/watch?v=GWZQiddlnVM 
// (Easiest one to find by far, YouTube video made by me explains the process very well)
constexpr std::uint64_t GWORLD   = 0x7BE9C50;
// "MulticastDelegateProperty"
// (search string in Cheat Engine & find start of FNamePool & get static pointer then subtract 0x10)
constexpr std::uint64_t GNAMES   = 0x79DC180;
// 48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1 EB 
// (Find sig in IDA)
constexpr std::uint64_t GOBJECTS = 0x7A7BB10;