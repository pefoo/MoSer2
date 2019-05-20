#include "dataprocessorhelper/base64.hpp"
#include <string>

///
/// \brief The SelectorMasks enum
/// \details Bitmasks to select a fixed amount of bits including bit position
/// from a single byte.
/// Note that 'first' means left.
///
enum SelectorMasks : int {
  first_six = 0xfc,
  first_four = 0xf0,
  first_two = 0xc0,
  last_two = 0x03,
  last_four = 0x0f,
  last_six = 0x3f
};

///
/// \brief The alphabet for base 64 encoding.
/// \details The last sign (=) is not part of the actual alphabet. It was added
/// to simplify translation
///
static const std::string kBase64Alphabet =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "+/"
    "=";

std::string dataprocessorhelper::EncodeBase64(const std::string& v) {
  auto data = v.c_str();
  auto length = v.size();
  std::string b64;
  // Iterate over the data in batches of 3
  // Therefore the length of the input is rounded to the next multiple of 3
  // Each iteration covers 24 bit (3 byte)
  // The data of each iteration is splitted into 4 groups of 6 bit each.
  for (size_t i = 0; i < length + (length % 3 == 0 ? 0 : (3 - length % 3));
       i += 3) {
    char b1 = *(data + i);
    char b2 = length > i + 1 ? *(data + 1 + i) : 0;
    char b3 = length > i + 2 ? *(data + 2 + i) : 0;

    // Bit operations are used to select bits from the groups
    uint x1 = (b1 & SelectorMasks::first_six) >> 2;
    uint x2 = uint(((b1 & SelectorMasks::last_two) << 4) +
                   ((b2 & SelectorMasks::first_four) >> 4));
    uint x3 = uint(length > i + 1 ? ((b2 & SelectorMasks::last_four) << 2) +
                                        ((b3 & SelectorMasks::first_two) >> 6)
                                  : 64);
    uint x4 = length > i + 2 ? b3 & SelectorMasks::last_six : 64;
    b64 += kBase64Alphabet[x1];
    b64 += kBase64Alphabet[x2];
    b64 += kBase64Alphabet[x3];
    b64 += kBase64Alphabet[x4];
  }
  return b64;
}
