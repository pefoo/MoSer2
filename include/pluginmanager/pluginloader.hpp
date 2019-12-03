#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <dlfcn.h>
#include <algorithm>
#include <functional>
#include <iplugin.hpp>
#include <limits>
#include <memory>
#include <random>
#include <string>
#include <utility>

namespace pluginmanager {

///
/// \brief Types for the create and destroy functions
/// \details The default create and destroy types accept no parameter. The
/// default destroy type is most likely sufficient.
/// \tparam PluginInterface The interface of the plugin implementation
///
template <typename PluginInterface>
struct FunctionTypes {
  ///
  /// \brief The default create type.
  ///
  using DefaultCreateType = PluginInterface*();

  ///
  /// \brief The default destroy type.
  ///
  using DefaultDestroyType = void(PluginInterface*);

  ///
  /// \brief A parameterized version of the create type.
  /// \tparam Args the parameter types
  ///
  template <typename... Args>
  using ParameterizedCreateType = PluginInterface*(Args...);
};

///
/// \brief The PluginLoader class
/// \tparam PluginInterface The interface of the plugins to load
/// \tparam Create The signature of the function that actually
/// instantiates the plugin
/// \tparam Destroy The signature of the function that actually destroys the
/// plugin instances
/// \details Loads constructor and destructor factory
/// functions for classes defined in a library Once this loader is destroyed,
/// the library is closed and thus the loaded classes are destroyed as well. The
/// plugin loader expects the factory functions called create and destroy to be
/// implemented in the library. To ensure name mangling does not take place,
/// mark the function as extern "C"
///
template <typename PluginInterface,
          typename Create =
              typename FunctionTypes<PluginInterface>::DefaultCreateType,
          typename Destroy =
              typename FunctionTypes<PluginInterface>::DefaultDestroyType>
class PluginLoader {
 public:
  using PluginWrapper = IPlugin<PluginInterface>;

  ///
  /// \brief PluginLoader
  /// \param libFile The shared library to load from
  /// \param create_name The name of the symbol to call to actually load the
  /// plugin
  /// \param destroy_name The name of the symbol to call to actually
  /// destroy the plugin
  ///
  explicit PluginLoader(const std::string& lib_file,
                        const std::string& create_name = "create",
                        const std::string& destroy_name = "destroy")
      : library_path_(lib_file),
        constructor_name_(create_name),
        destructor_name_(destroy_name),
        plugin_library_(new RaiiLibrary(lib_file)) {
    auto* create = reinterpret_cast<Create*>(
        dlsym(this->plugin_library_->handle, create_name.c_str()));
    const char* errors = dlerror();
    if (errors) {
      throw std::runtime_error(errors);
    }

    auto* destroy = reinterpret_cast<Destroy*>(
        dlsym(this->plugin_library_->handle, destroy_name.c_str()));
    errors = dlerror();
    if (errors) {
      throw std::runtime_error(errors);
    }
    this->plugin_factory_ = std::make_unique<PluginFactory>(create, destroy);
  }

  ///
  /// \brief Create a new instance of the plugin
  /// \param args The arguments to pass to the plugin constructor
  /// \return A new instance of the plugin wrapper
  ///
  template <typename... argsT>
  std::shared_ptr<PluginWrapper> CreateInstance(argsT... args) {
    return std::make_shared<Plugin>(this->plugin_factory_->destroy,
                                    this->plugin_factory_->create(args...),
                                    this->GetLibraryPath());
  }

  ///
  /// \brief Get the path to the library this loader holds
  /// \return The path to the library
  ///
  [[nodiscard]] std::string GetLibraryPath() const {
    return this->library_path_;
  }

  ///
  /// \brief Get the name of the function that creates actual instances of the
  /// plugin
  /// \return The function name
  ///
  [[nodiscard]] std::string GetConstructorName() const {
    return this->constructor_name_;
  }

  ///
  /// \brief Get the name of the function that actually destroys plugin
  /// instances
  /// \return The function name
  ///
  [[nodiscard]] std::string GetDestructorName() const {
    return this->destructor_name_;
  }

