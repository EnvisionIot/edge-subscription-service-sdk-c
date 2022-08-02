#include "edge_service_c_api/edge_service_c_api.h"

struct my_ctx {
    int aa;
    char bb[128];
};

void my_log_writer(struct LogInfoBox *log_info, void *user_ctx) {
    if (log_info == NULL) {
        return;
    }
    printf("[TEST-%s]:%s(file=%s, function=%s, line=%d)\n", log_info->module_name, log_info->log_str,
           log_info->file_path,
           log_info->func_name, log_info->line_num);

    //注意用完之后要释放
    delete_log_info_box(log_info);
}

void connect_callback(void *work_ctx, char *channel_id, void *user_ctx) {
    printf("[DATASERVICE]:connected, channel_id=%s(file=%s, function=%s, line=%d)\n",
           channel_id, __FILE__, __FUNCTION__, __LINE__);
    struct my_ctx *x = (struct my_ctx *) (user_ctx);
    if (x == NULL) {
        return;
    }
    printf("[DATASERVICE]:user_ctx->aa=%d, user_ctx->bb=%s(file=%s, function=%s, line=%d)\n",
           x->aa, x->bb, __FILE__, __FUNCTION__, __LINE__);
    return;
}

void close_callback(void *work_ctx, char *channel_id, void *user_ctx) {
    printf("[DATASERVICE]:closed, channel_id=%s(file=%s, function=%s, line=%d)\n", channel_id,
           __FILE__, __FUNCTION__, __LINE__);
    struct my_ctx *x = (struct my_ctx *) (user_ctx);
    if (x == NULL) {
        return;
    }
    printf("[DATASERVICE]:user_ctx->aa=%d, user_ctx->bb=%s(file=%s, function=%s, line=%d)\n",
           x->aa, x->bb, __FILE__, __FUNCTION__, __LINE__);
    return;
}

static void print_help(char *name) {
    printf("usage:<%s> <-topic topic_name> [-port 9150] [-p 123456] [-is_fast is_fast] <-ip 1.1.1.1 2.2.2.2 ...> <-send_count send_count> <-content_len content_len>\n",
           name);
    printf("-is_fast 0 ====> normal\n");
    printf("-is_fast 1 ====> fast\n");
    printf("is_fast default is 0\n");
    printf("port default value is 9150\n");
    printf("send_count <= 0, send continuously\n");
    printf("content_len need in (0, 100000]\n");
}

