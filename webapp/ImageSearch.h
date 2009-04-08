#ifndef IMAGE_SEARCH_APPLICATION_H
#define IMAGE_SEARCH_APPLICATION_H

#include <Wt/WApplication>
#include <string>

namespace Wt
{
  class WFileUpload;
  class WPushButton;
  class WImage;
};

namespace ImageSearch
{

  class ImageSearchBackend;

  class ImageSearchApplication : public Wt::WApplication
    {
    public:
      ImageSearchApplication (const Wt::WEnvironment& env);
      virtual ~ImageSearchApplication (void);

    private:
      Wt::WFileUpload *m_fileUpload;
      Wt::WPushButton *m_searchButton;
      Wt::WImage *m_currentSelection;
      ImageSearchBackend *m_backend;

      void uploadFile (void);
      void search (void);
      void enableSearchButton (void);
      void showCurrentSearch (const std::string &fileName);
      void hideCurrentSearch (void);
    };

};



#endif // IMAGE_SEARCH_APPLICATION_H
