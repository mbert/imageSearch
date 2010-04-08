#include "TextScoreTableSerializer.h"
#include "PreSelectScoreTable.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>

#include <stdexcept>

using namespace ImageSearch;

BOOST_CLASS_EXPORT_GUID (PreSelectScoreTable, "PreSelectScoreTable")

TextScoreTableSerializer::TextScoreTableSerializer (void)
{
}

TextScoreTableSerializer::~TextScoreTableSerializer (void)
{
}

void
TextScoreTableSerializer::doSave (const ScoreTable *scoreTable, const std::string &fileName)
{
  std::ofstream ofs (fileName.c_str ());
  boost::archive::text_oarchive oa (ofs);
  oa << scoreTable;
}

ScoreTable *
TextScoreTableSerializer::doLoad (const std::string &fileName)
{
  ScoreTable *result;
  std::ifstream ifs (fileName.c_str ());
  if (!ifs)
    {
      throw std::ios::failure ("error opening file " + fileName);
    }
  boost::archive::text_iarchive ia (ifs);
  ia >> result;
  return result;
}

