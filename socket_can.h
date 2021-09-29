#include <pthread.h>

#include <stdint.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <string>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/error.h>
#include <linux/net_tstamp.h>

#define SET_CAN0_BAUDRATE "sudo ip link set can0 type can bitrate "
#define SET_CAN1_BAUDRATE "sudo ip link set can1 type can bitrate "
#define CAN_FD_ON " dbitrate 2000000 berr-reporting on fd on"
#define CAN0_OPEN "sudo ip link set up can0"
#define CAN1_OPEN "sudo ip link set up can1"
/* set transmit queue length: small value for slower device with a high latency, hight value is recommended for server connected with high-speed internet connections the perform large data transfers */
#define CAN0_LENGTH "sudo ip link set can0 txqueuelen 1000" 
#define CAN1_LENGTH "sudo ip link set can1 txqueuelen 1000"

#define CAN0_CLOSE "sudo ip link set down can0"
#define CAN1_CLOSE "sudo ip link set down can1"

// bool Write(const vector<can_frame>&);
// int Read(vector<can_frame::Frame>&);

static void* ReceiveFunc(void* param);
static void* TransmitFunc(void* param);

pthread_t thread_0_;
pthread_t thread_1_;
static int run_flag_;
// vector<can_frame> can_buf_1_;
// vector<can_frame> can_buf_2_;
// void* handle_ = nullptr;
uint32_t receive_msgs_count;

uint8_t kbaudrate_port0 = 500000; // 250000 1000000
uint8_t kbaudrate_port1 = 500000;
uint8_t raw_socket_;
struct ifreq ifr_;
struct sockaddr_can addr_;
const char bus_name0[5] = "can0";
const char bus_name1[5] = "can1";

};
