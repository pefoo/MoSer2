#ifndef SQLITESETTINGS_H
#define SQLITESETTINGS_H

#include <string>
#include "persistenceservice/adaptersettings.hpp"

namespace persistenceservice {
namespace sqlite {
///
/// \brief Sqlite database adapter settings
///
class SqliteSettings : public AdapterSettings {
 public:
  ///
  /// \brief Create a new instance
  /// \param file_name The name (+path) of the database file
  ///
  explicit SqliteSettings(std::string file_name);

  ///
  /// \brief Get the database file name and path
  /// \return The database file name and path
  ///
  std::string file_name() const;

 private:
  std::string file_name_;
};
}  // namespace sqlite
}  // namespace persistenceservice

#endif  // SQLITESETTINGS_H
