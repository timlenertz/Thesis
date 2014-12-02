#ifndef PCF_ALIGNMENT_H_
#define PCF_ALIGNMENT_H_

#ifdef _MSC_VER
	#define ALIGNAS(n) __declspec( align(n) )
	#define ALIGNOF(t) __alignof(t)
#else
	#define ALIGNAS(n) alignas(n)
	#define ALIGNOF(t) alignof(t)
#endif

#endif