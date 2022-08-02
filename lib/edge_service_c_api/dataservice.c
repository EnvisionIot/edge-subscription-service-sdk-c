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

#include "edge_service_c_api/dataservice.h"
#include "protobuf/controlresponsedata/controlresponsepoint.pb-c.h"
#include "protobuf/ssrecorddata/ssrecordpoint.pb-c.h"
#include "protobuf/setmeasurepointresponsedata/setmeasurepointresponsepoint.pb-c.h"
#include "protobuf/subdata/subdata.pb-c.h"

//字节序相关-->
#define MY_NET_IS_BIGENDIAN 1//采用的网络字节序,0:小端,1:大端
static const int MY_Endian_Static = 0x01000002;

#define MY_TestBigEndian() (0x01 == *(char*)&MY_Endian_Static)//测试本机字节序是不是大端

#define MY_EndianNeedChange() (MY_TestBigEndian() != MY_NET_IS_BIGENDIAN)//主机字节序和网络字节序是否需要转换

#define MY_BigLittleSwap16(A) ((((unsigned short int)(A) & 0xff00) >> 8) | \
                                    (((unsigned short int)(A) & 0x00ff) << 8))

#define MY_BigLittleSwap32(A) ((((unsigned int)(A) & 0xff000000) >> 24) | \
                                    (((unsigned int)(A) & 0x00ff0000) >> 8) | \
                                    (((unsigned int)(A) & 0x0000ff00) << 8) | \
                                    (((unsigned int)(A) & 0x000000ff) << 24))

#define MY_BigLittleSwap64(A) ((((unsigned long long)(A) & 0xff00000000000000ULL) >> 56) | \
                                    (((unsigned long long)(A) & 0x00ff000000000000ULL) >> 40) | \
                                    (((unsigned long long)(A) & 0x0000ff0000000000ULL) >> 24) | \
                                    (((unsigned long long)(A) & 0x000000ff00000000ULL) >> 8) | \
                                    (((unsigned long long)(A) & 0x00000000ff000000ULL) << 8) | \
                                    (((unsigned long long)(A) & 0x0000000000ff0000ULL) << 24) | \
                                    (((unsigned long long)(A) & 0x000000000000ff00ULL) << 40) | \
                                    (((unsigned long long)(A) & 0x00000000000000ffULL) << 56))

//本机字节序的16位整型转为网络字节序的16位整型
#define MY_htons(A) (MY_EndianNeedChange() ? MY_BigLittleSwap16(A) : (A))

//网络字节序的16位整型转为本机字节序的16位整型
#define MY_ntohs(A) (MY_EndianNeedChange() ? MY_BigLittleSwap16(A) : (A))

//本机字节序的32位整型转为网络字节序的32位整型
#define MY_htonl(A) (MY_EndianNeedChange() ? MY_BigLittleSwap32(A) : (A))

//网络字节序的32位整型转为本机字节序的32位整型
#define MY_ntohl(A) (MY_EndianNeedChange() ? MY_BigLittleSwap32(A) : (A))

//本机字节序的64位整型转为网络字节序的64位整型
#define MY_htonl64(A) (MY_EndianNeedChange() ? MY_BigLittleSwap64(A) : (A))

//网络字节序的64位整型转为本机字节序的64位整型
#define MY_ntohl64(A) (MY_EndianNeedChange() ? MY_BigLittleSwap64(A) : (A))

//本机字节序的32位浮点型h转为网络字节序的32位浮点型n
#define MY_htonf32(h, n)                                            \
    do                                                              \
    {                                                               \
        float h_temp = (float)h;                                    \
        if(MY_EndianNeedChange())                                   \
        {                                                           \
            char *temp = (char *)&h_temp;                           \
            ((char *)&n)[0] = temp[3];                              \
            ((char *)&n)[1] = temp[2];                              \
            ((char *)&n)[2] = temp[1];                              \
            ((char *)&n)[3] = temp[0];                              \
        }                                                           \
        else                                                        \
        {                                                           \
            memcpy((void *)&n, (void *)&h_temp, sizeof(float));     \
        }                                                           \
    }while(0)

//网络字节序的32位浮点型n转为本机字节序的32位浮点型h
#define MY_ntohf32(n, h)                                            \
    do                                                              \
    {                                                               \
        float n_temp = (float)n;                                    \
        if(MY_EndianNeedChange())                                   \
        {                                                           \
            char *temp = (char *)&n_temp;                           \
            ((char *)&h)[0] = temp[3];                              \
            ((char *)&h)[1] = temp[2];                              \
            ((char *)&h)[2] = temp[1];                              \
            ((char *)&h)[3] = temp[0];                              \
        }                                                           \
        else                                                        \
        {                                                           \
            memcpy((void *)&h, (void *)&n_temp, sizeof(float));     \
        }                                                           \
    }while(0)

//本机字节序的64位浮点型h转为网络字节序的64位浮点型n
#define MY_htonf64(h, n)                                            \
    do                                                              \
    {                                                               \
        double h_temp = (double)h;                                  \
        if(MY_EndianNeedChange())                                   \
        {                                                           \
            char *temp = (char *)&h_temp;                           \
            ((char *)&n)[0] = temp[7];                              \
            ((char *)&n)[1] = temp[6];                              \
            ((char *)&n)[2] = temp[5];                              \
            ((char *)&n)[3] = temp[4];                              \
            ((char *)&n)[4] = temp[3];                              \
            ((char *)&n)[5] = temp[2];                              \
            ((char *)&n)[6] = temp[1];                              \
            ((char *)&n)[7] = temp[0];                              \
        }                                                           \
        else                                                        \
        {                                                           \
            memcpy((void *)&n, (void *)&h_temp, sizeof(double));    \
        }                                                           \
    }while(0)

//网络字节序的64位浮点型n转为本机字节序的64位浮点型h
#define MY_ntohf64(n, h)                                            \
    do                                                              \
    {                                                               \
        double n_temp = (double)n;                                  \
        if(MY_EndianNeedChange())                                   \
        {                                                           \
            char *temp = (char *)&n_temp;                           \
            ((char *)&h)[0] = temp[7];                              \
            ((char *)&h)[1] = temp[6];                              \
            ((char *)&h)[2] = temp[5];                              \
            ((char *)&h)[3] = temp[4];                              \
            ((char *)&h)[4] = temp[3];                              \
            ((char *)&h)[5] = temp[2];                              \
            ((char *)&h)[6] = temp[1];                              \
            ((char *)&h)[7] = temp[0];                              \
        }                                                           \
        else                                                        \
        {                                                           \
            memcpy((void *)&h, (void *)&n_temp, sizeof(double));    \
        }                                                           \
    }while(0)

//<--字节序相关

//static函数声明-->
//与服务端建立TCP连接，带超时时间，返回连接后的fd，失败时返回<0
static int connect_server_timeout(char *server_ip, unsigned short server_port, int timeout_ms);

//与服务端建立TCP连接，返回连接后的fd，失败时返回<0
static int connect_server(char *server_ip, unsigned short server_port);

//用加锁的方式获取当前需要连接的IP
static int get_ip_used_withlock(struct DataServiceCtx *ctx, char *ip_ret, int ip_ret_max);

//用加锁的方式设置当前需要连接的IP
static int set_ip_used_withlock(struct DataServiceCtx *ctx, struct IPBox *ip_box);

//直接获取当前需要连接的IP
static int get_ip_used(struct DataServiceCtx *ctx, char *ip_ret, int ip_ret_max);

//用加锁的方式获取当前启停状态
static int get_running_status_withlock(struct DataServiceCtx *ctx);

//用加锁的方式设置当前启停状态
static int set_running_status_withlock(struct DataServiceCtx *ctx, int new_status);

//用加锁的方式设置tcp_sock_fd
static int set_tcp_sock_fd_withlock(struct DataServiceCtx *ctx, int new_fd);

//用加锁的方式关闭tcp_sock_fd
static int close_tcp_sock_fd_withlock(struct DataServiceCtx *ctx);

//直接关闭tcp_sock_fd
static int close_tcp_sock_fd(struct DataServiceCtx *ctx);

//用加锁的方式获取nsq应用层初始化状态
static int get_nsq_init_status_withlock(struct DataServiceCtx *ctx);

//用加锁的方式设置nsq应用层初始化状态
static int set_nsq_init_status_withlock(struct DataServiceCtx *ctx, int new_nsq_init_status);

//用加锁的方式获取重连标志
static int get_need_reinit_withlock(struct DataServiceCtx *ctx);

//用加锁的方式设置重连标志
static int set_need_reinit_withlock(struct DataServiceCtx *ctx, int new_need_reinit);

//清空recv_buf_small内容，一般每次recv之前都要调用
static int clear_recv_buf_small(struct DataServiceCtx *ctx);

//清理recv，每接收完一帧完整报文调用一次
static int clear_recv_resource(struct DataServiceCtx *ctx);

//释放recv资源，包括缓存、标志位等，一般断开连接后调用
static int uninit_recv_resource(struct DataServiceCtx *ctx);

//初始化recv资源，包括缓存、标志位等，一般连接前调用
static int init_recv_resource(struct DataServiceCtx *ctx);

//根据输入的channel_id获取topic和channel
static int get_topic_and_channel_by_channel_id(char *channel_id, char *consumerGroup, int topic_type, char *topic,
                                               int topic_max_len, char *channel, int channel_max_len);

//解析消息（从protobuf转为结构）
static void *decode_msg(char *src_msg, int src_msg_len, int topic_type);

static int transform_SSRecordPoint_to_DataSubscribeSubStruct(Ssrecorddata__SSRecordPoint *src,
                                                             struct DataSubscribeSubStruct *dst);

static struct DataSubscribeStruct *
transform_SSRecordPoints_to_DataSubscribeStruct(Ssrecorddata__SSRecordPoints *ssrps);

static int transform_SubPoint_to_SimpleDataSubscribeSubStruct(Subdata__SubPoint *src,
                                                              struct SimpleDataSubscribeSubStruct *dst);

static struct SimpleDataSubscribeStruct *
transform_SubPoints_to_SimpleDataSubscribeStruct(Subdata__SubPoints *subps);

static int transform_ControlResponsePoint_to_ControlResponseSubStruct(Controlresponsedata__ControlResponsePoint *src,
                                                                      struct ControlResponseSubStruct *dst);

static struct ControlResponseStruct *
transform_ControlResponsePoints_to_ControlResponseStruct(Controlresponsedata__ControlResponsePoints *crps);

static int transform_SetMeasurepointResponsePoint_to_SetMeasurepointResponseSubStruct(
        Setmeasurepointresponsedata__SetMeasurepointResponsePoint *src,
        struct SetMeasurepointResponseSubStruct *dst);

static struct SetMeasurepointResponseStruct *
transform_SetMeasurepointResponsePoints_to_SetMeasurepointResponseStruct(
        Setmeasurepointresponsedata__SetMeasurepointResponsePoints *smrps);

static char *deep_copy_char_ptr(char *src);

static int delete_data_subscribe_sub_struct(struct DataSubscribeSubStruct *ptr);

static int delete_data_subscribe_struct(struct DataSubscribeStruct *ptr);

static int delete_control_response_sub_struct(struct ControlResponseSubStruct *ptr);

static int delete_control_response_struct(struct ControlResponseStruct *ptr);

static int delete_set_measurepoint_response_sub_struct(struct SetMeasurepointResponseSubStruct *ptr);

static int delete_set_measurepoint_response_struct(struct SetMeasurepointResponseStruct *ptr);

//主备检测线程
static void *master_slave_check_job_callback(void *arg);

//检查服务端是否工作正常的线程
static void *service_check_job(void *arg);

//主工作线程
static void *main_work_job(void *arg);

//处理一次recv调用接收到的数据,一般需要在这里拼成一个完整交互报文
static int ctx_deal_recv_small(struct DataServiceCtx *ctx, int len);

//处理一个完整交互报文
static int ctx_deal_recv_big(struct DataServiceCtx *ctx);

//处理response
static int ctx_deal_response_msg(struct DataServiceCtx *ctx, struct nsq_packet_header *nsq_packet_header_local_p);

//处理error
static int ctx_deal_error_msg(struct DataServiceCtx *ctx, struct nsq_packet_header *nsq_packet_header_local_p);

//处理message
static int ctx_deal_message_msg(struct DataServiceCtx *ctx, struct nsq_packet_header *nsq_packet_header_local_p);

//期望接收一帧nsq的OK反较
static int expect_recv_ok_frame(int sock, int timeout_ms);

//期望接收一帧nsq的auth正确反较
static int expect_recv_auth_ok_frame(int sock, int timeout_ms);

//期望接收一帧nsq的指定反较
static int expect_recv_specified_frame(int sock, char *expect_in, int expect_in_len, int timeout_ms);

//重连函数
static int ctx_reconnect(struct DataServiceCtx *ctx);

//连接并发送magic
static int ctx_connect_and_send_magic(struct DataServiceCtx *ctx);

//操作nsq初始化状态机
static int ctx_init_nsq(struct DataServiceCtx *ctx);

//发送tcp数据
static int ctx_tcp_send_data(int sock, char *send_buf, int send_len);

//获取数组中最大值
static int max_in_array(int *fd_array, int count);

//发送tcp数据并接受回复
static int ctx_tcp_send_data_and_wait_recv(int sock, char *send_buf, int send_len, char *recv_buf, int *recv_buf_len,
                                           int recv_buf_max, int timeout_sec);

//发送magic code
static int ctx_send_magic(int sock);

//发送auth信息
static int ctx_send_auth(int sock, char *p);

//关闭心跳
static int ctx_close_heartbeat(int sock);

//发送订阅消息
static int ctx_send_subscribe_info(int sock, char *topic, char *channel);

//发送RDY
static int ctx_send_rdy(int sock, int new_rdy);

//发送NOP
static int ctx_send_nop(int sock);

//发送FIN
static int ctx_send_fin(int sock, char *msg_id);

//发送消息
static int ctx_pub_msg(int sock, char *topic, char *msg, int msg_len);

static int ctx_start_for_send_normal(struct DataServiceCtx *ctx);

static int ctx_stop_for_send_normal(struct DataServiceCtx *ctx);
//<--static函数声明

