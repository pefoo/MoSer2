#include "easyloggingpp-9.96.5/src/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main() {
  el::Configurations conf("logger.conf");
  el::Loggers::reconfigureAllLoggers(conf);

  return 0;
}
