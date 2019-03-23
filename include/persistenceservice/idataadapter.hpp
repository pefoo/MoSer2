#ifndef IDATAADAPTER_H
#define IDATAADAPTER_H

#include "imonitoringplugin/plugindata.hpp"
namespace persistenceservice {
///
/// \brief The data adapter interface
/// \details Provides means to store and load data
///
class IDataAdapter {
 public:
  ///
  /// \brief ~IDataAdapter
  ///
  virtual ~IDataAdapter() {}

  ///
  /// \brief Store some data
  /// \return True, if storing succeeded
  ///
  virtual bool Store(const imonitorplugin::PluginData&) const = 0;
  // TODO add Load(...)
};

}  // namespace persistenceservice

#endif  // IDATAADAPTER_H
