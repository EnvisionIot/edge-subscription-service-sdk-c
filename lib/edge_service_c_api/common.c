/**
 *
 * Copyright 2018-present Envision Digital.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

//
// Created by yang.zhang4 on 2019/10/9.
//

#include "edge_service_c_api/edge_common.h"

struct LogConfig *edge_current_log_config = NULL;

//libcurl需要的http请求响应的回调函数
static size_t url_response_writedata_callback(void *buffer, size_t size, size_t nmemb, void *user_ctx) {
    int current_len = size * nmemb;
    if (current_len <= 0) {
        return 0;
    }

    struct CommonUrlResponse *common_url_response = (struct CommonUrlResponse *) user_ctx;
    if (common_url_response == NULL) {
        return (size_t) current_len;
    }

    int pre_data_len = common_url_response->data_len;
    int new_data_len = pre_data_len + current_len;
    int pre_data_capacity = common_url_response->data_capacity;
    int new_data_capacity = new_data_len + 1;
    char *data_temp = (char *) realloc(common_url_response->data, new_data_capacity);
    if (data_temp == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "data_temp realloc error, new_data_capacity=%d", new_data_capacity);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return (size_t) current_len;
    }
    common_url_response->data = data_temp;
    common_url_response->data_len = new_data_len;
    common_url_response->data_capacity = new_data_capacity;

    memset(&(common_url_response->data[pre_data_capacity]), 0, new_data_capacity - pre_data_capacity);
    memcpy(&(common_url_response->data[pre_data_len]), buffer, current_len);
    return (size_t) current_len;
}

//将ha接口返回的json格式数据转为数据结构
static int transform_ha_response_to_struct(char *response_str, struct HAResponse **ha_response) {
    if (response_str == NULL || ha_response == NULL) {
        return -1;
    }

    struct HAResponse *ha_response_temp = (struct HAResponse *) malloc(sizeof(struct HAResponse));
    if (ha_response_temp == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ha_response_temp malloc error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }
    memset(ha_response_temp, 0, sizeof(struct HAResponse));

    cJSON *item = NULL;
    int len_temp = 0;
    char *char_temp = NULL;
    int int_temp = 0;
    cJSON *root = cJSON_Parse(response_str);
    if (root == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "cJSON_Parse error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        delete_ha_response(ha_response_temp);
        return -1;
    }

    item = cJSON_GetObjectItem(root, "status");
    if (item != NULL) {
        int_temp = 0;
        int_temp = item->valueint;
        ha_response_temp->status = int_temp;
    }

    item = cJSON_GetObjectItem(root, "message");
    if (item != NULL) {
        len_temp = sizeof(ha_response_temp->message);
        char_temp = (char *) (ha_response_temp->message);
        if (item->valuestring != NULL) {
            snprintf(char_temp, len_temp, "%s", (char *) (item->valuestring));
        }
    }

    cJSON_Delete(root);

    *ha_response = ha_response_temp;

    return 0;
}

extern int edge_printf(void *print_arg, int print_level, const char *format, ...) {
    int current_log_level = get_log_level();

    if (print_level > current_log_level) {
        return -1;
    }

    char sprint_buf[1024];
    va_list args;
    int n;
    va_start(args, format);
    n = vsnprintf(sprint_buf, sizeof(sprint_buf) - 1, format, args);
    va_end(args);

    if (n < 0) {
        return -1;
    } else if (n > (int) (sizeof(sprint_buf))) {
        int buf_max = n;
        char *sprint_buf_temp = (char *) malloc(buf_max);
        if (sprint_buf_temp == NULL) {
            return -1;
        }
        va_start(args, format);
        n = vsnprintf(sprint_buf_temp, buf_max, format, args);
        va_end(args);

        if ((n < 0) || (n > buf_max)) {
            free(sprint_buf_temp);
            return -1;
        }

        write(STDOUT_FILENO, sprint_buf_temp, n);
        free(sprint_buf_temp);
        return n;
    } else {
        write(STDOUT_FILENO, sprint_buf, n);
        return n;
    }
}

extern int edge_log_print(const char *file_path, const char *func_name, const char *module_name, int line_num, int log_level, const char *log_str) {
    if (edge_current_log_config == NULL) {
        return -1;
    }

    int current_log_level = get_log_level();

    if (log_level > current_log_level) {
        return -1;
    }

    struct LogInfoBox *log_info_box = generate_log_info_box(file_path, func_name, module_name, line_num, log_level, log_str);
    edge_current_log_config->log_writer(log_info_box, edge_current_log_config->user_ctx);

    return 0;
}

extern int edge_log_printf(const char *file_path, const char *func_name, const char *module_name, int line_num, int log_level, const char *format, ...) {
    if (edge_current_log_config == NULL) {
        return -1;
    }

    int current_log_level = get_log_level();

    if (log_level > current_log_level) {
        return -1;
    }

    char sprint_buf[1024];
    memset(sprint_buf, 0, sizeof(sprint_buf));
    va_list args;
    int n;
    va_start(args, format);
    n = vsnprintf(sprint_buf, sizeof(sprint_buf) - 1, format, args);
    va_end(args);

    if (n < 0) {
        return -1;
    } else if (n >= (int) (sizeof(sprint_buf))) {
        int buf_max = n + 1;
        char *sprint_buf_temp = (char *) malloc(buf_max);
        if (sprint_buf_temp == NULL) {
            return -1;
        }
        memset(sprint_buf_temp, 0, buf_max);
        va_start(args, format);
        n = vsnprintf(sprint_buf_temp, buf_max - 1, format, args);
        va_end(args);

        if ((n < 0) || (n > buf_max - 1)) {
            free(sprint_buf_temp);
            return -1;
        }

        //write(STDOUT_FILENO, sprint_buf_temp, n);
        struct LogInfoBox *log_info_box = generate_log_info_box(file_path, func_name, module_name, line_num, log_level, sprint_buf_temp);
        edge_current_log_config->log_writer(log_info_box, edge_current_log_config->user_ctx);
        free(sprint_buf_temp);
        return n;
    } else {
        //write(STDOUT_FILENO, sprint_buf, n);
        struct LogInfoBox *log_info_box = generate_log_info_box(file_path, func_name, module_name, line_num, log_level, sprint_buf);
        edge_current_log_config->log_writer(log_info_box, edge_current_log_config->user_ctx);
        return n;
    }
}

extern int edge_sleep(long long int ms) {
    long ms_long = (long) ms;
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    Sleep(ms_long);
#else
    usleep(ms_long * 1000);
#endif
    return 0;
}

extern int init_edge_service_ctx() {
    edge_current_log_config = (struct LogConfig *) malloc(sizeof(struct LogConfig));
    if (edge_current_log_config == NULL) {
        return -1;
    }
    memset(edge_current_log_config, 0, sizeof(struct LogConfig));

    pthread_rwlock_init(&(edge_current_log_config->op_rwlock), NULL);
    edge_current_log_config->current_log_level = EDGE_LOG_INFO;
    edge_current_log_config->log_writer = default_log_writer;
    edge_current_log_config->user_ctx = NULL;

    return 0;
}

extern int uninit_edge_service_ctx() {
    if (edge_current_log_config == NULL) {
        return 0;
    }
    pthread_rwlock_destroy(&(edge_current_log_config->op_rwlock));
    memset(edge_current_log_config, 0, sizeof(struct LogConfig));

    free(edge_current_log_config);
    edge_current_log_config = NULL;

    return 0;
}

extern struct IPBox *generate_ip_box(int ip_count, ...) {
    if (ip_count <= 0) {
        return NULL;
    }

    struct IPBox *ip_box = (struct IPBox *) malloc(sizeof(struct IPBox));
    if (ip_box == NULL) {
        return NULL;
    }
    memset(ip_box, 0, sizeof(struct IPBox));

    ip_box->ip_list = (struct CommonStr *) malloc(ip_count * sizeof(struct CommonStr));
    if (ip_box->ip_list == NULL) {
        delete_ip_box(ip_box);
        return NULL;
    }
    memset(ip_box->ip_list, 0, ip_count * sizeof(struct CommonStr));

    int ii = 0;
    va_list args;
    va_start(args, ip_count);
    char *ip_ptr = NULL;
    int element_count = 0;
    for (ii = 0; ii < ip_count; ii++) {
        ip_ptr = va_arg(args, char *);

        if (ip_ptr == NULL) {
            delete_ip_box(ip_box);
            va_end(args);
            return NULL;
        }

        snprintf(ip_box->ip_list[element_count].str, sizeof(ip_box->ip_list[element_count].str), "%s", ip_ptr);
        element_count++;
    }
    va_end(args);

    ip_box->count = element_count;

    return ip_box;
}

//将ip_box2加入ip_box1，返回ip_box1指针，在ip_box1原来地址上操作
extern struct IPBox *add_ip_box_to_ip_box(struct IPBox *ip_box1, struct IPBox *ip_box2) {
    if (ip_box2 == NULL || ip_box2->count <= 0) {
        return ip_box1;
    }

    if (ip_box1 == NULL) {
        ip_box1 = deep_copy_ip_box(ip_box2);
        return ip_box1;
    }

    int old_count = ip_box1->count;
    int new_count = ip_box1->count + ip_box2->count;
    int old_size = old_count * sizeof(struct CommonStr);
    int new_size = new_count * sizeof(struct CommonStr);

    struct CommonStr *ip_list_temp = (struct CommonStr *) realloc(ip_box1->ip_list, new_size);
    if (ip_list_temp == NULL) {
        return ip_box1;
    }
    ip_box1->ip_list = ip_list_temp;
    memset(&(ip_box1->ip_list[old_count]), 0, new_size - old_size);
    memcpy(&(ip_box1->ip_list[old_count]), ip_box2->ip_list, new_size - old_size);
    ip_box1->count = new_count;

    return ip_box1;
}

//将ip加入ip_box，返回ip_box指针，在ip_box原来地址上操作
extern struct IPBox *add_ip_to_ip_box(struct IPBox *ip_box, char *ip) {
    if (ip == NULL) {
        return ip_box;
    }

    if (ip_box == NULL) {
        ip_box = generate_ip_box(1, ip);
        return ip_box;
    }

    int old_count = ip_box->count;
    int new_count = ip_box->count + 1;
    int old_size = old_count * sizeof(struct CommonStr);
    int new_size = new_count * sizeof(struct CommonStr);

    struct CommonStr *ip_list_temp = (struct CommonStr *) realloc(ip_box->ip_list, new_size);
    if (ip_list_temp == NULL) {
        return ip_box;
    }
    ip_box->ip_list = ip_list_temp;
    memset(&(ip_box->ip_list[old_count]), 0, new_size - old_size);
    snprintf(ip_box->ip_list[new_count - 1].str, sizeof(ip_box->ip_list[new_count - 1].str), "%s", ip);
    ip_box->count = new_count;

    return ip_box;
}

extern struct IPBox *deep_copy_ip_box(struct IPBox *ip_box) {
    if (ip_box == NULL) {
        return NULL;
    }

    if (ip_box->count <= 0 || ip_box->ip_list == NULL) {
        return NULL;
    }

    struct IPBox *ip_box_ret = (struct IPBox *) malloc(sizeof(struct IPBox));
    if (ip_box_ret == NULL) {
        return NULL;
    }
    memset(ip_box_ret, 0, sizeof(struct IPBox));

    ip_box_ret->count = ip_box->count;
    int ip_list_size = ip_box_ret->count * sizeof(struct CommonStr);
    ip_box_ret->ip_list = (struct CommonStr *) malloc(ip_list_size);
    if (ip_box_ret->ip_list == NULL) {
        free(ip_box_ret);
        return NULL;
    }
    memset(ip_box_ret->ip_list, 0, ip_list_size);
    memcpy(ip_box_ret->ip_list, ip_box->ip_list, ip_list_size);

    return ip_box_ret;
}

extern int delete_ip_box(struct IPBox *ip_box) {
    if (ip_box == NULL) {
        return 0;
    }

    if (ip_box->ip_list != NULL) {
        free(ip_box->ip_list);
        ip_box->ip_list = NULL;
    }

    free(ip_box);

    return 0;
}

extern int check_ip_box_is_equal(struct IPBox *ip_box1, struct IPBox *ip_box2) {
    if (ip_box1 == NULL && ip_box2 == NULL) {
        return 0;
    }

    if (ip_box1 == NULL && ip_box2 != NULL) {
        return -1;
    }

    if (ip_box1 != NULL && ip_box2 == NULL) {
        return -1;
    }

    if (ip_box1->count != ip_box2->count) {
        return -1;
    }

    int ii = 0;
    int jj = 0;
    int find_flag = 0;

    //查找ip_box1中的ip是否都在ip_box2中
    for (ii = 0; ii < ip_box1->count; ii++) {
        find_flag = 0;
        for (jj = 0; jj < ip_box2->count; jj++) {
            if (strcmp(ip_box1->ip_list[ii].str, ip_box2->ip_list[jj].str) == 0) {
                find_flag = 1;
                break;
            }
        }
        if (find_flag == 0) {
            return -1;
        }
    }

    //查找ip_box2中的ip是否都在ip_box1中
    for (ii = 0; ii < ip_box2->count; ii++) {
        find_flag = 0;
        for (jj = 0; jj < ip_box1->count; jj++) {
            if (strcmp(ip_box2->ip_list[ii].str, ip_box1->ip_list[jj].str) == 0) {
                find_flag = 1;
                break;
            }
        }
        if (find_flag == 0) {
            return -1;
        }
    }

    return 0;
}

extern int get_url_syn(struct CommonUrlResponse **response, char *request_url, int timeout_sec) {
    if ((request_url == NULL) || (response == NULL) || timeout_sec < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "para error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    struct CommonUrlResponse *response_temp = NULL;
    response_temp = (struct CommonUrlResponse *) malloc(sizeof(struct CommonUrlResponse));
    if (response_temp == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "response_temp malloc error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }
    memset(response_temp, 0, sizeof(struct CommonUrlResponse));
    response_temp->url_code = CURL_LAST;

    CURL *curl;
    CURLcode ret;
    curl = curl_easy_init();
    if (curl == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "curl_easy_init error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        free(response_temp);
        return -1;
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-type:application/json;charset=UTF-8");
    if (headers == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "curl_slist_append error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        free(response_temp);
        curl_easy_cleanup(curl);
        return -1;
    }

    //设置请求头为json格式
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    //设置请求url
    curl_easy_setopt(curl, CURLOPT_URL, request_url);
    //允许重定向
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    //重定向最大次数
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10);
    //连接超时时间(单位：秒)，为0则无限等待
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
    //传输超时时间(单位：秒)，为0则无限等待
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout_sec);
    //当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
    //如果不设置这个选项，libcurl将会发信号打断这个wait从而可能导致程序crash。
    //在多线程处理场景下使用超时选项时，会忽略signals对应的处理函数
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    //https时不校验证书
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    //https时不校验host
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) response_temp);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, url_response_writedata_callback);
    ret = curl_easy_perform(curl);
    response_temp->url_code = ret;

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    *response = response_temp;
    return 0;
}

extern int delete_common_url_response(struct CommonUrlResponse *common_url_response) {
    if (common_url_response == NULL) {
        return 0;
    }

    if (common_url_response->data != NULL) {
        free(common_url_response->data);
        common_url_response->data = NULL;
    }

    free(common_url_response);
    return 0;
}

extern int delete_ha_response(struct HAResponse *ha_response) {
    if (ha_response == NULL) {
        return 0;
    }

    free(ha_response);
    return 0;
}

extern struct LogInfoBox *
generate_log_info_box(const char *file_path, const char *func_name, const char *module_name, int line_num, int log_level, const char *log_str) {
    struct LogInfoBox *log_info_box_ret = NULL;
    log_info_box_ret = (struct LogInfoBox *) malloc(sizeof(struct LogInfoBox));
    if (log_info_box_ret == NULL) {
        edge_printf(NULL, EDGE_LOG_ERROR,
                    "[EDGE_SERVICE_C_API]log_info_box_ret malloc error(file=%s, function=%s, line=%d)\n", __FILE__,
                    __FUNCTION__, __LINE__);
        return NULL;
    }
    memset(log_info_box_ret, 0, sizeof(struct LogInfoBox));

    log_info_box_ret->file_path = deep_copy_string(file_path);
    log_info_box_ret->func_name = deep_copy_string(func_name);
    log_info_box_ret->module_name = deep_copy_string(module_name);
    log_info_box_ret->line_num = line_num;
    log_info_box_ret->log_level = log_level;
    log_info_box_ret->log_str = deep_copy_string(log_str);

    return log_info_box_ret;
}

extern int delete_log_info_box(struct LogInfoBox *log_info_box) {
    if (log_info_box == NULL) {
        return 0;
    }

    if (log_info_box->file_path != NULL) {
        free(log_info_box->file_path);
        log_info_box->file_path = NULL;
    }

    if (log_info_box->func_name != NULL) {
        free(log_info_box->func_name);
        log_info_box->func_name = NULL;
    }

    if (log_info_box->module_name != NULL) {
        free(log_info_box->module_name);
        log_info_box->module_name = NULL;
    }

    if (log_info_box->log_str != NULL) {
        free(log_info_box->log_str);
        log_info_box->log_str = NULL;
    }

    free(log_info_box);
    return 0;
}

extern char *deep_copy_string(const char *src) {
    if (src == NULL) {
        return NULL;
    }
    char *str_ret = NULL;
    int char_cap = 0;
    char_cap = strlen(src) + 1;
    str_ret = (char *) malloc(char_cap);
    if (str_ret == NULL) {
        return NULL;
    }
    memset(str_ret, 0, char_cap);
    memcpy(str_ret, src, char_cap - 1);

    return str_ret;
}

extern void default_log_writer(struct LogInfoBox *log_info, void *user_ctx) {
    if (log_info == NULL) {
        return;
    }
    edge_printf(NULL, log_info->log_level, "[%s]:%s(file=%s, function=%s, line=%d)\n", log_info->module_name,
                log_info->log_str, log_info->file_path, log_info->func_name, log_info->line_num);
    delete_log_info_box(log_info);
}

extern void set_log_level(int log_level) {
    if (edge_current_log_config == NULL) {
        return;
    }

    if (log_level < EDGE_LOG_NOTHING) {
        log_level = EDGE_LOG_NOTHING;
        return;
    }

    if (log_level > EDGE_LOG_ALL) {
        log_level = EDGE_LOG_ALL;
        return;
    }
    pthread_rwlock_wrlock(&(edge_current_log_config->op_rwlock));
    edge_current_log_config->current_log_level = log_level;
    pthread_rwlock_unlock(&(edge_current_log_config->op_rwlock));
}

extern int get_log_level() {
    if (edge_current_log_config == NULL) {
        return EDGE_LOG_NOTHING;
    }

    int current_log_level = EDGE_LOG_INFO;
    pthread_rwlock_rdlock(&(edge_current_log_config->op_rwlock));
    current_log_level = edge_current_log_config->current_log_level;
    pthread_rwlock_unlock(&(edge_current_log_config->op_rwlock));

    return current_log_level;
}

extern void set_log_writer(void (*log_writer)(struct LogInfoBox *log_info, void *user_ctx), void *user_ctx) {
    if (edge_current_log_config == NULL) {
//        pthread_rwlock_wrlock(&(edge_current_log_config->op_rwlock));
//        edge_current_log_config->log_writer = default_log_writer;
//        edge_current_log_config->user_ctx = user_ctx;
//        pthread_rwlock_unlock(&(edge_current_log_config->op_rwlock));
        return;
    }

    pthread_rwlock_wrlock(&(edge_current_log_config->op_rwlock));
    edge_current_log_config->log_writer = log_writer;
    edge_current_log_config->user_ctx = user_ctx;
    pthread_rwlock_unlock(&(edge_current_log_config->op_rwlock));
}

extern struct IPBox *get_current_master_ip_list(struct IPBox *ip_box, int ha_port) {
    if (ip_box == NULL || ip_box->count <= 0) {
        return NULL;
    }
    char request_url[512];
    struct CommonUrlResponse *response = NULL;
    struct HAResponse *ha_response = NULL;
    int ii = 0;
    int ret = 0;
    for (ii = 0; ii < ip_box->count; ii++) {
        memset(request_url, 0, sizeof(request_url));
        snprintf(request_url, sizeof(request_url), "%s:%d/%s", ip_box->ip_list[ii].str, ha_port, EDGE_HA_URL);
        ret = get_url_syn(&response, request_url, 10);
        if (ret < 0 || response == NULL) {
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content), "get_url_syn error");
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_WARN, log_content);
            delete_common_url_response(response);
            response = NULL;
            continue;
        }

        if (!(response->url_code == 200 || response->url_code == 0)) {
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content), "get_url_syn response->url_code=%d, error", response->url_code);
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_DEBUG, log_content);
            delete_common_url_response(response);
            response = NULL;
            continue;
        }

        if (response->data_len <= 0 || response->data == NULL) {
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content), "get_url_syn response->data_len <= 0, error");
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_WARN, log_content);
            delete_common_url_response(response);
            response = NULL;
            continue;
        }

        ret = transform_ha_response_to_struct(response->data, &ha_response);
        if (ret < 0 || ha_response == NULL) {
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content), "transform_ha_response_to_struct error");
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_WARN, log_content);
            delete_common_url_response(response);
            response = NULL;
            continue;
        }

        if (strstr(ha_response->message, "master") != NULL) {
            delete_ha_response(ha_response);
            ha_response = NULL;
            delete_common_url_response(response);
            response = NULL;

            return generate_ip_box(1, ip_box->ip_list[ii].str);
        }

        delete_ha_response(ha_response);
        ha_response = NULL;

        delete_common_url_response(response);
        response = NULL;
    }

    return NULL;
}

extern int check_service_available(char *ip, int port) {
    if (ip == NULL || strlen(ip) <= 0) {
        return -1;
    }
    char request_url[512];
    struct CommonUrlResponse *response = NULL;
    int ret = 0;

    memset(request_url, 0, sizeof(request_url));
    snprintf(request_url, sizeof(request_url), "%s:%d/%s", ip, port, EDGE_SERVICE_CHECK_URL);
    ret = get_url_syn(&response, request_url, 10);
    if (ret < 0 || response == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "get_url_syn error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_WARN, log_content);
        delete_common_url_response(response);
        response = NULL;
        return -1;
    }

    if (!(response->url_code == 200 || response->url_code == 0)) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "get_url_syn response->url_code=%d, error", response->url_code);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_WARN, log_content);
        delete_common_url_response(response);
        response = NULL;
        return -1;
    }

    if (response->data_len <= 0 || response->data == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "get_url_syn response->data_len <= 0, error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_WARN, log_content);
        delete_common_url_response(response);
        response = NULL;
        return -1;
    }

    if (strncmp(response->data, "OK", 2) == 0) {
        delete_common_url_response(response);
        response = NULL;
        return 0;
    }

    delete_common_url_response(response);
    response = NULL;

    return -1;
}