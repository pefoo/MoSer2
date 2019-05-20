#include "monitoringplugins/cpuplugin/cpuplugin.hpp"
#include <unistd.h>
#include <fstream>
#include <regex>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include "monitoringplugins/cpuplugin/constants.hpp"

namespace monitoringplugins {
namespace cpuplugin {
// Each field contains a vector (one value in vector per core)
//
// /proc/stat contains several fields for each cpu.
// field 1: user (time spent in user mode)
// field 2: nice (time spent in user mode with low priority)
// field 3: system (time spent in system mode)
// field 4: idle (time spent in idle task)
//
// There are 6 more fields for each CPU that i dont care for atm.
// https://phoxis.org/2013/09/05/finding-overall-and-per-core-cpu-utilization/
struct CpuPlugin::CpuStat {
  using d_type = int;
  using v_type = std::vector<d_type>;
  CpuStat(v_type user, v_type nice, v_type system, v_type idle)
      : user(std::move(user)),
        nice(std::move(nice)),
        system(std::move(system)),
        idle(std::move(idle)) {}
  CpuStat() = default;

  const v_type user;
  const v_type nice;
  const v_type system;
  const v_type idle;

  static d_type ToDType(const std::string& value) { return std::stoi(value); }
};

CpuPlugin::CpuPlugin()
    : monitoringpluginbase::MonitorPluginBase(constants::kPluginName),
      core_count_(std::thread::hardware_concurrency()) {}

imonitorplugin::PluginData::data_vector CpuPlugin::AcquireDataInternal() const {
  auto p0 = this->GetCpuStat();
  this->Sleep100ms();
  auto p1 = this->GetCpuStat();
  imonitorplugin::PluginData::data_vector usage;
  for (size_t i = 0; i < this->core_count_; ++i) {
    float user_delta = p1.user[i] - p0.user[i];
    float nice_delta = p1.nice[i] - p0.nice[i];
    float system_delta = p1.system[i] - p0.system[i];
    float idle_delta = p1.idle[i] - p0.idle[i];
    float delta_total = user_delta + nice_delta + system_delta + idle_delta;
    float delta_wo_idle = user_delta + nice_delta + system_delta;
    float core_usage;
    if (delta_total == 0.0f || delta_wo_idle == 0.0f) {
      core_usage = 0;
    } else {
      core_usage = ((user_delta + nice_delta + system_delta) /
                    (user_delta + nice_delta + system_delta + idle_delta)) *
                   100;
    }
    usage.push_back({"core" + std::to_string(i), std::move(core_usage)});
  }

  return usage;
}

CpuPlugin::CpuStat CpuPlugin::GetCpuStat() const {
  std::string proc_stat = "/proc/stat";
  if (access(proc_stat.c_str(), R_OK) == -1) {
    this->ThrowPluginException("Failed to read from /proc/stat");
  }
  CpuStat::v_type user, nice, system, idle;
  std::regex rgx(R"(cpu\d+ (\d+) (\d+) (\d+) (\d+))");
  std::smatch match;

  std::ifstream stream(proc_stat);
  std::string line;

  while (std::getline(stream, line)) {
    if (line.find("cpu") == 0) {
      if (std::regex_search(line, match, rgx)) {
        user.push_back(CpuStat::ToDType(match[1]));
        nice.push_back(CpuStat::ToDType(match[2]));
        system.push_back(CpuStat::ToDType(match[3]));
        idle.push_back(CpuStat::ToDType(match[4]));
      }
    }
  }

  return CpuStat(user, nice, system, idle);
}

}  // namespace cpuplugin
}  // namespace monitoringplugins
