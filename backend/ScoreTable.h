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

namespace boost
{
  namespace serialization
  {
    class access;
  }
}

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
    void appendImage (const ImageFeatures &image) { doAppendImage (image); ++m_nImages; }
    virtual std::string getImageNameById (const unsigned long id);
    inline int nImages (void) { return m_nImages; }
  protected:
    virtual void doLoadImages (const ImageFeaturesList &images);
    virtual void doAppendImage (const ImageFeatures &image);
  private:
    friend class boost::serialization::access;
    template<class Archive> void
      serialize (Archive & ar, const unsigned int version)
      {
        ar & m_lqcache;
	ar & m_nImages;
        ar & m_rows;
	ar & m_cols;
	ar & m_nKeptCoeffs;
	ar & m_averageY;
	ar & m_averageU;
	ar & m_averageV;
	ar & m_fileName;
      }
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
    int m_rows;
    int m_cols;
    int m_nKeptCoeffs;
    std::vector<float> m_averageY;
    std::vector<float> m_averageU;
    std::vector<float> m_averageV;
    std::vector<std::string> m_fileName;
  };

};

#endif // IMAGE_SEARCH_SCORE_TABLE_H
