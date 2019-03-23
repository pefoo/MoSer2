#ifndef ADAPTERSETTINGS_H
#define ADAPTERSETTINGS_H

#include "AdapterTypes.hpp"

namespace persistenceservice {
///
/// \brief Basic adapter settings class
/// \details Base class of any adapter settings used to create data adapters
///
class AdapterSettings {
 public:
  ///
  /// \brief Create new settings object
  /// \param adapter_type The type of the adapter
  ///
  explicit AdapterSettings(AdapterTypes adapter_type);

  ///
  /// \brief ~AdapterSettings
  ///
  virtual ~AdapterSettings();

  ///
  /// \brief The type of the adapter this settings belong to
  /// \return The adapter type
  ///
  AdapterTypes adapter_type() const;

 private:
  AdapterTypes adapter_type_;
};
}  // namespace persistenceservice

#endif  // ADAPTERSETTINGS_H