//static函数定义-->
static int connect_server_timeout(char *server_ip, unsigned short server_port, int timeout_ms) {
    int ret = 0;
    int flag = 0;
    int optval = 1;
    struct linger op;
    struct sockaddr_in server_addr;
    int tcp_sock_fd = -1;
    int error_no_get = 0;
    int getsocket_err = 0;
    int len = 0;

    tcp_sock_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (tcp_sock_fd < 0) {
        error_no_get = errno;
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "call socket funtion error, ret=%d, errno=%d", tcp_sock_fd,
                 error_no_get);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    flag = fcntl(tcp_sock_fd, F_GETFL);
    //设置为非阻塞模式
    fcntl(tcp_sock_fd, F_SETFL, flag | O_NONBLOCK);
    optval = 1;
    setsockopt(tcp_sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *) &optval, sizeof(optval));
    optval = 1;
    setsockopt(tcp_sock_fd, SOL_SOCKET, SO_KEEPALIVE, (char *) &optval, sizeof(optval));
    optval = EDGE_DATASERVICE_DEFAULT_TCP_KEEPIDLE;
    setsockopt(tcp_sock_fd, SOL_TCP, TCP_KEEPIDLE, &optval, sizeof(optval));
    optval = EDGE_DATASERVICE_DEFAULT_TCP_KEEPINTVL;
    setsockopt(tcp_sock_fd, SOL_TCP, TCP_KEEPINTVL, &optval, sizeof(optval));
    optval = EDGE_DATASERVICE_DEFAULT_TCP_KEEPCNT;
    setsockopt(tcp_sock_fd, SOL_TCP, TCP_KEEPCNT, &optval, sizeof(optval));
    optval = 1;
    setsockopt(tcp_sock_fd, IPPROTO_TCP, TCP_NODELAY, (char *) &optval, sizeof(optval));
    op.l_onoff = 1;
    op.l_linger = 0;
    setsockopt(tcp_sock_fd, SOL_SOCKET, SO_LINGER, (char *) &op, sizeof(op));

    memset((void *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = MY_htons((unsigned short) server_port);
    ret = inet_pton(AF_INET, server_ip, &(server_addr.sin_addr.s_addr));
    if (ret != 1) {
        error_no_get = errno;
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "call inet_pton funtion error, ret=%d, errno=%d", ret, error_no_get);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        close(tcp_sock_fd);
        return -1;
    }
//    sf_api_s->server_ip_uint = MY_ntohl(server_addr.sin_addr.s_addr);

    ret = connect(tcp_sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret == 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "connect success immediately, server ip=%s, server_port=%hu",
                 server_ip,
                 server_port);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO, log_content);
        return tcp_sock_fd;
    }

    error_no_get = errno;
    if (error_no_get != EINPROGRESS) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "connect error(errno != EINPROGRESS), errno=%d, server ip=%s, server_port=%hu", error_no_get,
                 server_ip,
                 server_port);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        close(tcp_sock_fd);
        return -1;
    }

    fd_set wset;
    struct timeval tval;

    FD_ZERO(&wset);
    FD_SET(tcp_sock_fd, &wset);
    tval.tv_sec = timeout_ms / 1000;
    tval.tv_usec = 0;

    ret = select(tcp_sock_fd + 1, NULL, &wset, NULL, &tval);
    if (ret <= 0) {
        error_no_get = errno;
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "connect error or timeout(select error), errno=%d, server ip=%s, server_port=%hu", error_no_get,
                 server_ip,
                 server_port);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        close(tcp_sock_fd);
        return -1;
    }

    if (!(FD_ISSET(tcp_sock_fd, &wset))) {
        error_no_get = errno;
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "connect error or timeout(select not socketfd found), errno=%d, server ip=%s, server_port=%hu",
                 error_no_get,
                 server_ip,
                 server_port);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        close(tcp_sock_fd);
        return -1;
    }

    len = sizeof(getsocket_err);
    //如果发生错误，Solaris实现的getsockopt返回-1，
    //把pending error设置给errno. Berkeley实现的
    //getsockopt返回0, pending error返回给error
    ret = getsockopt(tcp_sock_fd, SOL_SOCKET, SO_ERROR, (char *) &getsocket_err, (socklen_t *) &len);
    if (ret < 0) {
        error_no_get = errno;
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "connect error or timeout(getsockopt error), errno=%d, server ip=%s, server_port=%hu", error_no_get,
                 server_ip,
                 server_port);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        close(tcp_sock_fd);
        return -1;
    }

    if (getsocket_err != 0) {
        error_no_get = getsocket_err;
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "connect error or timeout(getsocket_err != 0), errno=%d, server ip=%s, server_port=%hu", error_no_get,
                 server_ip,
                 server_port);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        close(tcp_sock_fd);
        return -1;
    }

    //连接成功
    char log_content[1024];
    memset(log_content, 0, sizeof(log_content));
    snprintf(log_content, sizeof(log_content), "connect success, server ip=%s, server_port=%hu", server_ip,
             server_port);
    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO, log_content);
    return tcp_sock_fd;
}

static int connect_server(char *server_ip, unsigned short server_port) {
    return connect_server_timeout(server_ip, server_port, EDGE_DATASERVICE_TCP_CONNECT_DEFAULT_TIMEOUT_MS);
}

