#ifndef CPUPLUGINPROCESSORS_HPP
#define CPUPLUGINPROCESSORS_HPP
// LCOV_EXCL_START

#include "monitoringpluginbase/plugindataprocessor.hpp"
#include "monitoringpluginbase/plugindataprocessorcollection.hpp"
#include "monitoringplugins/cpuplugin/constants.hpp"

namespace monitoringplugins {
namespace cpuplugin {

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
CreateProcessors();

CREATE_PROCESSOR_CONSTRUCTOR_FACTORY(constants::kPluginName, CreateProcessors())
CREATE_PROCESSOR_DESTRUCTOR_FACTORY()
}  // namespace cpuplugin
}  // namespace monitoringplugins

// LCOV_EXCL_STOP
#endif  // CPUPLUGINPROCESSORS_HPP
