#ifndef __TCPAL_TYPES_H__
#define __TCPAL_TYPES_H__

#ifdef __cplusplus
extern    "C"
{
#endif

#ifdef __cplusplus
#ifdef NULL
#undef NULL
#endif
#define NULL 0
#define TCBB_FUNC extern "C"
#else
#ifdef NULL
#undef NULL
#endif
#define NULL (void*)0
#define TCBB_FUNC
#endif

typedef unsigned char I08U;	/*        */
typedef signed char I08S;	/*        */
typedef unsigned short I16U;	/*         */
typedef signed short I16S;	/*         */
typedef unsigned long I32U;	/*         */
typedef signed long I32S;	/*         */
typedef signed long long I64S;	/*         */
typedef unsigned long long I64U;	/*         */

typedef I64U TcpalTime_t;
typedef I32U TcpalSemaphore_t;
typedef I32U TcpalHandle_t;

#ifdef __cplusplus
	};
#endif

#endif
