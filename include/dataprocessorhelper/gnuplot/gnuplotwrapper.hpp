#ifndef GNUPLOTWRAPPER_H
#define GNUPLOTWRAPPER_H

#include <string>
#include <vector>
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "imonitoringplugin/plugindata.hpp"

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
/// \return The return code of the gnuplot interpreter
/// (most likely, actually depends on the implementation of std::system)
///
int ExecuteScript(const std::string& script, const std::string& output_file,
                  const GnuPlotParameterDict& parameter);

///
/// \brief Execute a gnuplot script and pipe the output to a file
/// \details To output a chart to a file, set your terminal to png, pngcairo or
/// something similar. Piping the output of plot calls for this terminal type
/// result in images. This function takes care of intermediate data file
/// creatio. X axis data is always written to the first column.
/// The records timestamps are always used for x axis.
/// \param script The script to execute
/// \param records The data to plot
/// \param data_separator The separator to use for the intermediate data file
/// \param data_file_var_name The name of the data file variable used in
/// the script.
/// This variable is added to as parameter
/// \param output_file The name of the output file to create
/// \param parameter The paramter for the script
/// \param column_names The names of the columns (x must be first)
/// \param time_format The time format to use (in the intermediate file)
/// \return The return code of the gnuplot interpreter
/// (most likely, actually depends on the implementation of std::system)
///
int ExecuteScript(const std::string& script,
                  std::vector<imonitorplugin::PluginData> records,
                  const std::string& data_file_var_name,
                  const std::string& output_file,
                  GnuPlotParameterDict parameter,
                  const std::string& data_separator = ";",
                  std::vector<std::string> column_names = {},
                  const std::string& time_format = "%Y-%m-%d %H:%M:%S");

///
/// \brief Execute a gnuplot script and convert the output to base 64
/// \param script The script the execute
/// \param parameter The parameter for the script
/// \return The base64 encoded output
///
std::string EncodeScriptOutputToBase64(const std::string& script,
                                       const GnuPlotParameterDict& parameter);

///
/// \brief Execute a gnuplot script and convert the output to base 64
/// The records timestamps are always used for x axis.
/// \param script The script the execute
/// \param records The data to plot
/// \param column_names The names of the columns (x must be first)
/// (count must match y_records count +1)
/// \param data_separator The separator to use for the intermediate data file
/// \param data_file_var_name The name of the data file variable used in
/// the script.
/// This variable is added to as parameter
/// \param parameter The parameter for the script
/// \param time_format The time format to use (in the intermediate file)
/// \return The base64 encoded output
///
std::string EncodeScriptOutputToBase64(
    const std::string& script,
    std::vector<imonitorplugin::PluginData> records,
    const std::string& data_file_var_name, GnuPlotParameterDict parameter,
    const std::string& data_separator = ";",
    std::vector<std::string> column_names = {},
    const std::string& time_format = "%Y-%m-%d %H:%M:%S");

}  // namespace gnuplot
}  // namespace dataprocessorhelper

#endif  // GNUPLOTWRAPPER_H
