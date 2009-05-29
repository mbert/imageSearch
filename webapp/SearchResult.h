#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include "Div.h"

#include <string>
#include <memory>

namespace Wt
{
  class WContainerWidget;
  class WImage;
  class WText;
  class WAnchor;
  class WResource;
};

namespace ImageSearch
{
  class SearchResult : public Div
  {
  public:
    SearchResult (Wt::WContainerWidget *parent);
    virtual ~SearchResult (void);
    void setImage (const std::string &thumbnail,
		   const std::string &mimeType,
		   const std::string &text,
		   const std::string &anchor);
    void resetImage (void);
  private:
    Div *m_imageDiv;
    Div *m_textDiv;
    Wt::WText *m_text;
    std::auto_ptr<Wt::WResource> m_resource;
  };
};

#endif // SEARCH_RESULT_H
