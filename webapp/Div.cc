#include "Div.h"

Div::Div(const std::string& id, Wt::WContainerWidget *parent)
  : WContainerWidget (parent)
{
  setId(id);
}

Div::Div(Wt::WContainerWidget *parent)
  : WContainerWidget (parent)
{
}

Div::~Div()
{
}
