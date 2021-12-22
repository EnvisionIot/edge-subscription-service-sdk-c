/**
 * Copyright 2018-present Envision Digital.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created by yang.zhang4 on 2019/10/9.
//

#ifndef EDGE_SERVICE_SDK_C_EDGE_COMMON_H
#define EDGE_SERVICE_SDK_C_EDGE_COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#if _MSC_VER
#if defined(EDGE_SERVICE_C_API_EXPORTS) || defined(edge_service_c_api_EXPORTS)
#define EDGE_SERVICE_C_API_DLL_EXPORT __declspec(dllexport)
#else
#define EDGE_SERVICE_C_API_DLL_EXPORT __declspec(dllimport)
#endif
#define _CRT_SECURE_NO_WARNINGS
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define write _write
#else
#define EDGE_SERVICE_C_API_DLL_EXPORT
#endif
#else
#define EDGE_SERVICE_C_API_DLL_EXPORT
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
//    #include <winsock2.h>
#include <windows.h>
#include <direct.h>
#include <io.h>

#else
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#endif

#include "pthread.h"
#include "curl/curl.h"
#include "cjson/cJSON.h"

//log level
enum EDGE_LOG_LEVEL {
    EDGE_LOG_NOTHING = 0,
    EDGE_LOG_ERROR,
    EDGE_LOG_WARN,
    EDGE_LOG_INFO,
    EDGE_LOG_DEBUG,
    EDGE_LOG_ALL
};

//current log level
//#define EDGE_CURRENT_LOG_LEVEL EDGE_LOG_INFO
//EDGE_SERVICE_C_API_DLL_EXPORT extern volatile char edge_current_log_level;

#define EDGE_MODULE_NAME "EDGE_SERVICE"
#define EDGE_HA_URL "status"
//20201030版本开始，edge端口范围限制在[8000, 9999]，这个参数用来判断edge新老版本
#define EDGE_PORT_FLAG 8000

//创建ctx时，nsq的port<EDGE_PORT_FLAG时，使用EDGE_HA_PORT_OLD，nsq的port>=EDGE_PORT_FLAG时，使用EDGE_HA_PORT
#define EDGE_HA_PORT 8000
#define EDGE_HA_PORT_OLD 58000
#define EDGE_SERVICE_CHECK_URL "ping"

//检查edge消息队列服务是否可用的port，这里定义的这个值不再使用，强制是创建ctx时，nsq的port+1
//#define EDGE_SERVICE_CHECK_PORT 9151
#define EDGE_DATASERVICE_DEFAULT_PORT 9150

struct EDGE_SERVICE_C_API_DLL_EXPORT CommonStr {
    char str[256];
};

struct EDGE_SERVICE_C_API_DLL_EXPORT LogInfoBox {
    char *file_path;
    char *func_name;
    char *module_name;
    int line_num;
    int log_level;
    char *log_str;
};

struct EDGE_SERVICE_C_API_DLL_EXPORT LogConfig {
    //读写锁
    pthread_rwlock_t op_rwlock;
    //当前日志等级
    int current_log_level;
    //当前用户自定义变量
    void *user_ctx;

    //当前日志打印函数
    void (*log_writer)(struct LogInfoBox *log_info, void *user_ctx);
};

struct EDGE_SERVICE_C_API_DLL_EXPORT IPBox {
    int count;
    struct CommonStr *ip_list;
};

struct EDGE_SERVICE_C_API_DLL_EXPORT CommonUrlResponse {
    int data_len;
    int data_capacity;
    char *data;
    CURLcode url_code;
};

struct EDGE_SERVICE_C_API_DLL_EXPORT HAResponse {
    int status;
    char message[128];
};

//一些全局变量-->
//日志配置
EDGE_SERVICE_C_API_DLL_EXPORT extern struct LogConfig *edge_current_log_config;
//<--一些全局变量

/************************************************************
 * name:edge_printf
 * desc:common printf function
 *
 * para:[in] print_arg          reserve
 *      [in] print_level        print_level
 *      [in] format             print format
 *      [in] ...                print parameters
 * return:>=0         success
 *        <0          fail
 * tips:
 ************************************************************/
