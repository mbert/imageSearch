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
    typedef std::vector<int> IdList;
    typedef IdList::iterator IdListIterator;
    typedef std::vector<IdList> IdListList;
    typedef IdListList::iterator IdListListIterator;

    virtual void p_query (ImageInformation &qY, ImageInformation &qU,
			  ImageInformation &qV, ImageScoreList &scores);
    void addImageFeatureVector (int index, const Features &src,
				IdListList &dest);
    void querySingleColor (ImageInformation &truncated, ImageScoreList &scores,
			   IdListList &positives, IdListList &negatives,
			   const float weights[]);

    IdListList m_positiveY;
    IdListList m_negativeY;
    IdListList m_positiveU;
    IdListList m_negativeU;
    IdListList m_positiveV;
    IdListList m_negativeV;
  };

};

#endif // IMAGE_SEARCH_PRESELECT_SCORE_TABLE_H
