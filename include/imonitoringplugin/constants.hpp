#ifndef IMONITORINGPLUGINCONSTANTS_HPP
#define IMONITORINGPLUGINCONSTANTS_HPP

namespace imonitoringplugin {

/*! \file constants.h
    \brief Contains definitions for factory function names and possibly other
   constants \details The names of the factory functions are defined as macros.
   The expanded name is used in the macros to create the actual factory
   function. Further, the expansions are converted to string const expressions.
   These may be used when loading the functions from a library.
 */

///
/// \brief The name of the monitoring plugin factory method
/// \note Do not use this macros expansion to get the symbol.
///
#define MONITORING_PLUGIN_CONSTRUCTOR create
///
/// \brief The name of the monitoring plugin destructor
/// \note Do not use this macros expansion to get the symbol.
/// \details Get this symbol to destroy the actual monitoring plugin
/// implementation
///
#define MONITORING_PLUGIN_DESTRUCTOR destroy
///
/// \brief The name of processor factory method
/// \note Do not use this macros expansion to get the symbol.
/// \details Get this symbol to create the processors the plugin provides
///
#define DATA_PROCESSOR_CONSTRUCTOR create_processors
///
/// \brief The name of processor destructor method
/// \note Do not use this macros expansion to get the symbol.
/// \details Get this symbol to destroy the processors the plugin provides
///
#define DATA_PROCESSOR_DESTRUCTOR destroy_processors
///
/// \brief Convert a macro expansion to string
///
#define TO_STRING2(x) #x
#define TO_STRING(x) TO_STRING2(x)

///
/// \brief \copybrief imonitoringplugin::MONITORING_PLUGIN_CONSTRUCTOR
/// \note Use this to get the symbol from the shared library.
///
static constexpr char kMonitoringPluginConstructor[] =
    TO_STRING(MONITORING_PLUGIN_CONSTRUCTOR);

///
/// \brief \copybrief imonitoringplugin::MONITORING_PLUGIN_DESTRUCTOR
/// \note Use this to get the symbol from the shared library.
///
static constexpr char kMonitoringPluginDestructor[] =
    TO_STRING(MONITORING_PLUGIN_DESTRUCTOR);

///
/// \brief \copybrief imonitoringplugin::DATA_PROCESSOR_CONSTRUCTOR
/// \note Use this to get the symbol from the shared library.
///
static constexpr char kDataProcessorConstructor[] =
    TO_STRING(DATA_PROCESSOR_CONSTRUCTOR);

///
/// \brief \copybrief imonitoringplugin::DATA_PROCESSOR_DESTRUCTOR
/// \note Use this to get the symbol from the shared library.
///
static constexpr char kDataProcessorDestructor[] =
    TO_STRING(DATA_PROCESSOR_DESTRUCTOR);

}  // namespace imonitoringplugin

#endif  // CONSTANTS_HPP
