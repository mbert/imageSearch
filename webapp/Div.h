#ifndef DIV_H
#define DIV_H

#include <Wt/WContainerWidget>
#include <string>

/* Shortcut for a <div id=""> */
class Div : public Wt::WContainerWidget
{
public:
  Div (const std::string& id, Wt::WContainerWidget *parent);
  Div (Wt::WContainerWidget *parent);
  virtual ~Div (void);
};

#endif // DIV_H
