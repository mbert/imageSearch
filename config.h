#ifndef IMAGE_SEARCH_CONFIG_H
#define IMAGE_SEARCH_CONFIG_H

#define MAX_WEIGHT_IDX 5
#define MY_WEIGHTS

#define MAX_RESULTS 16

#define THUMB_ROWS 250
#define THUMB_COLS 250

#define DB_PREFIX "/images/shirts"
//#define DB_FILE   "../imagedb.txt"
//#define SERIALIZER_CLASS TextScoreTableSerializer
#define DB_FILE   "../imagedb.bin"
#define SERIALIZER_CLASS BinaryScoreTableSerializer

#define DB_IMAGE_COLS 128
#define DB_IMAGE_ROWS 128

#define KEPT_COEFFS 40

#define DB_NAME "images"
#define TABLE_NAME "images"
#define HOSTADDR "127.0.0.1"
#define USERNAME "md"
#define PASSWORD "md"

#endif // IMAGE_SEARCH_CONFIG_H
