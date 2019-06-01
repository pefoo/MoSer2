#ifndef LOADPLUGINPROCESSORS_HPP
#define LOADPLUGINPROCESSORS_HPP

#include "monitoringpluginbase/plugindataprocessor.hpp"
#include "monitoringpluginbase/plugindataprocessorcollection.hpp"

namespace monitoringplugins {
namespace loadplugin {

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
CreateProcessors();

CREATE_PROCESSOR_CONSTRUCTOR_FACTORY("LoadPlugin", CreateProcessors())
CREATE_PROCESSOR_DESTRUCTOR_FACTORY()
}  // namespace loadplugin
}  // namespace monitoringplugins

#endif  // LOADPLUGINPROCESSORS_HPP
