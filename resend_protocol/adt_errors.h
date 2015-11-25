#ifndef ADT_ERRORS_H
#define ADT_ERRORS_H



#ifndef ADT_ERROR_BEGIN_VALUE
#define ADT_ERROR_BEGIN_VALUE	(-1000)
#endif

#pragma pack(1)

#ifdef __cplusplus
enum ADT_ERROR
#else
typedef enum _ADT_ERROR
#endif
{
    ADT_ERROR_BEGIN	= ADT_ERROR_BEGIN_VALUE,
    POOL_NOT_GOOD,
    QUEUE_NOT_GOOD,
    POOL_MALLOC_ERROR,
    POOL_FREE_ERROR,
    POOL_FREE_LOGIC_ERROR,
    ENQUEUE_IS_FULL,
    DEQUEUE_IS_EMPTY,
    ADT_ERROR_END
}
#ifdef __cplusplus
;
#else
ADT_ERROR;
#endif

#pragma pack()

#endif
