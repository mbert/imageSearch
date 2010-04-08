#include "BinaryScoreTableSerializer.h"
#include "PreSelectScoreTable.h"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>

#include <stdexcept>

using namespace ImageSearch;

BOOST_CLASS_EXPORT_GUID (PreSelectScoreTable, "PreSelectScoreTable")

BinaryScoreTableSerializer::BinaryScoreTableSerializer (void)
{
}

BinaryScoreTableSerializer::~BinaryScoreTableSerializer (void)
{
}

void
BinaryScoreTableSerializer::doSave (const ScoreTable *scoreTable, const std::string &fileName)
{
  std::ofstream ofs (fileName.c_str (), std::ios::out | std::ios::binary);
  boost::archive::binary_oarchive oa (ofs);
  oa << scoreTable;
}

ScoreTable *
BinaryScoreTableSerializer::doLoad (const std::string &fileName)
{
  ScoreTable *result;
  std::ifstream ifs (fileName.c_str (), std::ios::in | std::ios::binary);
  if (!ifs)
    {
      throw std::ios::failure ("error opening file " + fileName);
    }
  boost::archive::binary_iarchive ia (ifs);
  ia >> result;
  return result;
}

