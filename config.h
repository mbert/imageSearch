#ifndef IMAGE_SEARCH_CONFIG_H
#define IMAGE_SEARCH_CONFIG_H

#define MAX_WEIGHT_IDX 5
#define MY_WEIGHTS

#define MAX_RESULTS 16

#define THUMB_ROWS 250
#define THUMB_COLS 250

//#define CONFIG_DEHDABEHS
#define CONFIG_SHIRTS
//#define CONFIG_COVERS

#ifdef CONFIG_COVERS
#define DB_PREFIX "/images/covers"
#define DB_FILE   "../covers.bin"
#define URL_PREFIX_STRATEGY_CLASS CoverScanUrlPrefixStrategy
#endif

#ifdef CONFIG_SHIRTS
#define DB_PREFIX "/images/shirts"
#define DB_FILE   "../shirts.bin"
#define URL_PREFIX_STRATEGY_CLASS FlatUrlPrefixStrategy
#endif

#ifdef CONFIG_DEHDABEHS
#define DB_PREFIX "/images/dehdabehs"
#define DB_FILE   "../imagedb.bin"
#define URL_PREFIX_STRATEGY_CLASS FlatUrlPrefixStrategy
#endif

#define SERIALIZER_CLASS BinaryScoreTableSerializer

#define AVG_DIVIDEND 1
//#define AVG_DIVIDEND 10
#define DECOMP_TYPE STANDARD
//#define DECOMP_TYPE PYRAMID

#define DB_IMAGE_COLS 128
#define DB_IMAGE_ROWS 128

#define KEPT_COEFFS 40

#define DB_NAME "images"
#define TABLE_NAME "images"
#define HOSTADDR "127.0.0.1"
#define USERNAME "md"
#define PASSWORD "md"

#endif // IMAGE_SEARCH_CONFIG_H
