#ifndef SQLITEADAPTER_H
#define SQLITEADAPTER_H

#include <sqlite3.h>
#include <memory>
#include <string>
#include <vector>
#include "persistenceservice/adaptersettings.hpp"
#include "persistenceservice/idataadapter.hpp"

namespace persistenceservice {
namespace sqlite {
///
/// \brief The sqlite adapter implementation
///
class SqliteAdapter : public IDataAdapter {
 public:
  ///
  /// \brief Create a new instance
  /// \param adapter_settings The settings for the new adapter
  ///
  explicit SqliteAdapter(AdapterSettings *);

  ///
  /// \brief ~SqliteAdapter
  ///
  ~SqliteAdapter() override;

  ///
  /// \copydoc IDataAdapter::Store()
  ///
  void Store(const imonitorplugin::PluginData &) override;

 private:
  std::vector<std::string> known_plugins_;
  // c interfaces dont like smart pointers...
  sqlite3 *db_;
  void ThrowIfBadCall(int rc, const std::string &action,
                      const std::string &execution_info = "") const;
  void ThrowIfBadCall(int rc, const std::string &action,
                      char *execution_info) const;
};

}  // namespace sqlite
}  // namespace persistenceservice

#endif  // SQLITEADAPTER_H
