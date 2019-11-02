#include "monitoringplugins/logwatcherplugin/logwatcherpluginprocessors.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "dataprocessorhelper/settingshelper.hpp"
#include "monitoringplugins/logwatcherplugin/constants.hpp"
#include "settingsprovider/isettingsprovider.hpp"

using namespace monitoringplugins::logwatcherplugin::constants;

static constexpr char kTableStart[] = "<table class=default>";
static constexpr char kTableEnd[] = "</table>";
static constexpr char kTableRowStart[] = "<tr class=default>";
static constexpr char kTableRowEnd[] = "</tr>";
static constexpr char kTableHeaderStart[] = "<th class=default>";
static constexpr char kTableHeaderEnd[] = "</th>";
static constexpr char kTableColumnStart[] = "<td class=default>";
static constexpr char kTableColumnEnd[] = "</td>";

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
monitoringplugins::logwatcherplugin::CreateProcessors() {
  auto settings = dataprocessorhelper::GetPluginSettings(kPluginName);
  std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>
      processors{};
  auto tags = std::stringstream{
      settings->GetValue(kReportTags, kSettingsSectionReporter)};
  std::string tag;
  while (std::getline(tags, tag, ';')) {
    processors.push_back(
        std::make_shared<monitoringpluginbase::PluginDataProcessor>(
            "%%LOG_TABLE_" + tag + "%%",
            [tag](std::vector<imonitorplugin::PluginData> records)
                -> std::string {
              std::stringstream out{};
              std::stringstream table_content{};
              int row_counter = 0;
              for (const auto& record : records) {
                std::string tags = "";
                std::string entry = "";
                for (const auto& field : record.data()) {
                  if (field.first == kDbFieldEntry) {
                    entry = std::any_cast<std::string>(field.second);
                    continue;
                  } else if (field.first == kDbFieldTags) {
                    tags = std::any_cast<std::string>(field.second);
                  }
                }
                if (tags.find(tag) == std::string::npos) {
                  continue;
                }
                ++row_counter;
                table_content << kTableRowStart << kTableColumnStart << entry
                              << kTableColumnEnd << kTableRowEnd;
              }

              if (row_counter != 0) {
                out << kTableStart << kTableRowStart << kTableHeaderStart << tag
                    << " [" << row_counter << "]" << kTableHeaderEnd
                    << kTableRowEnd;
                out << table_content.rdbuf();
                out << kTableEnd;
              }
              return out.str();
            }));
  }
  return processors;
}
