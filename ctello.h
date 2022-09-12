#ifndef CTELLO_H
#define CTELLO_H
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>

#include <optional>
#include <vector>
#include <string>

const char* const TELLO_SERVER_IP{"192.168.10.1"};
const char* const TELLO_SERVER_COMMAND_PORT{"8889"};
const int LOCAL_CLIENT_COMMAND_PORT{9000};
const int LOCAL_SERVER_STATE_PORT{8890};

namespace ctello {
class Tello
{
public:
    Tello();
    ~Tello();
    bool Bind(int local_client_command_port = LOCAL_CLIENT_COMMAND_PORT);
    bool SendCommand(const std::string& command);
    std::optional<std::string> ReceiveResponse();
    std::optional<std::string> GetState();

    Tello(const Tello&) = delete;
    Tello(const Tello&&) = delete;
    Tello& operator=(const Tello&) = delete;
    Tello& operator=(const Tello&&) = delete;
    void say_hello();
private:
    void FindTello();
    void ShowTelloInfo();

private:
    int m_command_sockfd{0};
    int m_state_sockfd{0};
    int m_local_client_command_port{LOCAL_CLIENT_COMMAND_PORT};
    sockaddr_storage m_tello_server_command_addr{};
};

}

#endif // CTELLO_H
