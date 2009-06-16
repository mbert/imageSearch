#ifndef IMAGE_SEARCH_SCORE_TABLE_H
#define IMAGE_SEARCH_SCORE_TABLE_H

#include "ImageScore.h"
#include "DBImage.h"

#include <WTools/ImageInformation.hh>

#include <string>
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
    std::string getWeightsInfo () const;
  private:
    virtual void p_query (ImageInformation &qY, ImageInformation &qU,
			  ImageInformation &qV, ImageScoreList &scores) = 0;
  protected:
    static const float m_weightY[];
    static const float m_weightU[];
    static const float m_weightV[];

    int getLevel (int i);
    int bin (int y, int x);

    std::vector<int> m_lqcache;
    const int m_rows;
    const int m_cols;
    const int m_nImages;
    const int m_nKeptCoeffs;
    std::vector<float> m_averageY;
    std::vector<float> m_averageU;
    std::vector<float> m_averageV;
  };

};

#endif // IMAGE_SEARCH_SCORE_TABLE_H
