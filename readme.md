# 通过 C SDK 使用 EnOS Edge 服务
EnOS Edge 支持使用 C SDK 订阅并发布数据，以下为基本操作步骤。
## 安装与编译

<a name="installcmake"></a>

### 安装
使用 EnOS Edge 数据订阅的 C 语言 SDK 需要先安装 [CMake](https://cmake.org/)。

<a name="compile"></a>
### 编译

- 调试版本

  ```
  cmake -D PLATFORM_DEFINE=linux_x86_normal_64_local -D DEBUG_MODE=ON .
  make //使用 "make VERBOSE=1" 获取更多信息
  ```

- 发布版本

  ```
  cmake -D PLATFORM_DEFINE=linux_x86_normal_64_local -D DEBUG_MODE=OFF .
  make //使用 "make VERBOSE=1" 获取更多信息
  ```

### 公共参数说明

| 参数                              | 说明                                                         |
| :-------------------------------- | :----------------------------------------------------------- |
| dir: dst                          | 编译结果                                                     |
| PLATFORM_DEFINE=<platform_type>   | 选择待构建的平台，格式为 `os_arch_desc_bits_local-or-cross`<br/>例如：PLATFORM_DEFINE=<linux_x86_normal_64_local> |
| -G “xxx”                          | 选择不同 CMake 结果，例如<br/>- G “MSYS Makefiles”: windows msys 文件<br/>- G “MinGW Makefiles”: windows mingw 文件 |
| ./clean_all_cmake_tmp.sh          | 清除所有 CMake tmp 文件和目录（CMake 会自动生成临时文件如 `cmake_install.cmake`, `CMakeCache.txt`, `Makefile`） |
| support/linux_x86_normal_64_local | CentOS_7.4_x64 中预编译的参数                                |
| DATATYPE                          | 包含的数据类型为：<br/>”ARRAY”<br/>”BOOL”<br/>”DATE”<br/>”ENUM”<br/>”INT”<br/>”FLOAT”<br/>”DOUBLE”<br/>”STRUCT”<br/>”STRING”<br/>”TIMESTAMP”<br/>”FILE” |
| SUB_DATATYPE                      | 包含的数据类型为：<br/>”INT”<br/>”FLOAT”<br/>”DOUBLE”<br/>”STRING” |
| POINT_TYPE                        | 包含的数据类型为：<br/>0：模型点类型为 Measurepoint<br/>1：模型点类型为 Event<br/>2：模型点类型为 Attribute<br/>3：模型点类型为 Service |
| POLICY_TYPE                       | 包含的数据类型为：<br/>”RAW”<br/>”NORMALIZED”<br/>”DI”<br/>”GENERIC”<br/>”PI” |
| SIGNAL_TYPE                       | 包含的数据类型为：<br/>”AI”<br/>”DI”<br/>”PI”<br/>”Generic”<br/>”UNKNOWN” |
| CALL_TYPE                         | 包含的数据类型为：<br/>”SYNC”<br/>”ASYNC”                    |


<a name="sample-code"></a>

## 使用编译好的库

假设编译结果 dst 路径为 `/home/program/edge-service-sdk-c/dst`，待编译的文件路径为 `test.cpp`。

```
//Include header files in this way
#ifdef __cplusplus
extern "C" {
#endif
#include "edge_service_c_api/edge_service_c_api.h"
#ifdef __cplusplus
}
#endif

//Environment variable settings
if [ $LD_LIBRARY_PATH ]
then
    export LD_LIBRARY_PATH=/home/program/edge-service-sdk-c/dst/lib:/home/program/edge-service-sdk-c/dst/lib/support:/home/program/edge-service-sdk-c/dst/lib/3rd:$LD_LIBRARY_PATH
else
    export LD_LIBRARY_PATH=/home/program/edge-service-sdk-c/dst/lib:/home/program/edge-service-sdk-c/dst/lib/support:/home/program/edge-service-sdk-c/dst/lib/3rd
fi

//compile
g++ -Wno-write-strings -g -o test test.cpp -I/home/program/edge-service-sdk-c/dst/include -I/home/program/edge-service-sdk-c/dst/include/support -I/home/program/edge-service-sdk-c/dst/include/support -I/home/program/edge-service-sdk-c/dst/include/3rd -I/home/program/edge-service-sdk-c/dst/include/support/libev -I/home/program/edge-service-sdk-c/dst/include/3rd/evbuffsock -L/home/program/edge-service-sdk-c/dst/lib -L/home/program/edge-service-sdk-c/dst/lib/support -L/home/program/edge-service-sdk-c/dst/lib/3rd -lev -lcurl -ljansson -lprotobuf-c -lcjson -lnsq -levbuffsock -ledge_service_c_api
```

## 代码示例

以下为使用 C SDK 的代码示例：

<a name="subscribe data"></a>
### 订阅数据
#### 消费实时数据代码示例

```
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
        default:
            printf("[DATASERVICE]:unsupported msg->topic_type=%d(file=%s, function=%s, line=%d)\n",
                   msg->topic_type, __FILE__, __FUNCTION__, __LINE__);
            break;
    }

    //注意用完后要释放
    delete_data_service_msg(msg);
    return;
}

int main(int argc, char *argv[]) {
    int ret = 0;
    struct IPBox *ip_list = NULL;
    char topic_name[60];
    memset(topic_name, 0, sizeof(topic_name));
    char consumerGroup[60];
    memset(consumerGroup, 0, sizeof(consumerGroup));

    //需要连接的ip
    ip_list = add_ip_to_ip_box(ip_list, "127.0.0.1");
    //订阅的topic类型，实时数据订阅可以选TOPIC_TYPE_DATA_SUBSCRIBE或TOPIC_TYPE_AUTO
    int topic_type = TOPIC_TYPE_DATA_SUBSCRIBE;
    //端口
    int port = EDGE_DATASERVICE_DEFAULT_PORT;
    //topic名字，实时数据订阅的topic都是以DATASVC.SUB.开头
    snprintf(topic_name, sizeof(topic_name), "%s", "DATASVC.SUB.APP.SUBTEST");
    //consumerGroup，非必填，如果consumerGroup是NULL，则默认为default，详细说明参考new_data_service_ctx函数注释
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

    //4.调用new_data_service_ctx函数，初始化必要的上下文，线程安全，注意当只填一个IP时，无论这个IP是主还是备，都会去向这个IP订阅数据，填多个IP时，会根据主备情况自动切换
    ctx = new_data_service_ctx(
            ip_list,
            port,
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
        printf("[DATASERVICE_TEST]:new_data_service_ctx error(file=%s, function=%s, line=%d)\n", __FILE__, __FUNCTION__,
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

    //不使用时需要stop和delete

    //ip_box不使用时需要释放
    delete_ip_box(ip_list);

    //6.可以调用data_service_ctx_stop函数暂停接收数据，start和stop的调用要成对，不能在没有调用start的情况下调用stop，也不能再已经start的情况下调用start，非线程安全
    data_service_ctx_stop(ctx);
    //7.delete_data_service_ctx;释放new_data_service_ctx占用的资源，退出时需要调用，需要在调用stop之后调用该函数，非线程安全
    delete_data_service_ctx(ctx);
    //8.uninit_edge_service_ctx;释放init_edge_service_ctx占用的资源，退出时需要调用，需要在调用delete_data_service_ctx之后调用该函数，非线程安全
    uninit_edge_service_ctx();

    free(user_ctx);

    return 0;
}
```

#### 消费设备异步控制的返回结果

```
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
        case TOPIC_TYPE_CONTROL_RESPONSE:
            print_control_response_msg((struct ControlResponseStruct *) (msg->msg));
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

int main(int argc, char *argv[]) {
    int ret = 0;
    struct IPBox *ip_list = NULL;
    char topic_name[60];
    memset(topic_name, 0, sizeof(topic_name));
    char consumerGroup[60];
    memset(consumerGroup, 0, sizeof(consumerGroup));

    //需要连接的ip
    ip_list = add_ip_to_ip_box(ip_list, "127.0.0.1");
    //订阅的topic类型，控制反较数据订阅可以选TOPIC_TYPE_CONTROL_RESPONSE或TOPIC_TYPE_AUTO
    int topic_type = TOPIC_TYPE_CONTROL_RESPONSE;
    //端口
    int port = EDGE_DATASERVICE_DEFAULT_PORT;
    //topic名字，控制反较数据订阅的topic都是以DATASVC.CONTROL.开头
    snprintf(topic_name, sizeof(topic_name), "%s", "DATASVC.CONTROL.APP.SUBTEST");
    //consumerGroup，非必填，如果consumerGroup是NULL，则默认为default，详细说明参考new_data_service_ctx函数注释
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

    //4.调用new_data_service_ctx函数，初始化必要的上下文，线程安全，注意当只填一个IP时，无论这个IP是主还是备，都会去向这个IP订阅数据，填多个IP时，会根据主备情况自动切换
    ctx = new_data_service_ctx(
            ip_list,
            port,
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
        printf("[DATASERVICE_TEST]:new_data_service_ctx error(file=%s, function=%s, line=%d)\n", __FILE__, __FUNCTION__,
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

    //不使用时需要stop和delete

    //ip_box不使用时需要释放
    delete_ip_box(ip_list);

    //6.可以调用data_service_ctx_stop函数暂停接收数据，start和stop的调用要成对，不能在没有调用start的情况下调用stop，也不能再已经start的情况下调用start，非线程安全
    data_service_ctx_stop(ctx);
    //7.delete_data_service_ctx;释放new_data_service_ctx占用的资源，退出时需要调用，需要在调用stop之后调用该函数，非线程安全
    delete_data_service_ctx(ctx);
    //8.uninit_edge_service_ctx;释放init_edge_service_ctx占用的资源，退出时需要调用，需要在调用delete_data_service_ctx之后调用该函数，非线程安全
    uninit_edge_service_ctx();

    free(user_ctx);

    return 0;
}
```

#### 消费设备异步设点的返回结果

```
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
        case TOPIC_TYPE_SET_MEASUREPOINT_RESPONSE:
            print_set_measurepoint_response_msg((struct SetMeasurepointResponseStruct *) (msg->msg));
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

int main(int argc, char *argv[]) {
    int ret = 0;
    struct IPBox *ip_list = NULL;
    char topic_name[60];
    memset(topic_name, 0, sizeof(topic_name));
    char consumerGroup[60];
    memset(consumerGroup, 0, sizeof(consumerGroup));

    //需要连接的ip
    ip_list = add_ip_to_ip_box(ip_list, "127.0.0.1");
    //订阅的topic类型，实时数据订阅可以选TOPIC_TYPE_SET_MEASUREPOINT_RESPONSE或TOPIC_TYPE_AUTO
    int topic_type = TOPIC_TYPE_SET_MEASUREPOINT_RESPONSE;
    //端口
    int port = EDGE_DATASERVICE_DEFAULT_PORT;
    //topic名字，写值反较数据订阅的topic都是以DATASVC.SET.开头
    snprintf(topic_name, sizeof(topic_name), "%s", "DATASVC.SET.APP.SUBTEST");
    //consumerGroup，非必填，如果consumerGroup是NULL，则默认为default，详细说明参考new_data_service_ctx函数注释
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

    //4.调用new_data_service_ctx函数，初始化必要的上下文，线程安全，注意当只填一个IP时，无论这个IP是主还是备，都会去向这个IP订阅数据，填多个IP时，会根据主备情况自动切换
    ctx = new_data_service_ctx(
            ip_list,
            port,
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
        printf("[DATASERVICE_TEST]:new_data_service_ctx error(file=%s, function=%s, line=%d)\n", __FILE__, __FUNCTION__,
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

    //不使用时需要stop和delete

    //ip_box不使用时需要释放
    delete_ip_box(ip_list);

    //6.可以调用data_service_ctx_stop函数暂停接收数据，start和stop的调用要成对，不能在没有调用start的情况下调用stop，也不能再已经start的情况下调用start，非线程安全
    data_service_ctx_stop(ctx);
    //7.delete_data_service_ctx;释放new_data_service_ctx占用的资源，退出时需要调用，需要在调用stop之后调用该函数，非线程安全
    delete_data_service_ctx(ctx);
    //8.uninit_edge_service_ctx;释放init_edge_service_ctx占用的资源，退出时需要调用，需要在调用delete_data_service_ctx之后调用该函数，非线程安全
    uninit_edge_service_ctx();

    free(user_ctx);

    return 0;
}
```

#### 消费自定义订阅数据示例

```
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

int main(int argc, char *argv[]) {
    int ret = 0;
    struct IPBox *ip_list = NULL;
    char topic_name[60];
    memset(topic_name, 0, sizeof(topic_name));
    char consumerGroup[60];
    memset(consumerGroup, 0, sizeof(consumerGroup));

    //需要连接的ip
    ip_list = add_ip_to_ip_box(ip_list, "127.0.0.1");
    //订阅的topic类型，实时数据订阅可以选TOPIC_TYPE_CUSTOM或TOPIC_TYPE_AUTO，custom类型不会解析订阅到的数据，会原样返回给调用方
    int topic_type = TOPIC_TYPE_CUSTOM;
    //端口
    int port = EDGE_DATASERVICE_DEFAULT_PORT;
    //topic名字，custom订阅方式时，可以订阅任意topic，包括实时数据、控制反较、写值反较、其他数据
    snprintf(topic_name, sizeof(topic_name), "%s", "custom_topic");
    //consumerGroup，非必填，如果consumerGroup是NULL，则默认为default，详细说明参考new_data_service_ctx函数注释
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

    //4.调用new_data_service_ctx函数，初始化必要的上下文，线程安全，注意当只填一个IP时，无论这个IP是主还是备，都会去向这个IP订阅数据，填多个IP时，会根据主备情况自动切换
    ctx = new_data_service_ctx(
            ip_list,
            port,
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
        printf("[DATASERVICE_TEST]:new_data_service_ctx error(file=%s, function=%s, line=%d)\n", __FILE__, __FUNCTION__,
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

    //不使用时需要stop和delete

    //ip_box不使用时需要释放
    delete_ip_box(ip_list);

    //6.可以调用data_service_ctx_stop函数暂停接收数据，start和stop的调用要成对，不能在没有调用start的情况下调用stop，也不能再已经start的情况下调用start，非线程安全
    data_service_ctx_stop(ctx);
    //7.delete_data_service_ctx;释放new_data_service_ctx占用的资源，退出时需要调用，需要在调用stop之后调用该函数，非线程安全
    delete_data_service_ctx(ctx);
    //8.uninit_edge_service_ctx;释放init_edge_service_ctx占用的资源，退出时需要调用，需要在调用delete_data_service_ctx之后调用该函数，非线程安全
    uninit_edge_service_ctx();

    free(user_ctx);

    return 0;
}
```

<a name="publish data"></a>
### 发布数据
```
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
    printf("usage:<%s> <-topic topic_name> [-port 5150]  [-is_fast is_fast] <-ip 1.1.1.1 2.2.2.2 ...> <-content content>\n",
           name);
    printf("-is_fast 0 ====> normal\n");
    printf("-is_fast 1 ====> fast\n");
    printf("is_fast default is 0\n");
    printf("port default value is 5150\n");
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
    int port_flag = 0;
    int content_flag = 0;
    int is_fast_flag = 0;
    struct IPBox *ip_list = NULL;
    char topic_name[128];
    memset(topic_name, 0, sizeof(topic_name));
    char content_input[1024];
    memset(content_input, 0, sizeof(content_input));
    int is_fast = 0;
    int port = EDGE_DATASERVICE_DEFAULT_PORT;
    for (ii = 1; ii < argc; ii++) {
        if (strcmp(argv[ii], "-ip") == 0) {
            topic_flag = 0;
            port_flag = 0;
            ip_flag = 1;
            content_flag = 0;
            is_fast_flag = 0;
            continue;
        }

        if (strcmp(argv[ii], "-content") == 0) {
            topic_flag = 0;
            port_flag = 0;
            ip_flag = 0;
            content_flag = 1;
            is_fast_flag = 0;
            continue;
        }

        if (strcmp(argv[ii], "-topic") == 0) {
            topic_flag = 1;
            port_flag = 0;
            ip_flag = 0;
            content_flag = 0;
            is_fast_flag = 0;
            continue;
        }

        if (strcmp(argv[ii], "-port") == 0) {
            topic_flag = 0;
            port_flag = 1;
            ip_flag = 0;
            content_flag = 0;
            is_fast_flag = 0;
            continue;
        }

        if (strcmp(argv[ii], "-is_fast") == 0) {
            topic_flag = 0;
            port_flag = 0;
            ip_flag = 0;
            content_flag = 0;
            is_fast_flag = 1;
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

        if (content_flag == 1) {
            snprintf(content_input, sizeof(content_input), "%s", argv[ii]);
            continue;
        }

        if (port_flag == 1) {
            sscanf(argv[ii], "%d", &port);
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

    if (strlen(content_input) <= 0) {
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

    //4.调用new_data_service_ctx_for_publish函数，初始化必要的上下文，线程安全，注意当只填一个IP时，无论这个IP是主还是备，都会去连接这个IP，填多个IP时，会根据主备情况自动切换
    ctx = new_data_service_ctx_for_publish(
            ip_list,
            port,
            "aaa",
            "bbb",
            1,
            is_fast,
            user_ctx,
            connect_callback,
            close_callback
    );

    if (ctx == NULL) {
        printf("[DATASERVICE_TEST]:new_data_service_ctx_for_publish error(file=%s, function=%s, line=%d)\n", __FILE__,
               __FUNCTION__,
               __LINE__);
        delete_ip_box(ip_list);
        ip_list = NULL;
        free(user_ctx);
        user_ctx = NULL;
        uninit_edge_service_ctx();
        return -1;
    }

    //5.调用data_service_ctx_start函数，启动相关模块，建立连接，与服务端进行应用层协商，非线程安全
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

    if (is_fast != 0) {
        edge_sleep(3000);//等待连接及初始化成功

        //6.发送数据，线程安全(connect_callback触发之前，调用data_service_ctx_pub_fast发送将会发送失败)
        ret = data_service_ctx_pub_fast(ctx, topic_name, content_input, strlen(content_input));
        if (ret < 0) {
            printf("[DATASERVICE_TEST]:data_service_ctx_pub_fast error, content=%s(file=%s, function=%s, line=%d)\n",
                   content_input, __FILE__,
                   __FUNCTION__, __LINE__);
        } else {
            printf("[DATASERVICE_TEST]:data_service_ctx_pub_fast success, content=%s(file=%s, function=%s, line=%d)\n",
                   content_input, __FILE__,
                   __FUNCTION__, __LINE__);
        }

        edge_sleep(3000);//等待连接及初始化成功
    } else {
        //6.发送数据，线程安全
        ret = data_service_ctx_pub_normal(ctx, topic_name, content_input, strlen(content_input), 10000);
        if (ret < 0) {
            printf("[DATASERVICE_TEST]:data_service_ctx_pub_normal error, content=%s(file=%s, function=%s, line=%d)\n",
                   content_input, __FILE__,
                   __FUNCTION__, __LINE__);
        } else {
            printf("[DATASERVICE_TEST]:data_service_ctx_pub_normal success, content=%s(file=%s, function=%s, line=%d)\n",
                   content_input, __FILE__,
                   __FUNCTION__, __LINE__);
        }
    }

    //不使用时需要stop和delete
    //ip_box不使用时需要释放
    delete_ip_box(ip_list);

    //7.可以调用data_service_ctx_stop函数停止相关模块，断开连接，start和stop的调用要成对，不能在没有调用start的情况下调用stop，也不能再已经start的情况下调用start，非线程安全，需要注意的是，stop会立即断开tcp连接，还在tcp发送缓存中的数据将会丢失
    data_service_ctx_stop(ctx);
    //8.delete_data_service_ctx;释放new_data_service_ctx占用的资源，退出时需要调用，需要在调用stop之后调用该函数，非线程安全
    delete_data_service_ctx(ctx);
    //9.uninit_edge_service_ctx;释放init_edge_service_ctx占用的资源，退出时需要调用，需要在调用delete_data_service_ctx之后调用该函数，非线程安全
    uninit_edge_service_ctx();

    free(user_ctx);

    return 0;
}

```

<a name="how-to"></a>

## 相关信息

点击 [数据订阅概述](https://support.envisioniot.com/docs/enos-edge/zh_CN/preview/learn/data_subscription.html)， 进一步了解 EnOS Edge 数据订阅服务。