static int get_ip_used_withlock(struct DataServiceCtx *ctx, char *ip_ret, int ip_ret_max) {
    if (ctx == NULL) {
        return -1;
    }
    pthread_rwlock_rdlock(&(ctx->ip_box_used_rwlock));
    if (ctx->ip_box_used->count <= 0) {
        pthread_rwlock_unlock(&(ctx->ip_box_used_rwlock));
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ip list is empty");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }
    snprintf(ip_ret, ip_ret_max, "%s", ctx->ip_box_used->ip_list[0].str);
    pthread_rwlock_unlock(&(ctx->ip_box_used_rwlock));

    return 0;
}

static int set_ip_used_withlock(struct DataServiceCtx *ctx, struct IPBox *ip_box) {
    if (ctx == NULL) {
        return -1;
    }
    pthread_rwlock_wrlock(&(ctx->ip_box_used_rwlock));
    if (ctx->ip_box_used != NULL) {
        delete_ip_box(ctx->ip_box_used);
        ctx->ip_box_used = NULL;
    }
    ctx->ip_box_used = deep_copy_ip_box(ip_box);
    pthread_rwlock_unlock(&(ctx->ip_box_used_rwlock));
    return 0;
}

static int get_ip_used(struct DataServiceCtx *ctx, char *ip_ret, int ip_ret_max) {
    if (ctx == NULL) {
        return -1;
    }

    if (ctx->ip_box_used->count <= 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ip list is empty");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }
    snprintf(ip_ret, ip_ret_max, "%s", ctx->ip_box_used->ip_list[0].str);

    return 0;
}

static int get_running_status_withlock(struct DataServiceCtx *ctx) {
    if (ctx == NULL) {
        return -1;
    }

    int status_ret = 0;
    pthread_rwlock_rdlock(&(ctx->start_status_rwlock));
    status_ret = ctx->start_status;
    pthread_rwlock_unlock(&(ctx->start_status_rwlock));

    return status_ret;
}

static int set_running_status_withlock(struct DataServiceCtx *ctx, int new_status) {
    if (ctx == NULL) {
        return -1;
    }

    pthread_rwlock_wrlock(&(ctx->start_status_rwlock));
    ctx->start_status = new_status;
    pthread_rwlock_unlock(&(ctx->start_status_rwlock));

    return 0;
}

static int set_tcp_sock_fd_withlock(struct DataServiceCtx *ctx, int new_fd) {
    if (ctx == NULL) {
        return -1;
    }

    pthread_rwlock_wrlock(&(ctx->tcp_sock_fd_rwlock));
    ctx->tcp_sock_fd = new_fd;
    pthread_rwlock_unlock(&(ctx->tcp_sock_fd_rwlock));

    return 0;
}

static int close_tcp_sock_fd_withlock(struct DataServiceCtx *ctx) {
    if (ctx == NULL) {
        return -1;
    }

    pthread_rwlock_wrlock(&(ctx->tcp_sock_fd_rwlock));
    if (ctx->tcp_sock_fd >= 0) {
        close(ctx->tcp_sock_fd);
        ctx->tcp_sock_fd = -1;
        pthread_rwlock_unlock(&(ctx->tcp_sock_fd_rwlock));

        if (ctx->close_callback != NULL) {
            ctx->close_callback(NULL, ctx->channel_id, ctx->user_ctx);
        }
        return 1;
    }
    pthread_rwlock_unlock(&(ctx->tcp_sock_fd_rwlock));

    return 0;
}

static int close_tcp_sock_fd(struct DataServiceCtx *ctx) {
    if (ctx == NULL) {
        return -1;
    }

    if (ctx->tcp_sock_fd >= 0) {
        close(ctx->tcp_sock_fd);
        ctx->tcp_sock_fd = -1;

        if (ctx->close_callback != NULL) {
            ctx->close_callback(NULL, ctx->channel_id, ctx->user_ctx);
        }
    }

    return 0;
}

static int get_nsq_init_status_withlock(struct DataServiceCtx *ctx) {
    if (ctx == NULL) {
        return -1;
    }

    int nsq_init_status_ret = 0;
    pthread_rwlock_rdlock(&(ctx->nsq_init_status_rwlock));
    nsq_init_status_ret = ctx->nsq_init_status;
    pthread_rwlock_unlock(&(ctx->nsq_init_status_rwlock));

    return nsq_init_status_ret;
}

static int set_nsq_init_status_withlock(struct DataServiceCtx *ctx, int new_nsq_init_status) {
    if (ctx == NULL) {
        return -1;
    }

    pthread_rwlock_wrlock(&(ctx->nsq_init_status_rwlock));
    ctx->nsq_init_status = new_nsq_init_status;
    pthread_rwlock_unlock(&(ctx->nsq_init_status_rwlock));

    return 0;
}

static int get_need_reinit_withlock(struct DataServiceCtx *ctx) {
    if (ctx == NULL) {
        return -1;
    }

    int need_reinit_ret = 0;
    pthread_rwlock_rdlock(&(ctx->reinit_rwlock));
    need_reinit_ret = ctx->need_reinit;
    pthread_rwlock_unlock(&(ctx->reinit_rwlock));

    return need_reinit_ret;
}

static int set_need_reinit_withlock(struct DataServiceCtx *ctx, int new_need_reinit) {
    if (ctx == NULL) {
        return -1;
    }

    pthread_rwlock_wrlock(&(ctx->reinit_rwlock));
    ctx->need_reinit = new_need_reinit;
    pthread_rwlock_unlock(&(ctx->reinit_rwlock));

    return 0;
}


//清空recv_buf_small内容，一般每次recv之前都要调用
static int clear_recv_buf_small(struct DataServiceCtx *ctx) {
    memset((void *) (ctx->recv_buf_small), 0, sizeof(ctx->recv_buf_small));
    return 0;
}

//清理recv，每接收完一帧完整报文调用一次
static int clear_recv_resource(struct DataServiceCtx *ctx) {
    if (ctx == NULL) {
        return -1;
    }

    ctx->recv_syn_flag = 0;
    ctx->has_recv_bytes = 0;
    ctx->should_recv_bytes = 0;
    ctx->recv_buf_big_max = EDGE_DATASERVICE_BUF_BIG_SIZE;
    if (ctx->recv_buf_big != NULL) {
        memset(ctx->recv_buf_big, 0, ctx->recv_buf_big_max);
    }

    return 0;
}

//释放recv资源，包括缓存、标志位等，一般断开连接后调用
static int uninit_recv_resource(struct DataServiceCtx *ctx) {
    if (ctx == NULL) {
        return -1;
    }

    ctx->recv_syn_flag = 0;
    ctx->has_recv_bytes = 0;
    ctx->should_recv_bytes = 0;
    memset(ctx->recv_buf_small, 0, sizeof(ctx->recv_buf_small));
    ctx->recv_buf_big_max = EDGE_DATASERVICE_BUF_BIG_SIZE;
    if (ctx->recv_buf_big != NULL) {
        free(ctx->recv_buf_big);
        ctx->recv_buf_big = NULL;
    }

    return 0;
}

//初始化recv资源，包括缓存、标志位等，一般连接前调用
static int init_recv_resource(struct DataServiceCtx *ctx) {
    if (ctx == NULL) {
        return -1;
    }

    uninit_recv_resource(ctx);

    ctx->recv_syn_flag = 0;
    ctx->has_recv_bytes = 0;
    ctx->should_recv_bytes = 0;
    memset(ctx->recv_buf_small, 0, sizeof(ctx->recv_buf_small));
    ctx->recv_buf_big_max = EDGE_DATASERVICE_BUF_BIG_SIZE;
    ctx->recv_buf_big = (char *) malloc(ctx->recv_buf_big_max);
    if (ctx->recv_buf_big == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "recv_buf_big malloc error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }
    memset(ctx->recv_buf_big, 0, ctx->recv_buf_big_max);
    return 0;
}

//根据输入的channel_id获取topic和channel
static int get_topic_and_channel_by_channel_id(char *channel_id, char *consumerGroup, int topic_type, char *topic,
                                               int topic_max_len,
                                               char *channel, int channel_max_len) {
    int topic_type_actual = 0;
    if (topic_type == TOPIC_TYPE_AUTO) {
        topic_type_actual = get_topic_type_by_channel_id(channel_id);
    } else {
        topic_type_actual = topic_type;
    }

    if (topic_type_actual == TOPIC_TYPE_DATA_SUBSCRIBE_ALL) {
        snprintf(topic, topic_max_len, "%s", EDGE_DATASERVICE_SUBSCRIBE_ALL_TOPIC);
        if (strlen(consumerGroup) <= 0) {
            snprintf(channel, channel_max_len, "%s", channel_id);
        } else {
            snprintf(channel, channel_max_len, "%s_%s", channel_id, consumerGroup);
        }

        return 0;
    }

    snprintf(topic, topic_max_len, "%s", channel_id);
    if (strlen(consumerGroup) <= 0) {
        snprintf(channel, channel_max_len, "%s", EDGE_DATASERVICE_DEFAULT_CONSUMER_GROUP);
    } else {
        snprintf(channel, channel_max_len, "%s", consumerGroup);
    }

    return 0;
}


static void *decode_msg(char *src_msg, int src_msg_len, int topic_type) {
    Ssrecorddata__SSRecordPoints *ssrps_ptr = NULL;
    Controlresponsedata__ControlResponsePoints *crps_ptr = NULL;
    Setmeasurepointresponsedata__SetMeasurepointResponsePoints *smrps_ptr = NULL;
    Subdata__SubPoints *subps_ptr = NULL;

    struct DataSubscribeStruct *dss_ptr = NULL;
    struct ControlResponseStruct *crs_ptr = NULL;
    struct SetMeasurepointResponseStruct *smrs_ptr = NULL;
    struct SimpleDataSubscribeStruct *sdss_ptr = NULL;
    char *custom_ptr = NULL;

    switch (topic_type) {
        case TOPIC_TYPE_DATA_SUBSCRIBE:
        case TOPIC_TYPE_DATA_SUBSCRIBE_ALL:
            ssrps_ptr = ssrecorddata__ssrecord_points__unpack(NULL, (size_t) src_msg_len, (uint8_t *) src_msg);
            dss_ptr = transform_SSRecordPoints_to_DataSubscribeStruct(ssrps_ptr);
            ssrecorddata__ssrecord_points__free_unpacked(ssrps_ptr, NULL);
            return (void *) dss_ptr;
        case TOPIC_TYPE_CONTROL_RESPONSE:
            crps_ptr = controlresponsedata__control_response_points__unpack(NULL, (size_t) src_msg_len,
                                                                            (uint8_t *) src_msg);
            crs_ptr = transform_ControlResponsePoints_to_ControlResponseStruct(crps_ptr);
            controlresponsedata__control_response_points__free_unpacked(crps_ptr, NULL);
            return (void *) crs_ptr;
        case TOPIC_TYPE_SET_MEASUREPOINT_RESPONSE:
            smrps_ptr = setmeasurepointresponsedata__set_measurepoint_response_points__unpack(NULL,
                                                                                              (size_t) src_msg_len,
                                                                                              (uint8_t *) src_msg);
            smrs_ptr = transform_SetMeasurepointResponsePoints_to_SetMeasurepointResponseStruct(smrps_ptr);
            setmeasurepointresponsedata__set_measurepoint_response_points__free_unpacked(smrps_ptr, NULL);
            return (void *) smrs_ptr;
        case TOPIC_TYPE_SIMPLE_DATA_SUBSCRIBE:
            subps_ptr = subdata__sub_points__unpack(NULL, (size_t) src_msg_len, (uint8_t *) src_msg);
            sdss_ptr = transform_SubPoints_to_SimpleDataSubscribeStruct(subps_ptr);
            subdata__sub_points__free_unpacked(subps_ptr, NULL);
            return (void *) sdss_ptr;
        case TOPIC_TYPE_CUSTOM:
            custom_ptr = (char *) malloc(src_msg_len + 1);
            if (custom_ptr == NULL) {
                char log_content[1024];
                memset(log_content, 0, sizeof(log_content));
                snprintf(log_content, sizeof(log_content), "custom_ptr malloc error");
                edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                               log_content);
                return NULL;
            }
            memcpy(custom_ptr, src_msg, src_msg_len);
            custom_ptr[src_msg_len] = 0;
            return (void *) custom_ptr;
        default:
            return NULL;
    }

    return NULL;
}

static int transform_SSRecordPoint_to_DataSubscribeSubStruct(Ssrecorddata__SSRecordPoint *src,
                                                             struct DataSubscribeSubStruct *dst) {
    if (src == NULL || dst == NULL) {
        return 0;
    }

    dst->orgid = deep_copy_char_ptr(src->orgid);
    dst->modelid = deep_copy_char_ptr(src->modelid);
    dst->assetid = deep_copy_char_ptr(src->assetid);
    dst->collectdeviceid = deep_copy_char_ptr(src->collectdeviceid);
    dst->pointid = deep_copy_char_ptr(src->pointid);
    dst->time = src->time;
    dst->value = deep_copy_char_ptr(src->value);
    dst->quality = src->quality;
    dst->dq = src->dq;
    dst->modelpath = deep_copy_char_ptr(src->modelpath);
    dst->policytype = deep_copy_char_ptr(src->policytype);
    dst->signaltype = deep_copy_char_ptr(src->signaltype);
    dst->hasquality = src->hasquality;
    dst->datatype = deep_copy_char_ptr(src->datatype);
    dst->subdatatype = deep_copy_char_ptr(src->subdatatype);
    dst->attr = deep_copy_char_ptr(src->attr);
    dst->usingoem = src->usingoem;
    dst->oemtime = src->oemtime;
    dst->pointtype = src->pointtype;

    return 0;
}

static struct DataSubscribeStruct *
transform_SSRecordPoints_to_DataSubscribeStruct(Ssrecorddata__SSRecordPoints *ssrps) {
    if (ssrps == NULL) {
        return NULL;
    }

    struct DataSubscribeStruct *dss_ptr = (struct DataSubscribeStruct *) malloc(sizeof(struct DataSubscribeStruct));
    if (dss_ptr == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "dss_ptr malloc error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return NULL;
    }
    memset(dss_ptr, 0, sizeof(struct DataSubscribeStruct));

    dss_ptr->point_count = (int) (ssrps->n_points);
    int size_temp = dss_ptr->point_count * sizeof(struct DataSubscribeSubStruct);
    dss_ptr->points = (struct DataSubscribeSubStruct *) malloc(size_temp);
    if (dss_ptr->points == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "dss_ptr->points malloc error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        free(dss_ptr);
        return NULL;
    }
    memset(dss_ptr->points, 0, size_temp);

    int ii = 0;
    for (ii = 0; ii < dss_ptr->point_count; ii++) {
        transform_SSRecordPoint_to_DataSubscribeSubStruct(ssrps->points[ii], &(dss_ptr->points[ii]));
    }

    return dss_ptr;
}

static int transform_SubPoint_to_SimpleDataSubscribeSubStruct(Subdata__SubPoint *src,
                                                              struct SimpleDataSubscribeSubStruct *dst) {
    if (src == NULL || dst == NULL) {
        return 0;
    }

    dst->assetid = deep_copy_char_ptr(src->assetid);
    dst->pointid = deep_copy_char_ptr(src->pointid);
    dst->time = src->time;
    dst->value = deep_copy_char_ptr(src->value);
    dst->quality = src->quality;
    dst->edq = src->edq;
    dst->datatype = deep_copy_char_ptr(src->datatype);
    dst->subdatatype = deep_copy_char_ptr(src->subdatatype);
    dst->oemtime = src->oemtime;
    dst->attr = deep_copy_char_ptr(src->attr);

    return 0;
}

static struct SimpleDataSubscribeStruct *
transform_SubPoints_to_SimpleDataSubscribeStruct(Subdata__SubPoints *subps) {
    if (subps == NULL) {
        return NULL;
    }

    struct SimpleDataSubscribeStruct *sdss_ptr = (struct SimpleDataSubscribeStruct *) malloc(
            sizeof(struct SimpleDataSubscribeStruct));
    if (sdss_ptr == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "sdss_ptr malloc error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return NULL;
    }
    memset(sdss_ptr, 0, sizeof(struct SimpleDataSubscribeStruct));

    sdss_ptr->point_count = (int) (subps->n_points);
    int size_temp = sdss_ptr->point_count * sizeof(struct SimpleDataSubscribeSubStruct);
    sdss_ptr->points = (struct SimpleDataSubscribeSubStruct *) malloc(size_temp);
    if (sdss_ptr->points == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "sdss_ptr->points malloc error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        free(sdss_ptr);
        return NULL;
    }
    memset(sdss_ptr->points, 0, size_temp);

    int ii = 0;
    for (ii = 0; ii < sdss_ptr->point_count; ii++) {
        transform_SubPoint_to_SimpleDataSubscribeSubStruct(subps->points[ii], &(sdss_ptr->points[ii]));
    }

    return sdss_ptr;
}

static int transform_ControlResponsePoint_to_ControlResponseSubStruct(Controlresponsedata__ControlResponsePoint *src,
                                                                      struct ControlResponseSubStruct *dst) {
    if (src == NULL || dst == NULL) {
        return 0;
    }

    dst->requestid = deep_copy_char_ptr(src->requestid);
    dst->messageid = deep_copy_char_ptr(src->messageid);
    dst->requestmethod = deep_copy_char_ptr(src->requestmethod);
    dst->calltype = deep_copy_char_ptr(src->calltype);
    dst->controlchannelid = deep_copy_char_ptr(src->controlchannelid);
    dst->productkey = deep_copy_char_ptr(src->productkey);
    dst->devicekey = deep_copy_char_ptr(src->devicekey);
    dst->assetid = deep_copy_char_ptr(src->assetid);
    dst->servicename = deep_copy_char_ptr(src->servicename);
    dst->serviceid = deep_copy_char_ptr(src->serviceid);
    dst->callbackurl = deep_copy_char_ptr(src->callbackurl);
    dst->inputdata = deep_copy_char_ptr(src->inputdata);
    dst->outputdata = deep_copy_char_ptr(src->outputdata);
    dst->status = src->status;
    dst->msg = deep_copy_char_ptr(src->msg);
    dst->submsg = deep_copy_char_ptr(src->submsg);
    dst->timeout = src->timeout;
    dst->gmtservicerequest = src->gmtservicerequest;
    dst->gmtservicereply = src->gmtservicereply;
    dst->gmtdevicereply = src->gmtdevicereply;
    dst->attr = deep_copy_char_ptr(src->attr);

    return 0;
}

static struct ControlResponseStruct *
transform_ControlResponsePoints_to_ControlResponseStruct(Controlresponsedata__ControlResponsePoints *crps) {
    if (crps == NULL) {
        return NULL;
    }

    struct ControlResponseStruct *crs_ptr = (struct ControlResponseStruct *) malloc(
            sizeof(struct ControlResponseStruct));
    if (crs_ptr == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "crs_ptr malloc error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return NULL;
    }
    memset(crs_ptr, 0, sizeof(struct ControlResponseStruct));

    crs_ptr->point_count = (int) (crps->n_points);
    int size_temp = crs_ptr->point_count * sizeof(struct ControlResponseSubStruct);
    crs_ptr->points = (struct ControlResponseSubStruct *) malloc(size_temp);
    if (crs_ptr->points == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "crs_ptr->points malloc error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        free(crs_ptr);
        return NULL;
    }
    memset(crs_ptr->points, 0, size_temp);

    int ii = 0;
    for (ii = 0; ii < crs_ptr->point_count; ii++) {
        transform_ControlResponsePoint_to_ControlResponseSubStruct(crps->points[ii], &(crs_ptr->points[ii]));
    }

    return crs_ptr;
}

static int transform_SetMeasurepointResponsePoint_to_SetMeasurepointResponseSubStruct(
        Setmeasurepointresponsedata__SetMeasurepointResponsePoint *src,
        struct SetMeasurepointResponseSubStruct *dst) {
    if (src == NULL || dst == NULL) {
        return 0;
    }

    dst->requestid = deep_copy_char_ptr(src->requestid);
    dst->orgid = deep_copy_char_ptr(src->orgid);
    dst->calltype = deep_copy_char_ptr(src->calltype);
    dst->setmeasurepointchannelid = deep_copy_char_ptr(src->setmeasurepointchannelid);
    dst->productkey = deep_copy_char_ptr(src->productkey);
    dst->devicekey = deep_copy_char_ptr(src->devicekey);
    dst->assetid = deep_copy_char_ptr(src->assetid);
    dst->measurepointid = deep_copy_char_ptr(src->measurepointid);
    dst->callbackurl = deep_copy_char_ptr(src->callbackurl);
    dst->inputdata = deep_copy_char_ptr(src->inputdata);
    dst->status = src->status;
    dst->msg = deep_copy_char_ptr(src->msg);
    dst->submsg = deep_copy_char_ptr(src->submsg);
    dst->timeout = src->timeout;
    dst->gmtsetmeasurepointrequest = src->gmtsetmeasurepointrequest;
    dst->gmtsetmeasurepointreply = src->gmtsetmeasurepointreply;
    dst->attr = deep_copy_char_ptr(src->attr);

    return 0;
}

static struct SetMeasurepointResponseStruct *
transform_SetMeasurepointResponsePoints_to_SetMeasurepointResponseStruct(
        Setmeasurepointresponsedata__SetMeasurepointResponsePoints *smrps) {
    if (smrps == NULL) {
        return NULL;
    }

    struct SetMeasurepointResponseStruct *smrs_ptr = (struct SetMeasurepointResponseStruct *) malloc(
            sizeof(struct SetMeasurepointResponseStruct));
    if (smrs_ptr == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "smrs_ptr malloc error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return NULL;
    }
    memset(smrs_ptr, 0, sizeof(struct SetMeasurepointResponseStruct));

    smrs_ptr->point_count = (int) (smrps->n_points);
    int size_temp = smrs_ptr->point_count * sizeof(struct SetMeasurepointResponseSubStruct);
    smrs_ptr->points = (struct SetMeasurepointResponseSubStruct *) malloc(size_temp);
    if (smrs_ptr->points == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "smrs_ptr->points malloc error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        free(smrs_ptr);
        return NULL;
    }
    memset(smrs_ptr->points, 0, size_temp);

    int ii = 0;
    for (ii = 0; ii < smrs_ptr->point_count; ii++) {
        transform_SetMeasurepointResponsePoint_to_SetMeasurepointResponseSubStruct(smrps->points[ii],
                                                                                   &(smrs_ptr->points[ii]));
    }

    return smrs_ptr;
}

static char *deep_copy_char_ptr(char *src) {
    if (src == NULL) {
        return NULL;
    }
    int dst_size = strlen(src) + 1;
    char *dst = (char *) malloc(dst_size);
    if (dst == NULL) {
        return NULL;
    }
    memcpy(dst, src, dst_size - 1);
    dst[dst_size - 1] = '\0';

    return dst;
}

static int delete_data_subscribe_sub_struct(struct DataSubscribeSubStruct *ptr) {
    if (ptr == NULL) {
        return 0;
    }

    if (ptr->orgid != NULL) {
        free(ptr->orgid);
        ptr->orgid = NULL;
    }

    if (ptr->modelid != NULL) {
        free(ptr->modelid);
        ptr->modelid = NULL;
    }

    if (ptr->assetid != NULL) {
        free(ptr->assetid);
        ptr->assetid = NULL;
    }

    if (ptr->collectdeviceid != NULL) {
        free(ptr->collectdeviceid);
        ptr->collectdeviceid = NULL;
    }

    if (ptr->pointid != NULL) {
        free(ptr->pointid);
        ptr->pointid = NULL;
    }

    if (ptr->value != NULL) {
        free(ptr->value);
        ptr->value = NULL;
    }

    if (ptr->modelpath != NULL) {
        free(ptr->modelpath);
        ptr->modelpath = NULL;
    }

    if (ptr->policytype != NULL) {
        free(ptr->policytype);
        ptr->policytype = NULL;
    }

    if (ptr->signaltype != NULL) {
        free(ptr->signaltype);
        ptr->signaltype = NULL;
    }

    if (ptr->datatype != NULL) {
        free(ptr->datatype);
        ptr->datatype = NULL;
    }

    if (ptr->subdatatype != NULL) {
        free(ptr->subdatatype);
        ptr->subdatatype = NULL;
    }

    if (ptr->attr != NULL) {
        free(ptr->attr);
        ptr->attr = NULL;
    }

    //这里不需要再free本身，在delete_data_subscribe_struct会一起释放
    return 0;
}

static int delete_data_subscribe_struct(struct DataSubscribeStruct *ptr) {
    if (ptr == NULL) {
        return 0;
    }

    int ii = 0;
    if (ptr->points != NULL) {
        for (ii = 0; ii < ptr->point_count; ii++) {
            delete_data_subscribe_sub_struct(&(ptr->points[ii]));
        }
        free(ptr->points);
        ptr->points = NULL;
    }

    free(ptr);
    return 0;
}

static int delete_simple_data_subscribe_sub_struct(struct SimpleDataSubscribeSubStruct *ptr) {
    if (ptr == NULL) {
        return 0;
    }

    if (ptr->assetid != NULL) {
        free(ptr->assetid);
        ptr->assetid = NULL;
    }

    if (ptr->pointid != NULL) {
        free(ptr->pointid);
        ptr->pointid = NULL;
    }

    if (ptr->value != NULL) {
        free(ptr->value);
        ptr->value = NULL;
    }

    if (ptr->datatype != NULL) {
        free(ptr->datatype);
        ptr->datatype = NULL;
    }

    if (ptr->subdatatype != NULL) {
        free(ptr->subdatatype);
        ptr->subdatatype = NULL;
    }

    if (ptr->attr != NULL) {
        free(ptr->attr);
        ptr->attr = NULL;
    }

    //这里不需要再free本身，在delete_simple_data_subscribe_struct会一起释放
    return 0;
}

static int delete_simple_data_subscribe_struct(struct SimpleDataSubscribeStruct *ptr) {
    if (ptr == NULL) {
        return 0;
    }

    int ii = 0;
    if (ptr->points != NULL) {
        for (ii = 0; ii < ptr->point_count; ii++) {
            delete_simple_data_subscribe_sub_struct(&(ptr->points[ii]));
        }
        free(ptr->points);
        ptr->points = NULL;
    }

    free(ptr);
    return 0;
}

static int delete_control_response_sub_struct(struct ControlResponseSubStruct *ptr) {
    if (ptr == NULL) {
        return 0;
    }

    if (ptr->requestid != NULL) {
        free(ptr->requestid);
        ptr->requestid = NULL;
    }

    if (ptr->messageid != NULL) {
        free(ptr->messageid);
        ptr->messageid = NULL;
    }

    if (ptr->requestmethod != NULL) {
        free(ptr->requestmethod);
        ptr->requestmethod = NULL;
    }

    if (ptr->calltype != NULL) {
        free(ptr->calltype);
        ptr->calltype = NULL;
    }

    if (ptr->controlchannelid != NULL) {
        free(ptr->controlchannelid);
        ptr->controlchannelid = NULL;
    }

    if (ptr->productkey != NULL) {
        free(ptr->productkey);
        ptr->productkey = NULL;
    }

    if (ptr->devicekey != NULL) {
        free(ptr->devicekey);
        ptr->devicekey = NULL;
    }

    if (ptr->assetid != NULL) {
        free(ptr->assetid);
        ptr->assetid = NULL;
    }

    if (ptr->servicename != NULL) {
        free(ptr->servicename);
        ptr->servicename = NULL;
    }

    if (ptr->serviceid != NULL) {
        free(ptr->serviceid);
        ptr->serviceid = NULL;
    }

    if (ptr->callbackurl != NULL) {
        free(ptr->callbackurl);
        ptr->callbackurl = NULL;
    }

    if (ptr->inputdata != NULL) {
        free(ptr->inputdata);
        ptr->inputdata = NULL;
    }

    if (ptr->outputdata != NULL) {
        free(ptr->outputdata);
        ptr->outputdata = NULL;
    }

    if (ptr->msg != NULL) {
        free(ptr->msg);
        ptr->msg = NULL;
    }

    if (ptr->submsg != NULL) {
        free(ptr->submsg);
        ptr->submsg = NULL;
    }

    if (ptr->attr != NULL) {
        free(ptr->attr);
        ptr->attr = NULL;
    }

    //这里不需要再free本身，在delete_data_subscribe_struct会一起释放
    return 0;
}

static int delete_control_response_struct(struct ControlResponseStruct *ptr) {
    if (ptr == NULL) {
        return 0;
    }

    int ii = 0;
    if (ptr->points != NULL) {
        for (ii = 0; ii < ptr->point_count; ii++) {
            delete_control_response_sub_struct(&(ptr->points[ii]));
        }
        free(ptr->points);
        ptr->points = NULL;
    }

    free(ptr);
    return 0;
}

static int delete_set_measurepoint_response_sub_struct(struct SetMeasurepointResponseSubStruct *ptr) {
    if (ptr == NULL) {
        return 0;
    }

    if (ptr->requestid != NULL) {
        free(ptr->requestid);
        ptr->requestid = NULL;
    }

    if (ptr->orgid != NULL) {
        free(ptr->orgid);
        ptr->orgid = NULL;
    }

    if (ptr->calltype != NULL) {
        free(ptr->calltype);
        ptr->calltype = NULL;
    }

    if (ptr->setmeasurepointchannelid != NULL) {
        free(ptr->setmeasurepointchannelid);
        ptr->setmeasurepointchannelid = NULL;
    }

    if (ptr->productkey != NULL) {
        free(ptr->productkey);
        ptr->productkey = NULL;
    }

    if (ptr->devicekey != NULL) {
        free(ptr->devicekey);
        ptr->devicekey = NULL;
    }

    if (ptr->assetid != NULL) {
        free(ptr->assetid);
        ptr->assetid = NULL;
    }

    if (ptr->measurepointid != NULL) {
        free(ptr->measurepointid);
        ptr->measurepointid = NULL;
    }

    if (ptr->callbackurl != NULL) {
        free(ptr->callbackurl);
        ptr->callbackurl = NULL;
    }

    if (ptr->inputdata != NULL) {
        free(ptr->inputdata);
        ptr->inputdata = NULL;
    }

    if (ptr->msg != NULL) {
        free(ptr->msg);
        ptr->msg = NULL;
    }

    if (ptr->submsg != NULL) {
        free(ptr->submsg);
        ptr->submsg = NULL;
    }

    if (ptr->attr != NULL) {
        free(ptr->attr);
        ptr->attr = NULL;
    }

    //这里不需要再free本身，在delete_data_subscribe_struct会一起释放
    return 0;
}

static int delete_set_measurepoint_response_struct(struct SetMeasurepointResponseStruct *ptr) {
    if (ptr == NULL) {
        return 0;
    }

    int ii = 0;
    if (ptr->points != NULL) {
        for (ii = 0; ii < ptr->point_count; ii++) {
            delete_set_measurepoint_response_sub_struct(&(ptr->points[ii]));
        }
        free(ptr->points);
        ptr->points = NULL;
    }

    free(ptr);
    return 0;
}

//主备检测线程
static void *master_slave_check_job_callback(void *arg) {
    //开启接收cancel信号的功能
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    //收到cancel信号后使用pthread_testcancel()退出
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    struct DataServiceCtx *ctx = (struct DataServiceCtx *) arg;
    if (ctx == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx is NULL");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return NULL;
    }

    struct timeval time_previous;
    gettimeofday(&time_previous, NULL);
    struct timeval time_current;
    int time_passed_ms = 0;

    while (1) {
        pthread_testcancel();

        gettimeofday(&time_current, NULL);
        time_passed_ms = (time_current.tv_sec - time_previous.tv_sec) * 1000 +
                         (time_current.tv_usec - time_previous.tv_usec) / 1000;
        if (time_passed_ms >= EDGE_DATASERVICE_MASTER_SLAVE_THREAD_CHECK_PERIOD) {
            time_previous.tv_sec = time_current.tv_sec;
            time_previous.tv_usec = time_current.tv_usec;
            //获取主机IP
            struct IPBox *master_ip_box = get_current_master_ip_list(ctx->ip_box, ctx->ha_port);
            int equal_flag = 0;

            if (master_ip_box != NULL) {
                //加读锁进行比较
                pthread_rwlock_rdlock(&(ctx->reinit_rwlock));
                equal_flag = check_ip_box_is_equal(master_ip_box, ctx->ip_box_used);
                pthread_rwlock_unlock(&(ctx->reinit_rwlock));

                if (equal_flag != 0) {
                    char log_content[1024];
                    memset(log_content, 0, sizeof(log_content));
                    char old_master_content[1024];
                    memset(old_master_content, 0, sizeof(old_master_content));
                    char new_master_content[1024];
                    memset(new_master_content, 0, sizeof(new_master_content));

                    if (ctx->ip_box_used == NULL || ctx->ip_box_used->count <= 0) {
                        snprintf(old_master_content, sizeof(old_master_content), "empty");
                    } else {
                        snprintf(old_master_content, sizeof(old_master_content), "%s",
                                 ctx->ip_box_used->ip_list[0].str);
                    }

                    if (master_ip_box == NULL || master_ip_box->count <= 0) {
                        snprintf(new_master_content, sizeof(new_master_content), "empty");
                    } else {
                        snprintf(new_master_content, sizeof(new_master_content), "%s", master_ip_box->ip_list[0].str);
                    }

                    snprintf(log_content, sizeof(log_content), "master changed, old master is:%s, new master is:%s",
                             old_master_content, new_master_content);
                    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO, log_content);

                    if (master_ip_box != NULL && master_ip_box->count > 0) {
                        //加写锁进行赋值
                        pthread_rwlock_wrlock(&(ctx->reinit_rwlock));
                        set_ip_used_withlock(ctx, master_ip_box);
                        pthread_rwlock_unlock(&(ctx->reinit_rwlock));
                    }

                    //需要重连
                    set_need_reinit_withlock(ctx, 1);
                }
            }
            delete_ip_box(master_ip_box);
        }

        edge_sleep(1000);
    }

    return NULL;
}

//检查服务端是否工作正常的线程
static void *service_check_job(void *arg) {
    //开启接收cancel信号的功能
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    //收到cancel信号后使用pthread_testcancel()退出
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    struct DataServiceCtx *ctx = (struct DataServiceCtx *) arg;
    if (ctx == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx is NULL");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return NULL;
    }

    char ip_need_check[128];
    int ret = 0;
    int error_count = 0;
    struct timeval time_previous;
    gettimeofday(&time_previous, NULL);
    struct timeval time_current;
    int time_passed_ms = 0;

    while (1) {
        pthread_testcancel();

        gettimeofday(&time_current, NULL);
        time_passed_ms = (time_current.tv_sec - time_previous.tv_sec) * 1000 +
                         (time_current.tv_usec - time_previous.tv_usec) / 1000;
        if (time_passed_ms >= EDGE_DATASERVICE_SERVICE_CHECK_PERIOD) {
            time_previous.tv_sec = time_current.tv_sec;
            time_previous.tv_usec = time_current.tv_usec;

            memset(ip_need_check, 0, sizeof(ip_need_check));
            //加读锁获取IP
            get_ip_used_withlock(ctx, ip_need_check, sizeof(ip_need_check));

            if (strlen(ip_need_check) > 0) {
                ret = check_service_available(ip_need_check, ctx->service_check_port);
                if (ret < 0) {
                    error_count++;
                    if (error_count >= EDGE_DATASERVICE_SERVICE_CHECK_MAX_COUNT) {
                        char log_content[1024];
                        memset(log_content, 0, sizeof(log_content));
                        snprintf(log_content, sizeof(log_content), "service(%s) is not available, need reconnect",
                                 ip_need_check);
                        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO, log_content);

                        //需要重连
                        set_need_reinit_withlock(ctx, 1);
                    }
                } else {
                    error_count = 0;
                }
            }
        }

        edge_sleep(1000);
    }

    return NULL;
}

