#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <process.h>
#include <stdatomic.h>
#pragma comment(lib, "Ws2_32.lib")

#define BUFLEN 512
#define PORT 9999
#define ADDRESS "127.0.0.1" // aka "localhost"

// global running variable
_Atomic char running = 0; // default false

DWORD WINAPI sendThreadFunc(LPVOID lpParam);


int server()
{
    printf("\n");

    int res, sendRes;

    // INITIALIZATION ===========================
    WSADATA wsaData; // configuration data
    res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res)
    {
        printf("Error al arrancar: %d\n", res);
        return 1;
    }
    // ==========================================

    // SETUP SERVER =============================

    // construct socket
    SOCKET listener;
    listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == INVALID_SOCKET)
    {
        printf("Error con la construccion:  %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // bind to address
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ADDRESS);
    address.sin_port = htons(PORT);
    res = bind(listener, (struct sockaddr *)&address, sizeof(address));
    if (res == SOCKET_ERROR)
    {
        printf("Fallo al bin:  %d\n", WSAGetLastError());
        closesocket(listener);
        WSACleanup();
        return 1;
    }

    // set as a listener
    res = listen(listener, SOMAXCONN);
    if (res == SOCKET_ERROR)
    {
        printf("Fallo al escuchar: %d\n", WSAGetLastError());
        closesocket(listener);
        WSACleanup();
        return 1;
    }
    // ==========================================

    printf("Aceptando en %s:%d\n", ADDRESS, PORT);

    // HANDLE A CLIENT ==========================

    // accept client socket
    SOCKET client;
    struct sockaddr_in clientAddr;
    int clientAddrlen;
    client = accept(listener, NULL, NULL);
    if (client == INVALID_SOCKET)
    {
        printf("No se pudo aceptar: %d\n", WSAGetLastError());
        closesocket(listener);
        WSACleanup();
        return 1;
    }

    // get client information
    getpeername(client, (struct sockaddr *)&clientAddr, &clientAddrlen);
    printf("Cliente conectado en %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    // send welcome message
    char *welcome = "Bienvenido al servidor:";
    sendRes = send(client, welcome, strlen(welcome), 0);
    if (sendRes != strlen(welcome))
    {
        printf("Error al enviar: %d\n", WSAGetLastError());
        shutdown(client, SD_BOTH);
        closesocket(client);
    }

    // receive messages
    char recvbuf[BUFLEN];
    do
    {
        res = recv(client, recvbuf, BUFLEN, 0);
        if (res > 0)
        {
            recvbuf[res] = '\0';
            printf("Mensaje recibido (%d): %s\n", res, recvbuf);

            if (!memcmp(recvbuf, "/quit", 5 * sizeof(char)))
            {
                // received quit command
                printf("Cerrando conexion.\n");
                break;
            }

            // echo message back
            sendRes = send(client, recvbuf, res, 0);
            if (sendRes != res)
            {
                printf("Error al enviar: %d\n", WSAGetLastError());
                shutdown(client, SD_BOTH);
                closesocket(client);
                break;
            }
        }
        else if (!res)
        {
            // client disconnected
            printf("Cerrando conexion.\n");
            break;
        }
        else
        {
            printf("Error al recibir %d\n", WSAGetLastError());
            break;
        }
    } while (res > 0);

    // shutdown client
    res = shutdown(client, SD_BOTH);
    if (res == SOCKET_ERROR)
    {
        printf("Fallo al desconectar cliente: %d\n", WSAGetLastError());
    }
    closesocket(client);

    // ==========================================

    // CLEANUP ==================================
    // shut down server socket
    closesocket(listener);

    // cleanup WSA
    res = WSACleanup();
    if (res)
    {
        printf("Fallo al cleanup: %d\n", res);
        return 1;
    }
    // ==========================================

    printf("Apagando\n\n");

    return 0;
}

int client()
{
    // printf("Hello, world!\n");

    int res;

    // INITIALIZATION ===========================
    WSADATA wsaData;
    res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res)
    {
        printf("Error al arrancar: %d\n", res);
        return 1;
    }
    // ==========================================

    // SETUP CLIENT SOCKET ======================

    // construct socket
    SOCKET client;
    client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client == INVALID_SOCKET)
    {
        printf("Error con la construccion: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // connect to address
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ADDRESS);
    address.sin_port = htons(PORT);
    res = connect(client, (struct sockaddr *)&address, sizeof(address));
    if (res == SOCKET_ERROR)
    {
        printf("Fallo al conectar: %d\n", WSAGetLastError());
        closesocket(client);
        WSACleanup();
        return 1;
    }
    else if (client == INVALID_SOCKET)
    {
        printf("Fallo al conectar: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // set as running
    printf("Conectado satisfactoriamente en %s:%d\n", ADDRESS, PORT);
    running = !0; // true

    // ==========================================

    // MAIN LOOP ================================

    // start send thread
    DWORD thrdId;
    HANDLE sendThread = CreateThread(
        NULL,
        0,
        sendThreadFunc,
        &client,
        0,
        &thrdId);

    if (sendThread)
    {
        printf("Send thread started with thread ID: %d\n", thrdId);
    }
    else
    {
        printf("Send thread failed: %d\n", GetLastError());
    }

    // receive loop
    char recvbuf[BUFLEN];
    do
    {
        res = recv(client, recvbuf, BUFLEN, 0);
        recvbuf[res] = '\0';
        if (res > 0)
        {
            printf("Recibido (%d): %s\n", res, recvbuf);
        }
        else if (!res)
        {
            printf("Conexion cerrada.\n");
            running = 0;
        }
        else
        {
            printf("Error al recibir: %d\n", WSAGetLastError());
            running = 0;
        }
    } while (running && res > 0);
    running = 0;

    // connection finished, terminate send thread
    if (CloseHandle(sendThread))
    {
        printf("Send thread closed successfully.\n");
    }

    // ==========================================

    // CLEANUP ==================================

    res = shutdown(client, SD_BOTH);
    if (res == SOCKET_ERROR)
    {
        printf("Error al desconectar %d\n", WSAGetLastError());
        closesocket(client);
        WSACleanup();
        return 1;
    }

    closesocket(client);
    WSACleanup();

    // ==========================================

    return 0;
}

DWORD WINAPI sendThreadFunc(LPVOID lpParam)
{
    SOCKET client = *(SOCKET *)lpParam;

    char sendbuf[BUFLEN];
    int sendbuflen, res;

    while (running)
    {
        scanf("%s", sendbuf);

        if (!running)
        {
            break;
        }

        sendbuflen = strlen(sendbuf);
        res = send(client, sendbuf, sendbuflen, 0);

        if (res != sendbuflen)
        {
            printf("Error al recibir");
            break;
        }
        else if (!memcmp(sendbuf, "/leave", 6))
        {
            running = 0;
            break;
        }
    }

    return 0;
}
int proceso( int opcion){
    if(opcion == 1 ){
        server();
    }else{
        client();
    }
    
}
