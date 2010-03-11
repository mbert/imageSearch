#ifndef IMAGE_SEARCH_IMAGE_FEATURES_H
#define IMAGE_SEARCH_IMAGE_FEATURES_H

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

namespace ImageSearch
{

  typedef std::basic_string<unsigned char> Features;

  class ImageFeatures {
  public:
    virtual const std::string &getFileName (void) const = 0;
    virtual const Features &getFeaturesYPlus (void) const = 0;
    virtual const Features &getFeaturesYMinus (void) const = 0;
    virtual const Features &getFeaturesUPlus (void) const = 0;
    virtual const Features &getFeaturesUMinus (void) const = 0;
    virtual const Features &getFeaturesVPlus (void) const = 0;
    virtual const Features &getFeaturesVMinus (void) const = 0;
    virtual float getAverageY (void) const = 0;
    virtual float getAverageU (void) const = 0;
    virtual float getAverageV (void) const = 0;
  };

  typedef std::vector<boost::shared_ptr<ImageFeatures> > ImageFeaturesList;
  typedef ImageFeaturesList::const_iterator ImageFeaturesConstIterator;
};


#endif // IMAGE_SEARCH_IMAGE_FEATURES_H
