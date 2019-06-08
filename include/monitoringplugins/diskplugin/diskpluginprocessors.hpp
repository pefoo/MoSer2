#ifndef CPUPLUGINPROCESSORS_HPP
#define CPUPLUGINPROCESSORS_HPP
// LCOV_EXCL_START
#include "monitoringpluginbase/plugindataprocessor.hpp"
#include "monitoringpluginbase/plugindataprocessorcollection.hpp"
#include "monitoringplugins/diskplugin/constants.hpp"

namespace monitoringplugins {
namespace diskplugin {

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
CreateProcessors();

CREATE_PROCESSOR_CONSTRUCTOR_FACTORY(constants::kPluginName, CreateProcessors())
CREATE_PROCESSOR_DESTRUCTOR_FACTORY()
}  // namespace diskplugin
}  // namespace monitoringplugins
// LCOV_EXCL_STOP
#endif  // CPUPLUGINPROCESSORS_HPP
