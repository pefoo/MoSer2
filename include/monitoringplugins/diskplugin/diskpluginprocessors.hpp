#ifndef CPUPLUGINPROCESSORS_HPP
#define CPUPLUGINPROCESSORS_HPP
// LCOV_EXCL_START
#include "monitoringpluginbase/plugindataprocessor.hpp"
#include "monitoringpluginbase/plugindataprocessorcollection.hpp"
#include "monitoringplugins/diskplugin/constants.hpp"

namespace monitoringplugins {
namespace diskplugin {

///
/// \brief Get a disk time series data token
/// \param disk The name of the disk
/// \return The disk specific time series data token
/// \details The result of this token should be embedded into an img tag with
/// data type set to base64.
///
std::string TokenDiskTimeSeriesData(const std::string& disk) {
  return "%%DISK_" + disk + "_TIME_SERIES_DATA%%";
}

///
/// \brief The disk usage table.
/// \details A html table that displays the amount of space used on all
/// configured disks.
///
static constexpr char kTokenDiskUsage[] = "%%DISK_USAGE%%";

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
CreateProcessors();

CREATE_PROCESSOR_CONSTRUCTOR_FACTORY(constants::kPluginName, CreateProcessors())
CREATE_PROCESSOR_DESTRUCTOR_FACTORY()
}  // namespace diskplugin
}  // namespace monitoringplugins
// LCOV_EXCL_STOP
#endif  // CPUPLUGINPROCESSORS_HPP
