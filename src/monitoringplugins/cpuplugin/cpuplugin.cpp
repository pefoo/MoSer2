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
// field 5: iowait (time spent while waiting for to complete)
// field 6: irq (time spent in servicing interrupts)
// field 7: softirq (time spent servicing soft interrupts)
// field 8: steal (involuntary wait)
//
struct CpuPlugin::CpuStat {
  using d_type = int;
  using v_type = std::vector<d_type>;
  CpuStat(v_type user, v_type nice, v_type system, v_type idle, v_type iowait,
          v_type irq, v_type softirq, v_type steal)
      : user(std::move(user)),
        nice(std::move(nice)),
        system(std::move(system)),
        idle(std::move(idle)),
        iowait(std::move(iowait)),
        irq(std::move(irq)),
        softirq(std::move(softirq)),
        steal(std::move(steal)) {}
  CpuStat() = default;

  const v_type user;
  const v_type nice;
  const v_type system;
  const v_type idle;
  const v_type iowait;
  const v_type irq;
  const v_type softirq;
  const v_type steal;

  static d_type ToDType(const std::string& value) { return std::stoi(value); }
};

CpuPlugin::CpuPlugin()
    : monitoringpluginbase::MonitorPluginBase(constants::kPluginName),
      core_count_(std::thread::hardware_concurrency()) {
  this->RegisterFileToRead("/proc/stat");
}

std::vector<std::string> CpuPlugin::DoSanityCheck() const {
  std::vector<std::string> msg;
  if (this->core_count_ <= 0) {
    msg.push_back("Failed to get the cpu core count.");
  }
  return msg;
}

imonitorplugin::PluginData::data_vector CpuPlugin::AcquireDataInternal(
    std::unordered_map<std::string, imonitorplugin::InputFileContent>&&
        input_file) {
  auto p0 = this->GetCpuStat(input_file["/proc/stat"].snapshot_1());
  auto p1 = this->GetCpuStat(input_file["/proc/stat"].snapshot_2());
  imonitorplugin::PluginData::data_vector usage;
  for (size_t i = 0; i < this->core_count_; ++i) {
    float user_delta = p1.user[i] - p0.user[i];
    float nice_delta = p1.nice[i] - p0.nice[i];
    float system_delta = p1.system[i] - p0.system[i];
    float idle_delta = p1.idle[i] - p0.idle[i];
    float iowait_delta = p1.iowait[i] - p0.iowait[i];
    float irq_delta = p1.irq[i] - p0.irq[i];
    float softirq_elta = p1.softirq[i] - p0.softirq[i];
    float steal_delta = p1.steal[i] - p0.steal[i];
    float delta_total = user_delta + nice_delta + system_delta + idle_delta +
                        iowait_delta + irq_delta + softirq_elta + steal_delta;
    float delta_wo_idle = delta_total - (idle_delta + iowait_delta);
    float core_usage;
    if (delta_total == 0.0f || delta_wo_idle == 0.0f) {
      core_usage = 0;
    } else {
      core_usage = ((delta_wo_idle) / (delta_total)) * 100;
    }
    usage.push_back({"core" + std::to_string(i), core_usage});
  }

  return usage;
}

CpuPlugin::CpuStat CpuPlugin::GetCpuStat(const std::string& snapshot) const {
  CpuStat::v_type user, nice, system, idle, iowait, irq, softirq, steal;
  std::regex rgx(R"(cpu\d+ (\d+) (\d+) (\d+) (\d+) (\d+) (\d+) (\d+) (\d+))");
  std::smatch match;

  std::stringstream stream{snapshot};
  std::string line;

  while (std::getline(stream, line, '\n')) {
    // line starts with cpu
    if (line.rfind("cpu", 0) == 0) {
      if (std::regex_search(line, match, rgx)) {
        user.push_back(CpuStat::ToDType(match[1]));
        nice.push_back(CpuStat::ToDType(match[2]));
        system.push_back(CpuStat::ToDType(match[3]));
        idle.push_back(CpuStat::ToDType(match[4]));
        iowait.push_back(CpuStat::ToDType(match[5]));
        irq.push_back(CpuStat::ToDType(match[6]));
        softirq.push_back(CpuStat::ToDType(match[7]));
        steal.push_back(CpuStat::ToDType(match[8]));
      }
    }
  }

  return CpuStat(user, nice, system, idle, iowait, irq, softirq, steal);
}

}  // namespace cpuplugin
}  // namespace monitoringplugins
