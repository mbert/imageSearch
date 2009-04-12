#include "PostgresQl.h"
#include "DBImage.h"

#include "PgInsert.h"
#include "PgSelectCount.h"

#include <iostream>

using namespace ImageSearch;

#define TABLE_NAME "images_256_256"

PostgresQl::PostgresQl (const std::string &hostAddr,
			const std::string &dbName,
			const std::string &userName,
			const std::string &password)
  : m_hostAddr (hostAddr), m_dbName (dbName),
    m_userName (userName), m_password (password)
{
}

PostgresQl::~PostgresQl (void)
{
}

std::auto_ptr<pqxx::connection>
PostgresQl::createConnection (void)
{
  return std::auto_ptr<pqxx::connection>
    (new pqxx::connection ("dbname=" + m_dbName
			   + " hostaddr=" + m_hostAddr
			   + " user=" + m_userName
			   + " password=" + m_password));
}

void
PostgresQl::save (const DBImage &image)
{
  try
    {
      std::auto_ptr<pqxx::connection> con = createConnection ();
      Pg::Escaper escaper (*con);
      Pg::InsertImage insert (TABLE_NAME, escaper, image);
      con->perform (insert);
    }
  catch (const std::exception &e)
    {
      std::cerr << "PostgresQl::save: Exception caught: "
		<< e.what () << std::endl;
      throw (e);
    }
}

int
PostgresQl::getLastId (void)
{
  try
    {
      std::auto_ptr<pqxx::connection> con = createConnection ();
      int result = 0;
      Pg::SelectCount selectCount (TABLE_NAME, result);
      con->perform (selectCount);
      return result - 1;
    }
  catch (const std::exception &e)
    {
      std::cerr << "PostgresQl::getLastId: Exception caught: "
		<< e.what () << std::endl;
      throw (e);
    }
  return -1;
}

