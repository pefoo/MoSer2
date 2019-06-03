#!/bin/bash

if [ "$#" -ne 2 ];then 
  echo "Usage $0 {project_root} {plugin_name}"
  exit 1
fi

project_root="$1"
plugin_name="$2"
plugin_name_lower="$(echo $plugin_name | tr '[:upper:]' '[:lower:]')"
plugin_header_dir="${project_root}/include/monitoringplugins/$plugin_name_lower"
plugin_src_dir="${project_root}/src/monitoringplugins/$plugin_name_lower"

cmake_file="${plugin_src_dir}/CMakeLists.txt"
plugin_src_file="${plugin_src_dir}/${plugin_name_lower}.cpp"
plugin_processor_src_file="${plugin_src_dir}/${plugin_name_lower}processors.cpp"

plugin_header_file="${plugin_header_dir}/${plugin_name_lower}.hpp"
plugin_processor_header_file="${plugin_header_dir}/${plugin_name_lower}processors.hpp"
plugin_constants_header_file="${plugin_header_dir}/constants.hpp"

plugin_include_guard="$(echo $plugin_name | tr '[:lower:]' '[:upper:]')_H"
plugin_processor_include_guard="$(echo $plugin_name | tr '[:lower:]' '[:upper:]')PROCESSORS_H"
plugin_constants_include_guard="$(echo $plugin_name | tr '[:lower:]' '[:upper:]')CONSTANTS_H"

plugin_gp_scirpt_name="${plugin_name_lower}_charts.gp"
plugin_gp_script="${project_root}/etc/${plugin_gp_scirpt_name}"

mkdir -p "$plugin_header_dir"
mkdir -p "$plugin_src_dir"

#
# The cmake file
#
cat << EOF >> "$cmake_file"
configure_file("\${ETC_DIR}/$plugin_gp_scirpt_name" "\${CMAKE_BINARY_DIR}/$plugin_gp_scirpt_name" COPYONLY)

add_library($plugin_name SHARED $(basename $plugin_src_file) $(basename $plugin_processor_src_file) "\${CMAKE_BINARY_DIR}/$plugin_gp_scirpt_name")
target_link_libraries($plugin_name MeasurementPluginBase DataProcessorHelper)

install(TARGETS $plugin_name
  ARCHIVE  DESTINATION \${CMAKE_INSTALL_LIBDIR}/\${PROJECT_NAME}
  LIBRARY  DESTINATION \${CMAKE_INSTALL_LIBDIR}/\${PROJECT_NAME}
  RUNTIME  DESTINATION \${CMAKE_INSTALL_BINDIR}/\${PROJECT_NAME})
install(FILES
  "\${CMAKE_BINARY_DIR}/${plugin_gp_scirpt_name}.gp"
  DESTINATION \${CMAKE_INSTALL_BINDIR}/\${PROJECT_NAME})
EOF

#
# The plugin source file
#
cat << EOF >> "$plugin_src_file"
#include "monitoringplugins/${plugin_name_lower}/${plugin_name_lower}.hpp"
#include "monitoringplugins/${plugin_name_lower}/constants.hpp"

namespace monitoringplugins {
namespace ${plugin_name_lower} {

${plugin_name}::${plugin_name}()
  : monitoringpluginbase::MonitorPluginBase(constants::kPluginName) {}

imonitorplugin::PluginData::data_vector ${plugin_name}::AcquireDataInternal(
    std::unordered_map<std::string, imonitorplugin::InputFileContent>
      &&input_file) const {}

}  // namespace ${plugin_name_lower}
}  // namespace monitoringplugins

EOF

#
# The plugin processor source file
#
cat << EOF >> "${plugin_processor_src_file}"
#include "monitoringplugins/${plugin_name_lower}/${plugin_name_lower}processors.hpp"
#include "monitoringplugins/${plugin_name_lower}/constants.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
monitoringplugins::${plugin_name_lower}::CreateProcessors() {
  return std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>{};
}
EOF

#
# The plugin constants header
#
cat << EOF >> "${plugin_constants_header_file}"
#ifndef ${plugin_constants_include_guard}
#define ${plugin_constants_include_guard}

namespace monitoringplugins {
namespace ${plugin_name_lower} {
namespace constants {

static constexpr char kPluginName[] = "${plugin_name}";

}  // namespace constants
}  // namespace ${plugin_name_lower}
}  // namespace monitoringplugins

#endif  // ${plugin_constants_include_guard}
EOF

#
# The plugin header
#
cat << EOF >> "${plugin_header_file}"
#ifndef ${plugin_include_guard}
#define ${plugin_include_guard}

#include <string>
#include <unordered_map>
#include "monitoringpluginbase/monitorpluginbase.hpp"

namespace monitoringplugins {
namespace ${plugin_name_lower} {

///
/// \\brief TODO documentation
///
class ${plugin_name} : public monitoringpluginbase::MonitorPluginBase {
 public:
  ${plugin_name}();

 protected:
  imonitorplugin::PluginData::data_vector AcquireDataInternal(
    std::unordered_map<std::string, imonitorplugin::InputFileContent>
      &&input_file) const override;
};

CREATE_DETAULT_CONSTRUCTOR_FACTORY(${plugin_name});
CREATE_DEFAULT_DESTRUCTOR_FACTORY(${plugin_name});

}  // namespace ${plugin_name_lower}
}  // namespace monitoringplugins

#endif  // ${plugin_include_gaurd}
EOF

#
# The plugin processor header
#
cat << EOF >> "${plugin_processor_header_file}"
#ifndef ${plugin_processor_include_guard}
#define ${plugin_processor_include_guard}

#include "monitoringpluginbase/plugindataprocessor.hpp"
#include "monitoringpluginbase/plugindataprocessorcollection.hpp"
#include "monitoringplugins/${plugin_name_lower}/constants.hpp"

namespace monitoringplugins {
namespace ${plugin_name_lower} {

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
CreateProcessors();

CREATE_PROCESSOR_CONSTRUCTOR_FACTORY(constants::kPluginName, CreateProcessors())
CREATE_PROCESSOR_DESTRUCTOR_FACTORY()

}  // namespace ${plugin_name_lower}
}  // namespace monitoringplugins

#endif  // ${plugin_processor_include_guard}
EOF

touch "$plugin_gp_script"

echo "Done creating the plugin. CMakeLists.txt file is located at ${cmake_file}"
