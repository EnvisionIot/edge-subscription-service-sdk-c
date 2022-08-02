//
// Created by yang.zhang4 on 2021/10/11.
//

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

void print_simple_data_subscribe_msg(struct SimpleDataSubscribeStruct *sdss_ptr) {
    printf("---------------------->\n");
    if (sdss_ptr == NULL) {
        printf("sdss_ptr == NULL\n");
        return;
    }
    printf("sdss_ptr->point_count=%d\n", sdss_ptr->point_count);
    int ii = 0;
    for (ii = 0; ii < sdss_ptr->point_count; ii++) {
        printf("*******>\n");
        printf("sdss_ptr->points[%d].assetid=%s\n", ii, sdss_ptr->points[ii].assetid);
        printf("sdss_ptr->points[%d].pointid=%s\n", ii, sdss_ptr->points[ii].pointid);
        printf("sdss_ptr->points[%d].time=%lld\n", ii, (long long int) (sdss_ptr->points[ii].time));
        printf("sdss_ptr->points[%d].value=%s\n", ii, sdss_ptr->points[ii].value);
        printf("sdss_ptr->points[%d].quality=%d\n", ii, sdss_ptr->points[ii].quality);
        printf("sdss_ptr->points[%d].edq=%d\n", ii, sdss_ptr->points[ii].edq);
        printf("sdss_ptr->points[%d].datatype=%s\n", ii, sdss_ptr->points[ii].datatype);
        printf("sdss_ptr->points[%d].subdatatype=%s\n", ii, sdss_ptr->points[ii].subdatatype);
        printf("sdss_ptr->points[%d].oemtime=%lld\n", ii, (long long int) (sdss_ptr->points[ii].oemtime));
        printf("sdss_ptr->points[%d].attr=%s\n", ii, sdss_ptr->points[ii].attr);
        printf("<*******\n");
    }

    printf("<----------------------\n\n");
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

void
msg_callback(void *work_ctx, char *channel_id, struct DataServiceMessage *msg, void *user_ctx) {
    printf("[DATASERVICE]:recv msg, channel_id=%s(file=%s, function=%s, line=%d)\n",
           channel_id, __FILE__, __FUNCTION__, __LINE__);
    struct my_ctx *x = (struct my_ctx *) (user_ctx);
    if (x == NULL) {
        //注意用完后要释放
        delete_data_service_msg(msg);
        return;
    }
    printf("[DATASERVICE]:user_ctx->aa=%d, user_ctx->bb=%s(file=%s, function=%s, line=%d)\n",
           x->aa, x->bb, __FILE__, __FUNCTION__, __LINE__);

    printf("[DATASERVICE]:msg->topic_type=%d(file=%s, function=%s, line=%d)\n",
           msg->topic_type, __FILE__, __FUNCTION__, __LINE__);

    switch (msg->topic_type) {
        case TOPIC_TYPE_SIMPLE_DATA_SUBSCRIBE:
            print_simple_data_subscribe_msg((struct SimpleDataSubscribeStruct *) (msg->msg));
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

//sdk使用流程-->
//订阅：
//1.init_edge_service_ctx;初始化全局变量，非线程安全
//2.set_log_level(EDGE_LOG_INFO);设置日志等级，不设置默认为EDGE_LOG_INFO，线程安全，可以随时调用这个函数，实时生效
//3.set_log_writer(my_log_writer, user_ctx);设置打印函数，sdk内部需要打印时会调用这个打印函数打印，如果不设置，默认打印到命令行终端，打印函数中注意数据用完后需要delete_log_info_box释放，非线程安全，一开始设置一次就可以了
//4.调用new_data_service_ctx_en函数，初始化必要的上下文，线程安全，注意当只填一个IP时，无论这个IP是主还是备，都会去向这个IP订阅数据，填多个IP时，会根据主备情况自动切换
//5.调用data_service_ctx_start函数，启动相关模块，开始从服务端接收数据，非线程安全
//6.可以调用data_service_ctx_stop函数暂停接收数据，start和stop的调用要成对，不能在没有调用start的情况下调用stop，也不能再已经start的情况下调用start，非线程安全
//7.delete_data_service_ctx;释放new_data_service_ctx_en占用的资源，退出时需要调用，需要在调用stop之后调用该函数，非线程安全
//8.uninit_edge_service_ctx;释放init_edge_service_ctx占用的资源，退出时需要调用，需要在调用delete_data_service_ctx之后调用该函数，非线程安全
//<--sdk使用流程
int main(int argc, char *argv[]) {
    int ret = 0;
    struct IPBox *ip_list = NULL;
    char topic_name[60];
    memset(topic_name, 0, sizeof(topic_name));
    char consumerGroup[60];
    memset(consumerGroup, 0, sizeof(consumerGroup));
    char p[64];
    memset(p, 0, sizeof(p));

    //需要连接的ip
    ip_list = add_ip_to_ip_box(ip_list, "127.0.0.1");
    //订阅的topic类型，实时数据订阅可以选TOPIC_TYPE_DATA_SUBSCRIBE或TOPIC_TYPE_AUTO
    int topic_type = TOPIC_TYPE_SIMPLE_DATA_SUBSCRIBE;
    //端口
    int port = EDGE_DATASERVICE_DEFAULT_PORT;
    //登录密码，不需要登录密码时，可以是NULL或空串
    snprintf(p, sizeof(p), "%s", "123456");
    //topic名字，实时数据订阅的topic都是以DATASVC.SUB.开头
    snprintf(topic_name, sizeof(topic_name), "%s", "DATASVC.SUB.APP.SUBTEST");
    //consumerGroup，非必填，如果consumerGroup是NULL，则默认为default，详细说明参考new_data_service_ctx_en函数注释
    snprintf(consumerGroup, sizeof(consumerGroup), "%s", "default");

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
    snprintf(user_ctx->bb, sizeof(user_ctx->bb), "%s", "user_custom_info");

    //2.set_log_level(EDGE_LOG_INFO);设置日志等级，不设置默认为EDGE_LOG_INFO，线程安全，可以随时调用这个函数，实时生效
    set_log_level(EDGE_LOG_INFO);
    //3.set_log_writer(my_log_writer, user_ctx);设置打印函数，sdk内部需要打印时会调用这个打印函数打印，如果不设置，默认打印到命令行终端，打印函数中注意数据用完后需要delete_log_info_box释放，非线程安全，一开始设置一次就可以了
    set_log_writer(my_log_writer, user_ctx);

    //4.调用new_data_service_ctx_en函数，初始化必要的上下文，线程安全，注意当只填一个IP时，无论这个IP是主还是备，都会去向这个IP订阅数据，填多个IP时，会根据主备情况自动切换
    ctx = new_data_service_ctx_en(
            ip_list,
            port,
            p,
            "your accessKey",
            "your secretKey",
            topic_name,
            consumerGroup,
            topic_type,
            1,
            user_ctx,
            connect_callback,
            close_callback,
            msg_callback
    );

    if (ctx == NULL) {
        printf("[DATASERVICE_TEST]:new_data_service_ctx_en error(file=%s, function=%s, line=%d)\n", __FILE__, __FUNCTION__,
               __LINE__);
        delete_ip_box(ip_list);
        ip_list = NULL;
        free(user_ctx);
        user_ctx = NULL;
        uninit_edge_service_ctx();
        return -1;
    }

    //5.调用data_service_ctx_start函数，启动相关模块，开始从服务端接收数据，非线程安全
    ret = data_service_ctx_start(ctx);
    if (ret < 0) {
        printf("[DATASERVICE_TEST]:data_service_ctx_start error(file=%s, function=%s, line=%d)\n", __FILE__,
               __FUNCTION__, __LINE__);
        delete_ip_box(ip_list);
        ip_list = NULL;
        free(user_ctx);
        user_ctx = NULL;
        delete_data_service_ctx(ctx);
        ctx = NULL;
        uninit_edge_service_ctx();
        return -1;
    }

    //运行60s后退出
    edge_sleep(60000);

    //不使用时记得stop和delete

    //ip_box不使用时是需要释放的
    delete_ip_box(ip_list);

    //6.可以调用data_service_ctx_stop函数暂停接收数据，start和stop的调用要成对，不能在没有调用start的情况下调用stop，也不能再已经start的情况下调用start，非线程安全
    data_service_ctx_stop(ctx);
    //7.delete_data_service_ctx;释放new_data_service_ctx_en占用的资源，退出时需要调用，需要在调用stop之后调用该函数，非线程安全
    delete_data_service_ctx(ctx);
    //8.uninit_edge_service_ctx;释放init_edge_service_ctx占用的资源，退出时需要调用，需要在调用delete_data_service_ctx之后调用该函数，非线程安全
    uninit_edge_service_ctx();

    free(user_ctx);

    return 0;
}

