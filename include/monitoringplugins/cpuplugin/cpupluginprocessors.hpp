#ifndef CPUPLUGINPROCESSORS_HPP
#define CPUPLUGINPROCESSORS_HPP
// LCOV_EXCL_START

#include "monitoringpluginbase/plugindataprocessor.hpp"
#include "monitoringpluginbase/plugindataprocessorcollection.hpp"
#include "monitoringplugins/cpuplugin/constants.hpp"

namespace monitoringplugins {
namespace cpuplugin {

///
/// \brief Cpu time series char in base 64 format.
/// \details The result of this token should be embedded into an img tag with
/// data type set to base64.
///
static constexpr char kTokenCpuTimeSeriesData[] = "%%CPU_TIME_SERIES_DATA%%";

///
/// \brief The cpu usage statistics table.
/// \details Returns a html table with the following stats: \n
/// Core | Average | Min | Max | Standard deviation
/// -----|---------|-----|-----|-------------------
/// {core id}| {xx.x} | {xx.x} | {xx.x}| {xx.x}
/// One core is displayed per row.
///
static constexpr char kTokenCpuUsageStats[] = "%%CPU_USAGE_STATS%%";

///
/// \brief The overall cpu usage average
/// \details Returns just the plain value (one decimal place)
///
static constexpr char kTokenCpuUsageAverage[] = "%%CPU_USAGE_AVERAGE%%";

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
CreateProcessors();

CREATE_PROCESSOR_CONSTRUCTOR_FACTORY(constants::kPluginName, CreateProcessors())
CREATE_PROCESSOR_DESTRUCTOR_FACTORY()
}  // namespace cpuplugin
}  // namespace monitoringplugins

// LCOV_EXCL_STOP
#endif  // CPUPLUGINPROCESSORS_HPP
