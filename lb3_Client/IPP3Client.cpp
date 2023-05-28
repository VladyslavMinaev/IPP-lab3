#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

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
    SOCKET clientSocket = INVALID_SOCKET;
    struct sockaddr_in serverAddr;
    char request[DEFAULT_BUFLEN];
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Failed to create socket.\n");
        WSACleanup();
        return 1;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(DEFAULT_PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr)) <= 0) {
        printf("Invalid address.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Connection failed.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    printf("Connected to server.\n");
    if (send(clientSocket, "list", strlen("list"), 0) == SOCKET_ERROR) {
        printf("Send failed.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    printf("\nList of Students:\n");
    int numStudents;
    if (recv(clientSocket, (char*)&numStudents, sizeof(int), 0) == SOCKET_ERROR) {
        printf("Receive failed.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    Student student;
    for (int i = 0; i < numStudents; ++i) {
        if (recv(clientSocket, (char*)&student, sizeof(Student), 0) == SOCKET_ERROR) {
            printf("Receive failed.\n");
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
        printf("Name: %s\n", student.name);
        printf("Surname: %s\n", student.surname);
        printf("Age: %d\n", student.age);
        printf("Average Score: %.2f\n\n", student.averageScore);
    }
    if (send(clientSocket, "older_than_18", strlen("older_than_18"), 0) == SOCKET_ERROR) {
        printf("Send failed.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    printf("\nList of Students Older than 18:\n");
    if (recv(clientSocket, (char*)&numStudents, sizeof(int), 0) == SOCKET_ERROR) {
        printf("Receive failed.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    for (int i = 0; i < numStudents; ++i) {
        if (recv(clientSocket, (char*)&student, sizeof(Student), 0) == SOCKET_ERROR) {
            printf("Receive failed.\n");
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
        printf("Name: %s\n", student.name);
        printf("Surname: %s\n", student.surname);
        printf("Age: %d\n", student.age);
        printf("Average Score: %.2f\n\n", student.averageScore);
    }
    if (send(clientSocket, "scholarship", strlen("scholarship"), 0) == SOCKET_ERROR) {
        printf("Send failed.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    printf("\nList of Students Receiving a Scholarship:\n");
    if (recv(clientSocket, (char*)&numStudents, sizeof(int), 0) == SOCKET_ERROR) {
        printf("Receive failed.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    for (int i = 0; i < numStudents; ++i) {
        if (recv(clientSocket, (char*)&student, sizeof(Student), 0) == SOCKET_ERROR) {
            printf("Receive failed.\n");
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
        printf("Name: %s\n", student.name);
        printf("Surname: %s\n", student.surname);
        printf("Age: %d\n", student.age);
        printf("Average Score: %.2f\n\n", student.averageScore);
    }
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
