#pragma once

using Checksum = unsigned long long;
using LengthType = long long;

constexpr Checksum initChecksumValue = 0xcbf29ce484222325ULL;
constexpr Checksum magicChecksumPrime = 0x100000001b3ULL;

Checksum updateFnv1a( const void* data, LengthType len, Checksum hval = initChecksumValue );
