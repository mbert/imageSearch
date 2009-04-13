#ifndef IMAGE_SEARCH_BLIMAGE_H
#define IMAGE_SEARCH_BLIMAGE_H

#include <string>
#include <vector>

namespace ImageSearch
{

  class BLImage {
  public:
    BLImage (const std::string &thumbnail, const std::string &mimeType,
	     const std::string &text, const std::string &targetLink)
      : m_thumbnail (thumbnail), m_mimeType (mimeType),
      m_text (text), m_targetLink (targetLink) { }

    virtual ~BLImage (void) {}

    inline const std::string & getThumbnail (void) const {
      return m_thumbnail;
    }
    inline const std::string & getMimeType (void) const {
      return m_mimeType;
    }
    inline const std::string & getText (void) const {
      return m_text;
    }
    inline const std::string & getTargetLink (void) const {
      return m_targetLink;
    }

  private:
    std::string m_thumbnail;
    std::string m_mimeType;
    std::string m_text;
    std::string m_targetLink;
  };

  typedef std::vector<BLImage> BlImageList;
  typedef BlImageList::const_iterator BlImageIterator;
};


#endif // IMAGE_SEARCH_BLIMAGE_H
