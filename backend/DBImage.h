#ifndef IMAGE_SEARCH_DBIMAGE_H
#define IMAGE_SEARCH_DBIMAGE_H

#include <string>
#include <vector>

namespace ImageSearch
{

  typedef std::basic_string<unsigned char> Features;

  class DBImage {
  public:
    DBImage (void) { }

    DBImage (unsigned long id,
	     const std::string &fileName,
	     const Features &featureVectorY,
	     const Features &featureVectorU,
	     const Features &featureVectorV,
	     float averageY,
	     float averageU,
	     float averageV)
      : m_id (id), m_fileName (fileName),
      m_featureVectorY (featureVectorY),
      m_featureVectorU (featureVectorU),
      m_featureVectorV (featureVectorV),
      m_averageY (averageY), m_averageU (averageU), m_averageV (averageV) { }

    unsigned long getId (void) const { return m_id; }
    const std::string &getFilename (void) const { return m_fileName; }
    const Features &getFeatureVectorY (void) const { return m_featureVectorY; }
    const Features &getFeatureVectorU (void) const { return m_featureVectorU; }
    const Features &getFeatureVectorV (void) const { return m_featureVectorV; }
    float getAverageY (void) const { return m_averageY; }
    float getAverageU (void) const { return m_averageU; }
    float getAverageV (void) const { return m_averageV; }

    void setId (unsigned long id) { m_id = id; }
    void setFilename (std::string &fileName) { m_fileName = fileName; }
    void setFeatureVectorY (Features &features) { m_featureVectorY = features; }
    void setFeatureVectorU (Features &features) { m_featureVectorU = features; }
    void setFeatureVectorV (Features &features) { m_featureVectorV = features; }
    void setAverageY (float averageY) { m_averageY = averageY; }
    void setAverageU (float averageU) { m_averageU = averageU; }
    void setAverageV (float averageV) { m_averageV = averageV; }

  private:
    unsigned long m_id;
    std::string m_fileName;
    int m_nRows;
    int m_nCols;
    Features m_featureVectorY;
    Features m_featureVectorU;
    Features m_featureVectorV;
    float m_averageY;
    float m_averageU;
    float m_averageV;
  };

  typedef std::vector<DBImage> DBImageList;
  typedef DBImageList::const_iterator DBImageIterator;
};


#endif // IMAGE_SEARCH_DBIMAGE_H
