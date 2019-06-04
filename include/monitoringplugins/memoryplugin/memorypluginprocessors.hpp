#ifndef MEMORYPLUGINPROCESSORS_HPP
#define MEMORYPLUGINPROCESSORS_HPP

#include "monitoringpluginbase/plugindataprocessor.hpp"
#include "monitoringpluginbase/plugindataprocessorcollection.hpp"
#include "monitoringplugins/memoryplugin/constants.hpp"

namespace monitoringplugins {
namespace memoryplugin {

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
CreateProcessors();

CREATE_PROCESSOR_CONSTRUCTOR_FACTORY(constants::kPluginName, CreateProcessors())
CREATE_PROCESSOR_DESTRUCTOR_FACTORY()
}  // namespace memoryplugin
}  // namespace monitoringplugins

#endif  // MEMORYPLUGINPROCESSORS_HPP
