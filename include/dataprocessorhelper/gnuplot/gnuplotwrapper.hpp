#ifndef GNUPLOTWRAPPER_H
#define GNUPLOTWRAPPER_H

#include <string>
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"

namespace dataprocessorhelper {
namespace gnuplot {

///
/// \brief Execute a gnuplot script and pipe the output to a file
/// \details To output a chart to a file, set your terminal to png, pngcairo or
/// something similar. Piping the output of plot calls for this terminal type
/// result in images.
/// \param script The script to execute
/// \param output_file The output file
/// \param parameter The parameter for the script
/// \return The return
/// code of the gnuplot interpreter (most likely, actually depends on the
/// implementation of std::system)
///
int ExecuteScript(const std::string& script, const std::string& output_file,
                  const GnuPlotParameterDict& parameter);

}  // namespace gnuplot
}  // namespace dataprocessorhelper

#endif  // GNUPLOTWRAPPER_H
