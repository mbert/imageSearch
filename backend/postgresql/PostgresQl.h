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
		const std::string &tableBaseName,
		const std::string &userName,
		const std::string &password,
		int dbImageRows, int dbImageCols, int nKeptCoeffs);
    virtual ~PostgresQl (void);
  private:
    virtual void p_save (const DBImage &image);
    virtual int p_getLastId (void);
    virtual ImageFeaturesList p_findAll (int nMax = 0);
    virtual std::auto_ptr<DBImage> p_getById (int id);
    std::string getTableName (void);
    std::auto_ptr<pqxx::connection> createConnection (void);
    std::string m_hostAddr;
    std::string m_dbName;
    std::string m_tableBaseName;
    std::string m_userName;
    std::string m_password;
    int m_dbImgRows;
    int m_dbImgCols;
    int m_nKeptCoeffs;
  };

};

#endif // IMAGE_SEARCH_POSTGRESQL_H
