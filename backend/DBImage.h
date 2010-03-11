#ifndef IMAGE_SEARCH_DBIMAGE_H
#define IMAGE_SEARCH_DBIMAGE_H

#include "ImageFeatures.h"

namespace ImageSearch
{

  class DBImage : public ImageFeatures {
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

    DBImage (unsigned long id, const ImageFeatures &image)
      : m_id (id), m_fileName (image.getFileName ()),
      m_featuresYPlus (image.getFeaturesYPlus ()),
      m_featuresYMinus (image.getFeaturesYMinus ()),
      m_featuresUPlus (image.getFeaturesUPlus ()),
      m_featuresUMinus (image.getFeaturesUMinus ()),
      m_featuresVPlus (image.getFeaturesVPlus ()),
      m_featuresVMinus (image.getFeaturesVMinus ()),
      m_averageY (image.getAverageY ()),
      m_averageU (image.getAverageU ()),
      m_averageV (image.getAverageV ()) { }

    virtual ~DBImage (void) {}

    virtual unsigned long getId (void) const { return m_id; }
    virtual const std::string &getFileName (void) const { return m_fileName; }
    virtual const Features &getFeaturesYPlus (void) const { return m_featuresYPlus; }
    virtual const Features &getFeaturesYMinus (void) const { return m_featuresYMinus; }
    virtual const Features &getFeaturesUPlus (void) const { return m_featuresUPlus; }
    virtual const Features &getFeaturesUMinus (void) const { return m_featuresUMinus; }
    virtual const Features &getFeaturesVPlus (void) const { return m_featuresVPlus; }
    virtual const Features &getFeaturesVMinus (void) const { return m_featuresVMinus; }
    virtual float getAverageY (void) const { return m_averageY; }
    virtual float getAverageU (void) const { return m_averageU; }
    virtual float getAverageV (void) const { return m_averageV; }

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

};


#endif // IMAGE_SEARCH_DBIMAGE_H
