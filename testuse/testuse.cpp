#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <cstring>
#include <chrono>
#include <iomanip>  // std::hex, std::setfill, std::setw
#include <windows.h>
#include "loguru.hpp"

#pragma comment(lib, "Ws2_32.lib")  // 链接 ws2_32 库

#define SERVER_IP "192.168.3.7"
#define SERVER_PORT 8887
#define TIMEOUT_MS 500  // 超时时间 10ms
#include <sstream>
#include <string>
using std::string;
void printVector(const std::vector<unsigned char>& vec,string input="") {
    std::ostringstream oss;
    oss << input;
    for (unsigned char byte : vec) {
        oss << "0x" << std::hex << std::uppercase << static_cast<int>(byte) << " ";
    }
    LOG_F(INFO, "%s", oss.str().c_str());
}

int sendorderandcheckrsp(std::vector<unsigned char> sendData, std::vector<unsigned char> rspData) {
    printVector(sendData);
    printVector(rspData);
    
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    unsigned char buffer[1024] = { 0 };

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        LOG_F(ERROR, "WSAStartup failed");
        return -1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        LOG_F(ERROR, "Socket creation failed");
        WSACleanup();
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        LOG_F(ERROR, "Connection failed");
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    if (send(clientSocket, reinterpret_cast<const char*>(sendData.data()), sendData.size(), 0) == SOCKET_ERROR) {
        LOG_F(ERROR, "Send failed");
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(clientSocket, &readfds);
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = TIMEOUT_MS * 1000;

    int selectResult = select(0, &readfds, NULL, NULL, &timeout);
    if (selectResult == 0) {
        LOG_F(INFO, "Timeout occurred, no response received");
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    } else if (selectResult == SOCKET_ERROR) {
        LOG_F(ERROR, "Select failed");
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // 接收数据
    int recvLen = recv(clientSocket, reinterpret_cast<char*>(buffer), sizeof(buffer), 0);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    if (recvLen > 0) {
        std::vector<unsigned char> receivedData(buffer, buffer + recvLen);
        LOG_F(INFO, "Received Data:");
        printVector(receivedData);
        if (recvLen == rspData.size() && memcmp(buffer, rspData.data(), recvLen) == 0) {
            LOG_F(INFO, "Received data matches sent data.");
            LOG_F(INFO, "Time taken from send to receive: %lld ms", duration);
            closesocket(clientSocket);
            WSACleanup();
            return 0;
        } else {
            LOG_F(ERROR, "Received data does not match sent data.");
            LOG_F(INFO, "Time taken from send to receive: %lld ms", duration);
            closesocket(clientSocket);
            WSACleanup();
            return -2;
        }
    } else {
        LOG_F(ERROR, "Receive failed");
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }
}
long long bytesToLongLong(const std::vector<unsigned char>& data) {
    if (data.size() < 4) {
        throw std::runtime_error("Input vector must have at least 4 bytes");
    }
    
    // 解析 4 字节数据 (MSB 在前，大端序)
    long long result = (static_cast<long long>(data[4]) << 24) |
                       (static_cast<long long>(data[5]) << 16) |
                       (static_cast<long long>(data[6]) << 8)  |
                       (static_cast<long long>(data[7]));

    return result;
}
int checkgetrsp(std::vector<unsigned char> sendData,long long& rsp)
{
    if(sendData.size()!=9)
    {
        return -1;
    }
    int sum=0;
    for(int i=0;i<8;i++)
    {
        sum+=sendData[i];
    }
    if(sendData[8]!=sum%256)
    {
        return -1;
    }
    rsp=bytesToLongLong(sendData);
    return 0;
}
int sendorderandrspans(std::vector<unsigned char> sendData, long long &getlongrspdata)
{
    printVector(sendData);
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    
    unsigned char buffer[1024] = { 0 };

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        LOG_F(ERROR, "WSAStartup failed");
        return -1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        LOG_F(ERROR, "Socket creation failed");
        WSACleanup();
        return -1;
    }
    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // 连接服务器
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        LOG_F(ERROR, "Connection failed");
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    if (send(clientSocket, reinterpret_cast<const char*>(sendData.data()), sendData.size(), 0) == SOCKET_ERROR) {
        LOG_F(ERROR, "Send failed");
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
    timeout.tv_usec = TIMEOUT_MS * 1000;

    int selectResult = select(0, &readfds, NULL, NULL, &timeout);
    if (selectResult == 0) {
        LOG_F(INFO, "Timeout occurred, no response received");
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }
    else if (selectResult == SOCKET_ERROR) {
        LOG_F(ERROR, "Select failed");
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    int recvLen = recv(clientSocket, reinterpret_cast<char*>(buffer), sizeof(buffer), 0);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    if (recvLen > 0) {
        std::vector<unsigned char> receivedData(buffer, buffer + recvLen);
        LOG_F(INFO, "Received Data:");
        printVector(receivedData);
        if (checkgetrsp(receivedData, getlongrspdata) == 0) {
            LOG_F(INFO, "Received data matches sent data.");
            LOG_F(INFO, "Time taken from send to receive: %lld ms", duration);
            LOG_F(INFO, "getlongrspdata: %lld", getlongrspdata);
            closesocket(clientSocket);
            WSACleanup();
            return 0;
        } else {
            LOG_F(ERROR, "Received data does not match sent data.");
            closesocket(clientSocket);
            WSACleanup();
            return -2;
        }
    } else {
        LOG_F(ERROR, "Receive failed");
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }
}
void dealSenddata(std::vector<unsigned char>& sendData)
{
    int sum=0;
    for(int i=0;i<8;i++)
    {
        sum+=sendData[i];
    }
    sendData[8]=sum;
}
// 控制旋转 上圆盘第x号孔
int  controlTopDiskRotation(int diskNumber)
{
    std::vector<unsigned char> sendData = { 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04 };
    sendData[2]=diskNumber;
    dealSenddata(sendData);
    std::vector<unsigned char> trspData=sendData;
    return sendorderandcheckrsp(sendData,trspData);
}
int  controlBottomDiskRotation(int diskNumber)
// 控制旋转 下圆盘第x号孔
{
    std::vector<unsigned char> sendData = { 0x01, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x04 };
    sendData[2]=diskNumber;
    dealSenddata(sendData);
    std::vector<unsigned char> trspData=sendData;
    return sendorderandcheckrsp(sendData,trspData);
}

// 上圆盘信息读取
//  order:   
// 02  目标位置
// 03  实时位置
// 04 复位完成标志 
// 05 运行电流
int  readTopDiskInfo(int order,long long& getRspData)
{
    
    std::vector<unsigned char> sendData = { 0x01, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04 };
    sendData[1]=order;
    dealSenddata(sendData);
    return sendorderandrspans(sendData,getRspData);
}
// 读取下圆盘信息，参数同上圆盘
int  readBottomDiskInfo(int order,long long& getRspData)
{
    
    std::vector<unsigned char> sendData = { 0x01, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x04 };
    sendData[1]=order;
    dealSenddata(sendData);
    return sendorderandrspans(sendData,getRspData);
}
// 读取聚焦信息，参数同上圆盘
int  readfocusInfo(int order,long long& getRspData)
{
    std::vector<unsigned char> sendData = { 0x01, 0x02, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x04 };
    sendData[1]=order;
    dealSenddata(sendData);
    return sendorderandrspans(sendData,getRspData);
}
// 读取光圈信息，参数同上圆盘
int  readapertureInfo(int order,long long& getRspData)
{
    std::vector<unsigned char> sendData = { 0x01, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x04 };
    sendData[1]=order;
    dealSenddata(sendData);
    return sendorderandrspans(sendData,getRspData);
}
// 光圈位置控制 取值范围0-------1500000
int  controlapertureRotation(long long value)
{
    LOG_F(INFO, "controlapertureRotation %lld", value);
    std::vector<unsigned char> sendData = { 0x01, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x04 };
      // 取低 32 位，并按照大端序（MSB 在前）存入 sendData
    sendData[4] = (value >> 24) & 0xFF;
    sendData[5] = (value >> 16) & 0xFF;
    sendData[6] = (value >> 8) & 0xFF;
    sendData[7] = value & 0xFF;
    dealSenddata(sendData);
    std::vector<unsigned char> trspData=sendData;
    return sendorderandcheckrsp(sendData,trspData);
}
// 聚焦位置控制 取值范围0-------1500000
int  controlfocusRotation(long long value)
{
    LOG_F(INFO, "controlfocusRotation %lld", value);
    std::vector<unsigned char> sendData = { 0x01, 0x01, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x04 };
    // 取低 32 位，并按照大端序（MSB 在前）存入 sendData
    sendData[4] = (value >> 24) & 0xFF;
    sendData[5] = (value >> 16) & 0xFF;
    sendData[6] = (value >> 8) & 0xFF;
    sendData[7] = value & 0xFF;
    dealSenddata(sendData);
    std::vector<unsigned char> trspData=sendData;
    return sendorderandcheckrsp(sendData,trspData);
}
int main() {

    loguru::add_file("my_log.logxx", loguru::Append, loguru::Verbosity_MAX);

    long long rsp;
    controlfocusRotation(1000);
    readfocusInfo(02, rsp);
    readfocusInfo(02, rsp);
    readfocusInfo(02, rsp);
    readfocusInfo(02, rsp);
    readfocusInfo(02, rsp);
    readfocusInfo(02, rsp);
    readfocusInfo(02, rsp);
    LOG_F(INFO, "**********");
    Sleep(1000); // 休眠 1000 毫秒（1 秒）

    controlfocusRotation(2000);
    readfocusInfo(02, rsp);
    readfocusInfo(02, rsp);
    readfocusInfo(02, rsp);
}
