#pragma once

/* Only for std imports*/

#include <vector>
#include <map>
#include <string>
#include <optional>
#include <cstddef>
#include <time.h>

using std::string;

using std::vector;
using std::map;
using std::pair;
using std::optional;

// 256 bit in sha_256, 8 bits in a byte ==> 32 bytes
constexpr auto BYTES = 256 / 8;
