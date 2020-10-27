// SocketLabNetworks.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "winsock2.h"
#include "ws2tcpip.h"
#include <stdlib.h>    

using namespace std;

int main()
{
    int pwdLength;
    char* password = new char [255];
    cout << "Type password (max length = 255, use lower & uppercase letters + numbers)";
    if (cin >> password)
    {
        int pwdLength = strlen(password);
        cout << pwdLength << endl;
        //----------------------
        // Initialize Winsock
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != NO_ERROR)
            printf("Error at WSAStartup()\n");

        //----------------------
        // Create a SOCKET for listening for 
        // incoming connection requests
        SOCKET ListenSocket;
        ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ListenSocket == INVALID_SOCKET) {
            printf("Error at socket(): %ld\n", WSAGetLastError());
            WSACleanup();
            return 0;
        }
        //----------------------
        // The sockaddr_in structure specifies the address family,
        // IP address, and port for the socket that is being bound.
        sockaddr_in service;
        service.sin_family = PF_INET;
        inet_pton(AF_INET, "127.0.0.1", &service.sin_addr.s_addr);
        std::cout << service.sin_addr.s_addr << std::endl;
        service.sin_port = htons(1028);

        //----------------------
        // Bind the socket.
        if (bind(ListenSocket,
            (SOCKADDR*)&service,
            sizeof(service)) == SOCKET_ERROR) {
            printf("bind() failed.\n");
            closesocket(ListenSocket);
            return 0;
        }

        //----------------------
        // Listen for incoming connection requests 
        // on the created socket
        if (listen(ListenSocket, 1) == SOCKET_ERROR)  //можливе лише одне підключення
            printf("Error listening on socket.\n");

        printf("Listening on socket...\n");

        SOCKET AcceptSocket;
        printf("Waiting for client to connect...\n");

        // Accept the connection.
        while (1) {
            AcceptSocket = SOCKET_ERROR;
            while (AcceptSocket == SOCKET_ERROR) {
                AcceptSocket = accept(ListenSocket, NULL, NULL);
            }
            printf("Client connected.\n");
            ListenSocket = AcceptSocket;
            break;
        }

        int bytes = send(AcceptSocket, "2", 255, 0);

        bool isPasswordHacked = false;
        const char* password = new char[255];
        password = "ADA";
        while (!isPasswordHacked)
        {
            char* attemptedPasswordcStyle = new char[255];
            char* recievedResponse = new char[255];
            int bytesSent = 0;
            int bytesRecieved = SOCKET_ERROR;
            while (bytesRecieved == SOCKET_ERROR)
            {
                bytesRecieved = recv(ListenSocket, attemptedPasswordcStyle, 255, 0);
            }

            if (strcmp(attemptedPasswordcStyle, password) == 0)
            {
                bytesSent = send(ListenSocket, "YES!", 255, 0);
                isPasswordHacked = true;
            }
            else
            {
                bytesSent = send(ListenSocket, "NO!", 255, 0);
            }

            cout << "Password:" << attemptedPasswordcStyle << " , is hacked:" << isPasswordHacked << ", pwd:" << attemptedPasswordcStyle << endl;
            //Sleep(1000);

            delete[] recievedResponse;
            delete[] attemptedPasswordcStyle;
        }

        return 0;
    }
    else 
    {
        cout << "Program couldn't parse your password. Terminating..";
        return -1;
    }
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
