#include "PostgresQl.h"
#include "DBImage.h"

#include <cxxutil/utils.h>

#include <pqxx/connection>
#include <pqxx/transactor>

#include <iostream>

using namespace ImageSearch;

#define TABLE_NAME "images_256_256"

class Escaper {
public:
  Escaper (pqxx::connection &connection) : m_connection (connection) {}
  std::string operator()(Features features) {
    return m_connection.esc_raw (features.c_str (), features.size ());
  }
private:
  pqxx::connection & m_connection;
};

class InsertImage : public pqxx::transactor<> {
public:
  InsertImage (Escaper &escaper, const DBImage &image)
    : pqxx::transactor<> ("InsertImage"),
      m_escaper (escaper), m_image (image) {}
  virtual ~InsertImage (void) {}
  void operator()(argument_type &t) {
    t.exec(std::string ("insert into ") + TABLE_NAME
	   + "(id, file_name, feature_vector_y, feature_vector_u,"
	   + " feature_vector_v, average_y, average_u, average_v) values ("
	   + CxxUtil::itoa (m_image.getId ()) + ", '"
	   + m_image.getFilename () + "', E'"
	   + m_escaper (m_image.getFeatureVectorY()) + "'::bytea, E'"
	   + m_escaper (m_image.getFeatureVectorU()) + "'::bytea, E'"
	   + m_escaper (m_image.getFeatureVectorV()) + "'::bytea, "
	   + CxxUtil::dtoa (m_image.getAverageY ()) + ", "
	   + CxxUtil::dtoa (m_image.getAverageU ()) + ", "
	   + CxxUtil::dtoa (m_image.getAverageV ()) + ")");
  }

private:
  Escaper &m_escaper;
  const DBImage &m_image;
};


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

void
PostgresQl::save (const DBImage &image)
{
  try
    {
      pqxx::connection con
	("dbname=" + m_dbName
	 + " hostaddr=" + m_hostAddr
	 + " user=" + m_userName
	 + " password=" + m_password);
      Escaper escaper (con);
      InsertImage insert (escaper, image);
      con.perform (insert);
    }
  catch (const std::exception &e)
    {
      std::cerr << "Exception caught: " << e.what () << std::endl;
      throw (e);
    }
}
