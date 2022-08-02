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

void print_data_subscribe_msg(struct DataSubscribeStruct *dss_ptr) {
    printf("---------------------->\n");
    if (dss_ptr == NULL) {
        printf("dss_ptr == NULL\n");
        return;
    }
    printf("dss_ptr->point_count=%d\n", dss_ptr->point_count);
    int ii = 0;
    for (ii = 0; ii < dss_ptr->point_count; ii++) {
        printf("*******>\n");
        printf("dss_ptr->points[%d].orgid=%s\n", ii, dss_ptr->points[ii].orgid);
        printf("dss_ptr->points[%d].modelid=%s\n", ii, dss_ptr->points[ii].modelid);
        printf("dss_ptr->points[%d].assetid=%s\n", ii, dss_ptr->points[ii].assetid);
        printf("dss_ptr->points[%d].collectdeviceid=%s\n", ii, dss_ptr->points[ii].collectdeviceid);
        printf("dss_ptr->points[%d].pointid=%s\n", ii, dss_ptr->points[ii].pointid);
        printf("dss_ptr->points[%d].time=%lld\n", ii, (long long int) (dss_ptr->points[ii].time));
        printf("dss_ptr->points[%d].value=%s\n", ii, dss_ptr->points[ii].value);
        printf("dss_ptr->points[%d].quality=%d\n", ii, dss_ptr->points[ii].quality);
        printf("dss_ptr->points[%d].dq=%lld\n", ii, (long long int) (dss_ptr->points[ii].dq));
        printf("dss_ptr->points[%d].modelpath=%s\n", ii, dss_ptr->points[ii].modelpath);
        printf("dss_ptr->points[%d].policytype=%s\n", ii, dss_ptr->points[ii].policytype);
        printf("dss_ptr->points[%d].signaltype=%s\n", ii, dss_ptr->points[ii].signaltype);
        printf("dss_ptr->points[%d].hasquality=%d\n", ii, dss_ptr->points[ii].hasquality);
        printf("dss_ptr->points[%d].datatype=%s\n", ii, dss_ptr->points[ii].datatype);
        printf("dss_ptr->points[%d].subdatatype=%s\n", ii, dss_ptr->points[ii].subdatatype);
        printf("dss_ptr->points[%d].attr=%s\n", ii, dss_ptr->points[ii].attr);
        printf("dss_ptr->points[%d].usingoem=%d\n", ii, dss_ptr->points[ii].usingoem);
        printf("dss_ptr->points[%d].oemtime=%lld\n", ii, (long long int) (dss_ptr->points[ii].oemtime));
        printf("dss_ptr->points[%d].pointtype=%d\n", ii, dss_ptr->points[ii].pointtype);
        printf("<*******\n");
    }

    printf("<----------------------\n\n");
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

void print_control_response_msg(struct ControlResponseStruct *crs_ptr) {
    printf("---------------------->\n");
    if (crs_ptr == NULL) {
        printf("crs_ptr == NULL\n");
        return;
    }
    printf("crs_ptr->point_count=%d\n", crs_ptr->point_count);
    int ii = 0;
    for (ii = 0; ii < crs_ptr->point_count; ii++) {
        printf("*******>\n");
        printf("crs_ptr->points[%d].requestid=%s\n", ii, crs_ptr->points[ii].requestid);
        printf("crs_ptr->points[%d].messageid=%s\n", ii, crs_ptr->points[ii].messageid);
        printf("crs_ptr->points[%d].requestmethod=%s\n", ii,
               crs_ptr->points[ii].requestmethod);
        printf("crs_ptr->points[%d].calltype=%s\n", ii, crs_ptr->points[ii].calltype);
        printf("crs_ptr->points[%d].controlchannelid=%s\n", ii,
               crs_ptr->points[ii].controlchannelid);
        printf("crs_ptr->points[%d].productkey=%s\n", ii, crs_ptr->points[ii].productkey);
        printf("crs_ptr->points[%d].devicekey=%s\n", ii, crs_ptr->points[ii].devicekey);
        printf("crs_ptr->points[%d].assetid=%s\n", ii, crs_ptr->points[ii].assetid);
        printf("crs_ptr->points[%d].servicename=%s\n", ii, crs_ptr->points[ii].servicename);
        printf("crs_ptr->points[%d].serviceid=%s\n", ii, crs_ptr->points[ii].serviceid);
        printf("crs_ptr->points[%d].callbackurl=%s\n", ii, crs_ptr->points[ii].callbackurl);
        printf("crs_ptr->points[%d].inputdata=%s\n", ii, crs_ptr->points[ii].inputdata);
        printf("crs_ptr->points[%d].outputdata=%s\n", ii, crs_ptr->points[ii].outputdata);
        printf("crs_ptr->points[%d].status=%lld\n", ii, (long long int) (crs_ptr->points[ii].status));
        printf("crs_ptr->points[%d].msg=%s\n", ii, crs_ptr->points[ii].msg);
        printf("crs_ptr->points[%d].submsg=%s\n", ii, crs_ptr->points[ii].submsg);
        printf("crs_ptr->points[%d].timeout=%lld\n", ii, (long long int) (crs_ptr->points[ii].timeout));
        printf("crs_ptr->points[%d].gmtservicerequest=%lld\n", ii,
               (long long int) (crs_ptr->points[ii].gmtservicerequest));
        printf("crs_ptr->points[%d].gmtservicereply=%lld\n", ii,
               (long long int) (crs_ptr->points[ii].gmtservicereply));
        printf("crs_ptr->points[%d].gmtdevicereply=%lld\n", ii,
               (long long int) (crs_ptr->points[ii].gmtdevicereply));
        printf("crs_ptr->points[%d].attr=%s\n", ii, crs_ptr->points[ii].attr);
        printf("<*******\n");
    }

    printf("<----------------------\n\n");
}

void print_set_measurepoint_response_msg(struct SetMeasurepointResponseStruct *smrs_ptr) {
    printf("---------------------->\n");
    if (smrs_ptr == NULL) {
        printf("smrs_ptr == NULL\n");
        return;
    }
    printf("smrs_ptr->point_count=%d\n", smrs_ptr->point_count);
    int ii = 0;
    for (ii = 0; ii < smrs_ptr->point_count; ii++) {
        printf("*******>\n");
        printf("smrs_ptr->points[%d].requestid=%s\n", ii, smrs_ptr->points[ii].requestid);
        printf("smrs_ptr->points[%d].orgid=%s\n", ii, smrs_ptr->points[ii].orgid);
        printf("smrs_ptr->points[%d].calltype=%s\n", ii, smrs_ptr->points[ii].calltype);
        printf("smrs_ptr->points[%d].setmeasurepointchannelid=%s\n", ii,
               smrs_ptr->points[ii].setmeasurepointchannelid);
        printf("smrs_ptr->points[%d].productkey=%s\n", ii, smrs_ptr->points[ii].productkey);
        printf("smrs_ptr->points[%d].devicekey=%s\n", ii, smrs_ptr->points[ii].devicekey);
        printf("smrs_ptr->points[%d].assetid=%s\n", ii, smrs_ptr->points[ii].assetid);
        printf("smrs_ptr->points[%d].measurepointid=%s\n", ii,
               smrs_ptr->points[ii].measurepointid);
        printf("smrs_ptr->points[%d].callbackurl=%s\n", ii, smrs_ptr->points[ii].callbackurl);
        printf("smrs_ptr->points[%d].inputdata=%s\n", ii, smrs_ptr->points[ii].inputdata);
        printf("smrs_ptr->points[%d].status=%lld\n", ii, (long long int) (smrs_ptr->points[ii].status));
        printf("smrs_ptr->points[%d].msg=%s\n", ii, smrs_ptr->points[ii].msg);
        printf("smrs_ptr->points[%d].submsg=%s\n", ii, smrs_ptr->points[ii].submsg);
        printf("smrs_ptr->points[%d].timeout=%lld\n", ii, (long long int) (smrs_ptr->points[ii].timeout));
        printf("smrs_ptr->points[%d].gmtsetmeasurepointrequest=%lld\n", ii,
               (long long int) (smrs_ptr->points[ii].gmtsetmeasurepointrequest));
        printf("smrs_ptr->points[%d].gmtsetmeasurepointreply=%lld\n", ii,
               (long long int) (smrs_ptr->points[ii].gmtsetmeasurepointreply));
        printf("smrs_ptr->points[%d].attr=%s\n", ii, smrs_ptr->points[ii].attr);
        printf("<*******\n");
    }

    printf("<----------------------\n\n");
}

void print_custom_msg(char *custom_ptr, int len) {
    printf("---------------------->\n");
    if (custom_ptr == NULL) {
        printf("custom_ptr == NULL\n");
        return;
    }

    int ii = 0;
    printf("hex:");
    for (ii = 0; ii < len; ii++) {
        printf("%hhx ", custom_ptr[ii]);
    }
    printf("\n");
    printf("str:%s\n", custom_ptr);
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
        case TOPIC_TYPE_DATA_SUBSCRIBE:
        case TOPIC_TYPE_DATA_SUBSCRIBE_ALL:
            print_data_subscribe_msg((struct DataSubscribeStruct *) (msg->msg));
            break;
        case TOPIC_TYPE_CONTROL_RESPONSE:
            print_control_response_msg((struct ControlResponseStruct *) (msg->msg));
            break;
        case TOPIC_TYPE_SET_MEASUREPOINT_RESPONSE:
            print_set_measurepoint_response_msg((struct SetMeasurepointResponseStruct *) (msg->msg));
            break;
        case TOPIC_TYPE_SIMPLE_DATA_SUBSCRIBE:
            print_simple_data_subscribe_msg((struct SimpleDataSubscribeStruct *) (msg->msg));
            break;
        case TOPIC_TYPE_CUSTOM:
            print_custom_msg((char *) (msg->msg), msg->msg_len);
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

//    edge_sleep(20000);

    while (1) {
        edge_sleep(10000);
    }

    //测试反复stop和start
//    while (1) {
//        edge_sleep(10000);
//        data_service_ctx_stop(ctx);
//        printf("[DATASERVICE_TEST]:stopped xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx(file=%s, function=%s, line=%d)\n",
//               __FILE__,
//               __FUNCTION__, __LINE__);
//        edge_sleep(10000);
//        ret = data_service_ctx_start(ctx);
//        if (ret < 0) {
//            printf("[DATASERVICE_TEST]:data_service_ctx_start error(file=%s, function=%s, line=%d)\n", __FILE__,
//                   __FUNCTION__, __LINE__);
//            delete_ip_box(ip_list);
//            ip_list = NULL;
//            delete_data_service_ctx(ctx);
//            ctx = NULL;
//            uninit_edge_service_ctx();
//            free(user_ctx);
//            user_ctx = NULL;
//            return -1;
//        }
//        printf("[DATASERVICE_TEST]:started xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx(file=%s, function=%s, line=%d)\n",
//               __FILE__,
//               __FUNCTION__, __LINE__);
//    }

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

