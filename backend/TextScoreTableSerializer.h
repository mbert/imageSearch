#ifndef IMAGE_SEARCH_SCORE_TABLE_TEXT_SERIALIZER_H
#define IMAGE_SEARCH_SCORE_TABLE_TEXT_SERIALIZER_H

#include "ScoreTableSerializer.h"

namespace ImageSearch
{

  class TextScoreTableSerializer : public ScoreTableSerializer {
  public:
    TextScoreTableSerializer (void);
    virtual ~TextScoreTableSerializer (void);
    virtual void doSave (const ScoreTable *scoreTable, const std::string &fileName);
    virtual ScoreTable *doLoad (const std::string &fileName);
  };

};

#endif // IMAGE_SEARCH_SCORE_TABLE_TEXT_SERIALIZER_H
