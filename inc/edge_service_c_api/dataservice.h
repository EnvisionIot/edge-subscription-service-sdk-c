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
// Created by yang.zhang4 on 2019/10/8.
//

#ifndef EDGE_SERVICE_SDK_C_DATASERVICE_H
#define EDGE_SERVICE_SDK_C_DATASERVICE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "edge_service_c_api/edge_common.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "stdarg.h"
#include "unistd.h"
#include "pthread.h"
#include "signal.h"
#include "errno.h"
#include "fcntl.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "sys/select.h"
#include "sys/stat.h"
#include "sys/file.h"
#include "sys/time.h"
#include "sys/ioctl.h"
#include "netinet/in.h"
#include "netinet/tcp.h"
#include "arpa/inet.h"

#define EDGE_DATASERVICE_DEFAULT_TCP_KEEPIDLE 10//tcp keepalive空闲时间
#define EDGE_DATASERVICE_DEFAULT_TCP_KEEPINTVL 4//tcp keepalive间隔时间
#define EDGE_DATASERVICE_DEFAULT_TCP_KEEPCNT 3//tcp keepalive探测次数
#define EDGE_DATASERVICE_TCP_CONNECT_TIMEOUT_MS 8000 //单位毫秒，tcp连接超时时间
#define EDGE_DATASERVICE_MAX_NORMAL_SEND_TIMEOUT_MS 60000 //单位毫秒，normal send时最大超时时间
#define EDGE_DATASERVICE_BUF_SMALL_SIZE 8192 //一次recv/send调用所用buf大小
#define EDGE_DATASERVICE_BUF_BIG_SIZE 1024*1024 //一次完整交互消息所用buf最大大小
#define EDGE_DATASERVICE_COMMAND_RECV_TIMEOUT 8 //单位秒，nsq应用层初始化超时时间
#define EDGE_DATASERVICE_SUBSCRIBE_ALL_TOPIC "DATASVC.SUBALL"
#define EDGE_DATASERVICE_DATA_SUBSCRIBE_TOPIC_PREFIX "DATASVC.SUB."
#define EDGE_DATASERVICE_DATA_SUBSCRIBE_ALL_TOPIC_PREFIX "DATASVC.SUBALL."
#define EDGE_DATASERVICE_CONTROL_RESPONSE_TOPIC_PREFIX "DATASVC.CONTROL."
#define EDGE_DATASERVICE_SET_MEASUREPOINT_RESPONSE_TOPIC_PREFIX "DATASVC.SET."
#define EDGE_DATASERVICE_DEFAULT_CONSUMER_GROUP "default"
#define EDGE_DATASERVICE_MAX_CHANNEL_ID_LEN 60
#define EDGE_DATASERVICE_MASTER_SLAVE_THREAD_CHECK_PERIOD 10000 //单位毫秒，检测主备的那个线程请求主备的周期
#define EDGE_DATASERVICE_SERVICE_CHECK_PERIOD 6000 //单位毫秒，检测服务端是否正常的检测周期
#define EDGE_DATASERVICE_SERVICE_CHECK_MAX_COUNT 3 //单位毫秒，连续N次服务端不返回或返回失败就认为需要重连

enum EDGE_SERVICE_C_API_DLL_EXPORT TOPIC_TYPE {
    TOPIC_TYPE_AUTO = 0,
    TOPIC_TYPE_DATA_SUBSCRIBE,
    TOPIC_TYPE_DATA_SUBSCRIBE_ALL,
    TOPIC_TYPE_CONTROL_RESPONSE,
    TOPIC_TYPE_SET_MEASUREPOINT_RESPONSE,
    TOPIC_TYPE_CUSTOM
};

enum EDGE_SERVICE_C_API_DLL_EXPORT SUBSCRIBE_MODE {
    SUBSCRIBE_MODE_NSQD = 0,
    SUBSCRIBE_MODE_NSQLOOKUPD
};

enum EDGE_SERVICE_C_API_DLL_EXPORT CTX_TYPE {
    CTX_TYPE_ONLY_CONSUMER = 0,
    CTX_TYPE_ONLY_PUBLISH
};

enum EDGE_SERVICE_C_API_DLL_EXPORT RUNNING_STATUS {
    RUNNING_STATUS_STOP = 0,
    RUNNING_STATUS_START
};

