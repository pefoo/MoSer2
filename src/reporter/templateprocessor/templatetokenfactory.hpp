#ifndef TEMPLATETOKENFACTORY_H
#define TEMPLATETOKENFACTORY_H

#include <memory>
#include <vector>
#include "imonitoringplugin/iplugindataprocessorcollection.hpp"
#include "persistenceservice/idataadapter.hpp"
#include "reporter/templateprocessor/templatetoken.hpp"

namespace reporter {
namespace templateprocessor {

///
/// \brief A factory to create tokens from processors.
///
class TemplateTokenFactory {
 public:
  ///
  /// \brief Create a new instance
  /// \param data_adapter The data adapter to use.
  ///
  TemplateTokenFactory(
      std::shared_ptr<persistenceservice::IDataAdapter> data_adapter);

  ///
  /// \brief Build tokens using the data adapter and the data processor provided
  /// by the processor collection
  /// \param processor_collection The processor collection defined in some
  /// plugin
  /// \param min_age The minimal age of the data to acquire (unix timestamp)
  /// \return A vector with template tokens
  ///
  std::vector<TemplateToken> BuildTokens(
      const imonitorplugin::IPluginDataProcessorCollection* const
          processor_collection,
      int64_t min_age = 0);

 private:
  std::shared_ptr<persistenceservice::IDataAdapter> data_adapter_;
};

}  // namespace templateprocessor
}  // namespace reporter

#endif  // TEMPLATETOKENFACTORY_H
