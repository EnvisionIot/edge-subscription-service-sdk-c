#include "edge_service_c_api/edge_service_c_api.h"
#include <sys/time.h>

struct my_ctx {
    //读写锁
    pthread_rwlock_t op_rwlock;

    long long int packet_count;
    long long int packet_count_all;
    long long int point_count;
    long long int point_count_all;
};

//统计输出线程
static void *statistics_job(void *arg) {
    //开启接收cancel信号的功能
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    //收到cancel信号后使用pthread_testcancel()退出
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    struct my_ctx *x = (struct my_ctx *) arg;
    if (x == NULL) {
        printf("[DATASERVICE]:x is NULL(file=%s, function=%s, line=%d)\n", __FILE__,
               __FUNCTION__, __LINE__);
        return NULL;
    }

    long long int packet_count = 0;
    long long int packet_count_all = 0;
    long long int point_count = 0;
    long long int point_count_all = 0;
    struct timeval tv_temp;
    long long int time_now = 0;//距离1970年的毫秒
    long long int time_last = 0;//距离1970年的毫秒
    double time_used = 0;//秒

    memset(&tv_temp, 0, sizeof(struct timeval));
    gettimeofday(&tv_temp, NULL);
    time_last = ((long long int) tv_temp.tv_sec) * 1000 + tv_temp.tv_usec / 1000;

    edge_sleep(60000);

    while (1) {
        pthread_testcancel();

        memset(&tv_temp, 0, sizeof(struct timeval));
        gettimeofday(&tv_temp, NULL);
        time_now = ((long long int) tv_temp.tv_sec) * 1000 + tv_temp.tv_usec / 1000;
        time_used = ((double) (time_now - time_last)) / 1000;
        //加写锁
        pthread_rwlock_wrlock(&(x->op_rwlock));
        packet_count = x->packet_count;
        packet_count_all = x->packet_count_all;
        point_count = x->point_count;
        point_count_all = x->point_count_all;
        x->packet_count = 0;
        x->point_count = 0;
        pthread_rwlock_unlock(&(x->op_rwlock));

        time_last = time_now;

        printf("====================================>>\n");
        printf("time_used(s)=%lf\n", time_used);
        printf("packet_count=%lld, packet_count per sec=%lf\n", packet_count,
               ((double) (packet_count)) / time_used);
        printf("point_count=%lld, point_count per sec=%lf\n", point_count,
               ((double) (point_count)) / time_used);
        printf("packet_count_all=%lld\n", packet_count_all);
        printf("point_count_all=%lld\n", point_count_all);
        printf("<<====================================\n");

        edge_sleep(60000);
    }
}

void connect_callback(void *work_ctx, char *channel_id, void *user_ctx) {
    printf("[DATASERVICE]:connected, channel_id=%s(file=%s, function=%s, line=%d)\n",
           channel_id, __FILE__, __FUNCTION__, __LINE__);
    struct my_ctx *x = (struct my_ctx *) (user_ctx);
    if (x == NULL) {
        return;
    }
}

void close_callback(void *work_ctx, char *channel_id, void *user_ctx) {
    printf("[DATASERVICE]:closed, channel_id=%s(file=%s, function=%s, line=%d)\n", channel_id,
           __FILE__, __FUNCTION__, __LINE__);
    struct my_ctx *x = (struct my_ctx *) (user_ctx);
    if (x == NULL) {
        return;
    }
}