enum EDGE_SERVICE_C_API_DLL_EXPORT NSQ_INIT_STATUS {
    NSQ_INIT_STATUS_NOTHING = 0,
    NSQ_INIT_STATUS_WAIT_SUB_RESPONSE,
    NSQ_INIT_STATUS_SUB_SUCCESS,
    NSQ_INIT_STATUS_INIT_SUCCESS
};

enum EDGE_SERVICE_C_API_DLL_EXPORT NSQ_FRAME_TYPE {
    NSQ_FRAME_TYPE_RESPONSE = 0,
    NSQ_FRAME_TYPE_ERROR = 1,
    NSQ_FRAME_TYPE_MESSAGE = 2
};

#pragma pack(push)
#pragma pack(1)//此处结构要直接组包，所以1字节对齐

//nsq包头
struct nsq_packet_header {
    int size;
    int frame_type;
};

//nsq消息头
struct nsq_message_header {
    long long int timestamp_ns;
    unsigned short int attempts;
    char message_id[16];
};

#pragma pack(pop)//还原成默认对齐方式

//一条nsq消息
struct nsq_packet_struct {
    int frame_type;

    //frame_type为NSQ_FRAME_TYPE_MESSAGE时有效-->
    long long int timestamp_ns;
    unsigned short int attempts;
    char message_id[16];
    //<--frame_type为NSQ_FRAME_TYPE_MESSAGE时有效

    int content_len;
    char *content;
};

struct EDGE_SERVICE_C_API_DLL_EXPORT DataSubscribeSubStruct {
    char *orgid;
    char *modelid;
    char *assetid;
    char *collectdeviceid;
    char *pointid;
    int64_t time;
    char *value;
    int32_t quality;
    int64_t dq;
    char *modelpath;
    char *policytype;
    char *signaltype;
    int hasquality;
    char *datatype;
    char *subdatatype;
    char *attr;
    int32_t usingoem;
    int64_t oemtime;
    int32_t pointtype;
};

//订阅点信息结构
struct DataSubscribeStruct {
    int point_count;
    struct DataSubscribeSubStruct *points;
};

struct EDGE_SERVICE_C_API_DLL_EXPORT ControlResponseSubStruct {
    char *requestid;
    char *messageid;
    char *requestmethod;
    char *calltype;
    char *controlchannelid;
    char *productkey;
    char *devicekey;
    char *assetid;
    char *servicename;
    char *serviceid;
    char *callbackurl;
    char *inputdata;
    char *outputdata;
    int64_t status;
    char *msg;
    char *submsg;
    int64_t timeout;
    int64_t gmtservicerequest;
    int64_t gmtservicereply;
    int64_t gmtdevicereply;
    char *attr;
};

//控制响应信息结构
struct EDGE_SERVICE_C_API_DLL_EXPORT ControlResponseStruct {
    int point_count;
    struct ControlResponseSubStruct *points;
};

struct EDGE_SERVICE_C_API_DLL_EXPORT SetMeasurepointResponseSubStruct {
    char *requestid;
    char *orgid;
    char *calltype;
    char *setmeasurepointchannelid;
    char *productkey;
    char *devicekey;
    char *assetid;
    char *measurepointid;
    char *callbackurl;
    char *inputdata;
    int64_t status;
    char *msg;
    char *submsg;
    int64_t timeout;
    int64_t gmtsetmeasurepointrequest;
    int64_t gmtsetmeasurepointreply;
    char *attr;
};

//写值点响应信息结构
struct EDGE_SERVICE_C_API_DLL_EXPORT SetMeasurepointResponseStruct {
    int point_count;
    struct SetMeasurepointResponseSubStruct *points;
};

struct EDGE_SERVICE_C_API_DLL_EXPORT DataServiceMessage {
    int topic_type;
    int msg_len;//topic_type为TOPIC_TYPE_CUSTOM时有意义，其他情况为0
    void *msg;
};

struct EDGE_SERVICE_C_API_DLL_EXPORT DataServiceWorkCtx {
    //recv-->
    int recv_syn_flag;//接收时同步标志
    int has_recv_bytes;//已经接收到的字节数
    int should_recv_bytes;//还需要接收的字节数
    char recv_buf_small[EDGE_DATASERVICE_BUF_SMALL_SIZE];//一次recv调用所用buf
    int recv_buf_big_max;
    char *recv_buf_big;//一次完整交互报文所用buf
    //<--recv
};

