#include "SearchResult.h"
#include "Div.h"

#include <Wt/WText>
#include <Wt/WImage>
#include <Wt/WAnchor>
#include <Wt/WContainerWidget>
#include <Wt/WFileResource>


using namespace ImageSearch;

SearchResult::SearchResult (Wt::WContainerWidget *parent)
{
  m_container = new Div ("searchResult", parent);
  m_image = new Wt::WImage (m_container);
  m_text = new Wt::WText (m_container);
  m_anchor = new Wt::WAnchor (m_container);
  resetImage ();
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
  m_container->show ();
}

void
SearchResult::resetImage (void)
{
  m_container->hide ();
}

