#ifndef IMAGE_SEARCH_READONLY_IMAGE_H
#define IMAGE_SEARCH_READONLY_IMAGE_H

#include "ImageFeatures.h"
#include <boost/serialization/split_member.hpp>

namespace boost
{
  namespace serialization
  {
    class access;
  }
}

namespace ImageSearch
{

  class ReadOnlyImage : public ImageFeatures {
  public:
    ReadOnlyImage (void) { }
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
    friend class boost::serialization::access;
    template<class Archive>
      void save (Archive & ar, const unsigned int version) const
      {
	ar & m_fileName;
	std::basic_string<char> s;
	s = fromFeatures (m_featuresYPlus);
	ar & s;
	s = fromFeatures (m_featuresUPlus);
	ar & s;
	s = fromFeatures (m_featuresVPlus);
	ar & s;
	s = fromFeatures (m_featuresYMinus);
	ar & s;
	s = fromFeatures (m_featuresUMinus);
	ar & s;
	s = fromFeatures (m_featuresVMinus);
	ar & s;
	ar & m_averageY;
	ar & m_averageU;
	ar & m_averageV;
      }
    template<class Archive>
      void load (Archive & ar, const unsigned int version)
      {
	std::basic_string<char> s;
	ar & m_fileName;
	ar & s;
	m_featuresYPlus = toFeatures (s);
	ar & s;
	m_featuresUPlus = toFeatures (s);
	ar & s;
	m_featuresVPlus = toFeatures (s);
	ar & s;
	m_featuresYMinus = toFeatures (s);
	ar & s;
	m_featuresUMinus = toFeatures (s);
	ar & s;
	m_featuresVMinus = toFeatures (s);
	ar & m_averageY;
	ar & m_averageU;
	ar & m_averageV;
      }
    BOOST_SERIALIZATION_SPLIT_MEMBER ()
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
    std::basic_string<char> fromFeatures (const Features &f) const {
      std::basic_string<char> s;
      s.assign (f.size (), (char)0);
      for (int i = 0; i < f.size (); ++i)
	{
	  s[i] = (char)f[i];
	}
      return s;
    }
    Features toFeatures (const std::basic_string<char> &s) const {
      Features f;
      f.assign (s.size (), (unsigned char)0);
      for (int i = 0; i < s.size (); ++i)
	{
	  f[i] = (unsigned char)s[i];
	}
      return f;
    }
  };

};


#endif // IMAGE_SEARCH_READONLY_IMAGE_H
