#ifndef IMAGE_SEARCH_POSTGRESQL_H
#define IMAGE_SEARCH_POSTGRESQL_H

#include "Database.h"

#include <pqxx/connection>

#include <string>
#include <memory>

namespace ImageSearch
{

  class PostgresQl : public Database {
  public:
    PostgresQl (const std::string &hostAddr,
		const std::string &dbName,
		const std::string &userName,
		const std::string &password);
    virtual ~PostgresQl (void);
    virtual void save (const DBImage &image);
    virtual int getLastId (void);
  private:
    std::auto_ptr<pqxx::connection> createConnection (void);
    std::string m_hostAddr;
    std::string m_dbName;
    std::string m_userName;
    std::string m_password;
  };

};

#endif // IMAGE_SEARCH_POSTGRESQL_H
