#ifndef SQLITEADAPTER_H
#define SQLITEADAPTER_H

#include <sqlite3.h>
#include <memory>
#include <string>
#include "persistenceservice/adaptersettings.hpp"
#include "persistenceservice/idataadapter.hpp"

// TODO Implement the adapter
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
  bool Store(const imonitorplugin::PluginData &) const override;

 private:
  // c interfaces dont like smart pointers...
  sqlite3 *db_;
  void ThrowIfBadCall(int rc, const std::string &action,
                      const std::string &execution_info = "") const;
};

}  // namespace sqlite
}  // namespace persistenceservice

#endif  // SQLITEADAPTER_H
