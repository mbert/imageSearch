#ifndef IMAGE_SEARCH_PG_INSERT_H
#define IMAGE_SEARCH_PG_INSERT_H

#include <pqxx/connection>
#include <pqxx/transactor>

#include <cxxutil/utils.h>

#include <string>

namespace ImageSearch
{

  namespace Pg
  {

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
      InsertImage (const std::string &tableName,
		   Escaper &escaper, const DBImage &image)
	: pqxx::transactor<> ("InsertImage"),
	m_tableName (tableName), m_escaper (escaper), m_image (image) {}
      virtual ~InsertImage (void) {}
      void operator()(argument_type &t) {
	t.exec("insert into " + m_tableName
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
      std::string m_tableName;
    };

  };

};


#endif // IMAGE_SEARCH_PG_INSERT_H
