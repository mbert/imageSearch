#include "PostgresQl.h"

PostgresQl::PostgresQl (const std::string &hostAddr,
			const std::string &dbName,
			const std::string &userName,
			const std::string &password)
  : m_hostAddr (hostAddr), m_dbName (dbName),
    m_userName (userName), m_password (password)
{
}

    virtual ~PostgresQl (void);
    virtual void save (const DBImage &image) = 0;
  private:
