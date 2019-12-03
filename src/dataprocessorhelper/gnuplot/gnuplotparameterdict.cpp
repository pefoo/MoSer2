#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include <string>

namespace dataprocessorhelper {
namespace gnuplot {

GnuPlotParameterDict::GnuPlotParameterDict() = default;

void GnuPlotParameterDict::AddParameter(const std::string &key,
                                        const std::string &value, bool quote,
                                        bool replace_existing) {
  if (!replace_existing && this->parameter_.count(key)) {
    return;
  }
  this->parameter_[key] = {value, quote};
}

void GnuPlotParameterDict::AddParameter(const GnuPlotParameterDict &dict,
                                        bool replace_existing) {
  for (const auto &[key, value] : dict.parameter_) {
    this->AddParameter(key, value.first, value.second, replace_existing);
  }
}

std::string GnuPlotParameterDict::ToString() const {
  if (this->parameter_.empty()) {
    return "";
  }
  std::string arg = "\"";

  for (const auto &[key, value] : this->parameter_) {
    arg += key + "=" + (value.second ? "'" : "") + value.first +
           (value.second ? "'" : "") + ";";
  }

  arg = arg.substr(0, arg.size() - 1);
  arg += "\"";
  return arg;
}

}  // namespace gnuplot
}  // namespace dataprocessorhelper
