#ifndef LOGWATCHERPLUGINPROCESSORS_H
#define LOGWATCHERPLUGINPROCESSORS_H

#include "monitoringpluginbase/plugindataprocessor.hpp"
#include "monitoringpluginbase/plugindataprocessorcollection.hpp"
#include "monitoringplugins/logwatcherplugin/constants.hpp"

namespace monitoringplugins {
namespace logwatcherplugin {

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
CreateProcessors();

CREATE_PROCESSOR_CONSTRUCTOR_FACTORY(constants::kPluginName, CreateProcessors())
CREATE_PROCESSOR_DESTRUCTOR_FACTORY()

}  // namespace logwatcherplugin
}  // namespace monitoringplugins

#endif  // LOGWATCHERPLUGINPROCESSORS_H
