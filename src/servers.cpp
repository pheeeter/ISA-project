// File: servers.cpp
// Description: Implementation of connecting to localhost and DNS server
// Author: Peter Koprda <xkoprd00@stud.fit.vutbr.cz>


#include "dns.hpp"

/**
 * Connects to DNS server
 * @param arguments structure with parameters
 * @param socket_DNS pointer to address where is socket stored
 * @return structure of DNS address
 * */
struct sockaddr_in connectDNS(Args arguments, int *socket_DNS){
    string port_str = to_string(arguments.port);
    int s;
    char addrstr[100];
    struct addrinfo *info;
    int optval = 1;
    struct sockaddr_in server_addr;

    if((s = getaddrinfo(arguments.server, port_str.c_str(), NULL, &info)) != 0){
        error_exit("getaddrinfo: %s", gai_strerror(s));
    }

    inet_ntop(info->ai_family, info->ai_addr->sa_data, addrstr, 100);
    switch (info->ai_family){
        case AF_INET:
            inet_ntop(info->ai_family, &((struct sockaddr_in *) info->ai_addr)->sin_addr, addrstr, 100);
            break;
            
        case AF_INET6:
            inet_ntop(info->ai_family, &((struct sockaddr_in6 *) info->ai_addr)->sin6_addr, addrstr, 100);
            break;
    }


    if((*socket_DNS = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        error_exit("%s", strerror(errno));
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(arguments.port);

    if(setsockopt(*socket_DNS, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
    (const void *)&optval, sizeof(int)) < 0){
        error_exit("Error while setting options on socket.");
    }

    if(bind(*socket_DNS, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0){
        error_exit("Could not bind.");
    }

    freeaddrinfo(info);
    return server_addr;
}

/**
 * Connects to local server
 * @param port port number
 * @param socket_fd pointer to address where is socket stored
 * @return structure of DNS address
 * */
struct sockaddr_in localServer(int port, int* socket_fd){
    int optval = 1;         // socket options
    struct sockaddr_in client_addr;

    if((*socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        error_exit("Error opening socket.");
    }
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = INADDR_ANY;
    client_addr.sin_port = htons(port);

    if(setsockopt(*socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
    (const void *)&optval, sizeof(int)) < 0){
        error_exit("Error while setting options on socket.");
    }

    if(bind(*socket_fd, (struct sockaddr*) &client_addr, sizeof(client_addr)) < 0){
        error_exit("Could not bind.");
    }

    return client_addr;
}
