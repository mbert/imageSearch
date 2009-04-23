#include "SearchResult.h"

#include <Wt/WText>
#include <Wt/WImage>
#include <Wt/WAnchor>
#include <Wt/WContainerWidget>
#include <Wt/WFileResource>


using namespace ImageSearch;

SearchResult::SearchResult (Wt::WContainerWidget *parent)
  : Div ("searchResult", parent)
{
  m_imageDiv = new Div ("image", this);
  m_image = new Wt::WImage (m_imageDiv);
  m_textDiv = new Div ("text", this);
  m_text = new Wt::WText (m_textDiv);
}

SearchResult::~SearchResult (void)
{
}

void
SearchResult::setImage (const std::string &thumbnail,
			const std::string &mimeType,
			const std::string &text,
			const std::string &anchor)
{
  m_image->setResource(new Wt::WFileResource (mimeType, thumbnail));
  m_text->setText (text);
  Wt::WAnchor *link = new Wt::WAnchor (anchor, m_image, m_imageDiv);
  show ();
}

void
SearchResult::resetImage (void)
{
  hide ();
}

