#ifndef DIV_H
#define DIV_H

#include <Wt/WContainerWidget>

#include <string>

namespace ImageSearch
{
  /* Shortcut for a <div id=""> */
  class Div : public Wt::WContainerWidget
    {
    public:
      Div (const std::string &cssClass);
      Div (const std::string& cssClass, Wt::WContainerWidget *parent);
      Div (const std::string& id, const std::string &cssClass,
	   Wt::WContainerWidget *parent);
      Div (Wt::WContainerWidget *parent);
      virtual ~Div (void);
    };
};

#endif // DIV_H
