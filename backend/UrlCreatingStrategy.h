#ifndef IMAGE_SEARCH_URL_PREFIX_STRATEGY_H
#define IMAGE_SEARCH_URL_PREFIX_STRATEGY_H

#include <string>

namespace ImageSearch
{

  class UrlPrefixStrategy {
  public:
    std::string makePrefix (const std::string &fileName) { return doMakePrefix (fileName); }
  private:
    virtual std::string doMakePrefix (const std::string &fileName) = 0;
  };

};

#endif // IMAGE_SEARCH_URL_PREFIX_STRATEGY_H
