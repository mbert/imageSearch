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

    // Danger, this transation only works with successive IDs, no gaps!
    class SelectAll : public pqxx::transactor<> {
    public:
      SelectAll (const std::string &tableName, ImageFeaturesList &result, int size);
      virtual ~SelectAll (void);
      void operator() (argument_type &t);

    private:
      void addChunkToResult(argument_type &t, int from, int maxNum);
      int m_size;
      std::string m_tableName;
      ImageFeaturesList &m_result;
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