//主工作线程
static void *main_work_job(void *arg) {
    //开启接收cancel信号的功能
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    //收到cancel信号后使用pthread_testcancel()退出
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    struct DataServiceCtx *ctx = (struct DataServiceCtx *) arg;
    if (ctx == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx is NULL");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return NULL;
    }

    fd_set fds;
    int fd_array[64];
    struct timeval wait;
    int ret = 0;
    int len = 0;
    int count = 0;
    int select_max = 0;
    int errno_temp = 0;
    memset((void *) fd_array, 0, sizeof(fd_array));

    //启动主循环
    while (1) {
        pthread_testcancel();

        ret = 0;
        //处理重连
        pthread_cleanup_push(pthread_rwlock_unlock, (void *) (&(ctx->reinit_rwlock)));
            pthread_rwlock_wrlock(&(ctx->reinit_rwlock));
            if (ctx->need_reinit == 1) {
                edge_sleep(1000);
                ret = ctx_reconnect(ctx);//这里面有一些系统函数默认含有取消点，需要使用pthread_cleanup_push和pthread_cleanup_pop防止死锁
                if (ret < 0) {
                    ctx->need_reinit = 1;
                } else {
                    ctx->need_reinit = 0;
                }
            }
            pthread_rwlock_unlock(&(ctx->reinit_rwlock));
        pthread_cleanup_pop(0);

        if (ret < 0) {
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content), "ctx_reconnect error");
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
            edge_sleep(1000);
            continue;
        }

        //操作nsq初始化状态机
        pthread_cleanup_push(pthread_rwlock_unlock, (void *) (&(ctx->nsq_init_status_rwlock)));
            pthread_rwlock_wrlock(&(ctx->nsq_init_status_rwlock));
            ret = ctx_init_nsq(ctx);//这里面有一些系统函数默认含有取消点，需要使用pthread_cleanup_push和pthread_cleanup_pop防止死锁
            pthread_rwlock_unlock(&(ctx->nsq_init_status_rwlock));
        pthread_cleanup_pop(0);
        if (ret < 0) {
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content), "ctx_init_nsq error, need reconncet");
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);

            //需要重连
            set_need_reinit_withlock(ctx, 1);
            continue;
        }

        //收数据
        count = 0;
        memset((void *) fd_array, 0, sizeof(fd_array));
        FD_ZERO(&fds);

        if (ctx->tcp_sock_fd >= 0) {
            fd_array[count] = ctx->tcp_sock_fd;
            FD_SET(fd_array[count], &fds);
            count++;
        }

        select_max = max_in_array(fd_array, count);
        select_max++;
        wait.tv_sec = 1;
        wait.tv_usec = 0;
        ret = select(select_max, &fds, NULL, NULL, &wait);
        if (ret <= 0) {
            errno_temp = errno;
            if (0 == ret) { //超时
                continue;
            }

            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content),
                     "select error while processing recv, errno=%d, need reconnect",
                     errno_temp);
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                           log_content);
            set_need_reinit_withlock(ctx, 1);
            continue;
        }

        if (ctx->tcp_sock_fd >= 0) {
            if (FD_ISSET(ctx->tcp_sock_fd, &fds)) {
                clear_recv_buf_small(ctx);
                len = recv(ctx->tcp_sock_fd, ctx->recv_buf_small, EDGE_DATASERVICE_BUF_SMALL_SIZE, 0);
                if (len > 0) {
                    ctx_deal_recv_small(ctx, len);
                } else {
                    errno_temp = errno;
                    char log_content[1024];
                    memset(log_content, 0, sizeof(log_content));
                    snprintf(log_content, sizeof(log_content), "recv function return=%d, errno=%d, need reconnect",
                             len,
                             errno_temp);
                    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                                   log_content);
                    set_need_reinit_withlock(ctx, 1);
                    continue;
                }
            }
        }
    }

}

//处理一次recv调用接收到的数据,一般需要在这里拼成一个完整交互报文
static int ctx_deal_recv_small(struct DataServiceCtx *ctx, int len) {
    int ii = 0;
    int ret = 0;
    struct nsq_packet_header *nsq_packet_header_p = NULL;
    for (ii = 0; ii < len; ii++) {
        if (!(ctx->recv_syn_flag)) {
            ctx->recv_buf_big[sizeof(struct nsq_packet_header) - 1] = ctx->recv_buf_small[ii];
            nsq_packet_header_p = (struct nsq_packet_header *) (ctx->recv_buf_big);
            ctx->has_recv_bytes++;

            if (ctx->has_recv_bytes >= sizeof(struct nsq_packet_header)) {
                ctx->recv_syn_flag = 1;
                ctx->should_recv_bytes = MY_ntohl(nsq_packet_header_p->size) + 4;
                if (ctx->should_recv_bytes >= EDGE_DATASERVICE_BUF_BIG_SIZE) {
                    char log_content[1024];
                    memset(log_content, 0, sizeof(log_content));
                    snprintf(log_content, sizeof(log_content),
                             "frame size=%d is too large(>=%d), maybe something error, need reconnect",
                             ctx->should_recv_bytes,
                             EDGE_DATASERVICE_BUF_BIG_SIZE);
                    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                                   log_content);
                    set_need_reinit_withlock(ctx, 1);
                    return -1;
                }
                if (ctx->has_recv_bytes >= ctx->should_recv_bytes) {
                    ret = ctx_deal_recv_big(ctx);
                    clear_recv_resource(ctx);
                }
            } else {
                memmove(ctx->recv_buf_big, &(ctx->recv_buf_big[1]), sizeof(struct nsq_packet_header) - 1);
            }
        } else {
            if (ctx->has_recv_bytes >= EDGE_DATASERVICE_BUF_BIG_SIZE) {
                char log_content[1024];
                memset(log_content, 0, sizeof(log_content));
                snprintf(log_content, sizeof(log_content),
                         "has_recv_bytes=%d is too large(>=%d), maybe something error, need reconnect",
                         ctx->has_recv_bytes, EDGE_DATASERVICE_BUF_BIG_SIZE);
                edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                               log_content);
                set_need_reinit_withlock(ctx, 1);
                return -1;
            }

            ctx->recv_buf_big[ctx->has_recv_bytes] = ctx->recv_buf_small[ii];
            ctx->has_recv_bytes++;

            if (ctx->has_recv_bytes >= ctx->should_recv_bytes) {
                ret = ctx_deal_recv_big(ctx);
                clear_recv_resource(ctx);
            }

        }
    }

    return ret;
}

//处理一个完整交互报文
static int ctx_deal_recv_big(struct DataServiceCtx *ctx) {
    struct nsq_packet_header *nsq_packet_header_p = NULL;
    struct nsq_packet_header nsq_packet_header_local;
    int ret = 0;

    memset((void *) &nsq_packet_header_local, 0, sizeof(struct nsq_packet_header));

    nsq_packet_header_p = (struct nsq_packet_header *) (ctx->recv_buf_big);
    nsq_packet_header_local.size = MY_ntohl(nsq_packet_header_p->size);
    nsq_packet_header_local.frame_type = MY_ntohl(nsq_packet_header_p->frame_type);

    switch (nsq_packet_header_local.frame_type) {
        case NSQ_FRAME_TYPE_RESPONSE://命令反较，心跳等
            return ctx_deal_response_msg(ctx, &nsq_packet_header_local);
            break;
        case NSQ_FRAME_TYPE_ERROR://错误返回
            return ctx_deal_error_msg(ctx, &nsq_packet_header_local);
            break;
        case NSQ_FRAME_TYPE_MESSAGE://消息
            return ctx_deal_message_msg(ctx, &nsq_packet_header_local);
            break;
        default:
            break;
    }

    return ret;
}

