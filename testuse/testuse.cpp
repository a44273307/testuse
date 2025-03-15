#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <cstring>
#include <chrono>
#include <iomanip>  // std::hex, std::setfill, std::setw

#pragma comment(lib, "Ws2_32.lib")  // 链接 ws2_32 库

#define SERVER_IP "192.168.3.7"
#define SERVER_PORT 8887
#define TIMEOUT_MS 500  // 超时时间 10ms

void printVector(const std::vector<unsigned char>& vec) {
    std::cout << "Hex data: ";
    for (unsigned char byte : vec) {
        std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl; // 恢复十进制输出
}
int sendorderandcheckrsp(std::vector<unsigned char> sendData,std::vector<unsigned char> rspData)
{
    printVector(sendData);
    printVector(rspData);
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    
    unsigned char buffer[1024] = { 0 };

    // 初始化 Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return -1;
    }

    // 创建 socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return -1;
    }

    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // 连接服务器
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed\n";
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // 发送数据
    if (send(clientSocket, reinterpret_cast<const char*>(sendData.data()), sendData.size(), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed\n";
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    auto start = std::chrono::high_resolution_clock::now(); // 记录发送后的时间

    // 设置超时时间（使用 select 方式）
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(clientSocket, &readfds);
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = TIMEOUT_MS * 1000;  // 10ms

    int selectResult = select(0, &readfds, NULL, NULL, &timeout);
    if (selectResult == 0) {
        std::cout << "Timeout occurred, no response received\n";
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }
    else if (selectResult == SOCKET_ERROR) {
        std::cerr << "Select failed\n";
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // 接收数据
    int recvLen = recv(clientSocket, reinterpret_cast<char*>(buffer), sizeof(buffer), 0);
    auto end = std::chrono::high_resolution_clock::now(); // 记录接收后的时间
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    if (recvLen > 0) {
        if (recvLen == rspData.size() && memcmp(buffer, rspData.data(), recvLen) == 0) {
            std::cout << "Received data matches sent data.\n";
            std::cout << "Time taken from send to receive: " << duration << " ms\n";
            closesocket(clientSocket);
            WSACleanup();
            return 0;  // 数据匹配，返回 0
        }
        else {
            std::cerr << "Received data does not match sent data.\n";
            std::cout << "Time taken from send to receive: " << duration << " ms\n";
            closesocket(clientSocket);
            WSACleanup();
            return -2;  // 数据不匹配，返回 -2
        }
    }
    else {
        std::cerr << "Receive failed\n";
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }
}

// 控制旋转选择指令上圆盘1号孔
int  controlTopDiskRotation(int diskNumber)
{
    std::vector<unsigned char> sendData = { 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04 };
    sendData[2]=diskNumber;
    int sum=0;
    for(int i=0;i<8;i++)
    {
        sum+=sendData[i];
    }
    sendData[8]=sum;
    std::vector<unsigned char> trspData=sendData;
    return sendorderandcheckrsp(sendData,trspData);
}

int main() {
   /* std::vector<unsigned char> sendData = { 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04 };    
    std::vector<unsigned char> trspData = { 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04 };    
    return sendorderandcheckrsp(sendData,trspData);*/
    controlTopDiskRotation(1);
    controlTopDiskRotation(2);
}
