#pragma once
#include <string>

namespace monitoringplugins
{
  namespace diskplugin
  {
    /** 
     * brief Resolve the /dev/root device node 
     * returns The actual device (fully qualified)
    */
    std::string ResolveDevRoot();
  } // namespace diskplugin
} // namespace monitoringplugin