//处理response
static int ctx_deal_response_msg(struct DataServiceCtx *ctx, struct nsq_packet_header *nsq_packet_header_local_p) {
    char *msg_ptr = NULL;
    msg_ptr = ctx->recv_buf_big + sizeof(struct nsq_packet_header);

    int ret = 0;
    if (ctx->ctx_type == CTX_TYPE_ONLY_CONSUMER) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "recv response=%s", msg_ptr);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO,
                       log_content);
    }

    if (strncmp(msg_ptr, "OK", 2) == 0) {
        //命令成功反较
        int status = get_nsq_init_status_withlock(ctx);
        if (status == NSQ_INIT_STATUS_WAIT_SUB_RESPONSE) {
            set_nsq_init_status_withlock(ctx, NSQ_INIT_STATUS_SUB_SUCCESS);
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content),
                     "recv response=%s, previous status=NSQ_INIT_STATUS_WAIT_SUB_RESPONSE, change to NSQ_INIT_STATUS_SUB_SUCCESS",
                     msg_ptr);
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO,
                           log_content);
        }
    } else if (strncmp(msg_ptr, "_heartbeat_", 11) == 0) {
        //心跳
        //发送NOP，这边需要加锁
        pthread_cleanup_push(pthread_rwlock_unlock, (void *) (&(ctx->nsq_send_rwlock)));
            pthread_rwlock_wrlock(&(ctx->nsq_send_rwlock));
            ret = ctx_send_nop(ctx->tcp_sock_fd);
            pthread_rwlock_unlock(&(ctx->nsq_send_rwlock));
        pthread_cleanup_pop(0);

        if (ret < 0) {
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content), "recv=%s, send NOP error, need reconnect", msg_ptr);
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO, log_content);

            set_need_reinit_withlock(ctx, 1);
        }
    } else if (strncmp(msg_ptr, "{", 1) == 0) {
        //auth成功反较
        int status = get_nsq_init_status_withlock(ctx);
        if (status == NSQ_INIT_STATUS_WAIT_AUTH_RESPONSE) {
            set_nsq_init_status_withlock(ctx, NSQ_INIT_STATUS_AUTH_SUCCESS);
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content),
                     "recv response=%s, previous status=NSQ_INIT_STATUS_WAIT_AUTH_RESPONSE, change to NSQ_INIT_STATUS_AUTH_SUCCESS",
                     msg_ptr);
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO,
                           log_content);
        } else {
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content), "unknown msg=%s", msg_ptr);
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO, log_content);
        }
    } else {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "unknown msg=%s", msg_ptr);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO, log_content);
    }

    return 0;
}

//处理error
static int ctx_deal_error_msg(struct DataServiceCtx *ctx, struct nsq_packet_header *nsq_packet_header_local_p) {
    char *msg_ptr = NULL;
    msg_ptr = ctx->recv_buf_big + sizeof(struct nsq_packet_header);

    char log_content[1024];
    memset(log_content, 0, sizeof(log_content));
    snprintf(log_content, sizeof(log_content), "recv error msg=%s", msg_ptr);
    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO,
                   log_content);

    int status = get_nsq_init_status_withlock(ctx);
    if (status == NSQ_INIT_STATUS_WAIT_SUB_RESPONSE) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "recv error msg=%s, previous status=NSQ_INIT_STATUS_WAIT_SUB_RESPONSE, need reconnect",
                 msg_ptr);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO,
                       log_content);

        set_need_reinit_withlock(ctx, 1);
    } else if (status == NSQ_INIT_STATUS_WAIT_AUTH_RESPONSE) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "recv error msg=%s, auth failed, need reconnect",
                 msg_ptr);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO,
                       log_content);

        set_need_reinit_withlock(ctx, 1);
    }

    return 0;
}

//处理message
static int ctx_deal_message_msg(struct DataServiceCtx *ctx, struct nsq_packet_header *nsq_packet_header_local_p) {
    char *msg_ptr = NULL;
    msg_ptr = ctx->recv_buf_big + sizeof(struct nsq_packet_header);
    int ret = 0;

    int msg_len = nsq_packet_header_local_p->size - 4 - sizeof(struct nsq_message_header);
    if (msg_len < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "bad packet, maybe something error, need reconnect");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                       log_content);
        set_need_reinit_withlock(ctx, 1);
        return -1;
    }

    struct nsq_message_header *nsq_message_header_ptr = NULL;
    struct nsq_message_header nsq_message_header_local;
    memset((void *) &nsq_message_header_local, 0, sizeof(struct nsq_message_header));

    nsq_message_header_ptr = (struct nsq_message_header *) msg_ptr;
    nsq_message_header_local.timestamp_ns = MY_ntohl64(nsq_message_header_ptr->timestamp_ns);
    nsq_message_header_local.attempts = MY_ntohs(nsq_message_header_ptr->attempts);

    char msg_id[32];
    memset(msg_id, 0, sizeof(msg_id));
    memcpy(msg_id, nsq_message_header_ptr->message_id, sizeof(nsq_message_header_ptr->message_id));

    msg_ptr += sizeof(struct nsq_message_header);

    if (ctx->msg_callback != NULL) {
        do {
            struct DataServiceMessage *ds_msg = (struct DataServiceMessage *) malloc(
                    sizeof(struct DataServiceMessage));
            if (ds_msg == NULL) {
                char log_content[1024];
                memset(log_content, 0, sizeof(log_content));
                snprintf(log_content, sizeof(log_content), "DataServiceMessage malloc error");
                edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
                break;
            }
            memset(ds_msg, 0, sizeof(struct DataServiceMessage));

            ds_msg->topic_type = ctx->topic_type_actual;
            if (ds_msg->topic_type == TOPIC_TYPE_CUSTOM) {
                ds_msg->msg_len = msg_len;
            }
            ds_msg->msg = decode_msg(msg_ptr, msg_len, ds_msg->topic_type);
            if (ds_msg->msg == NULL) {
                char log_content[1024];
                memset(log_content, 0, sizeof(log_content));
                snprintf(log_content, sizeof(log_content), "decode_msg error");
                edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
                free(ds_msg);
                break;
            }

            ctx->msg_callback(NULL, ctx->channel_id, ds_msg, ctx->user_ctx);
        } while (0);
    }

    //这边需要加锁
    pthread_cleanup_push(pthread_rwlock_unlock, (void *) (&(ctx->nsq_send_rwlock)));
        pthread_rwlock_wrlock(&(ctx->nsq_send_rwlock));
        ret = ctx_send_fin(ctx->tcp_sock_fd, msg_id);
        if (ret < 0) {
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content), "ctx_send_fin error, need reconnect");
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        } else {
            ret = ctx_send_rdy(ctx->tcp_sock_fd, 1);
            if (ret < 0) {
                char log_content[1024];
                memset(log_content, 0, sizeof(log_content));
                snprintf(log_content, sizeof(log_content), "ctx_send_rdy error, need reconnect");
                edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
            }
        }
        pthread_rwlock_unlock(&(ctx->nsq_send_rwlock));
    pthread_cleanup_pop(0);

    if (ret < 0) {
        set_need_reinit_withlock(ctx, 1);
        return -1;
    }

    return 0;
}

//期望接收一帧nsq的OK反较
static int expect_recv_ok_frame(int sock, int timeout_ms) {
    return expect_recv_specified_frame(sock, "OK", 2, timeout_ms);
}

//期望接收一帧nsq的auth正确反较
static int expect_recv_auth_ok_frame(int sock, int timeout_ms) {
    return expect_recv_specified_frame(sock, "{", 1, timeout_ms);
}

//期望接收一帧nsq的指定反较
static int expect_recv_specified_frame(int sock, char *expect_in, int expect_in_len, int timeout_ms) {
    if (sock < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "sock is illegal");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    fd_set fds;
    int fd_array[64];
    struct timeval wait;
    struct timeval time_begin;
    struct timeval time_current;
    int time_used_msec;
    int ret = 0;
    int len = 0;
    int count = 0;
    int select_max = 0;
    int errno_temp = 0;
    memset((void *) fd_array, 0, sizeof(fd_array));
    char small_recv_buf[128];
    memset((void *) small_recv_buf, 0, sizeof(small_recv_buf));
    char big_recv_buf[2048];
    memset((void *) big_recv_buf, 0, sizeof(big_recv_buf));
    int has_recv_bytes = 0;//已经接收到的字节数
    int should_recv_bytes = -1;//还需要接收的字节数
    struct nsq_packet_header *nsq_packet_header_p = NULL;
    struct nsq_packet_header nsq_packet_header_local;
    memset((void *) &nsq_packet_header_local, 0, sizeof(struct nsq_packet_header));
    char *content_ptr = NULL;

    gettimeofday(&time_begin, NULL);
    while (1) {
        gettimeofday(&time_current, NULL);
        time_used_msec =
                (time_current.tv_sec - time_begin.tv_sec) * 1000 + (time_current.tv_usec - time_begin.tv_usec) / 1000;
        if (time_used_msec > timeout_ms) {
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content), "wait recv timeout, need reconnect");
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);

            return -1;
        }

        //收数据
        count = 0;
        memset((void *) fd_array, 0, sizeof(fd_array));
        FD_ZERO(&fds);

        if (sock >= 0) {
            fd_array[count] = sock;
            FD_SET(fd_array[count], &fds);
            count++;
        }

        select_max = max_in_array(fd_array, count);
        select_max++;
        wait.tv_sec = 1;
        wait.tv_usec = 0;
        ret = select(select_max, &fds, NULL, NULL, &wait);
        if (ret <= 0) {
            errno_temp = errno;
            if (0 == ret) { //超时
                continue;
            }

            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content),
                     "select error while processing recv, errno=%d, need reconnect",
                     errno_temp);
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                           log_content);
            return -1;
        }

        if (sock >= 0) {
            if (FD_ISSET(sock, &fds)) {
                memset((void *) small_recv_buf, 0, sizeof(small_recv_buf));
                len = recv(sock, small_recv_buf, sizeof(small_recv_buf), 0);
                if (len <= 0) {
                    errno_temp = errno;
                    char log_content[1024];
                    memset(log_content, 0, sizeof(log_content));
                    snprintf(log_content, sizeof(log_content), "recv function return=%d, errno=%d, need reconnect",
                             len,
                             errno_temp);
                    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                                   log_content);
                    return -1;
                }

                if (has_recv_bytes + len > sizeof(big_recv_buf) - 1) {
                    char log_content[1024];
                    memset(log_content, 0, sizeof(log_content));
                    snprintf(log_content, sizeof(log_content),
                             "recv len is too large, maybe something error, need reconnect");
                    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                                   log_content);
                    return -1;
                }

                memcpy(big_recv_buf + has_recv_bytes, small_recv_buf, len);
                has_recv_bytes += len;
                if (has_recv_bytes < sizeof(struct nsq_packet_header)) {
                    continue;
                }

                nsq_packet_header_p = (struct nsq_packet_header *) (big_recv_buf);
                if (should_recv_bytes < 0) {
                    should_recv_bytes = MY_ntohl(nsq_packet_header_p->size) + 4;
                }
                if (has_recv_bytes < should_recv_bytes) {
                    continue;
                }

                nsq_packet_header_local.size = MY_ntohl(nsq_packet_header_p->size);
                nsq_packet_header_local.frame_type = MY_ntohl(nsq_packet_header_p->frame_type);

                char log_content[1024];
                content_ptr = big_recv_buf + sizeof(struct nsq_packet_header);
                switch (nsq_packet_header_local.frame_type) {
                    case NSQ_FRAME_TYPE_RESPONSE://命令反较，心跳等
                        if (strncmp(content_ptr, expect_in, expect_in_len) == 0) {
                            return 0;
                        } else {
                            memset(log_content, 0, sizeof(log_content));
                            snprintf(log_content, sizeof(log_content),
                                     "expect %s, but recv response message(%s), maybe something error, need reconnect",
                                     expect_in, content_ptr);
                            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                                           log_content);
                            return -1;
                        }
                        break;
                    case NSQ_FRAME_TYPE_ERROR://错误返回
                        memset(log_content, 0, sizeof(log_content));
                        snprintf(log_content, sizeof(log_content),
                                 "expect %s, but recv error message(%s), maybe something error, need reconnect",
                                 expect_in, content_ptr);
                        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                                       log_content);
                        return -1;
                        break;
                    case NSQ_FRAME_TYPE_MESSAGE://消息
                        memset(log_content, 0, sizeof(log_content));
                        snprintf(log_content, sizeof(log_content),
                                 "expect %s, but recv subscribe message, maybe something error, need reconnect",
                                 expect_in);
                        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                                       log_content);
                        return -1;
                        break;
                    default:
                        memset(log_content, 0, sizeof(log_content));
                        snprintf(log_content, sizeof(log_content),
                                 "expect %s, but recv unknown message, maybe something error, need reconnect",
                                 expect_in);
                        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                                       log_content);
                        return -1;
                }
            }
        }
    }

    return 0;
}

//重连函数
static int ctx_reconnect(struct DataServiceCtx *ctx) {
    if (ctx == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx is NULL");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    //关闭tcp连接
    close_tcp_sock_fd(ctx);
    //清理recv资源
    uninit_recv_resource(ctx);
    //清理nsq应用层初始化标志位
    set_nsq_init_status_withlock(ctx, NSQ_INIT_STATUS_NOTHING);

    int ret = 0;

    //获取ip
    char ip_used[128];
    memset(ip_used, 0, sizeof(ip_used));
    ret = get_ip_used(ctx, ip_used, sizeof(ip_used));
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "get_ip_used error while reconnecting, need reconnect aggin, ip=%s, port=%d", ip_used, ctx->port);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    //初始化recv资源
    ret = init_recv_resource(ctx);
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "init_recv_resource error while reconnecting, need reconnect aggin, ip=%s, port=%d", ip_used,
                 ctx->port);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    //tcp连接
    int sock_fd = connect_server(ip_used, (unsigned short) (ctx->port));
    if (sock_fd < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "connect_server error while reconnecting, need reconnect aggin, ip=%s, port=%d", ip_used,
                 ctx->port);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    ctx->tcp_sock_fd = sock_fd;
    char log_content[1024];
    memset(log_content, 0, sizeof(log_content));
    snprintf(log_content, sizeof(log_content),
             "connect_server success while reconnecting, ip=%s, port=%d", ip_used, ctx->port);
    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO, log_content);

    return 0;
}

//连接并发送magic
static int ctx_connect_and_send_magic(struct DataServiceCtx *ctx) {
    int ret = 0;
    char ip_used[128];
    memset(ip_used, 0, sizeof(ip_used));
    ret = get_ip_used_withlock(ctx, ip_used, sizeof(ip_used));
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "get ip error, can not start");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    //tcp连接
    int sock_fd = connect_server(ip_used, (unsigned short) (ctx->port));
    if (sock_fd < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "connect_server error while ctx_connect_and_send_magic, ip=%s, port=%d", ip_used, ctx->port);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_WARN, log_content);
        return -1;
    }

    //发送magic code
    ret = ctx_send_magic(sock_fd);
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "ctx_send_magic error while ctx_connect_and_send_magic, ip=%s, port=%d", ip_used, ctx->port);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_WARN, log_content);
        close(sock_fd);
        return -1;
    }

    //关闭心跳
    ret = ctx_close_heartbeat(sock_fd);
    if (ret >= 0) {
        ret = expect_recv_ok_frame(sock_fd, 10000);
    }
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "ctx_close_heartbeat error while ctx_connect_and_send_magic, ip=%s, port=%d", ip_used, ctx->port);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_WARN, log_content);
        close(sock_fd);
        return -1;
    }

    //发送auth
    if (strlen(ctx->p) > 0) {
        ret = ctx_send_auth(sock_fd, ctx->p);
        if (ret >= 0) {
            ret = expect_recv_auth_ok_frame(sock_fd, 10000);
        }
        if (ret < 0) {
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content),
                     "ctx_send_auth error while ctx_connect_and_send_magic, ip=%s, port=%d", ip_used, ctx->port);
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_WARN, log_content);
            close(sock_fd);
            return -1;
        }
    }

    set_tcp_sock_fd_withlock(ctx, sock_fd);
    char log_content[1024];
    memset(log_content, 0, sizeof(log_content));
    snprintf(log_content, sizeof(log_content),
             "connect_server success while ctx_connect_and_send_magic, ip=%s, port=%d", ip_used, ctx->port);
    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO, log_content);

    if (ctx->connect_callback != NULL) {
        ctx->connect_callback(NULL, ctx->channel_id, ctx->user_ctx);
    }

    return 0;
}

