#ifndef ADPLUG_LOAD_HELPER_H
#define ADPLUG_LOAD_HELPER_H

#include <cassert>
#include <cstdint>
#include <cstring>
#include <type_traits>

inline uint16_t adplug_byteswap(const uint16_t val)
{
  return
    ((val & 0x00FF) << 8) |
    ((val & 0xFF00) >> 8);
}

inline uint32_t adplug_byteswap(const uint32_t val)
{
  return
    ((val & 0x000000FF) << 24) |
    ((val & 0x0000FF00) <<  8) |
    ((val & 0x00FF0000) >>  8) |
    ((val & 0xFF000000) >> 24);
}

inline uint64_t adplug_byteswap(const uint64_t val)
{
  return
    ((val & 0x00000000000000FFull) << 56) |
    ((val & 0x000000000000FF00ull) << 40) |
    ((val & 0x0000000000FF0000ull) << 24) |
    ((val & 0x00000000FF000000ull) <<  8) |
    ((val & 0x000000FF00000000ull) >>  8) |
    ((val & 0x0000FF0000000000ull) >> 24) |
    ((val & 0x00FF000000000000ull) >> 40) |
    ((val & 0xFF00000000000000ull) >> 56);
}

enum class load_endian {
  little,
  big
};

template <typename T>
inline T load_unaligned(const unsigned char* src, const load_endian endian)
{
  static_assert(
    std::is_same<T, uint16_t>::value ||
    std::is_same<T, uint32_t>::value ||
    std::is_same<T, uint64_t>::value,
    "load_unaligned can only be used with uint16_t, uint32_t or uint64_t arguments");

  T result;
  std::memcpy(&result, src, sizeof(T));

#ifdef WORDS_BIGENDIAN
  // big-endian CHOST
  if (endian == load_endian::little)
#else
  // little-endian CHOST
  if (endian == load_endian::big)
#endif
  {
    // have to do a byte-swap
    result = adplug_byteswap(result);
  }

  return result;
}

inline uint16_t get_uint16_t(const unsigned char* src, const load_endian endian = load_endian::little)
{
  return load_unaligned<uint16_t>(src, endian);
}

inline uint32_t get_uint32_t(const unsigned char* src, const load_endian endian = load_endian::little)
{
  return load_unaligned<uint32_t>(src, endian);
}

inline uint64_t get_uint64_t(const unsigned char* src, const load_endian endian = load_endian::little)
{
  return load_unaligned<uint64_t>(src, endian);
}

#endif  // ADPLUG_LOAD_HELPER_H
