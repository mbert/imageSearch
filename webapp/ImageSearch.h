#ifndef IMAGE_SEARCH_APPLICATION_H
#define IMAGE_SEARCH_APPLICATION_H

#include <Wt/WApplication>
#include <string>
#include <vector>
#include <memory>

namespace Wt
{
  class WFileUpload;
  class WPushButton;
  class WImage;
  class WText;
  class WResource;
};

namespace ImageSearch
{

  class ImageSearchBackend;
  class SearchResult;

  class ImageSearchApplication : public Wt::WApplication
    {
    public:
      ImageSearchApplication (const Wt::WEnvironment& env);
      virtual ~ImageSearchApplication (void);

    private:
      Wt::WFileUpload *m_fileUpload;
      Wt::WPushButton *m_searchButton;
      Wt::WImage *m_currentSelection;
      Wt::WText *m_resultText;
      ImageSearchBackend *m_backend;
      std::vector<SearchResult*> m_searchResults;
      std::auto_ptr<Wt::WResource> m_currentSearchResource;

      void p_uploadFile (void);
      void p_afterSearch (const std::string &fileName);
      void p_searchByUpload (void);
      void p_searchByImageId (const unsigned long imageId);
      void p_fileTooLarge (void);
      void p_enableSearchButton (void);
      void p_showCurrentSearch (const std::string &fileName);
      void p_hideCurrentSearch (void);
      void p_setupTopSection (void);
      void p_setupInputs (void);
      void p_setupSearchResults (void);
      void p_updateSearchResults (void);
    };

};



#endif // IMAGE_SEARCH_APPLICATION_H
