// TCP/IP client
// Windows
#include <xbox_tcp.hpp>

XboxNode::XboxNode()
{
  readinput_thread_ = std::thread(&XboxNode::ReadInputThread, this);
  std::cout << "Thread (Xbox) is created." << std::endl;
}

XboxNode::~XboxNode(){}

void XboxNode::InitController() {

}

void XboxNode::ReadInputThread() {
  /**
   * @brief read xbox controller input
  */
  while (true)
  {
    /* code */
    this->input_.axis_x_right = 0;
    this->input_.axis_y_right = 0;
    this->input_.axis_x_left = 0;
    this->input_.axis_y_left = 0;
    this->input_.button_A = 0;
    this->input_.button_B = 0;
    this->input_.button_X = 0;
    this->input_.button_Y = 0;
    std::cout << "[Xbox] read " << std::endl;
    Sleep(1000);
  }
  
}


/**
 * @author DY
 * @brief TCP client socket open
 *        1 - ip setting
 *        2 - port setting
 *        3 - TCP configuration
 *        spin
*/
TCPClientNode::TCPClientNode()
{
  // ***********************
  // 1 - ip setting
  // ***********************
  while(true){
    std::cout << "[Command] Enter IP address. Enter is using dafault (default = " << IP_ADDRESS << ") : ";
    std::getline(std::cin, this->ip_);
    static uint8_t cnt = 0;

    // set default4
    if(this->ip_ == "") {this->ip_ = IP_ADDRESS; std::cout << this->ip_ << std::endl; break;} 
    
    // set manual
    for(int i=0; i<this->ip_.length(); i++) { if(this->ip_[i] == '.') cnt++; }          
    if(cnt != 3) std::cout << "[ERROR] Check your ip address (3 .)" << std::endl;
    else {break;}
  }
  // ***********************
  // 2 - port setting
  // ***********************
  while(true){
    std::cout << "[Command] Enter Port. Enter is using dafault (default = " << PORT_NUMBER << ") : ";
    std::getline(std::cin, this->s_port_);
    uint8_t tf = 0;

    // set default
    if(this->s_port_=="") {std::cout << this->s_port_ << std::endl; break;} 

    // set manual
    for(int i=0; i<this->s_port_.length(); i++) 
    {
      tf = isdigit(this->s_port_[i]);
      if(tf==0){
        std::cout << "[ERROR] Check your Port Number (int)" << std::endl;
        break;
      }
    }
    if(tf) {   
      this->port_ = std::stoi(this->s_port_);
      break;
    }
  }
  std::cout << "[INFO] Set is done -> " << "IP = " << this->ip_ << " port = " << this->port_ << std::endl;

  // ***********************
  // 3 - TCP configuration
  // ***********************
  if(WSAStartup(MAKEWORD(2,2), &this->WsaData_) != 0) std::cout << "[ERROR] WSAStartup() error" << std::endl;
  this->hSocket_ = socket(PF_INET, SOCK_STREAM, 0);
  if(this->hSocket_ == INVALID_SOCKET) std::cout << "[ERROR] hSocket() error" << std::endl;

  memset(&this->ServerAddress_, 0, sizeof(this->ServerAddress_));
  this->ServerAddress_.sin_family = AF_INET;
  this->ServerAddress_.sin_addr.s_addr = inet_addr(this->ip_.c_str());
  // if 'inet_addr()' error is occured, then you will use 'inet_pton()'
  //this->ServerAddress_.sin_addr.s_addr = inet_pton(AF_INET, this->ip_.c_str(), &this->ServerAddress_);
  this->ServerAddress_.sin_port = htons(this->port_);
  std::cout << "[INFO] connecting to server...";

  if(connect(this->hSocket_, (SOCKADDR*)&this->ServerAddress_, sizeof(this->ServerAddress_)) == SOCKET_ERROR) {
    std::cout << "[ERROR] connect() error" << std::endl;
    return;
  } else {
    std::cout << "done" << std::endl;
  } 

  // ***********************
  // Make Threads
  // ***********************
  this->send_thread_ = std::thread(&TCPClientNode::SendThread, this);
  this->recv_thread_ = std::thread(&TCPClientNode::RecvThread, this);
  std::cout << "Thread (Send, Recv) is created." << std::endl;

  //signal(SIGINT, this->signal_callback_handler);
  // while(true){
  //   /**
  //    * @brief spin
  //   */
  // }
  return;
}

TCPClientNode::~TCPClientNode(){}


void TCPClientNode::SendThread()
{
  std::cout << "TCP Send Thread Start" << std::endl;
  long send_val[BUFFER_SIZE];
  
#if SINEWAVE_TEST
  static uint64_t counter = 0;
  while (true) {
    for(int i=0; i<NUM_OF_MOTORS; i++){
      send_val[i] = 10*sin(counter*0.01);
      memcpy(this->send_msg_ + i*sizeof(long), &send_val[i], sizeof(send_val[i]));
    }
    uint32_t size_ofsendMsg = send(this->hSocket_, this->send_msg_, BUFFER_SIZE, 0);
  }
#else
  while (true) {
    /**
     * @author DY
     * @brief struct에 접근하여 input으로 던져줄 것.
     *        ROS2 topic의 경우 배열로 던지고 받을 것.
    */
    for(int i=0; i<NUM_OF_MOTORS; i++){
      send_val[i] = this->ros2_input_[i];
      memcpy(this->send_msg_ + i*sizeof(long), &send_val[i], sizeof(send_val[i]));
    }
    uint32_t size_ofsendMsg = send(this->hSocket_, this->send_msg_, BUFFER_SIZE, 0);
  }
#endif
  
}

void TCPClientNode::RecvThread()
{
  std::cout << "TCP Receive Thread Start" << std::endl;
  long recv_val[BUFFER_SIZE];

  // spin
  while(true) {
    /**
     * @brief spin
    */
    this->send_strlen_ = recv(this->hSocket_, this->recv_msg_, BUFFER_SIZE, 0);
    if (this->send_strlen_ == -1) {
      std::cout << "[Send Thread] read() error." << std::endl;
      break;
    }
  
    // Little-Endian
    memcpy(recv_val, this->recv_msg_, BUFFER_SIZE);
    system("cls");  // clear every time
    for(int n=0; n<NUM_OF_MOTORS; n++){
        std::cout << n << " pos : " << recv_val[0+n*2] << " / vel : " << recv_val[1+n*2] << std::endl;
    }
  }
}

/**
 * @brief Ctrl+C handler
*/
void TCPClientNode::signal_callback_handler (int signum) {
  std::cout << "Signal " << signum << std::endl;
  exit(signum); // terminate program
}



void signal_callback_handler (int signum) {
  std::cout << "Signal " << signum << std::endl;
  exit(signum); // terminate program
}

/**
 * @brief main() : spin operation
*/
int main(int argc, char* argv[]){
  TCPClientNode tcp_PCtoMasterMACS;

  signal(SIGINT, signal_callback_handler);
  while(true) {

  }
  return EXIT_SUCCESS;
}


