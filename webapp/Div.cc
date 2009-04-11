#include "Div.h"

using namespace ImageSearch;

Div::Div(const std::string& cssClass, Wt::WContainerWidget *parent)
  : WContainerWidget (parent)
{
  setStyleClass (cssClass);
}

Div::Div(const std::string& id, const std::string &cssClass,
	 Wt::WContainerWidget *parent)
  : WContainerWidget (parent)
{
  setId(id);
  setStyleClass (cssClass);
}

Div::Div(Wt::WContainerWidget *parent)
  : WContainerWidget (parent)
{
}

Div::~Div()
{
}
