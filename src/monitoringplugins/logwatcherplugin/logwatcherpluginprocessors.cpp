#include "monitoringplugins/logwatcherplugin/logwatcherpluginprocessors.hpp"
#include <regex>
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "dataprocessorhelper/settingshelper.hpp"
#include "monitoringplugins/logwatcherplugin/constants.hpp"
#include "settingsprovider/isettingsprovider.hpp"
#include "utility/datastructure/table.hpp"
#include "utility/datastructure/tablefilter.hpp"
#include "utility/helper/stringhelper.hpp"

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
            TokenLogTable(tag),
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
    processors.push_back(std::make_shared<
                         monitoringpluginbase::PluginDataProcessor>(
        TokenLogChart(tag),
        [tag](utility::datastructure::Table data) -> std::string {
          if (data.MaxSize() == 0) {
            return "";
          }
          dataprocessorhelper::gnuplot::GnuPlotParameterDict params{};
          params.AddParameter(
              monitoringplugins::logwatcherplugin::constants::kGpArgYLabel,
              "Invalid ssh user names", true);
          // Only timestamps of events are required
          auto column_filter = [](const std::string& c) {
            return c != "timestamp";
          };
          auto field_filter = [tag](const std::string& c,
                                    const std::string& v) {
            if (c !=
                monitoringplugins::logwatcherplugin::constants::kDbFieldTags)
              return false;

            return utility::helper::StringRgxGrep(v, "\\b" + tag);
          };
          return dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
              monitoringplugins::logwatcherplugin::constants::kGpScriptName,
              data, params, column_filter, field_filter);
        }));
  }
  return processors;
}
