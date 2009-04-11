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
  Div *imageDiv = new Div ("image", this);
  m_image = new Wt::WImage (imageDiv);
  Div *textDiv = new Div ("text", this);
  m_text = new Wt::WText (textDiv);
  m_anchor = new Wt::WAnchor (this);
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
  m_anchor->setRef (anchor);
  show ();
}

void
SearchResult::resetImage (void)
{
  hide ();
}

