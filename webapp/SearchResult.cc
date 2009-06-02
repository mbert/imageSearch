#include "SearchResult.h"

#include <Wt/WText>
#include <Wt/WImage>
#include <Wt/WAnchor>
#include <Wt/WContainerWidget>
#include <Wt/WFileResource>

#include <cxxutil/utils.h>

#include <iostream>

using namespace ImageSearch;

SearchResult::SearchResult (Wt::WContainerWidget *parent)
  : Div ("searchResult", parent)
{
  m_imageDiv = new Div ("image", this);
  m_textDiv = new Div ("text", this);
  m_text = new Wt::WText (m_textDiv);
  m_anchor = new Wt::WAnchor ();
  m_similar = new Wt::WAnchor (m_textDiv);
  m_imageDiv->addWidget (m_anchor);
}

SearchResult::~SearchResult (void)
{
}

void
SearchResult::setImage (const std::string &thumbnail,
			const std::string &mimeType,
			const std::string &text,
			const unsigned long id,
			const std::string &anchor)
{
  m_resource = std::auto_ptr<Wt::WResource> (new Wt::WFileResource (mimeType,
								    thumbnail));
  Wt::WString wText (text + "<br/>");
  m_text->setText (wText);
  m_anchor->setRef (anchor);
  Wt::WImage *image = new Wt::WImage ();
  image->setResource (m_resource.get ());
  m_anchor->setImage (image);
  m_similar->setText ("Find similar images...");
  m_similar->setRef ("/?imageQueryId=" + CxxUtil::itoa (id));
  show ();
}

void
SearchResult::resetImage (void)
{
  hide ();
}

