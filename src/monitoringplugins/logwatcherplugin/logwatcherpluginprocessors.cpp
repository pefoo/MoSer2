#include "monitoringplugins/logwatcherplugin/logwatcherpluginprocessors.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "dataprocessorhelper/settingshelper.hpp"
#include "monitoringplugins/logwatcherplugin/constants.hpp"
#include "settingsprovider/isettingsprovider.hpp"
#include "utility/datastructure/table.hpp"

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
            [tag](utility::datastructure::Table data) -> std::string {
              std::stringstream out{};
              std::stringstream table_content{};

              auto entries_col =
                  data.GetDataColumn<std::string>(kDbFieldEntry).data();
              auto tags_col =
                  data.GetDataColumn<std::string>(kDbFieldTags).data();
              size_t row;
              for (row = 0; row < data.MaxSize(); ++row) {
                auto entry = entries_col.at(row);
                auto tags = tags_col.at(row);
                if (tags.find(tag) == std::string::npos) continue;
                table_content << kTableRowStart << kTableColumnStart << entry
                              << kTableColumnEnd << kTableRowEnd;
              }

              if (row != 0) {
                out << kTableStart << kTableRowStart << kTableHeaderStart << tag
                    << " [" << row << "]" << kTableHeaderEnd << kTableRowEnd;
                out << table_content.rdbuf();
                out << kTableEnd;
              }
              return out.str();
            }));
  }
  return processors;
}