//操作nsq初始化状态机
static int ctx_init_nsq(struct DataServiceCtx *ctx) {
    if (ctx == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx is NULL while ctx_init_nsq");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    if (ctx->tcp_sock_fd < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "tcp_sock_fd < 0 while ctx_init_nsq");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    int ret = 0;
    int status = ctx->nsq_init_status;
    char log_content[1024];
    struct timeval time_current;
    switch (status) {
        case NSQ_INIT_STATUS_NOTHING:
            gettimeofday(&(ctx->nsq_init_start_ts), NULL);
            //设置协议版本
            ret = ctx_send_magic(ctx->tcp_sock_fd);
            if (ret < 0) {
                memset(log_content, 0, sizeof(log_content));
                snprintf(log_content, sizeof(log_content), "ctx_send_magic error");
                edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
                return ret;
            }


            if (strlen(ctx->p) > 0) {
                //发送auth信息
                ret = ctx_send_auth(ctx->tcp_sock_fd, ctx->p);
                if (ret < 0) {
                    memset(log_content, 0, sizeof(log_content));
                    snprintf(log_content, sizeof(log_content), "ctx_send_auth error");
                    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
                    return ret;
                }
                ctx->nsq_init_status = NSQ_INIT_STATUS_WAIT_AUTH_RESPONSE;
                memset(log_content, 0, sizeof(log_content));
                snprintf(log_content, sizeof(log_content),
                         "after sent magic and auth info, previous status=NSQ_INIT_STATUS_NOTHING, change to NSQ_INIT_STATUS_WAIT_AUTH_RESPONSE");
                edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO,
                               log_content);
            } else {
                if (ctx->ctx_type == CTX_TYPE_ONLY_CONSUMER) {
                    //发送订阅消息
                    ret = ctx_send_subscribe_info(ctx->tcp_sock_fd, ctx->topic, ctx->channel);
                    if (ret < 0) {
                        memset(log_content, 0, sizeof(log_content));
                        snprintf(log_content, sizeof(log_content),
                                 "ctx_send_subscribe_info error, topic=%s, channel=%s",
                                 ctx->topic, ctx->channel);
                        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
                        return ret;
                    }
                    ctx->nsq_init_status = NSQ_INIT_STATUS_WAIT_SUB_RESPONSE;
                    memset(log_content, 0, sizeof(log_content));
                    snprintf(log_content, sizeof(log_content),
                             "after sent magic and subscribe_info, previous status=NSQ_INIT_STATUS_NOTHING, change to NSQ_INIT_STATUS_WAIT_SUB_RESPONSE");
                    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO,
                                   log_content);
                } else {
                    ctx->nsq_init_status = NSQ_INIT_STATUS_INIT_SUCCESS;
                    memset(log_content, 0, sizeof(log_content));
                    snprintf(log_content, sizeof(log_content),
                             "after sent magic, previous status=NSQ_INIT_STATUS_NOTHING, change to NSQ_INIT_STATUS_INIT_SUCCESS");
                    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO,
                                   log_content);
                    if (ctx->connect_callback != NULL) {
                        ctx->connect_callback(NULL, ctx->channel_id, ctx->user_ctx);
                    }
                }
            }
            break;
        case NSQ_INIT_STATUS_WAIT_AUTH_RESPONSE:
            gettimeofday(&time_current, NULL);
            if (time_current.tv_sec - ctx->nsq_init_start_ts.tv_sec > EDGE_DATASERVICE_COMMAND_RECV_TIMEOUT) {
                memset(log_content, 0, sizeof(log_content));
                snprintf(log_content, sizeof(log_content), "nsq init timeout(wait auth response)");
                edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
                return -1;
            };
            break;
        case NSQ_INIT_STATUS_AUTH_SUCCESS:
            if (ctx->ctx_type == CTX_TYPE_ONLY_CONSUMER) {
                //发送订阅消息
                ret = ctx_send_subscribe_info(ctx->tcp_sock_fd, ctx->topic, ctx->channel);
                if (ret < 0) {
                    memset(log_content, 0, sizeof(log_content));
                    snprintf(log_content, sizeof(log_content),
                             "ctx_send_subscribe_info error, topic=%s, channel=%s",
                             ctx->topic, ctx->channel);
                    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
                    return ret;
                }
                ctx->nsq_init_status = NSQ_INIT_STATUS_WAIT_SUB_RESPONSE;
                memset(log_content, 0, sizeof(log_content));
                snprintf(log_content, sizeof(log_content),
                         "after sent magic and auth and subscribe_info, previous status=NSQ_INIT_STATUS_AUTH_SUCCESS, change to NSQ_INIT_STATUS_WAIT_SUB_RESPONSE");
                edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO,
                               log_content);
            } else {
                ctx->nsq_init_status = NSQ_INIT_STATUS_INIT_SUCCESS;
                memset(log_content, 0, sizeof(log_content));
                snprintf(log_content, sizeof(log_content),
                         "after sent magic and auth, previous status=NSQ_INIT_STATUS_AUTH_SUCCESS, change to NSQ_INIT_STATUS_INIT_SUCCESS");
                edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO,
                               log_content);
                if (ctx->connect_callback != NULL) {
                    ctx->connect_callback(NULL, ctx->channel_id, ctx->user_ctx);
                }
            }
            break;
        case NSQ_INIT_STATUS_WAIT_SUB_RESPONSE:
            gettimeofday(&time_current, NULL);
            if (time_current.tv_sec - ctx->nsq_init_start_ts.tv_sec > EDGE_DATASERVICE_COMMAND_RECV_TIMEOUT) {
                memset(log_content, 0, sizeof(log_content));
                snprintf(log_content, sizeof(log_content), "nsq init timeout(wait sub response)");
                edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
                return -1;
            };
            break;
        case NSQ_INIT_STATUS_SUB_SUCCESS:
            ret = ctx_send_rdy(ctx->tcp_sock_fd, 1);
            if (ret < 0) {
                memset(log_content, 0, sizeof(log_content));
                snprintf(log_content, sizeof(log_content), "ctx_send_rdy error, rdy=1");
                edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
                return ret;
            }
            ctx->nsq_init_status = NSQ_INIT_STATUS_INIT_SUCCESS;
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content),
                     "after sent rdy, previous status=NSQ_INIT_STATUS_SUB_SUCCESS, change to NSQ_INIT_STATUS_INIT_SUCCESS");
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO,
                           log_content);
            if (ctx->connect_callback != NULL) {
                ctx->connect_callback(NULL, ctx->channel_id, ctx->user_ctx);
            }
            break;
        case NSQ_INIT_STATUS_INIT_SUCCESS:
            return 0;
        default:
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content), "unsupported status=%d", status);
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
            return -1;
    }

    return 0;
}

//发送tcp数据
static int ctx_tcp_send_data(int sock, char *send_buf, int send_len) {
    int num = 0;
    int ptr = 0;
    int ii = 0;
    int errno_temp = 0;
    fd_set fds;
    struct timeval wait;
    int ret = 0;
    char *send_temp = NULL;
    if (send_buf != NULL) {
        send_temp = send_buf;
    } else {
        return -1;
    }

    ptr = 0;
    while (send_len > 0) {
        if (send_len > EDGE_DATASERVICE_BUF_SMALL_SIZE) {
            num = EDGE_DATASERVICE_BUF_SMALL_SIZE;
            send_len -= EDGE_DATASERVICE_BUF_SMALL_SIZE;
        } else {
            num = send_len;
            send_len = 0;
        }

        while (num > 0) {
            ii = send(sock, &(send_temp[ptr]), num, 0);
            if (ii < 0) {
                if (errno == EAGAIN) //暂时不可写,发送缓冲满了
                {
                    FD_ZERO(&fds);
                    FD_SET(sock, &fds);
                    wait.tv_sec = 15;
                    wait.tv_usec = 0;
                    ret = select(sock + 1, NULL, &fds, NULL, &wait);//判断是否可写
                    if (ret <= 0) {
                        errno_temp = errno;
                        if (0 == ret) { //超时
                            char log_content[1024];
                            memset(log_content, 0, sizeof(log_content));
                            snprintf(log_content, sizeof(log_content),
                                     "tcp send fd select timeout, maybe the cable has been unplugged, give up");
                            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                                           log_content);
                            return -2;
                        } else {
                            if (errno_temp == EINTR) {
                                char log_content[1024];
                                memset(log_content, 0, sizeof(log_content));
                                snprintf(log_content, sizeof(log_content), "errno=EINTR, ignore it and send again");
                                edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_WARN,
                                               log_content);
                                usleep(100000);
                                continue;
                            }
                            char log_content[1024];
                            memset(log_content, 0, sizeof(log_content));
                            snprintf(log_content, sizeof(log_content),
                                     "tcp send fd select error, errno_temp=%d, give up", errno_temp);
                            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                                           log_content);
                            return -2;
                        }
                    }

                    if (sock >= 0) {
                        if (FD_ISSET(sock, &fds)) {
                            continue;
                        }
                    }

                    char log_content[1024];
                    memset(log_content, 0, sizeof(log_content));
                    snprintf(log_content, sizeof(log_content), "tcp send error1, errno=%d, send_len:%d num:%d",
                             errno,
                             send_len, num);
                    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                                   log_content);
                    return -2;
                } else {
                    char log_content[1024];
                    memset(log_content, 0, sizeof(log_content));
                    snprintf(log_content, sizeof(log_content), "tcp send error2, errno=%d, send_len:%d num:%d",
                             errno,
                             send_len, num);
                    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                                   log_content);
                    return -1;
                }
            } else {
                num = num - ii;
                ptr = ptr + ii;
            }
        }
    }
    return 0;
}

//获取数组中最大值
static int max_in_array(int *fd_array, int count) {
    int ii = 0;
    int ret = fd_array[0];
    for (ii = 0; ii < count; ii++) {
        if (ret < fd_array[ii]) {
            ret = fd_array[ii];
        }
    }
    return ret;
}

//发送tcp数据并接受回复
static int
ctx_tcp_send_data_and_wait_recv(int sock, char *send_buf, int send_len, char *recv_buf, int *recv_buf_len,
                                int recv_buf_max, int timeout_sec) {
    int ret = 0;
    //发送
    ret = ctx_tcp_send_data(sock, send_buf, send_len);
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx_tcp_send_data error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                       log_content);
        return ret;
    }

    fd_set fds;
    int fd_array[64];
    struct timeval wait;
    int len = 0;
    int count = 0;
    int select_max = 0;
    int errno_temp = 0;

    count = 0;
    memset((void *) fd_array, 0, sizeof(fd_array));
    FD_ZERO(&fds);

    fd_array[count] = sock;
    FD_SET(fd_array[count], &fds);
    count++;

    select_max = max_in_array(fd_array, count);
    select_max++;
    wait.tv_sec = timeout_sec;
    wait.tv_usec = 0;
    ret = select(select_max, &fds, NULL, NULL, &wait);

    if (ret <= 0) {
        errno_temp = errno;
        if (0 == ret) { //超时
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content), "wait recv timeout");
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                           log_content);
            return -1;
        }

        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "select error while waiting recv, errno=%d", errno_temp);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                       log_content);
        return -1;
    }

    if (sock >= 0) {
        if (FD_ISSET(sock, &fds)) {
            len = recv(sock, recv_buf, recv_buf_max, 0);
            if (len > 0) {
                *recv_buf_len = len;
                return 0;
            }

            errno_temp = errno;
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content), "recv ret=%d, errno=%d", len, errno_temp);
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                           log_content);
            return -1;
        }
    }

    char log_content[1024];
    memset(log_content, 0, sizeof(log_content));
    snprintf(log_content, sizeof(log_content), "sock fd is invalid");
    edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                   log_content);
    return -1;
}

//发送magic code
static int ctx_send_magic(int sock) {
    char send_buf[1024];
    memset(send_buf, 0, sizeof(send_buf));
    snprintf(send_buf, sizeof(send_buf), "  V2");
    int send_len = strlen(send_buf);

    int ret = 0;
    ret = ctx_tcp_send_data(sock, send_buf, send_len);
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx_tcp_send_data error, send_buf=%s", send_buf);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                       log_content);
        return ret;
    }
    return 0;
}

//发送auth信息
static int ctx_send_auth(int sock, char *p) {
    if (p == NULL) {
        return 0;
    }

    char *prefix = "AUTH\n";
    char send_buf[1024];
    memset(send_buf, 0, sizeof(send_buf));
    int str_len = strlen(p);
    int str_len_net = MY_htonl(str_len);
    int send_len = str_len + strlen(prefix) + 4;

    if (send_len > sizeof(send_buf)) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "strlen(p)=%d is too long", str_len);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                       log_content);
        return -1;
    }

    char *ptr_temp = send_buf;
    memcpy(ptr_temp, prefix, strlen(prefix));
    ptr_temp += strlen(prefix);
    memcpy(ptr_temp, (void *) &str_len_net, 4);
    ptr_temp += 4;
    memcpy(ptr_temp, p, str_len);
    ptr_temp += str_len;

    int ret = 0;
    ret = ctx_tcp_send_data(sock, send_buf, send_len);
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx_tcp_send_data error, send_buf=%s", send_buf);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                       log_content);
        return ret;
    }
    return 0;
}

//关闭心跳
static int ctx_close_heartbeat(int sock) {
    char *heartbeat_interval_str = "{\"heartbeat_interval\":-1}";
    char *prefix = "IDENTIFY\n";
    char send_buf[1024];
    memset(send_buf, 0, sizeof(send_buf));
    int str_len = strlen(heartbeat_interval_str);
    int str_len_net = MY_htonl(str_len);
    int send_len = strlen(heartbeat_interval_str) + strlen(prefix) + 4;

    char *ptr_temp = send_buf;
    memcpy(ptr_temp, prefix, strlen(prefix));
    ptr_temp += strlen(prefix);
    memcpy(ptr_temp, (void *) &str_len_net, 4);
    ptr_temp += 4;
    memcpy(ptr_temp, heartbeat_interval_str, strlen(heartbeat_interval_str));
    ptr_temp += strlen(heartbeat_interval_str);

    int ret = 0;
    ret = ctx_tcp_send_data(sock, send_buf, send_len);
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx_tcp_send_data error, send_buf=%s", send_buf);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                       log_content);
        return ret;
    }
    return 0;
}

//发送订阅消息
static int ctx_send_subscribe_info(int sock, char *topic, char *channel) {
    char send_buf[1024];
    memset(send_buf, 0, sizeof(send_buf));
    snprintf(send_buf, sizeof(send_buf), "SUB %s %s\n", topic, channel);
    int send_len = strlen(send_buf);

    int ret = 0;
    ret = ctx_tcp_send_data(sock, send_buf, send_len);
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx_tcp_send_data error, send_buf=%s", send_buf);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                       log_content);
        return ret;
    }

    return 0;
}

//发送RDY
static int ctx_send_rdy(int sock, int new_rdy) {
    char send_buf[1024];
    memset(send_buf, 0, sizeof(send_buf));
    snprintf(send_buf, sizeof(send_buf), "RDY %d\n", new_rdy);
    int send_len = strlen(send_buf);

    int ret = 0;
    ret = ctx_tcp_send_data(sock, send_buf, send_len);
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx_tcp_send_data error, send_buf=%s", send_buf);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                       log_content);
        return ret;
    }
    return 0;
}

//发送NOP
static int ctx_send_nop(int sock) {
    char send_buf[1024];
    memset(send_buf, 0, sizeof(send_buf));
    snprintf(send_buf, sizeof(send_buf), "NOP\n");
    int send_len = strlen(send_buf);

    int ret = 0;
    ret = ctx_tcp_send_data(sock, send_buf, send_len);
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx_tcp_send_data error, send_buf=%s", send_buf);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                       log_content);
        return ret;
    }
    return 0;
}

//发送FIN
static int ctx_send_fin(int sock, char *msg_id) {
    char send_buf[1024];
    memset(send_buf, 0, sizeof(send_buf));
    snprintf(send_buf, sizeof(send_buf), "FIN %s\n", msg_id);
    int send_len = strlen(send_buf);

    int ret = 0;
    ret = ctx_tcp_send_data(sock, send_buf, send_len);
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx_tcp_send_data error, send_buf=%s", send_buf);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                       log_content);
        return ret;
    }
    return 0;
}

