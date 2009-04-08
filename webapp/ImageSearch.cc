#include "Div.h"
#include "ImageSearch.h"

#include <Backend.h>

#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WImage>
#include <Wt/WFileUpload>
#include <Wt/WFileResource>
#include <Wt/WPushButton>

#include <iostream>

#define ROWS 250
#define COLS 250

using namespace ImageSearch;

/*
 * The env argument contains information about the new session, and
 * the initial request. It must be passed to the WApplication
 * constructor so it is typically also an argument for your custom
 * application constructor.
*/
ImageSearchApplication::ImageSearchApplication (const Wt::WEnvironment& env)
  : Wt::WApplication (env)
{

  m_backend = new ImageSearchBackend (ROWS, COLS);

  setTitle ("Image Search");

  Div *atTop = new Div ("atTop", root ());
  atTop->setStyleClass ("atTop");

  Wt::WImage *image = new Wt::WImage ("images/daubechies-d4-wavelet.png",
				      atTop);
  image->setStyleClass ("image");

  Wt::WText *title = new Wt::WText ("Image Search", atTop);
  title->setStyleClass ("title");

  m_currentSelection = new Wt::WImage (atTop);
  m_currentSelection->setStyleClass ("image");

  Div *topContent = new Div ("topContent", root ());
  topContent->setStyleClass ("topContent");
  Div *descriptionDiv = new Div (topContent);
  descriptionDiv->setStyleClass ("searchInputLine");
  descriptionDiv->addWidget (new Wt::WText ("Enter your image file: "));

  Div *uploadDiv = new Div (topContent);
  uploadDiv->setStyleClass ("searchInputLine");
  m_fileUpload = new Wt::WFileUpload (uploadDiv);

  Div *submitDiv = new Div (topContent);
  submitDiv->setStyleClass ("searchInputLine");
  m_searchButton = new Wt::WPushButton ("Send image file.", submitDiv);
  m_searchButton->disable();

  Div *clearDiv = new Div (topContent);
  clearDiv->setStyleClass ("clear");

  useStyleSheet ("imageSearch.css");

  m_fileUpload->uploaded.connect (SLOT (this, ImageSearchApplication::search));
  m_fileUpload->changed.connect (SLOT (this, ImageSearchApplication::enableSearchButton));
  m_searchButton->clicked.connect (SLOT (this, ImageSearchApplication::uploadFile));

}

ImageSearchApplication::~ImageSearchApplication (void)
{
  m_backend->clearCurrentImage ();
}

void
ImageSearchApplication::uploadFile (void)
{
  m_searchButton->disable ();
  m_fileUpload->upload ();
}


void
ImageSearchApplication::search (void)
{
  std::string fileName = m_backend->setImage (m_fileUpload->spoolFileName (),
					      m_fileUpload->clientFileName().toUTF8 ());
  if (m_backend->isCurrentImageValid ())
    {
      showCurrentSearch (fileName);
    }
  else
    {
      hideCurrentSearch ();
    }
}


void
ImageSearchApplication::enableSearchButton (void)
{
  if (!m_searchButton->isEnabled ())
    {
      m_searchButton->enable ();
    }
}

void
ImageSearchApplication::showCurrentSearch (const std::string &fileName)
{
  std::string mimeType = m_backend->guessMimeType ();
  m_currentSelection->setResource (new Wt::WFileResource (mimeType, fileName));
  m_currentSelection->show ();
}

void
ImageSearchApplication::hideCurrentSearch (void)
{
  m_currentSelection->hide ();
}

