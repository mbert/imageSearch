#ifndef IMAGE_SEARCH_BITARRAY_SCORE_TABLE_H
#define IMAGE_SEARCH_BITARRAY_SCORE_TABLE_H

#include "ScoreTable.h"

namespace ImageSearch
{

  class BitArrayScoreTable : public ScoreTable {
  public:
    BitArrayScoreTable (int rows, int cols, int nKeptCoeffs,
			const DbImageList &images);
    virtual ~BitArrayScoreTable (void);
  private:
    virtual void p_query (ImageInformation &qY, ImageInformation &qU,
			  ImageInformation &qV, ImageScoreList &scores);
    void querySingleColor (ImageInformation &truncated, int pos,
			   ImageScore &score, float average,
			   const unsigned char *positives,
			   const unsigned char *negatives,
			   const float weights[]);
    bool isSet (const CoeffInformation &ci, int pos,
		const unsigned char positives[],
		const unsigned char negatives[]);
    int vectorStart (const CoeffInformation &ci);

    void addImageFeatureVector (int index, const Features &src,
				unsigned char *dest);
    int m_bufSize;
    int m_scoreListPerPixelSize;
    unsigned char *m_positiveY;
    unsigned char *m_negativeY;
    unsigned char *m_positiveU;
    unsigned char *m_negativeU;
    unsigned char *m_positiveV;
    unsigned char *m_negativeV;
  };

};

#endif // IMAGE_SEARCH_BITARRAY_SCORE_TABLE_H
