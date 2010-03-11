#ifndef IMAGE_SEARCH_READONLY_IMAGE_H
#define IMAGE_SEARCH_READONLY_IMAGE_H

#include "ImageFeatures.h"

namespace ImageSearch
{

  class ReadOnlyImage : public ImageFeatures {
  public:
    ReadOnlyImage (const std::string &fileName,
		   const Features &featuresYPlus, const Features &featuresYMinus,
		   const Features &featuresUPlus, const Features &featuresUMinus,
		   const Features &featuresVPlus, const Features &featuresVMinus,
		   const float averageY, const float averageU, const float averageV)
      : m_fileName (fileName),
      m_featuresYPlus (featuresYPlus), m_featuresYMinus (featuresYMinus),
      m_featuresUPlus (featuresUPlus), m_featuresUMinus (featuresUMinus),
      m_featuresVPlus (featuresVPlus), m_featuresVMinus (featuresVMinus),
      m_averageY (averageY), m_averageU (averageU), m_averageV (averageV) { }

    virtual ~ReadOnlyImage (void) {}

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

  private:
    const std::string m_fileName;
    const Features m_featuresYPlus;
    const Features m_featuresUPlus;
    const Features m_featuresVPlus;
    const Features m_featuresYMinus;
    const Features m_featuresUMinus;
    const Features m_featuresVMinus;
    const float m_averageY;
    const float m_averageU;
    const float m_averageV;
  };

};


#endif // IMAGE_SEARCH_READONLY_IMAGE_H