//sdk使用流程-->
//发消息：
//1.init_edge_service_ctx;初始化全局变量，非线程安全
//2.set_log_level(EDGE_LOG_INFO);设置日志等级，不设置默认为EDGE_LOG_INFO，线程安全，可以随时调用这个函数，实时生效
//3.set_log_writer(my_log_writer, user_ctx);设置打印函数，sdk内部需要打印时会调用这个打印函数打印，如果不设置，默认打印到命令行终端，打印函数中注意数据用完后需要delete_log_info_box释放，非线程安全，一开始设置一次就可以了
//4.调用new_data_service_ctx_en_for_publish函数，初始化必要的上下文，线程安全，注意当只填一个IP时，无论这个IP是主还是备，都会去连接这个IP，填多个IP时，会根据主备情况自动切换
//5.调用data_service_ctx_start函数，启动相关模块，建立连接，与服务端进行应用层协商，非线程安全
//6.发送数据，线程安全(connect_callback触发之前，调用data_service_ctx_pub_fast发送将会发送失败)
//7.可以调用data_service_ctx_stop函数停止相关模块，断开连接，start和stop的调用要成对，不能在没有调用start的情况下调用stop，也不能再已经start的情况下调用start，非线程安全，需要注意的是，stop会立即断开tcp连接，还在tcp发送缓存中的数据将会丢失
//8.delete_data_service_ctx;释放new_data_service_ctx占用的资源，退出时需要调用，需要在调用stop之后调用该函数，非线程安全
//9.uninit_edge_service_ctx;释放init_edge_service_ctx占用的资源，退出时需要调用，需要在调用delete_data_service_ctx之后调用该函数，非线程安全
//<--sdk使用流程
int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help(argv[0]);
        return -1;
    }

    int ii = 0;
    int ret = 0;
    int topic_flag = 0;
    int ip_flag = 0;
    int port_flag = 0;
    int p_flag = 0;
    int send_count_flag = 0;
    int content_len_flag = 0;
    int is_fast_flag = 0;
    struct IPBox *ip_list = NULL;
    char topic_name[128];
    memset(topic_name, 0, sizeof(topic_name));
    long long int send_count = 0;
    int content_len = 0;
    int port = EDGE_DATASERVICE_DEFAULT_PORT;
    int is_fast = 0;
    char p[64];
    memset(p, 0, sizeof(p));
    for (ii = 1; ii < argc; ii++) {
        if (strcmp(argv[ii], "-ip") == 0) {
            topic_flag = 0;
            port_flag = 0;
            ip_flag = 1;
            send_count_flag = 0;
            content_len_flag = 0;
            is_fast_flag = 0;
            p_flag = 0;
            continue;
        }

        if (strcmp(argv[ii], "-send_count") == 0) {
            topic_flag = 0;
            port_flag = 0;
            ip_flag = 0;
            send_count_flag = 1;
            content_len_flag = 0;
            is_fast_flag = 0;
            p_flag = 0;
            continue;
        }

        if (strcmp(argv[ii], "-content_len") == 0) {
            topic_flag = 0;
            port_flag = 0;
            ip_flag = 0;
            send_count_flag = 0;
            content_len_flag = 1;
            is_fast_flag = 0;
            p_flag = 0;
            continue;
        }

        if (strcmp(argv[ii], "-topic") == 0) {
            topic_flag = 1;
            port_flag = 0;
            ip_flag = 0;
            send_count_flag = 0;
            content_len_flag = 0;
            is_fast_flag = 0;
            p_flag = 0;
            continue;
        }

        if (strcmp(argv[ii], "-port") == 0) {
            topic_flag = 0;
            port_flag = 1;
            ip_flag = 0;
            send_count_flag = 0;
            content_len_flag = 0;
            is_fast_flag = 0;
            p_flag = 0;
            continue;
        }

        if (strcmp(argv[ii], "-p") == 0) {
            topic_flag = 0;
            port_flag = 0;
            ip_flag = 0;
            send_count_flag = 0;
            content_len_flag = 0;
            is_fast_flag = 0;
            p_flag = 1;
            continue;
        }

        if (strcmp(argv[ii], "-is_fast") == 0) {
            topic_flag = 0;
            port_flag = 0;
            ip_flag = 0;
            send_count_flag = 0;
            content_len_flag = 0;
            is_fast_flag = 1;
            p_flag = 0;
            continue;
        }

        if (ip_flag == 1) {
            ip_list = add_ip_to_ip_box(ip_list, argv[ii]);
            continue;
        }

        if (topic_flag == 1) {
            snprintf(topic_name, sizeof(topic_name), "%s", argv[ii]);
            continue;
        }

        if (send_count_flag == 1) {
            sscanf(argv[ii], "%lld", &send_count);
            continue;
        }

        if (content_len_flag == 1) {
            sscanf(argv[ii], "%d", &content_len);
            continue;
        }

        if (port_flag == 1) {
            sscanf(argv[ii], "%d", &port);
            continue;
        }

        if (p_flag == 1) {
            snprintf(p, sizeof(p), "%s", argv[ii]);
            continue;
        }

        if (is_fast_flag == 1) {
            sscanf(argv[ii], "%d", &is_fast);
            continue;
        }
    }

    if (ip_list == NULL || ip_list->count <= 0) {
        print_help(argv[0]);
        return -1;
    }

    if (strlen(topic_name) <= 0) {
        print_help(argv[0]);
        delete_ip_box(ip_list);
        return -1;
    }

    if (content_len <= 0 || content_len > 100000) {
        print_help(argv[0]);
        delete_ip_box(ip_list);
        return -1;
    }

    struct DataServiceCtx *ctx = NULL;

    //1.init_edge_service_ctx;初始化全局变量，非线程安全
    ret = init_edge_service_ctx();
    if (ret < 0) {
        printf("[DATASERVICE_TEST]:init_edge_service_ctx error(file=%s, function=%s, line=%d)\n", __FILE__,
               __FUNCTION__, __LINE__);
        delete_ip_box(ip_list);
        return -1;
    }

    struct my_ctx *user_ctx = (struct my_ctx *) malloc(sizeof(struct my_ctx));
    if (user_ctx == NULL) {
        printf("[DATASERVICE_TEST]:user_ctx malloc error(file=%s, function=%s, line=%d)\n", __FILE__, __FUNCTION__,
               __LINE__);
        delete_ip_box(ip_list);
        uninit_edge_service_ctx();
        return -1;
    }
    memset(user_ctx, 0, sizeof(struct my_ctx));
    user_ctx->aa = 10;
    snprintf(user_ctx->bb, sizeof(user_ctx->bb), "%s", "aabbcc");

    //2.set_log_level(EDGE_LOG_INFO);设置日志等级，不设置默认为EDGE_LOG_INFO，线程安全，可以随时调用这个函数，实时生效
    set_log_level(EDGE_LOG_INFO);
    //3.set_log_writer(my_log_writer, user_ctx);设置打印函数，sdk内部需要打印时会调用这个打印函数打印，如果不设置，默认打印到命令行终端，打印函数中注意数据用完后需要delete_log_info_box释放，非线程安全，一开始设置一次就可以了
    set_log_writer(my_log_writer, user_ctx);

    //4.调用new_data_service_ctx_en_for_publish函数，初始化必要的上下文，线程安全，注意当只填一个IP时，无论这个IP是主还是备，都会去连接这个IP，填多个IP时，会根据主备情况自动切换
    ctx = new_data_service_ctx_en_for_publish(
            ip_list,
            port,
            p,
            "aaa",
            "bbb",
            1,
            is_fast,
            user_ctx,
            connect_callback,
            close_callback
    );

    if (ctx == NULL) {
        printf("[DATASERVICE_TEST]:new_data_service_ctx_en_for_publish error(file=%s, function=%s, line=%d)\n", __FILE__,
               __FUNCTION__,
               __LINE__);
        delete_ip_box(ip_list);
        ip_list = NULL;
        uninit_edge_service_ctx();
        free(user_ctx);
        user_ctx = NULL;
        return -1;
    }

    //5.调用data_service_ctx_start函数，启动相关模块，建立连接，与服务端进行应用层协商，非线程安全
    ret = data_service_ctx_start(ctx);
    if (ret < 0) {
        printf("[DATASERVICE_TEST]:data_service_ctx_start error(file=%s, function=%s, line=%d)\n", __FILE__,
               __FUNCTION__, __LINE__);
        delete_ip_box(ip_list);
        ip_list = NULL;
        delete_data_service_ctx(ctx);
        ctx = NULL;
        uninit_edge_service_ctx();
        free(user_ctx);
        user_ctx = NULL;
        return -1;
    }

    if (is_fast != 0) {
        edge_sleep(5000);//等待连接及初始化成功
    }

    //6.发送数据，线程安全(connect_callback触发之前，调用data_service_ctx_pub_fast发送将会发送失败)
    char *content_send = (char *) malloc(content_len);
    if (content_send == NULL) {
        printf("[DATASERVICE_TEST]:content_send malloc error, content_len=%d(file=%s, function=%s, line=%d)\n",
               content_len, __FILE__,
               __FUNCTION__, __LINE__);
        delete_ip_box(ip_list);
        ip_list = NULL;
        delete_data_service_ctx(ctx);
        ctx = NULL;
        uninit_edge_service_ctx();
        free(user_ctx);
        user_ctx = NULL;
        return -1;
    }
    memset(content_send, 0, content_len);

    long long int send_count_current = 0;
    long long int send_count_current_temp = 0;
    struct timeval time_previous;
    struct timeval time_begin;
    struct timeval time_current;
    gettimeofday(&time_previous, NULL);
    time_begin.tv_sec = time_previous.tv_sec;
    time_begin.tv_usec = time_previous.tv_usec;
    long long int statistics_count = 100000;
    long long int time_used_ms = 0;

    printf("[DATASERVICE_TEST]:start send(file=%s, function=%s, line=%d)\n", __FILE__, __FUNCTION__, __LINE__);

    while (1) {
        snprintf(content_send, content_len, "%lld", send_count_current);
        if (is_fast != 0) {
            ret = data_service_ctx_pub_fast(ctx, topic_name, content_send, content_len);
        } else {
            ret = data_service_ctx_pub_normal(ctx, topic_name, content_send, content_len, 10000);
        }
        if (ret < 0) {
            printf("[DATASERVICE_TEST]:pub msg error, send_count_current=%lld(file=%s, function=%s, line=%d)\n",
                   send_count_current, __FILE__,
                   __FUNCTION__, __LINE__);
            delete_ip_box(ip_list);
            ip_list = NULL;
            data_service_ctx_stop(ctx);
            delete_data_service_ctx(ctx);
            ctx = NULL;
            uninit_edge_service_ctx();
            free(content_send);
            content_send = NULL;
            free(user_ctx);
            user_ctx = NULL;
            return -1;
        }
        send_count_current++;
        send_count_current_temp++;
        if (send_count_current_temp >= statistics_count) {
            gettimeofday(&time_current, NULL);
            time_used_ms = (time_current.tv_sec - time_previous.tv_sec) * 1000 +
                           (time_current.tv_usec - time_previous.tv_usec) / 1000;
            printf("[DATASERVICE_TEST]:send_count_current=%lld, last %lld packet send used ms=%lld(file=%s, function=%s, line=%d)\n",
                   send_count_current, send_count_current_temp, time_used_ms, __FILE__, __FUNCTION__, __LINE__);

            send_count_current_temp = 0;
            time_previous.tv_sec = time_current.tv_sec;
            time_previous.tv_usec = time_current.tv_usec;
        }

        if (send_count > 0) {
            if (send_count_current >= send_count) {
                gettimeofday(&time_current, NULL);
                time_used_ms = (time_current.tv_sec - time_begin.tv_sec) * 1000 +
                               (time_current.tv_usec - time_begin.tv_usec) / 1000;
                printf("[DATASERVICE_TEST]:send finish, send_count_current=%lld, used ms=%lld(file=%s, function=%s, line=%d)\n",
                       send_count_current, time_used_ms, __FILE__, __FUNCTION__, __LINE__);
                break;
            }
        }
    }
    free(content_send);
    content_send = NULL;

    if (is_fast != 0) {
        edge_sleep(5000);//等待发送完成
    }

    //不使用时记得stop和delete
    //ip_box不使用时是需要释放的
    delete_ip_box(ip_list);

    //7.可以调用data_service_ctx_stop函数停止相关模块，断开连接，start和stop的调用要成对，不能在没有调用start的情况下调用stop，也不能再已经start的情况下调用start，非线程安全，需要注意的是，stop会立即断开tcp连接，还在tcp发送缓存中的数据将会丢失
    data_service_ctx_stop(ctx);
    //8.delete_data_service_ctx;释放new_data_service_ctx_en_for_publish占用的资源，退出时需要调用，需要在调用stop之后调用该函数，非线程安全
    delete_data_service_ctx(ctx);
    //9.uninit_edge_service_ctx;释放init_edge_service_ctx占用的资源，退出时需要调用，需要在调用delete_data_service_ctx之后调用该函数，非线程安全
    uninit_edge_service_ctx();

    free(user_ctx);

    return 0;
}

