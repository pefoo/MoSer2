#include "utility/helper/stringhelper.hpp"

bool utility::helper::StringStartsWith(const std::string &a,
                                       const std::string &value) {
  return a.rfind(value, 0) == 0;
}

bool utility::helper::StringRgxGrep(const std::string &a,
                                    const std::string &rgx, std::smatch *sm) {
  return sm ? std::regex_match(a, *sm, std::regex{rgx})
            : std::regex_match(a, std::regex{rgx});
}