EDGE_SERVICE_C_API_DLL_EXPORT extern int edge_printf(void *print_arg, int print_level, const char *format, ...);

EDGE_SERVICE_C_API_DLL_EXPORT extern int
edge_log_print(const char *file_path, const char *func_name, const char *module_name, int line_num, int log_level,
               const char *log_str);

EDGE_SERVICE_C_API_DLL_EXPORT extern int
edge_log_printf(const char *file_path, const char *func_name, const char *module_name, int line_num, int log_level,
                const char *format, ...);

/************************************************************
 * name:edge_sleep
 * desc:sleep
 *
 * para:[in] ms       time to sleep(ms)
 * return:>=0         success
 *        <0          fail
 * tips:
 ************************************************************/
EDGE_SERVICE_C_API_DLL_EXPORT extern int edge_sleep(long long int ms);

EDGE_SERVICE_C_API_DLL_EXPORT extern int init_edge_service_ctx();

EDGE_SERVICE_C_API_DLL_EXPORT extern int uninit_edge_service_ctx();

EDGE_SERVICE_C_API_DLL_EXPORT extern struct IPBox *get_current_master_ip_list(struct IPBox *ip_box, int ha_port);

EDGE_SERVICE_C_API_DLL_EXPORT extern struct IPBox *generate_ip_box(int ip_count, ...);

//将ip_box2加入ip_box1，返回ip_box1指针，在ip_box1原来地址上操作
EDGE_SERVICE_C_API_DLL_EXPORT extern struct IPBox *add_ip_box_to_ip_box(struct IPBox *ip_box1, struct IPBox *ip_box2);

//将ip加入ip_box，返回ip_box指针，在ip_box原来地址上操作
EDGE_SERVICE_C_API_DLL_EXPORT extern struct IPBox *add_ip_to_ip_box(struct IPBox *ip_box, char *ip);

EDGE_SERVICE_C_API_DLL_EXPORT extern struct IPBox *deep_copy_ip_box(struct IPBox *ip_box);

EDGE_SERVICE_C_API_DLL_EXPORT extern int delete_ip_box(struct IPBox *ip_box);

EDGE_SERVICE_C_API_DLL_EXPORT extern int check_ip_box_is_equal(struct IPBox *ip_box1, struct IPBox *ip_box2);

EDGE_SERVICE_C_API_DLL_EXPORT extern int delete_common_url_response(struct CommonUrlResponse *common_url_response);

EDGE_SERVICE_C_API_DLL_EXPORT extern int delete_ha_response(struct HAResponse *ha_response);

EDGE_SERVICE_C_API_DLL_EXPORT extern struct LogInfoBox *
generate_log_info_box(const char *file_path, const char *func_name, const char *module_name, int line_num,
                      int log_level, const char *log_str);

EDGE_SERVICE_C_API_DLL_EXPORT extern int delete_log_info_box(struct LogInfoBox *log_info_box);

EDGE_SERVICE_C_API_DLL_EXPORT extern char *deep_copy_string(const char *src);

EDGE_SERVICE_C_API_DLL_EXPORT extern void default_log_writer(struct LogInfoBox *log_info, void *user_ctx);

EDGE_SERVICE_C_API_DLL_EXPORT extern void set_log_level(int log_level);

EDGE_SERVICE_C_API_DLL_EXPORT extern int get_log_level();

EDGE_SERVICE_C_API_DLL_EXPORT extern void
set_log_writer(void (*log_writer)(struct LogInfoBox *log_info, void *user_ctx), void *user_ctx);

//检查ip对应的订阅服务是否可用(<0不可用，>=0可用)
EDGE_SERVICE_C_API_DLL_EXPORT extern int check_service_available(char *ip, int port);

#ifdef __cplusplus
}
#endif

#endif //EDGE_SERVICE_SDK_C_EDGE_COMMON_H
