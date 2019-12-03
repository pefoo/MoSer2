#ifndef GNUPLOTWRAPPER_H
#define GNUPLOTWRAPPER_H

#include <functional>
#include <string>
#include <vector>
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "imonitoringplugin/plugindata.hpp"
#include "utility/datastructure/table.hpp"

namespace dataprocessorhelper {
namespace gnuplot {

constexpr char kDefaultDataFileSeparator = ';';
constexpr char kDefaultDataFileVariableName[] = "file_name";
constexpr char kDefaultDateTimeFormat[] = "%Y-%m-%d %H:%M:%S";

struct GnuPlotSettings {
  char data_file_separator = kDefaultDataFileSeparator;
  std::string data_file_variable_name = kDefaultDataFileVariableName;
  std::string default_date_time_format = kDefaultDateTimeFormat;
};

///
/// \brief A wrapper class for the gnuplot backend
/// \details Provides the possibility to test gnuplot dependent code without
/// actually invoking gnuplot. If mock_call is active, gnuplot is NOT invoked.
/// Passed parameter are written to the result file which was passed as
/// parameter.
///
class GnuPlotBackend {
 public:
  GnuPlotBackend(const GnuPlotBackend&) = delete;
  GnuPlotBackend& operator=(const GnuPlotBackend&) = delete;
  GnuPlotBackend(GnuPlotBackend&&) = delete;
  GnuPlotBackend& operator=(GnuPlotBackend&&) = delete;
  ///
  /// \brief Get the singleton instance
  ///
  static GnuPlotBackend& instance();

  ///
  /// \brief Get whether the gnuplot instance is configured to actually call
  /// gnuplot
  /// \return True, if mock call is active (NOT calling gnuplot)
  ///
  [[nodiscard]] bool mock_call() const;

  ///
  /// \brief Set call behavior
  /// \param mock_call True to bypass gnuplot
  ///
  void set_mock_call(bool mock_call);

  ///
  /// \brief Invoke gnuplot
  /// \param script The script to execute
  /// \param params The parameter to pass
  /// \param out_file The file to write the output to
  /// \return The return code of the gnuplot call
  ///
  int Invoke(const std::string& script,
             const dataprocessorhelper::gnuplot::GnuPlotParameterDict& params,
             const std::string& out_file);

  ///
  /// \brief The current settings
  ///
  std::shared_ptr<GnuPlotSettings> settings();

 private:
  GnuPlotBackend();
  bool mock_call_{false};
  std::shared_ptr<GnuPlotSettings> settings_;
};

///
/// \brief Execute a gnuplot script and pipe the output to a file
/// \details To output a chart to a file, set your terminal to png, pngcairo or
/// something similar. Piping the output of plot calls for this terminal type
/// result in images. This function takes care of intermediate data file
/// creatio. X axis data is always written to the first column.
/// The records timestamps are always used for x axis.
/// \param script The script to execute
/// \param data The data to plot
/// \param output_file The name of the output file to create
/// \param parameter The paramter for the script
/// \param filter A filter for the columns to include
/// \return The return code of the gnuplot interpreter
/// (most likely, actually depends on the implementation of std::system)
///
int ExecuteScript(
    const std::string& script, const utility::datastructure::Table& data,
    const std::string& output_file, GnuPlotParameterDict parameter,
    const std::function<bool(const std::string&)>& filter =
        [](const std::string&) { return true; });

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
/// \param data The data to plot
/// \param filter A function to filter record data based on the column name
/// \param parameter The parameter for the script
/// \return The base64 encoded output
///
std::string EncodeScriptOutputToBase64(
    const std::string& script, const utility::datastructure::Table& data,
    GnuPlotParameterDict parameter,
    const std::function<bool(const std::string&)>& filter =
        [](const std::string&) { return true; });

}  // namespace gnuplot
}  // namespace dataprocessorhelper

#endif  // GNUPLOTWRAPPER_H
