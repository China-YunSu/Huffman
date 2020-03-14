#ifndef _BOOLEAN_H_
#define _BOOLEAN_H_

#define  TRUE 	1
#define  FALSE	0



#define SET(v, i) (v |= (1 << ((i) ^ 7)))
#define CLR(v, i) (v &= ~(1 << ((i) ^ 7)))
#define GET(v, i) (((v) & (1 << ((i) ^ 7))) != 0)

typedef unsigned char boolean;
typedef boolean u8;



#endif


