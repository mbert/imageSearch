#ifndef IMAGE_SEARCH_SCORE_TABLE_SERIALIZER_H
#define IMAGE_SEARCH_SCORE_TABLE_SERIALIZER_H

#include "ScoreTable.h"
#include <iostream>

namespace ImageSearch
{

  class ScoreTableSerializer {
  public:
    void save (const ScoreTable *scoreTable, const std::string &fileName) { doSave (scoreTable, fileName); }
    ScoreTable *load (const std::string &fileName) { doLoad (fileName); }
    virtual void doSave (const ScoreTable *scoreTable, const std::string &fileName) = 0;
    virtual ScoreTable *doLoad (const std::string &fileName) = 0;
  };

};

#endif // IMAGE_SEARCH_SCORE_TABLE_SERIALIZER_H
