#include "PostgresQl.h"
#include "DBImage.h"

#include "PgInsert.h"
#include "PgSelect.h"

#include <iostream>

using namespace ImageSearch;

PostgresQl::PostgresQl (const std::string &hostAddr,
			const std::string &dbName,
			const std::string &tableBaseName,
			const std::string &userName,
			const std::string &password,
			int dbImageRows, int dbImageCols, int nKeptCoeffs)
  : m_hostAddr (hostAddr), m_dbName (dbName), m_tableBaseName (tableBaseName),
    m_userName (userName), m_password (password), m_dbImgRows (dbImageRows),
    m_dbImgCols (dbImageCols), m_nKeptCoeffs (nKeptCoeffs)
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
PostgresQl::p_save (const DBImage &image)
{
  try
    {
      std::auto_ptr<pqxx::connection> con = createConnection ();
      Pg::Escaper escaper (*con);
      Pg::InsertImage insert (getTableName (), escaper, image);
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
PostgresQl::p_getLastId (void)
{
  try
    {
      std::auto_ptr<pqxx::connection> con = createConnection ();
      int result = 0;
      Pg::SelectCount selectCount (getTableName (), result);
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

ImageFeaturesList
PostgresQl::p_findAll (int nMax)
{
  ImageFeaturesList result;
  try
    {
      std::auto_ptr<pqxx::connection> con = createConnection ();
      int nImages = 0;
      if (nMax > 0)
	{
	  nImages = nMax;
	}
      else
	{
	  Pg::SelectCount selectCount (getTableName (), nImages);
	  con->perform (selectCount);
	}
      Pg::SelectAll selectAll (getTableName (), result, nImages);
      con->perform (selectAll);
    }
  catch (const std::exception &e)
    {
      std::cerr << "PostgresQl::findAll: Exception caught: "
		<< e.what () << std::endl;
      throw (e);
    }
  return result;
}

std::auto_ptr<DBImage>
PostgresQl::p_getById (int id)
{
  std::auto_ptr<DBImage> result (new DBImage ());
  try
    {
      std::auto_ptr<pqxx::connection> con = createConnection ();
      Pg::SelectById selectById (getTableName (), *result, id);
      con->perform (selectById);
    }
  catch (const std::exception &e)
    {
      std::cerr << "PostgresQl::findById(" << id << "): Exception caught: "
		<< e.what () << std::endl;
      throw (e);
    }
  return result;
}

std::string
PostgresQl::getTableName (void)
{
  return m_tableBaseName + "_" + CxxUtil::itoa (m_dbImgRows) + "_"
    + CxxUtil::itoa (m_dbImgCols) + "_" + CxxUtil::itoa (m_nKeptCoeffs);
}