//struct EDGE_SERVICE_C_API_DLL_EXPORT DataServiceCtx {
//    //输入的信息-->
//    struct IPBox *ip_box;
//    int port;
//    char accessKey[256];
//    char secretKey[256];
//    char channel_id[256];
//    char consumerGroup[256];
//    int topic_type;
//    int need_auto_reconnect;
//    void *user_ctx;
//
//    void (*connect_callback)(void *work_ctx, char *channel_id, void *user_ctx);
//
//    void (*close_callback)(void *work_ctx, char *channel_id, void *user_ctx);
//
//    void (*msg_callback)(void *work_ctx, char *channel_id, struct DataServiceMessage *msg,
//                         void *user_ctx);
//    //<--输入的信息
//
//    volatile char start_flag;//表示程序是否为首次start
//    volatile char start_status;//表示程序当前是start还是stop
//    volatile char stop_success;//程序调用stop后，是否停止成功
//    //默认为0使用直连nsqd的方式，其他值是连接nsqlookupd
//    int subscribe_mode;
//    //当topic_type为TOPIC_TYPE_AUTO时，根据订阅的channel_id的名字，解析出的具体订阅类型，是实时数据还是遥控反较还是写值反较
//    int topic_type_actual;
//    //根据is_subscribe_all和channel_id解析出的实际使用的topic名字和channel名字
//    char topic[256];
//    char channel[256];
//
//    //实际使用的ip，这里会综合主备机的情况和输入的ip综合考虑
//    struct IPBox *ip_box_used;
//    //这个标志是pth_master_slave_check_job置的，当主备情况发生变化时，需要断开当前的连接，重新连接新的主机IP
//    int need_reinit;
//    //操作ip_box_used和need_reinit的锁
//    pthread_rwlock_t reinit_rwlock;
//
//
//    //主循环loop
//    struct ev_loop *loop;
//
//    //用于周期检查主备切换后是否需要重新初始化
//    struct ev_timer reinit_check_timer;
//
//    //单独的线程，用于周期性的向配置的IP列表请求主备情况，主备情况发生切换后，need_reinit置为1，并将新的主机IP写入ip_box_used
//    pthread_t pth_master_slave_check_job;
//    //单独的线程，用于周期性的检查服务端是否工作正常，连续N次请求不回复或连续N次请求回复不正常，则需要重连，相当于起一个应用层心跳的效果
//    pthread_t pth_service_check_job;
//    //用于让主循环在单独线程中执行，使data_service_ctx_start不会阻塞
//    pthread_t pth_main_work_job;
//
//    //这个结构里的东西重连时需要重新初始化
//    struct DataServiceWorkCtx *work_ctx;
//
//};

struct EDGE_SERVICE_C_API_DLL_EXPORT DataServiceCtx {
    //输入的信息-->
    struct IPBox *ip_box;
    int port;
    char accessKey[256];
    char secretKey[256];
    char channel_id[256];
    char consumerGroup[256];
    int topic_type;
    int need_auto_reconnect;
    void *user_ctx;

    void (*connect_callback)(void *work_ctx, char *channel_id, void *user_ctx);

    void (*close_callback)(void *work_ctx, char *channel_id, void *user_ctx);

    void (*msg_callback)(void *work_ctx, char *channel_id, struct DataServiceMessage *msg,
                         void *user_ctx);
    //<--输入的信息

    //edge的ha模块的端口
    int ha_port;
    //edge的消息队列服务是否正常的端口
    int service_check_port;
    //nsq client id
    char client_id[32];
    //这个ctx是用来发送还是消费的
    int ctx_type;
    //这个ctx是否是快速发送模式
    int is_fast;
    //当topic_type为TOPIC_TYPE_AUTO时，根据订阅的channel_id的名字，解析出的具体订阅类型，是实时数据还是遥控反较还是写值反较
    int topic_type_actual;
    //根据is_subscribe_all和channel_id解析出的实际使用的topic名字和channel名字
    char topic[256];
    char channel[256];

