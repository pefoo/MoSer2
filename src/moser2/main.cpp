#include "easyloggingpp-9.96.5/src/easylogging++.h"
#include "monitoringserver.hpp"

INITIALIZE_EASYLOGGINGPP

int main() {
  el::Configurations conf("logger.conf");
  el::Loggers::reconfigureAllLoggers(conf);
  moser2::MonitoringServer server{};
  return 0;
}
