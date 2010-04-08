#include "ImageSearch.h"
#include "Div.h"
#include "SearchResult.h"
#include "../config.h"

#include <SerializationBasedImageSearchBackend.h>

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

using namespace ImageSearch;

ImageSearchApplication::ImageSearchApplication (const Wt::WEnvironment& env)
  : Wt::WApplication (env)
{

  m_backend = new SerializationBasedImageSearchBackend (DB_PREFIX, DB_FILE);

  setTitle ("Image Search");

  p_setupTopSection ();

  p_setupInputs ();

  p_setupSearchResults();

  useStyleSheet ("imageSearch.css");

  Wt::Http::ParameterMap argumentMap = env.getParameterMap ();
  Wt::Http::ParameterValues image = argumentMap["imageQueryId"];
  if (image.size () > 0)
    {
      unsigned long imageId;
      try
	{
	  imageId = CxxUtil::atoi (image[0]);
	  p_searchByImageId (imageId);
	}
      catch (const std::exception e)
	{
	  std::cout << "Warning: Invalid image query ID "
		    << image[0] << std::endl;
	}
    }
}

void
ImageSearchApplication::p_setupTopSection (void)
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
ImageSearchApplication::p_setupInputs (void)
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

  m_fileUpload->uploaded().connect
    (SLOT (this, ImageSearchApplication::p_searchByUpload));
  m_fileUpload->changed().connect
    (SLOT (this, ImageSearchApplication::p_enableSearchButton));
  m_fileUpload->fileTooLarge().connect
    (SLOT (this, ImageSearchApplication::p_fileTooLarge));
  m_searchButton->clicked().connect
    (SLOT (this, ImageSearchApplication::p_uploadFile));

}

void
ImageSearchApplication::p_setupSearchResults (void)
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
ImageSearchApplication::p_updateSearchResults (void)
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
ImageSearchApplication::p_uploadFile (void)
{
  m_searchButton->disable ();
  m_fileUpload->upload ();
}


void
ImageSearchApplication::p_searchByUpload (void)
{
  std::string fileName
    = m_backend->setImage (m_fileUpload->spoolFileName (),
			   m_fileUpload->clientFileName().toUTF8 ());

  p_afterSearch (fileName);
}

void
ImageSearchApplication::p_searchByImageId (const unsigned long imageId)
{
  std::string fileName = m_backend->setImage (imageId);

  p_afterSearch (fileName);
}


void
ImageSearchApplication::p_afterSearch (const std::string &fileName)
{

  if (m_backend->isCurrentImageValid ())
    {
      m_resultText->setStyleClass ("title");
      m_resultText->setText ("Search Results:");
      p_showCurrentSearch (fileName);
    }
  else
    {
      m_resultText->setStyleClass ("error");
      m_resultText->setText ("Invalid file.");
      p_hideCurrentSearch ();
    }
  p_updateSearchResults ();
}


void
ImageSearchApplication::p_fileTooLarge (void)
{
  m_resultText->setStyleClass ("error");
  m_resultText->setText ("Image file too large.");
  p_hideCurrentSearch ();
}


void
ImageSearchApplication::p_enableSearchButton (void)
{
  if (!m_searchButton->isEnabled ())
    {
      m_searchButton->enable ();
    }
}

void
ImageSearchApplication::p_showCurrentSearch (const std::string &fileName)
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
ImageSearchApplication::p_hideCurrentSearch (void)
{
  m_currentSelection->hide ();
}

