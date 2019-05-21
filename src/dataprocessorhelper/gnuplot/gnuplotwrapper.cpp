#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include <string>

int dataprocessorhelper::gnuplot::ExecuteScript(
    const std::string &script, const std::string &output_file,
    const dataprocessorhelper::gnuplot::GnuPlotParameterDict &parameter) {
  auto call =
      "gnuplot -e " + parameter.ToString() + " " + script + " > " + output_file;
  return std::system(call.c_str());
}
