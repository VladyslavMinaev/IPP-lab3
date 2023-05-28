#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 1050

typedef struct {
    char name[50];
    char surname[50];
    int age;
    float averageScore;
} Student;

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char buffer[DEFAULT_BUFLEN];
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Failed to create socket.\n");
        WSACleanup();
        return 1;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(DEFAULT_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    printf("Server started. Listening for incoming connections...\n");
    if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen)) == INVALID_SOCKET) {
        printf("Accept failed.\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    printf("Client connected.\n");
    Student students[] = {
        { "John", "Doe", 20, 3.8f },
        { "Jane", "Smith", 18, 4.2f },
        { "Mike", "Johnson", 19, 3.5f },
        { "Emily", "Davis", 21, 4.8f },
        { "David", "Taylor", 17, 3.9f }
    };
    int numStudents = sizeof(students) / sizeof(Student);
    while (1) {
        int requestLen = recv(clientSocket, buffer, DEFAULT_BUFLEN, 0);
        if (requestLen == SOCKET_ERROR) {
            printf("Receive failed.\n");
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
        if (requestLen == 0) {
            break;
        }
        buffer[requestLen] = '\0';
        if (strcmp(buffer, "list") == 0) {
            if (send(clientSocket, (char*)&numStudents, sizeof(int), 0) == SOCKET_ERROR) {
                printf("Send failed.\n");
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }
            for (int i = 0; i < numStudents; ++i) {
                if (send(clientSocket, (char*)&students[i], sizeof(Student), 0) == SOCKET_ERROR) {
                    printf("Send failed.\n");
                    closesocket(clientSocket);
                    WSACleanup();
                    return 1;
                }
            }
        }
        else if (strcmp(buffer, "older_than_18") == 0) {
            int count = 0;
            for (int i = 0; i < numStudents; ++i) {
                if (students[i].age > 18) {
                    ++count;
                }
            }
            if (send(clientSocket, (char*)&count, sizeof(int), 0) == SOCKET_ERROR) {
                printf("Send failed.\n");
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }
            for (int i = 0; i < numStudents; ++i) {
                if (students[i].age > 18) {
                    if (send(clientSocket, (char*)&students[i], sizeof(Student), 0) == SOCKET_ERROR) {
                        printf("Send failed.\n");
                        closesocket(clientSocket);
                        WSACleanup();
                        return 1;
                    }
                }
            }
        }
        else if (strcmp(buffer, "scholarship") == 0) {
            int count = 0;
            for (int i = 0; i < numStudents; ++i) {
                if (students[i].averageScore > 4.0f) {
                    ++count;
                }
            }
            if (send(clientSocket, (char*)&count, sizeof(int), 0) == SOCKET_ERROR) {
                printf("Send failed.\n");
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }
            for (int i = 0; i < numStudents; ++i) {
                if (students[i].averageScore > 4.0f) {
                    if (send(clientSocket, (char*)&students[i], sizeof(Student), 0) == SOCKET_ERROR) {
                        printf("Send failed.\n");
                        closesocket(clientSocket);
                        WSACleanup();
                        return 1;
                    }
                }
            }
        }
        else {
            printf("Invalid request: %s\n", buffer);
        }
    }
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
