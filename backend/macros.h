#ifndef IMAGE_SEARCH_BITS_H
#define IMAGE_SEARCH_BITS_H

namespace ImageSearch
{

#define SINGLE_BIT_AT(i) (unsigned char)(((unsigned char)1) << (7 - i))
#define IS_BIT_SET(b, i) ((b | SINGLE_BIT_AT (i)) != 0)
#define SET_BIT(b, i) ((b) |= SINGLE_BIT_AT (i))
#define RESET_BIT(b, i) ((b) &= ~SINGLE_BIT_AT (i))

#define MIN(x,y) ((x) < (y)? (x): (y))
#define MAX(x,y) ((x) > (y)? (x): (y))

};

#endif // IMAGE_SEARCH_BITS_H