//发送消息
static int ctx_pub_msg(int sock, char *topic, char *msg, int msg_len) {
    char *send_buf;
    int send_buf_max = msg_len + strlen(topic) + 20;
    int send_len = 0;
    send_buf = (char *) malloc(send_buf_max);
    if (send_buf == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "send_buf malloc error, topic=%s, msg_len=%d, malloc len=%d",
                 topic,
                 msg_len, send_buf_max);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                       log_content);
        return -1;
    }
    memset(send_buf, 0, send_buf_max);
    snprintf(send_buf, send_buf_max, "PUB %s\n", topic);

    char *ptr = send_buf + strlen(send_buf);
    int msg_len_net = MY_htonl(msg_len);
    memcpy(ptr, &msg_len_net, sizeof(int));
    ptr += sizeof(int);
    memcpy(ptr, msg, msg_len);
    send_len = ptr - send_buf + msg_len;

    int ret = 0;
    ret = ctx_tcp_send_data(sock, send_buf, send_len);
    free(send_buf);
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx_tcp_send_data error, topic=%s, msg_len=%d, send_len=%d",
                 topic,
                 msg_len, send_len);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR,
                       log_content);
        return ret;
    }
    return 0;
}

static int ctx_start_for_send_normal(struct DataServiceCtx *ctx) {
    if (ctx == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx is NULL");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    int start_status = 0;
    start_status = get_running_status_withlock(ctx);
    if (start_status == RUNNING_STATUS_START) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "running status is RUNNING_STATUS_START, can not start again");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    //清理重新初始化标志位
    set_need_reinit_withlock(ctx, 0);

    //启动主备检测线程
    if (pthread_create(&(ctx->pth_master_slave_check_job), NULL, master_slave_check_job_callback, (void *) (ctx)) !=
        0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "pthread_create pth_master_slave_check_job error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    //启动检测服务是否可用的线程
    if (pthread_create(&(ctx->pth_service_check_job), NULL, service_check_job, (void *) (ctx)) != 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "pthread_create pth_service_check_job error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);

        //清理现场
        pthread_cancel(ctx->pth_master_slave_check_job);
        pthread_join(ctx->pth_master_slave_check_job, NULL);
        return -1;
    }

    set_running_status_withlock(ctx, RUNNING_STATUS_START);

    return 0;
}

static int ctx_stop_for_send_normal(struct DataServiceCtx *ctx) {
    if (ctx == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx is NULL");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    int start_status = 0;
    start_status = get_running_status_withlock(ctx);
    if (start_status == RUNNING_STATUS_STOP) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "running status is RUNNING_STATUS_STOP, can not stop again");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    //停止主备检测线程
    pthread_cancel(ctx->pth_master_slave_check_job);
    pthread_join(ctx->pth_master_slave_check_job, NULL);

    //停止检测服务是否可用的线程
    pthread_cancel(ctx->pth_service_check_job);
    pthread_join(ctx->pth_service_check_job, NULL);

    //关闭tcp连接
    close_tcp_sock_fd_withlock(ctx);

    //清理重新初始化标志位
    set_need_reinit_withlock(ctx, 0);

    set_running_status_withlock(ctx, RUNNING_STATUS_STOP);

    return 0;
}
//<--static函数定义

extern struct DataServiceCtx *
new_data_service_ctx(struct IPBox *ip_list, int port, char *accessKey, char *secretKey, char *channel_id,
                     char *consumerGroup, int topic_type, int need_auto_reconnect, void *user_ctx,
                     void (*connect_callback)(void *work_ctx, char *channel_id, void *user_ctx),
                     void (*close_callback)(void *work_ctx, char *channel_id, void *user_ctx),
                     void (*msg_callback)(void *work_ctx, char *channel_id, struct DataServiceMessage *msg,
                                          void *user_ctx)) {
    return new_data_service_ctx_en(ip_list, port, NULL, accessKey, secretKey, channel_id, consumerGroup, topic_type,
                                   need_auto_reconnect, user_ctx, connect_callback, close_callback, msg_callback);
}

extern struct DataServiceCtx *
new_data_service_ctx_en(struct IPBox *ip_list, int port, char *p, char *accessKey, char *secretKey, char *channel_id,
                        char *consumerGroup, int topic_type, int need_auto_reconnect, void *user_ctx,
                        void (*connect_callback)(void *work_ctx, char *channel_id, void *user_ctx),
                        void (*close_callback)(void *work_ctx, char *channel_id, void *user_ctx),
                        void (*msg_callback)(void *work_ctx, char *channel_id, struct DataServiceMessage *msg,
                                             void *user_ctx)) {
    signal(SIGPIPE, SIG_IGN);

    if (edge_current_log_config == NULL) {
        edge_printf(NULL, EDGE_LOG_ERROR,
                    "[%s]:need to call init_edge_service_ctx first(file=%s, function=%s, line=%d)\n",
                    EDGE_MODULE_NAME,
                    __FILE__,
                    __FUNCTION__, __LINE__);
        return NULL;
    }

    //检查参数合法性
    if (ip_list == NULL || ip_list->count <= 0 || ip_list->ip_list == NULL || port <= 0 || port >= 65536 ||
        accessKey == NULL || secretKey == NULL || channel_id == NULL || topic_type < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "para error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return NULL;
    }

    if (strlen(channel_id) > EDGE_DATASERVICE_MAX_CHANNEL_ID_LEN) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "length of channel_id is too large(need <= %d, actual = %d)",
                 EDGE_DATASERVICE_MAX_CHANNEL_ID_LEN, (int) (strlen(channel_id)));
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return NULL;
    }

    if (consumerGroup != NULL) {
        if (strlen(consumerGroup) > EDGE_DATASERVICE_MAX_CHANNEL_ID_LEN) {
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content),
                     "length of consumerGroup is too large(need <= %d, actual = %d)",
                     EDGE_DATASERVICE_MAX_CHANNEL_ID_LEN, (int) (strlen(consumerGroup)));
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
            return NULL;
        }
    }

    struct DataServiceCtx *ctx = (struct DataServiceCtx *) malloc(sizeof(struct DataServiceCtx));
    if (ctx == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "DataServiceCtx malloc error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return NULL;
    }
    memset(ctx, 0, sizeof(struct DataServiceCtx));

    ctx->tcp_sock_fd = -1;

    //初始化锁
    pthread_rwlock_init(&(ctx->reinit_rwlock), NULL);
    pthread_rwlock_init(&(ctx->start_status_rwlock), NULL);
    pthread_rwlock_init(&(ctx->ip_box_used_rwlock), NULL);
    pthread_rwlock_init(&(ctx->tcp_sock_fd_rwlock), NULL);
    pthread_rwlock_init(&(ctx->nsq_init_status_rwlock), NULL);
    pthread_rwlock_init(&(ctx->nsq_send_rwlock), NULL);

    //获取输入参数
    ctx->ip_box = deep_copy_ip_box(ip_list);
    if (ctx->ip_box == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "para ip_list is illegal");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        delete_data_service_ctx(ctx);
        return NULL;
    }

    ctx->port = port;
    if (p == NULL) {
        memset(ctx->p, 0, sizeof(ctx->p));
    } else {
        snprintf(ctx->p, sizeof(ctx->p), "%s", p);
    }
    snprintf(ctx->accessKey, sizeof(ctx->accessKey), "%s", accessKey);
    snprintf(ctx->secretKey, sizeof(ctx->secretKey), "%s", secretKey);
    if (consumerGroup == NULL) {
        memset(ctx->consumerGroup, 0, sizeof(ctx->consumerGroup));
    } else {
        snprintf(ctx->consumerGroup, sizeof(ctx->consumerGroup), "%s", consumerGroup);
    }
    ctx->topic_type = topic_type;
    snprintf(ctx->channel_id, sizeof(ctx->channel_id), "%s", channel_id);
    get_topic_and_channel_by_channel_id(ctx->channel_id, ctx->consumerGroup, ctx->topic_type, ctx->topic,
                                        (int) (sizeof(ctx->topic)),
                                        ctx->channel, (int) (sizeof(ctx->channel)));
    if (ctx->topic_type == TOPIC_TYPE_AUTO) {
        ctx->topic_type_actual = get_topic_type_by_channel_id(ctx->channel_id);
        if (ctx->topic_type_actual < 0) {
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content), "get_topic_type_by_channel_id error, ctx->channel_id=%s",
                     ctx->channel_id);
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
            delete_data_service_ctx(ctx);
            return NULL;
        }
    } else {
        ctx->topic_type_actual = ctx->topic_type;
    }
    if (ctx->topic_type == TOPIC_TYPE_DATA_SUBSCRIBE_ALL) {
        if (strlen(ctx->channel) > EDGE_DATASERVICE_MAX_CHANNEL_ID_LEN + 1) {
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content),
                     "channel_id's length + consumerGroup's length should <= %d while subscribing all",
                     EDGE_DATASERVICE_MAX_CHANNEL_ID_LEN);
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
            delete_data_service_ctx(ctx);
            return NULL;
        }
    }

    ctx->need_auto_reconnect = need_auto_reconnect;
    ctx->user_ctx = user_ctx;
    ctx->connect_callback = connect_callback;
    ctx->close_callback = close_callback;
    ctx->msg_callback = msg_callback;
    ctx->ctx_type = CTX_TYPE_ONLY_CONSUMER;
    ctx->service_check_port = ctx->port + 1;
    if (ctx->port < EDGE_PORT_FLAG) {
        ctx->ha_port = EDGE_HA_PORT_OLD;
    } else {
        ctx->ha_port = EDGE_HA_PORT;
    }

    //获取主机IP
    struct IPBox *master_ip_box = get_current_master_ip_list_timeout(ctx->ip_box, ctx->ha_port,
                                                                     EDGE_CURL_INIT_TIMEOUT_SEC);
    if (master_ip_box == NULL) {
        ctx->ip_box_used = deep_copy_ip_box(ctx->ip_box);
    } else {
        ctx->ip_box_used = deep_copy_ip_box(master_ip_box);
    }
    delete_ip_box(master_ip_box);

    return ctx;
}

extern struct DataServiceCtx *
new_data_service_ctx_for_publish(struct IPBox *ip_list, int port, char *accessKey, char *secretKey,
                                 int need_auto_reconnect, int is_fast, void *user_ctx,
                                 void (*connect_callback)(void *work_ctx, char *channel_id, void *user_ctx),
                                 void (*close_callback)(void *work_ctx, char *channel_id, void *user_ctx)) {
    return new_data_service_ctx_en_for_publish(ip_list, port, NULL, accessKey, secretKey, need_auto_reconnect, is_fast,
                                               user_ctx, connect_callback, close_callback);
}

extern struct DataServiceCtx *
new_data_service_ctx_en_for_publish(struct IPBox *ip_list, int port, char *p, char *accessKey, char *secretKey,
                                    int need_auto_reconnect, int is_fast, void *user_ctx,
                                    void (*connect_callback)(void *work_ctx, char *channel_id, void *user_ctx),
                                    void (*close_callback)(void *work_ctx, char *channel_id, void *user_ctx)) {
    signal(SIGPIPE, SIG_IGN);

    if (edge_current_log_config == NULL) {
        edge_printf(NULL, EDGE_LOG_ERROR,
                    "[%s]:need to call init_edge_service_ctx first(file=%s, function=%s, line=%d)\n",
                    EDGE_MODULE_NAME,
                    __FILE__,
                    __FUNCTION__, __LINE__);
        return NULL;
    }

    //检查参数合法性
    if (ip_list == NULL || ip_list->count <= 0 || ip_list->ip_list == NULL || port <= 0 || port >= 65536 ||
        accessKey == NULL || secretKey == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "para error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return NULL;
    }

    struct DataServiceCtx *ctx = (struct DataServiceCtx *) malloc(sizeof(struct DataServiceCtx));
    if (ctx == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "DataServiceCtx malloc error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return NULL;
    }
    memset(ctx, 0, sizeof(struct DataServiceCtx));

    ctx->tcp_sock_fd = -1;

    //初始化锁
    pthread_rwlock_init(&(ctx->reinit_rwlock), NULL);
    pthread_rwlock_init(&(ctx->start_status_rwlock), NULL);
    pthread_rwlock_init(&(ctx->ip_box_used_rwlock), NULL);
    pthread_rwlock_init(&(ctx->tcp_sock_fd_rwlock), NULL);
    pthread_rwlock_init(&(ctx->nsq_init_status_rwlock), NULL);
    pthread_rwlock_init(&(ctx->nsq_send_rwlock), NULL);

    //获取输入参数
    ctx->ip_box = deep_copy_ip_box(ip_list);
    if (ctx->ip_box == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "para ip_list is illegal");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        delete_data_service_ctx(ctx);
        return NULL;
    }

    ctx->port = port;
    if (p == NULL) {
        memset(ctx->p, 0, sizeof(ctx->p));
    } else {
        snprintf(ctx->p, sizeof(ctx->p), "%s", p);
    }
    snprintf(ctx->accessKey, sizeof(ctx->accessKey), "%s", accessKey);
    snprintf(ctx->secretKey, sizeof(ctx->secretKey), "%s", secretKey);

    ctx->need_auto_reconnect = need_auto_reconnect;
    ctx->user_ctx = user_ctx;
    ctx->connect_callback = connect_callback;
    ctx->close_callback = close_callback;
    ctx->ctx_type = CTX_TYPE_ONLY_PUBLISH;
    ctx->is_fast = is_fast;
    ctx->service_check_port = ctx->port + 1;
    if (ctx->port < EDGE_PORT_FLAG) {
        ctx->ha_port = EDGE_HA_PORT_OLD;
    } else {
        ctx->ha_port = EDGE_HA_PORT;
    }

    //获取主机IP
    struct IPBox *master_ip_box = get_current_master_ip_list_timeout(ctx->ip_box, ctx->ha_port,
                                                                     EDGE_CURL_INIT_TIMEOUT_SEC);
    if (master_ip_box == NULL) {
        ctx->ip_box_used = deep_copy_ip_box(ctx->ip_box);
    } else {
        ctx->ip_box_used = deep_copy_ip_box(master_ip_box);
    }
    delete_ip_box(master_ip_box);

    return ctx;
}

extern int delete_data_service_ctx(struct DataServiceCtx *ctx) {
    if (ctx == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx is NULL, do nothing");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO, log_content);
        return 0;
    }

    int start_status = 0;
    start_status = get_running_status_withlock(ctx);
    if (start_status == RUNNING_STATUS_START) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "running status is RUNNING_STATUS_START, can not delete");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    pthread_rwlock_destroy(&(ctx->reinit_rwlock));
    pthread_rwlock_destroy(&(ctx->start_status_rwlock));
    pthread_rwlock_destroy(&(ctx->ip_box_used_rwlock));
    pthread_rwlock_destroy(&(ctx->tcp_sock_fd_rwlock));
    pthread_rwlock_destroy(&(ctx->nsq_init_status_rwlock));
    pthread_rwlock_destroy(&(ctx->nsq_send_rwlock));

    if (ctx->ip_box != NULL) {
        delete_ip_box(ctx->ip_box);
        ctx->ip_box = NULL;
    }

    if (ctx->ip_box_used != NULL) {
        delete_ip_box(ctx->ip_box_used);
        ctx->ip_box_used = NULL;
    }

    free(ctx);

    return 0;
}

