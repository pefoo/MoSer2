#ifndef NETWORKPLUGINPROCESSORS_H
#define NETWORKPLUGINPROCESSORS_H
// LCOV_EXCL_START
#include "monitoringpluginbase/plugindataprocessor.hpp"
#include "monitoringpluginbase/plugindataprocessorcollection.hpp"
#include "monitoringplugins/networkplugin/constants.hpp"

namespace monitoringplugins {
namespace networkplugin {

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
CreateProcessors();

CREATE_PROCESSOR_CONSTRUCTOR_FACTORY(constants::kPluginName, CreateProcessors())
CREATE_PROCESSOR_DESTRUCTOR_FACTORY()

}  // namespace networkplugin
}  // namespace monitoringplugins
// LCOV_EXCL_STOP
#endif  // NETWORKPLUGINPROCESSORS_H