void
msg_callback(void *work_ctx, char *channel_id, struct DataServiceMessage *msg, void *user_ctx) {
    struct my_ctx *x = (struct my_ctx *) (user_ctx);
    if (x == NULL) {
        //注意用完后要释放
        delete_data_service_msg(msg);
        return;
    }

    //加写锁进行计数
    pthread_rwlock_wrlock(&(x->op_rwlock));
    x->packet_count++;
    x->packet_count_all++;
    pthread_rwlock_unlock(&(x->op_rwlock));

    struct DataSubscribeStruct *sub_p = NULL;
    struct ControlResponseStruct *control_p = NULL;
    struct SetMeasurepointResponseStruct *set_p = NULL;
    struct SimpleDataSubscribeStruct *ssub_p = NULL;
//    char *custom_ptr = NULL;

    switch (msg->topic_type) {
        case TOPIC_TYPE_DATA_SUBSCRIBE:
        case TOPIC_TYPE_DATA_SUBSCRIBE_ALL:
            sub_p = ((struct DataSubscribeStruct *) (msg->msg));
            pthread_rwlock_wrlock(&(x->op_rwlock));
            x->point_count += sub_p->point_count;
            x->point_count_all += sub_p->point_count;
            pthread_rwlock_unlock(&(x->op_rwlock));
            break;
        case TOPIC_TYPE_CONTROL_RESPONSE:
            control_p = ((struct ControlResponseStruct *) (msg->msg));
            pthread_rwlock_wrlock(&(x->op_rwlock));
            x->point_count += control_p->point_count;
            x->point_count_all += control_p->point_count;
            pthread_rwlock_unlock(&(x->op_rwlock));
            break;
        case TOPIC_TYPE_SET_MEASUREPOINT_RESPONSE:
            set_p = ((struct SetMeasurepointResponseStruct *) (msg->msg));
            pthread_rwlock_wrlock(&(x->op_rwlock));
            x->point_count += set_p->point_count;
            x->point_count_all += set_p->point_count;
            pthread_rwlock_unlock(&(x->op_rwlock));
            break;
        case TOPIC_TYPE_SIMPLE_DATA_SUBSCRIBE:
            ssub_p = ((struct SimpleDataSubscribeStruct *) (msg->msg));
            pthread_rwlock_wrlock(&(x->op_rwlock));
            x->point_count += ssub_p->point_count;
            x->point_count_all += ssub_p->point_count;
            pthread_rwlock_unlock(&(x->op_rwlock));
            break;
        case TOPIC_TYPE_CUSTOM:
//            custom_ptr = (char *) (msg->msg);
            break;
        default:
            printf("[DATASERVICE]:unsupported msg->topic_type=%d(file=%s, function=%s, line=%d)\n",
                   msg->topic_type, __FILE__, __FUNCTION__, __LINE__);
            break;
    }

    //注意用完后要释放
    delete_data_service_msg(msg);
    return;
}

