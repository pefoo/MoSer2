#ifndef GNUPLOTPARAMETERDICT_H
#define GNUPLOTPARAMETERDICT_H

#include <map>
#include <string>
#include <utility>

namespace dataprocessorhelper {
namespace gnuplot {

///
/// \brief A wrapper for gnuplot parameter
///
class GnuPlotParameterDict {
 public:
  ///
  /// \brief Create a new instance
  ///
  GnuPlotParameterDict();

  ///
  /// \brief Add a parameter
  /// \param key The parameter key
  /// \param value The parameter value
  /// \param quote True, if the value has to be escaped
  /// \param replace_existing True, if the parameter should overwrite a existing
  /// one with the same key
  ///
  void AddParameter(const std::string& key, const std::string& value,
                    bool quote, bool replace_existing = false);

  ///
  /// \brief Add a parameter
  /// \param dict The parameter dict to add parameters from
  /// \param replace_existing True, if the parameter should overwrite a existing
  /// one with the same key
  ///
  void AddParameter(const GnuPlotParameterDict& dict,
                    bool replace_existing = false);

  ///
  /// \brief Get the string representation, used to call gnuplot scripts
  /// \return The string representation of the parameter dict
  ///
  std::string ToString() const;

 private:
  std::map<std::string, std::pair<std::string, bool>> parameter_;
};

}  // namespace gnuplot
}  // namespace dataprocessorhelper

#endif  // GNUPLOTPARAMETERDICT_H