 private:
  ///
  /// \brief The plugin factory wrapper.
  /// \details Containts the factory functions to create and destroy classes
  /// \param create The function to create plugin instances
  /// \param destroy The function to destroy plugin instances
  ///
  struct PluginFactory {
    PluginFactory(Create* create, Destroy* destroy)
        : create(create), destroy(destroy) {}
    const std::function<Create> create;
    const std::function<Destroy> destroy;
  };

  ///
  /// \brief RAII wrapper for library handles
  /// \param lib_file The shared library to load
  ///
  struct RaiiLibrary {
    explicit RaiiLibrary(const std::string& lib_file) : lib_file_(lib_file) {
      this->handle = dlopen(lib_file.c_str(), RTLD_LAZY);
      if (!this->handle) {
        throw std::runtime_error("Failed to load the library " + lib_file);
      }
    }
    ~RaiiLibrary() {
      if (this->handle) {
        dlclose(this->handle);
      }
    }
    RaiiLibrary(const RaiiLibrary& o) : RaiiLibrary(o.lib_file_) {}
    RaiiLibrary(RaiiLibrary&& o) noexcept
        : handle(std::exchange(o.handle, nullptr)),
          lib_file_(std::move(o.lib_file_)) {}
    RaiiLibrary& operator=(const RaiiLibrary& o) {
      return *this = RaiiLibrary(o);
    }
    RaiiLibrary& operator=(RaiiLibrary&& o) = delete;
    void* handle{};

   private:
    const std::string lib_file_;
  };

  ///
  /// \brief A wrapper for the actual plugins
  /// \details Calls the loaded destructor once this object is destroyed.
  ///
  class Plugin : public IPlugin<PluginInterface> {
   public:
    ///
    /// \brief Create a new instance
    /// \param destructor   The destructor to call
    /// \param instance     The plugin instance to wrap
    ///
    Plugin(std::function<Destroy> destructor, PluginInterface* instance,
           std::string library)
        : instance_(instance),
          destructor_(std::move(destructor)),
          libraryPath_(std::move(std::move(library))),
          id_(createRandom()) {}

    ///
    /// \brief Dont copy construct ...
    ///
    Plugin(const Plugin& other) = delete;

    ///
    /// \brief Dont move construct
    ///
    Plugin(Plugin&& other) = delete;

    ///
    /// \brief Dont copy assign
    ///
    Plugin& operator=(const Plugin& other) = delete;

    ///
    /// \brief Dont move assign
    ///
    Plugin& operator=(Plugin&& other) = delete;

    ///
    /// \copydoc pluginmanager::IPlugin::Operator==()
    ///
    bool operator==(const IPlugin<PluginInterface>* const other) override {
      return this->id_ == dynamic_cast<const Plugin*>(other)->id_;
    }

    ///
    /// \brief Destructor. Calls the loaded destructor
    ///
    ~Plugin() {
      destructor_(this->instance_);
      this->instance_ = nullptr;
    }

    ///
    /// \copydoc pluginmanager::IPlugin::Instance()
    ///
    PluginInterface* Instance() override {
      if (!this->instance_) {
        throw std::runtime_error("This plugin was destroyed!");
      }
      return this->instance_;
    }

    ///
    /// \copydoc pluginmanager::IPlugin::GetLibraryPath()
    ///
    [[nodiscard]] std::string GetLibraryPath() const override {
      return this->libraryPath_;
    }

   private:
    PluginInterface* instance_;
    std::function<Destroy> destructor_;
    const std::string libraryPath_;
    const int id_;

    int createRandom() {
      static std::mt19937 gen{std::random_device{}()};
      static std::uniform_int_distribution<> dist{
          std::numeric_limits<int>::min(), std::numeric_limits<int>::max()};
      return dist(gen);
    }
  };

  const std::string library_path_;
  const std::string constructor_name_;
  const std::string destructor_name_;
  std::unique_ptr<RaiiLibrary> plugin_library_;
  std::unique_ptr<const PluginFactory> plugin_factory_;
};

}  // namespace pluginmanager
#endif  // PLUGIN_LOADER_H