    //表示程序当前是start还是stop
    int start_status;
    //操作start_status的锁
    pthread_rwlock_t start_status_rwlock;
    //实际使用的ip，这里会综合主备机的情况和输入的ip综合考虑
    struct IPBox *ip_box_used;
    //操作ip_box_used的锁
    pthread_rwlock_t ip_box_used_rwlock;
    //需要断开当前的连接，重新连接（当主备情况发生变化、与服务端连接断开时等情况会置为1）
    int need_reinit;
    //操作need_reinit的锁
    pthread_rwlock_t reinit_rwlock;
    //tcp连接的文件描述符
    int tcp_sock_fd;
    //操作tcp_sock_fd的锁
    pthread_rwlock_t tcp_sock_fd_rwlock;
    //nsq应用层初始化状态(一般tcp链路建立成功后，如果需要订阅消息，需要订阅topic和设置RDY)
    int nsq_init_status;
    //操作nsq_init_success的锁
    pthread_rwlock_t nsq_init_status_rwlock;
    //并行发送时的锁
    pthread_rwlock_t nsq_send_rwlock;

    //recv-->
    int recv_syn_flag;//接收时同步标志
    int has_recv_bytes;//已经接收到的字节数
    int should_recv_bytes;//还需要接收的字节数
    char recv_buf_small[EDGE_DATASERVICE_BUF_SMALL_SIZE];//一次recv调用所用buf
    int recv_buf_big_max;//允许的recv_buf_big的长度上限，一般设为EDGE_DATASERVICE_BUF_BIG_SIZE
    char *recv_buf_big;//一次完整交互报文所用buf
    //<--recv

    //初始化nsq应用层开始时间戳
    struct timeval nsq_init_start_ts;

    //单独的线程，用于周期性的向配置的IP列表请求主备情况，主备情况发生切换后，need_reinit置为1，并将新的主机IP写入ip_box_used
    pthread_t pth_master_slave_check_job;
    //单独的线程，用于周期性的检查服务端是否工作正常，连续N次请求不回复或连续N次请求回复不正常，则需要重连，相当于起一个应用层心跳的效果
    pthread_t pth_service_check_job;
    //主工作线程
    pthread_t pth_main_work_job;
};

/************************************************************
 * name:new_data_service_ctx
 * desc:初始化连接及订阅（只用于订阅），初始化必要的上下文，线程安全，注意当只填一个IP时，无论这个IP是主还是备，都会去向这个IP订阅数据，填多个IP时，会根据主备情况自动切换
 *
 * para:[in] ip_list              一组服务端的IP地址，可以是一个或多个，不能为NULL，注意当只填一个IP时，无论这个IP是主还是备，都会去向这个IP订阅数据，填多个IP时，会根据主备情况自动切换
 *      [in] port                 服务端端口(0<port<65536)
 *      [in] accessKey            app的accessKey，目前没用，随便填一个，不能为NULL
 *      [in] secretKey            app的secretKey，目前没用，随便填一个，不能为NULL
 *      [in] channel_id           订阅id，subscribeGroupId或controlChannelId或setMeasurepointChannelId，长度<=60字节，不能为NULL
 *      [in] consumerGroup        消费组id，同一个消费组且订阅了同一个channel_id的订阅者，同一个数据只能一个人收到，如果为NULL，将会使用默认组
 *      [in] topic_type           channel_id的类型，subscribeGroupId填TOPIC_TYPE_DATA_SUBSCRIBE，controlChannelId填TOPIC_TYPE_CONTROL_RESPONSE，
 *                                setMeasurepointChannelId填TOPIC_TYPE_SET_MEASUREPOINT_RESPONSE，或者填TOPIC_TYPE_AUTO，程序自动判别
 *      [in] need_auto_reconnect  是否自动重连，目前无论填什么都会自动重连
 *      [in] user_ctx             回调函数中用户自定义指针
 *      [in] connect_callback     连接成功回调
 *                                [in] work_ctx     不用关注这个，调试用，不要试图使用里面的任何值
 *                                [in] channel_id   标识是哪个订阅通道连接成功触发的回调
 *                                [in] user_ctx     用户自定义指针
 *      [in] close_callback       断开连接回调
 *                                [in] work_ctx     不用关注这个，调试用，不要试图使用里面的任何值
 *                                [in] channel_id   标识是哪个订阅通道断开触发的回调
 *                                [in] user_ctx     用户自定义指针
 *      [in] msg_callback         消息回调
 *                                [in] work_ctx     不用关注这个，调试用，不要试图使用里面的任何值
 *                                [in] channel_id   标识是哪个订阅通道消息触发的回调
 *                                [in] msg          具体的消息，注意用完后要调用delete_data_service_msg释放内存
 *                                [in] user_ctx     用户自定义指针
 * return:0         success
 *        <0        fail
 * tips:
 ************************************************************/
