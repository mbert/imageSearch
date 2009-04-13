#ifndef IMAGE_SEARCH_PG_SELECT_H
#define IMAGE_SEARCH_PG_SELECT_H

#include "DBImage.h"

#include <pqxx/transactor>
#include <pqxx/result>

#include <string>

namespace ImageSearch
{

  namespace Pg
  {

    class SelectCount : public pqxx::transactor<> {
    public:
      SelectCount (const std::string &tableName, int &result);
      virtual ~SelectCount (void);
      void operator() (argument_type &t);

    private:
      std::string m_tableName;
      int &m_result;
    };

    class SelectAll : public pqxx::transactor<> {
    public:
      SelectAll (const std::string &tableName, DbImageList &result, int size);
      virtual ~SelectAll (void);
      void operator() (argument_type &t);

    private:
      std::string m_tableName;
      DbImageList &m_result;
    };

    class SelectById : public pqxx::transactor<> {
    public:
      SelectById (const std::string &tableName, DBImage &result, int id);
      virtual ~SelectById (void);
      void operator()(argument_type &t);

    private:
      std::string m_tableName;
      DBImage &m_result;
      int m_id;
    };
  };

};


#endif // IMAGE_SEARCH_PG_SELECT_H
