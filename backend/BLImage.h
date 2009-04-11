#ifndef IMAGE_SEARCH_IMAGE_H
#define IMAGE_SEARCH_IMAGE_H

#include <string>

namespace ImageSearch
{

  class Image {
  public:
    Image (const std::string &thumbnail, const std::string &mimeType,
	   const std::string &text, const std::string &targetLink)
      : m_thumbnail (thumbnail), m_mimeType (mimeType),
	m_text (text), m_targetLink (targetLink) { }

    inline const std::string & getThumbnail const (void) {
      return m_thumbnail;
    }
    inline const std::string & getMimeType const (void) {
      return m_mimeType;
    }
    inline const std::string & getText const (void) {
      return m_text;
    }
    inline const std::string & getTargetLink const (void) {
      return m_targetLink;
    }

  private:
    std::string m_thumbnail;
    std::string m_mimeType;
    std::string m_text;
    std::string m_targetLink;
};

#endif // IMAGE_SEARCH_IMAGE_H
