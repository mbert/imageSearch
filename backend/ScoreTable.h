#ifndef IMAGE_SEARCH_SCORE_TABLE_H
#define IMAGE_SEARCH_SCORE_TABLE_H

#include "ImageScore.h"
#include "DBImage.h"

#include <WTools/ImageInformation.hh>

#include <memory>
#include <vector>

class ColorImage;
class Image;

namespace ImageSearch
{

  class ScoreTable {
  public:
    ScoreTable (int rows, int cols, int nKeptCoeffs, const DbImageList &images);
    virtual ~ScoreTable (void);
    void query (const ColorImage &image, ImageScoreList &scores);
  private:
    void querySingleColor (ImageInformation &truncated,
			   ImageScore &score,
			   float average,
			   const unsigned char *positives,
			   const unsigned char *negatives,
			   const float weights[]);
    std::vector<int> m_lqcache;
    int getLevel (int i);
    int bin (int y, int x);
    bool isSet (const CoeffInformation &ci, int pos,
		const unsigned char array[]);
    int vectorStart (const CoeffInformation &ci);

    void addImageFeatureVector (int index, const Features &src,
				unsigned char *dest);
    const int m_rows;
    const int m_cols;
    const int m_nImages;
    const int m_nKeptCoeffs;
    int m_bufSize;
    int m_scoreListPerPixelSize;
    unsigned char *m_positiveY;
    unsigned char *m_negativeY;
    unsigned char *m_positiveU;
    unsigned char *m_negativeU;
    unsigned char *m_positiveV;
    unsigned char *m_negativeV;
    std::vector<float> m_averageY;
    std::vector<float> m_averageU;
    std::vector<float> m_averageV;
  };

};

#endif // IMAGE_SEARCH_SCORE_TABLE_H
