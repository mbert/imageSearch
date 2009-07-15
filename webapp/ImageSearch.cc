#include "ImageSearch.h"
#include "Div.h"
#include "SearchResult.h"

#include <Backend.h>

#include <Wt/WBreak>
#include <Wt/WText>
#include <Wt/WImage>
#include <Wt/WAnchor>
#include <Wt/WFileUpload>
#include <Wt/WFileResource>
#include <Wt/WPushButton>
#include <Wt/WEnvironment>

#include <cxxutil/utils.h>

#include <iostream>
#include <fstream>

#define MAX_RESULTS 16

using namespace ImageSearch;

ImageSearchApplication::ImageSearchApplication (const Wt::WEnvironment& env)
  : Wt::WApplication (env)
{

  m_backend = new ImageSearchBackend ("/images/dehdabehs");

  setTitle ("Image Search");

  setupTopSection ();

  setupInputs ();

  setupSearchResults();

  useStyleSheet ("imageSearch.css");

  Wt::WEnvironment::ArgumentMap argumentMap = env.arguments ();
  Wt::WEnvironment::ArgumentValues image = argumentMap["imageQueryId"];
  if (image.size () > 0)
    {
      unsigned long imageId;
      try
	{
	  imageId = CxxUtil::atoi (image[0]);
	  searchByImageId (imageId);
	}
      catch (const std::exception e)
	{
	  std::cout << "Warning: Invalid image query ID "
		    << image[0] << std::endl;
	}
    }
}

void
ImageSearchApplication::setupTopSection (void)
{
  Div *atTop = new Div ("atTop", "atTop", root ());

  Wt::WImage *image = new Wt::WImage ("images/daubechies-d4-wavelet.png");
  image->setStyleClass ("image");
  Wt::WAnchor *a = new Wt::WAnchor ("http://www.wavelet.org", image, atTop);

  Wt::WText *title = new Wt::WText ("Image Search", atTop);
  title->setStyleClass ("title");

  m_currentSelection = new Wt::WImage (atTop);
  m_currentSelection->setStyleClass ("image");
}

void
ImageSearchApplication::setupInputs (void)
{
  Div *topContent = new Div ("topContent", "topContent", root ());
  Div *descriptionDiv = new Div ("searchInputLine", topContent);
  descriptionDiv->addWidget (new Wt::WText ("Enter your image file: "));

  Div *uploadDiv = new Div ("searchInputLine", topContent);
  m_fileUpload = new Wt::WFileUpload (uploadDiv);

  Div *submitDiv = new Div ("searchIputLine", topContent);
  m_searchButton = new Wt::WPushButton ("Send image file.", submitDiv);
  m_searchButton->disable();

  Div *clearDiv = new Div ("clear", topContent);

  m_fileUpload->uploaded.connect
    (SLOT (this, ImageSearchApplication::searchByUpload));
  m_fileUpload->changed.connect
    (SLOT (this, ImageSearchApplication::enableSearchButton));
  m_fileUpload->fileTooLarge.connect
    (SLOT (this, ImageSearchApplication::fileTooLarge));
  m_searchButton->clicked.connect
    (SLOT (this, ImageSearchApplication::uploadFile));

}

void
ImageSearchApplication::setupSearchResults (void)
{
  Div *resultSection = new Div ("resultSection", "resultSection", root ());
  Div *resultTextDiv = new Div ("resultText", "resultText", resultSection);
  m_resultText = new Wt::WText (resultTextDiv);

  for (int i = 0; i < MAX_RESULTS; ++i)
    {
      m_searchResults.push_back (new SearchResult (resultSection));
    }

  Div *footerSection = new Div ("footerSection", "footerSection", root ());
  footerSection->addWidget (new Div ("clear"));
  std::string infoText = "<p>" + m_backend->getDbInfoText () + "</p>"
    + "<p>"  + m_backend->getScoreTableInfoText () + "</p>";
  footerSection->addWidget (new Wt::WText (infoText));
}

void
ImageSearchApplication::updateSearchResults (void)
{
  BlImageConstIterator resIt = m_backend->performSearch ();
  int i = 0;
  for (; i < MAX_RESULTS && m_backend->hasMore (resIt); ++i, ++resIt)
    {
      BLImage image = *resIt;
      std::ifstream ifs (image.getThumbnail().c_str (), std::ifstream::in);
      if (ifs.fail ())
	{
	  std::cerr << "FAILURE " << image.getThumbnail () << std::endl;
	}
      m_searchResults[i]->setImage (image.getThumbnail (),
				    image.getMimeType (),
				    image.getText (),
				    image.getId (),
				    image.getTargetLink ());
    }
  for (; i < MAX_RESULTS; ++i)
    {
      m_searchResults[i]->resetImage ();
    }
}

ImageSearchApplication::~ImageSearchApplication (void)
{
  m_backend->clearCurrentImage ();
  delete m_backend;
}

void
ImageSearchApplication::uploadFile (void)
{
  m_searchButton->disable ();
  m_fileUpload->upload ();
}


void
ImageSearchApplication::searchByUpload (void)
{
  std::string fileName
    = m_backend->setImage (m_fileUpload->spoolFileName (),
			   m_fileUpload->clientFileName().toUTF8 ());

  afterSearch (fileName);
}

void
ImageSearchApplication::searchByImageId (const unsigned long imageId)
{
  std::string fileName = m_backend->setImage (imageId);

  afterSearch (fileName);
}


void
ImageSearchApplication::afterSearch (const std::string &fileName)
{

  if (m_backend->isCurrentImageValid ())
    {
      m_resultText->setStyleClass ("title");
      m_resultText->setText ("Search Results:");
      showCurrentSearch (fileName);
    }
  else
    {
      m_resultText->setStyleClass ("error");
      m_resultText->setText ("Invalid file.");
      hideCurrentSearch ();
    }
  updateSearchResults ();
}


void
ImageSearchApplication::fileTooLarge (void)
{
  m_resultText->setStyleClass ("error");
  m_resultText->setText ("Image file too large.");
  hideCurrentSearch ();
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
  std::string thumbName = ImageSearchBackend::thumbName (fileName);
  m_currentSearchResource =
    std::auto_ptr<Wt::WFileResource> (new Wt::WFileResource (mimeType,
							     thumbName));
  m_currentSelection->setResource (m_currentSearchResource.get ());
  m_currentSelection->show ();
}

void
ImageSearchApplication::hideCurrentSearch (void)
{
  m_currentSelection->hide ();
}

