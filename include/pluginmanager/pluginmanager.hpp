#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <algorithm>
#include <memory>
#include "pluginmanager/pluginloader.hpp"
#include <vector>

namespace pluginmanager {

///
/// \brief Plugin manager
/// \tparam PluginInterface The interface of the plugins to load
/// \tparam Create The signature of the function that actually
/// instantiates the plugin
/// \tparam Destroy The signature of the function that actually destroys the
/// plugin instances
/// \details Manages multiple plugin loaders
///
template <typename PluginInterface,
          typename Create =
              typename FunctionTypes<PluginInterface>::DefaultCreateType,
          typename Destroy =
              typename FunctionTypes<PluginInterface>::DefaultDestroyType>
class PluginManager {
 public:
  using InstanceLoader = PluginLoader<PluginInterface, Create, Destroy>;
  using PluginWrapper = typename InstanceLoader::PluginWrapper;

  ///
  /// \brief Load a plugin
  /// \param file_path The path to the library to laod the plugin from
  /// \param create_name The name of the symbol to call to actually load
  /// the plugin
  /// \param destroy_name The name of the symbol to call to
  /// actually destroy the plugin
  /// \param args The arguments to pass to the plugin constructor
  /// \tparam Args The types of the create function arguments
  /// \return A new instance of the requested plugin
  ///
  template <typename... Args>
  PluginWrapper* LoadPlugin(const std::string& file_path,
                            const std::string& create_name = "create",
                            const std::string& destroy_namee = "destroy",
                            Args... args) {
    for (auto& loaderInfo : this->open_loader_) {
      if (loaderInfo.Equals(file_path, create_name, destroy_namee)) {
        loaderInfo.instances.push_back(
            loaderInfo.loader->CreateInstance(args...));
        return loaderInfo.instances.back().get();
      }
    }

    LoaderInfo li{file_path, create_name, destroy_namee};
    li.instances.push_back(li.loader->CreateInstance(args...));
    this->open_loader_.push_back(li);
    return this->open_loader_.back().instances.back().get();
  }

  ///
  /// \brief Destroy a plugin
  /// \param plug   The plugin to destroy
  /// \details Note that this will unset your plugin pointer
  ///
  void DestroyPlugin(PluginWrapper*& plug) {
    for (auto& loaderInfo : this->open_loader_) {
      if (loaderInfo.loader->GetLibraryPath() == plug->GetLibraryPath()) {
        loaderInfo.instances.erase(
            std::remove_if(std::begin(loaderInfo.instances),
                           std::end(loaderInfo.instances),
                           [plug](auto& x) { return x->operator==(plug); }),
            std::end(loaderInfo.instances));
      }
    }
    this->open_loader_.erase(
        std::remove_if(std::begin(this->open_loader_),
                       std::end(this->open_loader_),
                       [](auto& x) { return x.instances.size() == 0; }),
        std::end(this->open_loader_));

    plug = nullptr;
  }

  ///
  /// \brief Destroy all plugins
  /// \details Note that this wont reset any pointers left.
  ///
  void DestroyAll() { this->open_loader_.clear(); }

 private:
  ///
  /// \brief Wrapper for plugin loaders
  /// \details Holds a single plugin loader and multiple instances loaded using
  /// this plugin loader
  ///
  struct LoaderInfo {
    std::unique_ptr<InstanceLoader> loader;
    std::vector<std::shared_ptr<PluginWrapper>> instances;

    ///
    /// \brief Create a new instance
    /// \param lib_path The path of the library to load from
    /// \param create_name The name of the function to create instances
    /// \param destroy_name The name of the function to destroy instances
    ///
    explicit LoaderInfo(const std::string lib_path,
                        const std::string create_name = "create",
                        const std::string destroy_name = "destroy")
        : loader(new InstanceLoader{lib_path, create_name, destroy_name}) {}

    ///
    /// \brief Copy construct
    ///
    LoaderInfo(const LoaderInfo& other)
        : loader(new InstanceLoader{other.loader->GetLibraryPath(),
                                    other.loader->GetConstructorName(),
                                    other.loader->GetDestructorName()}) {
      std::copy(std::begin(other.instances), std::end(other.instances),
                std::back_inserter(this->instances));
    }

    ///
    /// \brief Copy assignment
    ///
    LoaderInfo& operator=(LoaderInfo other) {
      this->loader.swap(other.loader);
      this->instances.swap(other.instances);
      return *this;
    }

    ///
    /// \brief Compare a existing loader with its key characteristics
    /// \param lib_path The path to the library
    /// \param create_name The name of the create function
    /// \param destroy_name The name of the destroy function
    /// \return True, if the characteristics match
    ///
    bool Equals(const std::string& lib_path, const std::string& create_name,
                const std::string& destroy_name) {
      return this->loader->GetLibraryPath() == lib_path &&
             this->loader->GetConstructorName() == create_name &&
             this->loader->GetDestructorName() == destroy_name;
    }
  };

  std::vector<LoaderInfo> open_loader_;
};
}  // namespace pluginmanager
#endif  // PLUGIN_MANAGER_H
