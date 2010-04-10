#ifndef IMAGE_SEARCH_URL_PREFIX_STRATEGY_H
#define IMAGE_SEARCH_URL_PREFIX_STRATEGY_H

#include <string>

namespace ImageSearch
{

  class UrlPrefixStrategy {
  public:
    std::string makePrefix (const std::string &fileName) const { return doMakePrefix (fileName); }
  private:
    virtual std::string doMakePrefix (const std::string &fileName) const = 0;
  };

  class FlatUrlPrefixStrategy : public UrlPrefixStrategy {
  public:
    FlatUrlPrefixStrategy (void) { }
    virtual ~FlatUrlPrefixStrategy (void) { }
  private:
    virtual std::string doMakePrefix (const std::string &fileName) const {
	return "";
      }
  };

  class CoverScanUrlPrefixStrategy : public UrlPrefixStrategy {
  public:
    CoverScanUrlPrefixStrategy (void) { }
    virtual ~CoverScanUrlPrefixStrategy (void) { }
  private:
    virtual std::string doMakePrefix (const std::string &fileName) const;
  };

};

#endif // IMAGE_SEARCH_URL_PREFIX_STRATEGY_H
