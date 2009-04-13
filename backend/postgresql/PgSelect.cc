#include "PgSelect.h"

#include <cxxutil/utils.h>

#include <pqxx/binarystring>

#include <iostream>

using namespace ImageSearch;

static DBImage *makeDbImage (pqxx::result::tuple tup);
static void fillDbImage (DBImage &image, pqxx::result::tuple tup);

Pg::SelectCount::SelectCount (const std::string &tableName, int &result)
  : pqxx::transactor<> ("SelectCount"),
    m_tableName (tableName), m_result (result)
{
}

Pg::SelectCount::~SelectCount (void)
{
}

void Pg::SelectCount::operator() (argument_type &t)
{
  pqxx::result r = t.exec("select count (*) from " + m_tableName);
  r.at(0).at(0).to (m_result);
}


Pg::SelectAll::SelectAll (const std::string &tableName,
			  DbImageList &result, int size)
  : pqxx::transactor<> ("SelectAll"),
    m_tableName (tableName), m_result (result)
{
  m_result.resize (size);
}

Pg::SelectAll::~SelectAll (void)
{
}

void
Pg::SelectAll::operator() (argument_type &t)
{
  pqxx::result r = t.exec("select * from " + m_tableName);
  //r.at(0).at(0).to (m_result);
  for (pqxx::result::const_iterator it = r.begin (); it != r.end (); ++it)
    {
      m_result.push_back (boost::shared_ptr<DBImage>(makeDbImage (*it)));
    }
}


Pg::SelectById::SelectById (const std::string &tableName,
			    DBImage &result, int id)
  : pqxx::transactor<> ("SelectAll"),
    m_tableName (tableName), m_result (result), m_id (id)
{
}

Pg::SelectById::~SelectById (void)
{
}

void
Pg::SelectById::operator() (argument_type &t)
{
  pqxx::result r = t.exec("select * from " + m_tableName + " where id = "
			  + CxxUtil::itoa (m_id));
  if (r.size () != 1)
    {
      throw std::invalid_argument ("No entry for id: " + CxxUtil::itoa (m_id));
    }
  ::fillDbImage (m_result, r[0]);
}

static void fillDbImage (DBImage &image, pqxx::result::tuple tup) 
{
  int id;
  std::string fileName;
  float averageY;
  float averageU;
  float averageV;

  tup[tup.column_number ("id")].to (id);
  tup[tup.column_number ("file_name")].to (fileName);
  pqxx::binarystring byP (tup[tup.column_number ("features_y_plus")]);
  pqxx::binarystring byM (tup[tup.column_number ("features_y_minus")]);
  pqxx::binarystring buP (tup[tup.column_number ("features_u_plus")]);
  pqxx::binarystring buM (tup[tup.column_number ("features_u_minus")]);
  pqxx::binarystring bvP (tup[tup.column_number ("features_v_plus")]);
  pqxx::binarystring bvM (tup[tup.column_number ("features_v_minus")]);
  tup[tup.column_number ("average_y")].to (averageY);
  tup[tup.column_number ("average_u")].to (averageU);
  tup[tup.column_number ("average_v")].to (averageV);

  image.setId (id);
  image.setFileName (fileName);

  Features fyP ((const unsigned char*)byP.c_ptr ());
  image.setFeaturesYPlus (fyP);
  Features fyM ((const unsigned char*)byM.c_ptr ());
  image.setFeaturesYMinus (fyM);
  Features fuP ((const unsigned char*)buP.c_ptr ());
  image.setFeaturesUPlus (fuP);
  Features fuM ((const unsigned char*)buM.c_ptr ());
  image.setFeaturesUMinus (fuM);
  Features fvP ((const unsigned char*)bvP.c_ptr ());
  image.setFeaturesVPlus (fvP);
  Features fvM ((const unsigned char*)bvM.c_ptr ());
  image.setFeaturesVMinus (fvM);

  image.setAverageY (averageY);
  image.setAverageU (averageU);
  image.setAverageV (averageV);

}

static DBImage *makeDbImage (pqxx::result::tuple tup) 
{

  DBImage *image = new DBImage ();
  fillDbImage (*image, tup);
  return image;
}
