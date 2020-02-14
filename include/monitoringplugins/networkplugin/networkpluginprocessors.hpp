#ifndef NETWORKPLUGINPROCESSORS_H
#define NETWORKPLUGINPROCESSORS_H
// LCOV_EXCL_START
#include "monitoringpluginbase/plugindataprocessor.hpp"
#include "monitoringpluginbase/plugindataprocessorcollection.hpp"
#include "monitoringplugins/networkplugin/constants.hpp"

namespace monitoringplugins {
namespace networkplugin {

///
/// \brief The network usage time series data in base64 format.
/// \details The result of this token should be embedded into an img tag with
/// data type set to base64.
///
static constexpr char kTokenNetworkTimeSeriesData[] =
    "%%NETWORK_TIME_SERIES_DATA%%";

///
/// \brief The network usage statistics
/// \details Returns a html table with the following stats: \n
/// Type | Average | Min | Max | Standard deviation
/// -----|---------|-----|-----|-------------------
/// rx| {xx.x} | {xx.x} | {xx.x}| {xx.x}
/// tx| {xx.x} | {xx.x} | {xx.x}| {xx.x}
///
static constexpr char kTokenNetworkUsageStats[] = "%%NETWORK_USAGE_STATS%%";

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
CreateProcessors();

CREATE_PROCESSOR_CONSTRUCTOR_FACTORY(constants::kPluginName, CreateProcessors())
CREATE_PROCESSOR_DESTRUCTOR_FACTORY()

}  // namespace networkplugin
}  // namespace monitoringplugins
// LCOV_EXCL_STOP
#endif  // NETWORKPLUGINPROCESSORS_H
