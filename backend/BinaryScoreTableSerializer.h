#ifndef IMAGE_SEARCH_SCORE_TABLE_BINARY_SERIALIZER_H
#define IMAGE_SEARCH_SCORE_TABLE_BINARY_SERIALIZER_H

#include "ScoreTableSerializer.h"

namespace ImageSearch
{

  class BinaryScoreTableSerializer : public ScoreTableSerializer {
  public:
    BinaryScoreTableSerializer (void);
    virtual ~BinaryScoreTableSerializer (void);
    virtual void doSave (const ScoreTable *scoreTable, const std::string &fileName);
    virtual ScoreTable *doLoad (const std::string &fileName);
  };

};

#endif // IMAGE_SEARCH_SCORE_TABLE_BINARY_SERIALIZER_H
