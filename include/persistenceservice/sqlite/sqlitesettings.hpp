#ifndef SQLITESETTINGS_H
#define SQLITESETTINGS_H

#include "persistenceservice/adaptersettings.hpp"

namespace persistenceservice {
namespace sqlite {
///
/// \brief TODO implement the sqlite data adapter settings
///
class SqliteSettings : public AdapterSettings {
 public:
  SqliteSettings();

 private:
};
}  // namespace sqlite
}  // namespace persistenceservice

#endif  // SQLITESETTINGS_H
