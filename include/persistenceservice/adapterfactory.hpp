#ifndef ADAPTERFACTORY_H
#define ADAPTERFACTORY_H

#include <memory>
#include "adaptersettings.hpp"
#include "idataadapter.hpp"

namespace persistenceservice {
///
/// \brief A factory to create adapters
/// \details Initialize with the specific adapter settings and used to create
/// new adapters to the data source or sink
///
class AdapterFactory {
 public:
  ///
  /// \brief Create a new instance
  /// \param adapter_settings The settings for the adapters to create using this
  /// factory
  ///
  explicit AdapterFactory(std::unique_ptr<AdapterSettings> adapter_settings);

  ///
  /// \brief Create a new adapter
  /// \return A new adapter of the requested type
  ///
  [[nodiscard]] std::unique_ptr<IDataAdapter> CreateAdapter() const;

 private:
  std::unique_ptr<AdapterSettings> adapter_settings_;
};
}  // namespace persistenceservice

#endif  // ADAPTERFACTORY_H
