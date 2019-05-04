#include "persistenceservice/persistencerunner.hpp"
#include "utility"

persistenceservice::PersistenceRunner::PersistenceRunner(
    std::unique_ptr<persistenceservice::IDataAdapter> data_adapter,
    std::function<bool(imonitorplugin::PluginData *)> source)
    : data_adapter_(std::move(data_adapter)), source_func_(std::move(source)) {}
