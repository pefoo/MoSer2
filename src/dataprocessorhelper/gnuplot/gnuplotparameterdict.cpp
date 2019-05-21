#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include <string>
namespace dataprocessorhelper {
namespace gnuplot {

GnuPlotParameterDict::GnuPlotParameterDict() {}

void GnuPlotParameterDict::AddParameter(const std::string &key,
                                        const std::string value, bool quote,
                                        bool replace_existing) {
  if (!replace_existing && this->parameter_.count(key)) {
    return;
  }
  this->parameter_.at(key) = {value, quote};
}

void GnuPlotParameterDict::AddParameter(const GnuPlotParameterDict &dict) {
  for (const auto &[key, value] : dict.parameter_) {
    this->parameter_.at(key) = value;
  }
}

std::string GnuPlotParameterDict::ToString() const {
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
