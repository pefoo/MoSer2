#ifndef LOGWATCHERPLUGINPROCESSORS_H
#define LOGWATCHERPLUGINPROCESSORS_H

#include "monitoringpluginbase/plugindataprocessor.hpp"
#include "monitoringpluginbase/plugindataprocessorcollection.hpp"
#include "monitoringplugins/logwatcherplugin/constants.hpp"

namespace monitoringplugins {
namespace logwatcherplugin {

///
/// \brief Get a log table token name
/// \param tag The tag(s) to include in the table
/// \return The token name
/// \details A html table that lists all the events that are marked with the
/// passed tag(s)
///
std::string TokenLogTable(const std::string& tag) {
  return "%%LOG_TABLE_" + tag + "%%";
}

///
/// \brief Get a log chart token name
/// \param tag The tag(s) to include in the chart
/// \return The token name
/// \details A histogram that displays the temporal distribution of the events.
/// The result of this token should be embedded into an img tag with
/// data type set to base64.
///
std::string TokenLogChart(const std::string& tag) {
  return "%%LOG_CHART_" + tag + "%%";
}

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
CreateProcessors();

CREATE_PROCESSOR_CONSTRUCTOR_FACTORY(constants::kPluginName, CreateProcessors())
CREATE_PROCESSOR_DESTRUCTOR_FACTORY()

}  // namespace logwatcherplugin
}  // namespace monitoringplugins

#endif  // LOGWATCHERPLUGINPROCESSORS_H