EDGE_SERVICE_C_API_DLL_EXPORT extern struct DataServiceCtx *
new_data_service_ctx(struct IPBox *ip_list, int port, char *accessKey, char *secretKey, char *channel_id,
                     char *consumerGroup, int topic_type, int need_auto_reconnect, void *user_ctx,
                     void (*connect_callback)(void *work_ctx, char *channel_id, void *user_ctx),
                     void (*close_callback)(void *work_ctx, char *channel_id, void *user_ctx),
                     void (*msg_callback)(void *work_ctx, char *channel_id,
                                          struct DataServiceMessage *msg, void *user_ctx));

/************************************************************
 * name:new_data_service_ctx_for_publish
 * desc:初始化连接（只用于发送），初始化必要的上下文，线程安全，注意当只填一个IP时，无论这个IP是主还是备，都会去连接这个IP，填多个IP时，会根据主备情况自动切换
 *
 * para:[in] ip_list              一组服务端的IP地址，可以是一个或多个，不能为NULL，注意当只填一个IP时，无论这个IP是主还是备，都会去连接这个IP，填多个IP时，会根据主备情况自动切换
 *      [in] port                 服务端端口(0<port<65536)
 *      [in] accessKey            app的accessKey，目前没用，随便填一个，不能为NULL
 *      [in] secretKey            app的secretKey，目前没用，随便填一个，不能为NULL
 *      [in] need_auto_reconnect  是否自动重连，目前无论填什么都会自动重连
 *      [in] is_fast              是否使用快速发送模式，0：普通模式，其他值：快速模式，快速模式发送后不会接收反较
 *      [in] user_ctx             回调函数中用户自定义指针
 *      [in] connect_callback     连接成功回调
 *                                [in] work_ctx     不用关注这个，调试用，不要试图使用里面的任何值
 *                                [in] channel_id   标识是哪个订阅通道连接成功触发的回调(总是为空串"")
 *                                [in] user_ctx     用户自定义指针
 *      [in] close_callback       断开连接回调
 *                                [in] work_ctx     不用关注这个，调试用，不要试图使用里面的任何值
 *                                [in] channel_id   标识是哪个订阅通道断开触发的回调(总是为空串"")
 *                                [in] user_ctx     用户自定义指针
 * return:0         success
 *        <0        fail
 * tips:
 ************************************************************/
EDGE_SERVICE_C_API_DLL_EXPORT extern struct DataServiceCtx *
new_data_service_ctx_for_publish(struct IPBox *ip_list, int port, char *accessKey, char *secretKey,
                                 int need_auto_reconnect, int is_fast, void *user_ctx,
                                 void (*connect_callback)(void *work_ctx, char *channel_id, void *user_ctx),
                                 void (*close_callback)(void *work_ctx, char *channel_id, void *user_ctx));

/************************************************************
 * name:delete_data_service_ctx
 * desc:释放ctx资源，退出时需要调用，需要在调用stop之后调用该函数，非线程安全
 *
 * para:[in] ctx                  new_data_service_ctx/new_data_service_ctx_for_publish返回的ctx
 * return:0         success
 *        <0        fail
 * tips:
 ************************************************************/
EDGE_SERVICE_C_API_DLL_EXPORT extern int delete_data_service_ctx(struct DataServiceCtx *ctx);

/************************************************************
 * name:data_service_ctx_start
 * desc:启动相关模块，建立连接，与服务端进行应用层协商，非线程安全
 *
 * para:[in] ctx                  new_data_service_ctx/new_data_service_ctx_for_publish返回的ctx
 * return:0         success
 *        <0        fail
 * tips:
 ************************************************************/
EDGE_SERVICE_C_API_DLL_EXPORT extern int data_service_ctx_start(struct DataServiceCtx *ctx);

