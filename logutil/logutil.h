/**
 * @file logutil.h
 * @brief A Log util
 * @details Print log with multiple level.
 * Need to define LOG_LEVEL
 * 
 * @author Pham Ngoc Thang (thangdc94)
 * @bug No known bug
 */
#ifndef __LOG_UTIL_H
#define __LOG_UTIL_H

#include <stdio.h>

/** No Log */
#define LOG_NONE     (0)
/** Log fatal error */
#define LOG_FATAL    (1)
/** Log error */
#define LOG_ERR      (2)
/** Log Warning */
#define LOG_WARN     (3)
/** Log information */
#define LOG_INFO     (4)
/** Log debug */
#define LOG_DBG      (5)

/** Log util function */
#define LOG(level, ...) do {  \
                            if (level <= LOG_LEVEL) { \
                                fprintf(stdout,"%s:%d:", __FILE__, __LINE__); \
                                fprintf(stdout, __VA_ARGS__); \
                                fprintf(stdout, "\n"); \
                                fflush(stdout); \
                            } \
                        } while (0)

#endif //__LOG_UTIL_H
