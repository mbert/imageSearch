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

      void uploadFile (void);
      void afterSearch (const std::string &fileName);
      void searchByUpload (void);
      void searchByImageId (const unsigned long imageId);
      void fileTooLarge (void);
      void enableSearchButton (void);
      void showCurrentSearch (const std::string &fileName);
      void hideCurrentSearch (void);
      void setupTopSection (void);
      void setupInputs (void);
      void setupSearchResults (void);
      void updateSearchResults (void);
    };

};



#endif // IMAGE_SEARCH_APPLICATION_H
