#include "ImageSearch.h"

static Wt::WApplication *
createApplication(const Wt::WEnvironment& env)
{
  return new ImageSearch::ImageSearchApplication(env);
}

int
main(int argc, char **argv)
{
  return Wt::WRun(argc, argv, &createApplication);
}

