

#ifndef _TLPLAT_TYPES_H_
#define _TLPLAT_TYPES_H_

#ifdef	__cplusplus
extern "C"{
#endif

typedef char s8;
typedef unsigned char u8;

typedef short s16;
typedef unsigned short u16;

typedef int s32;
typedef unsigned int u32;

#if (_CONPLAT_SZLONG == 64)

typedef long s64;
typedef unsigned long u64;

#else

typedef long long s64;
typedef unsigned long long u64;

#endif 

#ifdef	__cplusplus
}
#endif	/* end of __cplusplus */

#endif /* end of _TLPLAT_TYPES_H_ */
