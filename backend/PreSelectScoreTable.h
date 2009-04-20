#ifndef IMAGE_SEARCH_PRESELECT_SCORE_TABLE_H
#define IMAGE_SEARCH_PRESELECT_SCORE_TABLE_H

#include "ScoreTable.h"

namespace ImageSearch
{

  class PreSelectScoreTable : public ScoreTable {
  public:
    PreSelectScoreTable (int rows, int cols, int nKeptCoeffs,
			 const DbImageList &images);
    virtual ~PreSelectScoreTable (void);
  private:
    virtual void p_query (ImageInformation &qY, ImageInformation &qU,
			  ImageInformation &qV, ImageScoreList &scores);
  };

};

#endif // IMAGE_SEARCH_PRESELECT_SCORE_TABLE_H
