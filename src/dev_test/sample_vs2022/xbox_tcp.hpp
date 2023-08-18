#pragma once
/**
 * @brief C++ header
*/
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <string.h>
#include <math.h>
#include <chrono>
#include <time.h>

#include <future>
#include <memory>
#include <thread>
#include <signal.h>


/**
 * @brief global variables
*/
#define IP_ADDRESS "172.16.1.0"
#define PORT_NUMBER 7777

#define NUM_OF_MOTORS 5
#define BUFFER_SIZE 512
#define KEYBOARD_INPUT_MODE 0

#define SINEWAVE_TEST 1 // if test sine wave

class XboxNode
{
    struct JoystickData
    {
        int32_t axis_x_right;
        int32_t axis_y_right;
        int32_t axis_x_left;
        int32_t axis_y_left;
        int32_t button_X;
        int32_t button_Y;
        int32_t button_A;
        int32_t button_B;
    };

public:
    XboxNode();
    ~XboxNode();
    void ReadInputThread();

private:
    void InitController();

public:
    bool status_;
    XboxNode::JoystickData input_;
    int32_t ros2_input_[NUM_OF_MOTORS] = { 0 }; // ROS2

private:
    std::thread readinput_thread_;
};


class TCPClientNode : public XboxNode
{
public:
    TCPClientNode();
    ~TCPClientNode();

private:
    void Initialize();
    void SendThread();
    void RecvThread();
    static void signal_callback_handler(int signum);

public:
    struct sockaddr_in server_addr_;
    WSADATA WsaData_;
    SOCKET hSocket_;
    SOCKADDR_IN ServerAddress_;
    std::string ip_;
    std::string s_port_;
    uint32_t port_ = PORT_NUMBER;
    // char msg_[] = "";

private:
    uint32_t buffer_size_;
    std::thread send_thread_;
    std::thread recv_thread_;
    bool tcp_life = true;

    int send_strlen_;
    int recv_strlen_;
    char send_msg_[BUFFER_SIZE] = { 0, };
    char recv_msg_[BUFFER_SIZE] = { 0, };

    // std::shared_future<void> future_;
    // std::promise<void> exit_signal_;
};