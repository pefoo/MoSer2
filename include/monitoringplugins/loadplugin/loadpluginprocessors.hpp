#ifndef LOADPLUGINPROCESSORS_HPP
#define LOADPLUGINPROCESSORS_HPP
// LCOV_EXCL_START
#include "monitoringpluginbase/plugindataprocessor.hpp"
#include "monitoringpluginbase/plugindataprocessorcollection.hpp"

namespace monitoringplugins {
namespace loadplugin {

///
/// \brief Load time series char in base 64 format.
/// \details The result of this token should be embedded into an img tag with
/// data type set to base64.
///
static constexpr char kTokenLoadTimeSeriesData[] = "%%LOAD_TIME_SERIES_DATA%%";

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
CreateProcessors();

CREATE_PROCESSOR_CONSTRUCTOR_FACTORY("LoadPlugin", CreateProcessors())
CREATE_PROCESSOR_DESTRUCTOR_FACTORY()
}  // namespace loadplugin
}  // namespace monitoringplugins
// LCOV_EXCL_STOP
#endif  // LOADPLUGINPROCESSORS_HPP
