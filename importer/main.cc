/*
 * Copyright (C) 2006 Wim Dumon, Koen Deforche
 *
 * See the LICENSE file for terms of use.
 */

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