static void print_help(char *name) {
    printf("usage:<%s> <-topic topic_name> [-topic_type 0] [-port 9150] [-p 123456] <-ip 1.1.1.1 2.2.2.2 ...>\n", name);
    printf("-topic_type 0 ====> TOPIC_TYPE_AUTO(default)\n");
    printf("-topic_type 1 ====> TOPIC_TYPE_DATA_SUBSCRIBE\n");
    printf("-topic_type 2 ====> TOPIC_TYPE_DATA_SUBSCRIBE_ALL\n");
    printf("-topic_type 3 ====> TOPIC_TYPE_CONTROL_RESPONSE\n");
    printf("-topic_type 4 ====> TOPIC_TYPE_SET_MEASUREPOINT_RESPONSE\n");
    printf("-topic_type 5 ====> TOPIC_TYPE_CUSTOM\n");
    printf("-topic_type 6 ====> TOPIC_TYPE_SIMPLE_DATA_SUBSCRIBE\n");
    printf("port default value is 9150\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help(argv[0]);
        return -1;
    }

    int ii = 0;
    int ret = 0;
    int topic_flag = 0;
    int ip_flag = 0;
    int topic_type_flag = 0;
    int port_flag = 0;
    int p_flag = 0;
    struct IPBox *ip_list = NULL;
    char topic_name[128];
    memset(topic_name, 0, sizeof(topic_name));
    int topic_type = TOPIC_TYPE_AUTO;
    int port = EDGE_DATASERVICE_DEFAULT_PORT;
    char p[64];
    memset(p, 0, sizeof(p));
    for (ii = 1; ii < argc; ii++) {
        if (strcmp(argv[ii], "-ip") == 0) {
            topic_flag = 0;
            topic_type_flag = 0;
            port_flag = 0;
            ip_flag = 1;
            p_flag = 0;
            continue;
        }

        if (strcmp(argv[ii], "-topic_type") == 0) {
            topic_flag = 0;
            topic_type_flag = 1;
            port_flag = 0;
            ip_flag = 0;
            p_flag = 0;
            continue;
        }

        if (strcmp(argv[ii], "-topic") == 0) {
            topic_flag = 1;
            topic_type_flag = 0;
            port_flag = 0;
            ip_flag = 0;
            p_flag = 0;
            continue;
        }

        if (strcmp(argv[ii], "-port") == 0) {
            topic_flag = 0;
            topic_type_flag = 0;
            port_flag = 1;
            ip_flag = 0;
            p_flag = 0;
            continue;
        }

        if (strcmp(argv[ii], "-p") == 0) {
            topic_flag = 0;
            topic_type_flag = 0;
            port_flag = 0;
            ip_flag = 0;
            p_flag = 1;
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

        if (topic_type_flag == 1) {
            sscanf(argv[ii], "%d", &topic_type);
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

    struct DataServiceCtx *ctx = NULL;

    ret = init_edge_service_ctx();
    if (ret < 0) {
        printf("[DATASERVICE_TEST]:init_edge_service_ctx error(file=%s, function=%s, line=%d)\n",
               __FILE__, __FUNCTION__, __LINE__);
        delete_ip_box(ip_list);
        return -1;
    }

    struct my_ctx *user_ctx = (struct my_ctx *) malloc(sizeof(struct my_ctx));
    if (user_ctx == NULL) {
        printf("[DATASERVICE_TEST]:user_ctx malloc error(file=%s, function=%s, line=%d)\n",
               __FILE__, __FUNCTION__, __LINE__);
        delete_ip_box(ip_list);
        uninit_edge_service_ctx();
        return -1;
    }
    memset(user_ctx, 0, sizeof(struct my_ctx));
    //初始化锁
    pthread_rwlock_init(&(user_ctx->op_rwlock), NULL);

    ctx = new_data_service_ctx_en(
            ip_list,
            port,
            p,
            "aaa",
            "bbb",
            topic_name,
            NULL,
            topic_type,
            1,
            user_ctx,
            connect_callback,
            close_callback,
            msg_callback
    );

    if (ctx == NULL) {
        printf("[DATASERVICE_TEST]:new_data_service_ctx_en error(file=%s, function=%s, line=%d)\n",
               __FILE__, __FUNCTION__, __LINE__);
        delete_ip_box(ip_list);
        ip_list = NULL;
        free(user_ctx);
        user_ctx = NULL;
        uninit_edge_service_ctx();
        return -1;
    }

    ret = data_service_ctx_start(ctx);
    if (ret < 0) {
        printf("[DATASERVICE_TEST]:data_service_ctx_start error(file=%s, function=%s, line=%d)\n",
               __FILE__, __FUNCTION__, __LINE__);
        delete_ip_box(ip_list);
        ip_list = NULL;
        free(user_ctx);
        user_ctx = NULL;
        delete_data_service_ctx(ctx);
        ctx = NULL;
        uninit_edge_service_ctx();
        return -1;
    }

    //启动统计线程
    pthread_t pth_statistics_job;
    if (pthread_create(&pth_statistics_job, NULL, statistics_job, (void *) (user_ctx)) != 0) {
        printf("[DATASERVICE]pthread_create pth_statistics_job error(file=%s, function=%s, line=%d)\n",
               __FILE__, __FUNCTION__, __LINE__);
        delete_ip_box(ip_list);
        ip_list = NULL;
        pthread_rwlock_destroy(&(user_ctx->op_rwlock));
        free(user_ctx);
        user_ctx = NULL;
        data_service_ctx_stop(ctx);
        delete_data_service_ctx(ctx);
        ctx = NULL;
        uninit_edge_service_ctx();
        return -1;
    }

    while (1) {
        edge_sleep(10000);
    }

    //不使用时记得stop和delete，注意不能在没有调用start的情况下调用stop
    delete_ip_box(ip_list);
    pthread_rwlock_destroy(&(user_ctx->op_rwlock));
    data_service_ctx_stop(ctx);
    delete_data_service_ctx(ctx);
    pthread_cancel(pth_statistics_job);
    pthread_join(pth_statistics_job, NULL);
    uninit_edge_service_ctx();
    free(user_ctx);

    return 0;
}

