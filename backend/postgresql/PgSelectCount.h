#ifndef IMAGE_SEARCH_PG_SELECT_COUNT_H
#define IMAGE_SEARCH_PG_SELECT_COUNT_H

#include <pqxx/transactor>
#include <pqxx/result>

#include <cxxutil/utils.h>

#include <string>

namespace ImageSearch
{

  namespace Pg
  {

    class SelectCount : public pqxx::transactor<> {
    public:
      SelectCount (const std::string &tableName, int &result)
	: pqxx::transactor<> ("SelectCount"),
	m_tableName (tableName), m_result (result) {}
      virtual ~SelectCount (void) {}
      void operator()(argument_type &t) {
	pqxx::result r = t.exec("select count (*) from " + m_tableName);
	r.at(0).at(0).to (m_result);
      }

    private:
      std::string m_tableName;
      int &m_result;
    };

  };

};


#endif // IMAGE_SEARCH_PG_SELECT_COUNT_H
