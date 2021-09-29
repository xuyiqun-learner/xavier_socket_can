#include <socket_can.h>

#include <iostream>
#include <cstring>
using std::system;
using std::cout;
using std::endl;

int main(int argc, int** argv) {
// 初始化，打开can控制器
  system(CAN0_CLOSE);
  system(CAN1_CLOSE);
  system((SET_CAN0_BAUDRATE + std::to_string(kbaudrate_port0)).c_str());
  system((SET_CAN1_BAUDRATE + std::to_string(kbaudrate_port1)).c_str());
  system(CAN0_OPEN);
  system(CAN0_LENGTH);
  system(CAN1_OPEN);
  system(CAN1_LENGTH);
  
  raw_socket_ = socket(PF_CAN, SOCK_RAW, CAN_RAW); 
  addr_.can_ifindex = 0; /* 0 means any can interface */                          
  addr_.can_family = PF_CAN;   
  int ret = bind(raw_socket_, (struct sockaddr*)&addr_, sizeof(addr_);
  run_flag_ = 1;
  if(ret < 0) {
   cout << "Error while bind with socket" << endl;
   close(raw_socket_);
   run_flag_ = 0;
  }
  if(pthread_create(&thread_0_, NULL, ReceiveFunc, &run_flag_) != 0) {
  	std::perror("pthread_create ReceiveFunc error");
  }
  if(pthread_create(&thread_1_, NULL, TransmitFunc, &run_flag_)) {
  	std::perror("pthread_create ReceiveFunc error");
   }

  usleep(5000);
  close(raw_socket_);
  
 return 0; 
}

void* ReceiveFunc(void* param) {
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  const int timestamp_flag = SOF_TIMESTAMPING_RX_SOFTWARE;
  timeval socket_timestamp;
  can_frame frame_raw;
  socklen_t receive_length = sizeof(addr_);
  int* run = static_cast<int*>(param);
  setsockopt(raw_socket_, SOL_SOCKET, SO_TIMESTAMPING, &timestamp_flag, sizeof(timestamp_flag));

  while((*run)& 0x0f) {
      pthread_rwlock_wrlock(&socket_rwlock);
      int ret = recvfrom(raw_socket_, &frame_raw, sizeof(frame_raw), 0, (struct sockaddr*)&addr_, &receive_length);
      if(ret <= 0) { // ret is frame_raw.size()
          ROS_ERROR_STREAM("receive nothing or receive message fail!");
      }
      pthread_rwlock_unlock(&socket_rwlock);
      if(frame_raw.can_id == (uint)(0x120 & CAN_SFF_MASK) {
      	 receive_msgs_count++;
      	 cout << "Receive msgs 0x120:" << receive_msgs_count << endl;
      }
  }  
  pthread_exit(0);
}

void* TransmitFunc(void* param) {
can_frame transmit_msgs;
transmit_msgs.can_id = 0x501;
transmit_msgs.can_dlc = 5;
transmit_msgs.Data[0] = 0x11;
transmit_msgs.Data[1] = 0x22;
transmit_msgs.Data[2] = 0x33;
transmit_msgs.Data[3] = 0x44;
transmit_msgs.Data[4] = 0x55;
transmit_msgs.Data[5] = 0x66;

strcpy(ifr_.ifr_name, bus_name0); 
addr_.can_ifindex = ifr_.ifr_ifindex;
ioctl(raw_socket_, SIOGIFINDEX, &ifr_);
int nbytes = sendto(raw_socket_, &transmit_msgs, sizeof(struct can_frame), 0, (struct sockaddr*)&addr_, sizeof(addr_));
}
