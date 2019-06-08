#ifndef LOADPLUGINPROCESSORS_HPP
#define LOADPLUGINPROCESSORS_HPP
// LCOV_EXCL_START
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
// LCOV_EXCL_STOP
#endif  // LOADPLUGINPROCESSORS_HPP
