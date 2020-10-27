// Client_SocketLabNetworks.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#pragma warning(disable : 6386 6385)
#include <iostream>
#include <queue>
#include "string.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#include <stdlib.h>
#include <sstream>
using namespace std;

SOCKET ConnectSocket;
bool isPasswordHacked = false;
vector<string> alphabet = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
                            "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", 
                            "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};

void recursiveGenerator(string prefix, int lengthLeft) 
{
    if (isPasswordHacked)
    {
        return;
    }
    else  if (lengthLeft == 0) 
    {
        // 255 chars = max amount of chars in message = 254 + 1 terminate null-char
        char* attemptedPasswordcStyle = new char[255];

        char* recievedResponse = new char[255];

        // get last element in vector
        string attemptedPasswordString = prefix;

        strcpy_s(attemptedPasswordcStyle, 255, attemptedPasswordString.c_str());

        // send password to server
        int bytesSent = send(ConnectSocket, attemptedPasswordcStyle, 255, 0);
        int bytesRecieved = SOCKET_ERROR;
        while (bytesRecieved == SOCKET_ERROR && !isPasswordHacked)
        {
            bytesRecieved = recv(ConnectSocket, recievedResponse, 255, 0);
            //std::cout << "lol";
        }

        if (strcmp (recievedResponse, "YES!") == 0)
        {
            isPasswordHacked = true;
        }
        std::cout << "Password:" << attemptedPasswordString << " , is hacked:" << recievedResponse << endl;
        // delete array to prevent memory leaks
        delete[] attemptedPasswordcStyle;
        delete[] recievedResponse;
        //Sleep(1000)
    }
    else 
    {
        for (size_t i = 0; i < alphabet.size(); i++)
        {
           recursiveGenerator(prefix + alphabet[i], lengthLeft - 1);
        }
    }
}

void hackPwd(int length) 
{
    recursiveGenerator("", length);
}

int main()
{
   
    //----------------------
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
        printf("Error at WSAStartup()\n");

    //----------------------
    // Create a SOCKET for connecting to server
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }

    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port of the server to be connected to.
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(1028);

    //----------------------
    // Connect to server.
    if (connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        printf("Failed to connect.\n");
        WSACleanup();
        return 0;
    }

    printf("Connected to server.\n");

    
    char* buffer = new char[255];
    int bytes = recv(ConnectSocket, buffer, 255, 0);
    stringstream ss(buffer);
    int pwdLength = 0;
    if (ss >> pwdLength)
    {
        delete[] buffer;
        hackPwd(pwdLength);
    }
    else
    {
        return -1;
    }    
    
    WSACleanup();
    return 0;
}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
