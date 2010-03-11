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
    ScoreTable (int rows, int cols, int nKeptCoeffs);
    virtual ~ScoreTable (void);
    void query (const ColorImage &image, ImageScoreList &scores,
		bool debug = false);
    std::string getWeightsInfo () const;
    void loadImages (const ImageFeaturesList &images) { doLoadImages (images); }
    void appendImage (const unsigned long id, const ImageFeatures &image) { doAppendImage (id, image); }
  protected:
    virtual void doLoadImages (const ImageFeaturesList &images);
    virtual void doAppendImage (const unsigned long id, const ImageFeatures &image);
  private:
    virtual void p_query (ImageInformation &qY, ImageInformation &qU,
			  ImageInformation &qV, ImageScoreList &scores,
			  bool debug = false) = 0;
  protected:
    static const float *m_weightY;
    static const float *m_weightU;
    static const float *m_weightV;

    int getLevel (int i);
    int bin (int y, int x);

    std::vector<int> m_lqcache;
    int m_nImages;
    const int m_rows;
    const int m_cols;
    const int m_nKeptCoeffs;
    std::vector<float> m_averageY;
    std::vector<float> m_averageU;
    std::vector<float> m_averageV;
  };

};

#endif // IMAGE_SEARCH_SCORE_TABLE_H