extern int data_service_ctx_start(struct DataServiceCtx *ctx) {
    if (ctx == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx is NULL");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    if (ctx->ctx_type == CTX_TYPE_ONLY_PUBLISH && ctx->is_fast == 0) {
        return ctx_start_for_send_normal(ctx);
    }

    int ret = 0;
    int start_status = 0;
    start_status = get_running_status_withlock(ctx);
    if (start_status == RUNNING_STATUS_START) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "running status is RUNNING_STATUS_START, can not start again");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    char ip_used[128];
    memset(ip_used, 0, sizeof(ip_used));
    ret = get_ip_used_withlock(ctx, ip_used, sizeof(ip_used));
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "get ip error, can not start");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    //初始化recv资源
    ret = init_recv_resource(ctx);
    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "init_recv_resource error, can not start");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    //清理nsq应用层初始化标志位
    set_nsq_init_status_withlock(ctx, NSQ_INIT_STATUS_NOTHING);

    //清理重新初始化标志位
    set_need_reinit_withlock(ctx, 0);

    //tcp连接
    int sock_fd = connect_server_timeout(ip_used, (unsigned short) (ctx->port), EDGE_DATASERVICE_TCP_CONNECT_INIT_TIMEOUT_MS);
    if (sock_fd < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "connect_server error while starting, need reconnect, ip=%s, port=%d", ip_used, ctx->port);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_WARN, log_content);
        //需要重连
        set_need_reinit_withlock(ctx, 1);
    } else {
        set_tcp_sock_fd_withlock(ctx, sock_fd);
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "connect_server success while starting, ip=%s, port=%d", ip_used, ctx->port);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_INFO, log_content);
    }

    //启动主备检测线程
    if (pthread_create(&(ctx->pth_master_slave_check_job), NULL, master_slave_check_job_callback, (void *) (ctx)) !=
        0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "pthread_create pth_master_slave_check_job error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);

        //清理现场
        close_tcp_sock_fd_withlock(ctx);
        uninit_recv_resource(ctx);
        return -1;
    }

    //启动检测服务是否可用的线程
    if (pthread_create(&(ctx->pth_service_check_job), NULL, service_check_job, (void *) (ctx)) != 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "pthread_create pth_service_check_job error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);

        //清理现场
        pthread_cancel(ctx->pth_master_slave_check_job);
        pthread_join(ctx->pth_master_slave_check_job, NULL);
        close_tcp_sock_fd_withlock(ctx);
        uninit_recv_resource(ctx);
        return -1;
    }

    //启动主工作线程
    if (pthread_create(&(ctx->pth_main_work_job), NULL, main_work_job, (void *) (ctx)) !=
        0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "pthread_create pth_main_work_job error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);

        //清理现场
        pthread_cancel(ctx->pth_master_slave_check_job);
        pthread_join(ctx->pth_master_slave_check_job, NULL);
        pthread_cancel(ctx->pth_service_check_job);
        pthread_join(ctx->pth_service_check_job, NULL);
        close_tcp_sock_fd_withlock(ctx);
        uninit_recv_resource(ctx);
        return -1;
    }

    set_running_status_withlock(ctx, RUNNING_STATUS_START);

    return 0;
}

extern int data_service_ctx_stop(struct DataServiceCtx *ctx) {
    if (ctx == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx is NULL");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    if (ctx->ctx_type == CTX_TYPE_ONLY_PUBLISH && ctx->is_fast == 0) {
        return ctx_stop_for_send_normal(ctx);
    }

    int start_status = 0;
    start_status = get_running_status_withlock(ctx);
    if (start_status == RUNNING_STATUS_STOP) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "running status is RUNNING_STATUS_STOP, can not stop again");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    //停止主备检测线程
    pthread_cancel(ctx->pth_master_slave_check_job);
    pthread_join(ctx->pth_master_slave_check_job, NULL);

    //停止检测服务是否可用的线程
    pthread_cancel(ctx->pth_service_check_job);
    pthread_join(ctx->pth_service_check_job, NULL);

    //停止主工作线程
    pthread_cancel(ctx->pth_main_work_job);
    pthread_join(ctx->pth_main_work_job, NULL);

    //关闭tcp连接
    close_tcp_sock_fd_withlock(ctx);

    //清理资源
    uninit_recv_resource(ctx);

    //清理nsq应用层初始化标志位
    set_nsq_init_status_withlock(ctx, NSQ_INIT_STATUS_NOTHING);

    //清理重新初始化标志位
    set_need_reinit_withlock(ctx, 0);

    set_running_status_withlock(ctx, RUNNING_STATUS_STOP);

    return 0;
}

extern int delete_data_service_msg(struct DataServiceMessage *data_service_msg) {
    if (data_service_msg == NULL) {
        return 0;
    }

    struct DataSubscribeStruct *dss_ptr = NULL;
    struct ControlResponseStruct *crs_ptr = NULL;
    struct SetMeasurepointResponseStruct *smrs_ptr = NULL;
    struct SimpleDataSubscribeStruct *sdss_ptr = NULL;
    char log_content[1024];

    if (data_service_msg->msg != NULL) {
        switch (data_service_msg->topic_type) {
            case TOPIC_TYPE_DATA_SUBSCRIBE:
            case TOPIC_TYPE_DATA_SUBSCRIBE_ALL:
                dss_ptr = (struct DataSubscribeStruct *) (data_service_msg->msg);
                delete_data_subscribe_struct(dss_ptr);
                break;
            case TOPIC_TYPE_CONTROL_RESPONSE:
                crs_ptr = (struct ControlResponseStruct *) (data_service_msg->msg);
                delete_control_response_struct(crs_ptr);
                break;
            case TOPIC_TYPE_SET_MEASUREPOINT_RESPONSE:
                smrs_ptr = (struct SetMeasurepointResponseStruct *) (data_service_msg->msg);
                delete_set_measurepoint_response_struct(smrs_ptr);
                break;
            case TOPIC_TYPE_SIMPLE_DATA_SUBSCRIBE:
                sdss_ptr = (struct SimpleDataSubscribeStruct *) (data_service_msg->msg);
                delete_simple_data_subscribe_struct(sdss_ptr);
                break;
            case TOPIC_TYPE_CUSTOM:
                free(data_service_msg->msg);
                data_service_msg->msg = NULL;
                break;
            default:
                memset(log_content, 0, sizeof(log_content));
                snprintf(log_content, sizeof(log_content), "topic_type=%d, not supported",
                         data_service_msg->topic_type);
                edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
                return -1;
        }
        data_service_msg->msg = NULL;
    }

    free(data_service_msg);

    return 0;
}

extern int get_topic_type_by_channel_id(char *channel_id) {
    if (channel_id == NULL) {
        return -1;
    }

    int ret = 0;
    ret = strncmp(EDGE_DATASERVICE_DATA_SUBSCRIBE_TOPIC_PREFIX, channel_id,
                  strlen(EDGE_DATASERVICE_DATA_SUBSCRIBE_TOPIC_PREFIX));
    if (ret == 0) {
        return TOPIC_TYPE_DATA_SUBSCRIBE;
    }

    ret = strncmp(EDGE_DATASERVICE_DATA_SUBSCRIBE_ALL_TOPIC_PREFIX, channel_id,
                  strlen(EDGE_DATASERVICE_DATA_SUBSCRIBE_ALL_TOPIC_PREFIX));
    if (ret == 0) {
        return TOPIC_TYPE_DATA_SUBSCRIBE_ALL;
    }

    ret = strncmp(EDGE_DATASERVICE_CONTROL_RESPONSE_TOPIC_PREFIX, channel_id,
                  strlen(EDGE_DATASERVICE_CONTROL_RESPONSE_TOPIC_PREFIX));
    if (ret == 0) {
        return TOPIC_TYPE_CONTROL_RESPONSE;
    }

    ret = strncmp(EDGE_DATASERVICE_SET_MEASUREPOINT_RESPONSE_TOPIC_PREFIX, channel_id,
                  strlen(EDGE_DATASERVICE_SET_MEASUREPOINT_RESPONSE_TOPIC_PREFIX));
    if (ret == 0) {
        return TOPIC_TYPE_SET_MEASUREPOINT_RESPONSE;
    }

    return TOPIC_TYPE_CUSTOM;
}

extern int data_service_ctx_pub_fast(struct DataServiceCtx *ctx, char *channel_id, char *msg, int msg_len) {
    int ret = 0;

    if (ctx == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx is NULL");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    if (ctx->ctx_type != CTX_TYPE_ONLY_PUBLISH) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx type need CTX_TYPE_ONLY_PUBLISH, now is %d",
                 CTX_TYPE_ONLY_CONSUMER);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    if (ctx->is_fast == 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx send type is not fast mode, error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    //检查参数合法性
    if (channel_id == NULL || msg == NULL || msg_len <= 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "para error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    if (strlen(channel_id) > EDGE_DATASERVICE_MAX_CHANNEL_ID_LEN) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "length of channel_id is too large(need <= %d, actual = %d)",
                 EDGE_DATASERVICE_MAX_CHANNEL_ID_LEN, (int) (strlen(channel_id)));
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

//    int try_max = 5;
//    int try_count = 0;
//    while (1) {
//        //发送与重连互斥
//        pthread_rwlock_wrlock(&(ctx->reinit_rwlock));
//        //发送与nsq初始化状态机操作互斥
//        pthread_rwlock_wrlock(&(ctx->nsq_init_status_rwlock));
//        //与其他发送互斥
//        pthread_rwlock_wrlock(&(ctx->nsq_send_rwlock));
//        if (ctx->nsq_init_status != NSQ_INIT_STATUS_INIT_SUCCESS) {
//            char log_content[1024];
//            memset(log_content, 0, sizeof(log_content));
//            snprintf(log_content, sizeof(log_content),
//                     "try_count=%d/%d, nsq_init_status=%d, need NSQ_INIT_STATUS_INIT_SUCCESS, error, channel_id=%s, msg_len=%d",
//                     try_count + 1, try_max, ctx->nsq_init_status, channel_id, msg_len);
//            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_WARN, log_content);
//
//            pthread_rwlock_unlock(&(ctx->nsq_send_rwlock));
//            pthread_rwlock_unlock(&(ctx->nsq_init_status_rwlock));
//            pthread_rwlock_unlock(&(ctx->reinit_rwlock));
//
//            try_count++;
//            if (try_count >= try_max) {
//                char log_content[1024];
//                memset(log_content, 0, sizeof(log_content));
//                snprintf(log_content, sizeof(log_content),
//                         "finally nsq_init_status=%d, need NSQ_INIT_STATUS_INIT_SUCCESS, error, channel_id=%s, msg_len=%d",
//                         ctx->nsq_init_status, channel_id, msg_len);
//                edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
//                return -1;
//            }
//
//            edge_sleep(1000);
//            continue;
//        }
//
//        ret = ctx_pub_msg(ctx->tcp_sock_fd, channel_id, msg, msg_len);
//
//        pthread_rwlock_unlock(&(ctx->nsq_send_rwlock));
//        pthread_rwlock_unlock(&(ctx->nsq_init_status_rwlock));
//        pthread_rwlock_unlock(&(ctx->reinit_rwlock));
//        break;
//    }

    //发送与重连互斥
    pthread_rwlock_wrlock(&(ctx->reinit_rwlock));
    //发送与nsq初始化状态机操作互斥
    pthread_rwlock_wrlock(&(ctx->nsq_init_status_rwlock));
    //与其他发送互斥
    pthread_rwlock_wrlock(&(ctx->nsq_send_rwlock));
    if (ctx->nsq_init_status != NSQ_INIT_STATUS_INIT_SUCCESS) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content),
                 "nsq_init_status=%d, need NSQ_INIT_STATUS_INIT_SUCCESS, error, channel_id=%s, msg_len=%d",
                 ctx->nsq_init_status, channel_id, msg_len);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_WARN, log_content);

        pthread_rwlock_unlock(&(ctx->nsq_send_rwlock));
        pthread_rwlock_unlock(&(ctx->nsq_init_status_rwlock));
        pthread_rwlock_unlock(&(ctx->reinit_rwlock));
        return -1;
    }

    ret = ctx_pub_msg(ctx->tcp_sock_fd, channel_id, msg, msg_len);

    pthread_rwlock_unlock(&(ctx->nsq_send_rwlock));
    pthread_rwlock_unlock(&(ctx->nsq_init_status_rwlock));
    pthread_rwlock_unlock(&(ctx->reinit_rwlock));

    if (ret < 0) {
        if (ret == -2) {
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content),
                     "pub msg fatal error, channel_id=%s, msg_len=%d, need reconnect",
                     channel_id, msg_len);
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);

            set_need_reinit_withlock(ctx, 1);
            return ret;
        }

        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "pub msg error, channel_id=%s, msg_len=%d", channel_id, msg_len);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return ret;
    }

    return 0;
}

extern int
data_service_ctx_pub_normal(struct DataServiceCtx *ctx, char *channel_id, char *msg, int msg_len, int timeout_ms) {
    int ret = 0;

    if (ctx == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx is NULL");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    if (ctx->ctx_type != CTX_TYPE_ONLY_PUBLISH) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx type need CTX_TYPE_ONLY_PUBLISH, now is %d",
                 CTX_TYPE_ONLY_CONSUMER);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    if (ctx->is_fast != 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx send type is fast mode, error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    //检查参数合法性
    if (channel_id == NULL || msg == NULL || msg_len <= 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "para error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    if (timeout_ms <= 0 || timeout_ms > EDGE_DATASERVICE_MAX_NORMAL_SEND_TIMEOUT_MS) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "para error, timeout_ms need in (0, %d]",
                 EDGE_DATASERVICE_MAX_NORMAL_SEND_TIMEOUT_MS);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    if (strlen(channel_id) > EDGE_DATASERVICE_MAX_CHANNEL_ID_LEN) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "length of channel_id is too large(need <= %d, actual = %d)",
                 EDGE_DATASERVICE_MAX_CHANNEL_ID_LEN, (int) (strlen(channel_id)));
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    //与其他发送互斥
    pthread_rwlock_wrlock(&(ctx->nsq_send_rwlock));

    //发送检测是否需要重连时，其他人不能操作reinit
    pthread_rwlock_wrlock(&(ctx->reinit_rwlock));
    if (ctx->need_reinit != 0) {
        close_tcp_sock_fd_withlock(ctx);
        ctx->need_reinit = 0;
    }
    pthread_rwlock_unlock(&(ctx->reinit_rwlock));

    if (ctx->tcp_sock_fd < 0) {
        ret = ctx_connect_and_send_magic(ctx);
        if (ret < 0) {
            pthread_rwlock_unlock(&(ctx->nsq_send_rwlock));
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content),
                     "ctx_connect_and_send_magic error while publishing msg, channel_id=%s, msg_len=%d", channel_id,
                     msg_len);
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
            return ret;
        }
    }

    ret = ctx_pub_msg(ctx->tcp_sock_fd, channel_id, msg, msg_len);
    if (ret >= 0) {
        ret = expect_recv_ok_frame(ctx->tcp_sock_fd, timeout_ms);
    }

    pthread_rwlock_unlock(&(ctx->nsq_send_rwlock));

    if (ret < 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "pub msg error, channel_id=%s, msg_len=%d, need reconnect",
                 channel_id, msg_len);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);

        set_need_reinit_withlock(ctx, 1);
        return ret;
    }

    return 0;
}

extern int
data_service_ctx_connect_for_pub_normal(struct DataServiceCtx *ctx) {
    int ret = 0;

    if (ctx == NULL) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx is NULL");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    if (ctx->ctx_type != CTX_TYPE_ONLY_PUBLISH) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx type need CTX_TYPE_ONLY_PUBLISH, now is %d",
                 CTX_TYPE_ONLY_CONSUMER);
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    if (ctx->is_fast != 0) {
        char log_content[1024];
        memset(log_content, 0, sizeof(log_content));
        snprintf(log_content, sizeof(log_content), "ctx send type is fast mode, error");
        edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
        return -1;
    }

    //与其他发送互斥
    pthread_rwlock_wrlock(&(ctx->nsq_send_rwlock));

    //发送检测是否需要重连时，其他人不能操作reinit
    pthread_rwlock_wrlock(&(ctx->reinit_rwlock));
    if (ctx->need_reinit != 0) {
        close_tcp_sock_fd_withlock(ctx);
        ctx->need_reinit = 0;
    }
    pthread_rwlock_unlock(&(ctx->reinit_rwlock));

    if (ctx->tcp_sock_fd < 0) {
        ret = ctx_connect_and_send_magic(ctx);
        if (ret < 0) {
            pthread_rwlock_unlock(&(ctx->nsq_send_rwlock));
            char log_content[1024];
            memset(log_content, 0, sizeof(log_content));
            snprintf(log_content, sizeof(log_content),
                     "ctx_connect_and_send_magic error while connect");
            edge_log_print(__FILE__, __FUNCTION__, EDGE_MODULE_NAME, __LINE__, EDGE_LOG_ERROR, log_content);
            return ret;
        }
    }

    pthread_rwlock_unlock(&(ctx->nsq_send_rwlock));

    return 0;
}