/************************************************************
 * name:data_service_ctx_stop
 * desc:断开与服务端的连接，停止相关模块，start和stop的调用要成对，不能在没有调用start的情况下调用stop，也不能在已经start的情况下调用start，非线程安全
 *
 * para:[in] ctx                  new_data_service_ctx/new_data_service_ctx_for_publish返回的ctx
 * return:0         success
 *        <0        fail
 * tips:
 *     stop会立即断开tcp连接，还在tcp发送缓存中的数据将会丢失
 ************************************************************/
EDGE_SERVICE_C_API_DLL_EXPORT extern int data_service_ctx_stop(struct DataServiceCtx *ctx);

/************************************************************
 * name:delete_data_service_msg
 * desc:释放接收到的消息的内存
 *
 * para:[in] data_service_msg           msg_callback回调中的消息
 * return: 0        success
 *        <0        fail
 * tips:
 *     每条消息不使用时都需要调用这个函数释放
 ************************************************************/
EDGE_SERVICE_C_API_DLL_EXPORT extern int delete_data_service_msg(struct DataServiceMessage *data_service_msg);

/************************************************************
 * name:get_topic_type_by_channel_id
 * desc:根据channel_id获取channel类型
 *
 * para:[in] channel_id           channel_id
 * return: >=0      enum TOPIC_TYPE定义的类型
 *        <0        fail
 * tips:
 ************************************************************/
EDGE_SERVICE_C_API_DLL_EXPORT extern int get_topic_type_by_channel_id(char *channel_id);

/************************************************************
 * name:data_service_ctx_pub_fast
 * desc:发送数据
 *
 * para:[in] ctx                  只能是new_data_service_ctx_for_publish返回的ctx，不能是new_data_service_ctx返回的ctx，且is_fast必须不为0
 *      [in] channel_id           发送的目标topic
 *      [in] msg                  要发送的消息
 *      [in] msg_len              要发送的消息的长度
 * return:0         success
 *        <0        fail
 * tips:
 *     1.返回成功，说明数据成功加入协议栈发送缓存，并不代表对方收到
 *     2.在connect_callback回调触发之前发送数据，会发送失败
 *     3.调用data_service_ctx_stop后，还在tcp发送缓存中的数据将会丢失
 ************************************************************/
EDGE_SERVICE_C_API_DLL_EXPORT extern int
data_service_ctx_pub_fast(struct DataServiceCtx *ctx, char *channel_id, char *msg, int msg_len);

/************************************************************
 * name:data_service_ctx_pub_normal
 * desc:发送数据
 *
 * para:[in] ctx                  只能是new_data_service_ctx_for_publish返回的ctx，不能是new_data_service_ctx返回的ctx，且is_fast必须不为0
 *      [in] channel_id           发送的目标topic
 *      [in] msg                  要发送的消息
 *      [in] msg_len              要发送的消息的长度
 *      [in] timeout_ms           发送超时时间，单位毫秒，范围(0, 60000]
 * return:0         success
 *        <0        fail
 * tips:
 *     1.返回成功，说明数据服务端已经正确收到
 ************************************************************/
EDGE_SERVICE_C_API_DLL_EXPORT extern int
data_service_ctx_pub_normal(struct DataServiceCtx *ctx, char *channel_id, char *msg, int msg_len, int timeout_ms);

/************************************************************
 * name:data_service_ctx_connect_for_pub_normal
 * desc:用于调用方需要在不发送数据的情况下，主动连接服务端（没有特殊需求，可以不主动调用这个函数，因为data_service_ctx_pub_normal函数每次发送前会检查连接是否可用，如果可用，直接发送，如果不可用，先重连后发送）
 *
 * para:[in] ctx                  只能是new_data_service_ctx_for_publish返回的ctx，不能是new_data_service_ctx返回的ctx，且is_fast必须不为0
 * return:0         success
 *        <0        fail
 * tips:
 *     1.返回成功，说明连接成功
 ************************************************************/
EDGE_SERVICE_C_API_DLL_EXPORT extern int
data_service_ctx_connect_for_pub_normal(struct DataServiceCtx *ctx);

#ifdef __cplusplus
}
#endif

#endif //EDGE_SERVICE_SDK_C_DATASERVICE_H
