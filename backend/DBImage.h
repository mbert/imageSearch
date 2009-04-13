#ifndef IMAGE_SEARCH_DBIMAGE_H
#define IMAGE_SEARCH_DBIMAGE_H

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

namespace ImageSearch
{

  typedef std::basic_string<unsigned char> Features;

  class DBImage {
  public:
    DBImage (void) : m_id (-1) { }

    DBImage (unsigned long id, const std::string &fileName,
	     const Features &featuresYPlus, const Features &featuresYMinus,
	     const Features &featuresUPlus, const Features &featuresUMinus,
	     const Features &featuresVPlus, const Features &featuresVMinus,
	     float averageY, float averageU, float averageV)
      : m_id (id), m_fileName (fileName),
      m_featuresYPlus (featuresYPlus), m_featuresYMinus (featuresYMinus),
      m_featuresUPlus (featuresUPlus), m_featuresUMinus (featuresUMinus),
      m_featuresVPlus (featuresVPlus), m_featuresVMinus (featuresVMinus),
      m_averageY (averageY), m_averageU (averageU), m_averageV (averageV) { }

    virtual ~DBImage (void) {}

    unsigned long getId (void) const { return m_id; }
    const std::string &getFileName (void) const { return m_fileName; }
    const Features &getFeaturesYPlus (void) const { return m_featuresYPlus; }
    const Features &getFeaturesYMinus (void) const { return m_featuresYMinus; }
    const Features &getFeaturesUPlus (void) const { return m_featuresUPlus; }
    const Features &getFeaturesUMinus (void) const { return m_featuresUMinus; }
    const Features &getFeaturesVPlus (void) const { return m_featuresVPlus; }
    const Features &getFeaturesVMinus (void) const { return m_featuresVMinus; }
    float getAverageY (void) const { return m_averageY; }
    float getAverageU (void) const { return m_averageU; }
    float getAverageV (void) const { return m_averageV; }

    void setId (unsigned long id) { m_id = id; }
    void setFileName (std::string &fileName) { m_fileName = fileName; }
    void setFeaturesYPlus (Features &features) { m_featuresYPlus = features; }
    void setFeaturesYMinus (Features &features) { m_featuresYMinus = features; }
    void setFeaturesUPlus (Features &features) { m_featuresUPlus = features; }
    void setFeaturesUMinus (Features &features) { m_featuresUMinus = features; }
    void setFeaturesVPlus (Features &features) { m_featuresVPlus = features; }
    void setFeaturesVMinus (Features &features) { m_featuresVMinus = features; }
    void setAverageY (float averageY) { m_averageY = averageY; }
    void setAverageU (float averageU) { m_averageU = averageU; }
    void setAverageV (float averageV) { m_averageV = averageV; }

  private:
    unsigned long m_id;
    std::string m_fileName;
    Features m_featuresYPlus;
    Features m_featuresUPlus;
    Features m_featuresVPlus;
    Features m_featuresYMinus;
    Features m_featuresUMinus;
    Features m_featuresVMinus;
    float m_averageY;
    float m_averageU;
    float m_averageV;
  };

  typedef std::vector<boost::shared_ptr<DBImage> > DbImageList;
  typedef DbImageList::const_iterator DbImageIterator;
};


#endif // IMAGE_SEARCH_DBIMAGE_H
