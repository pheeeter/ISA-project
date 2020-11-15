#include "dns.hpp"


void print_usage(){
    cerr << "Usage: ./dns -s server [-p port] -f filter_file\n" << endl
         << "Options:" << endl
         << "  -h\t\t\t prints this help and exit" << endl
         << "  -s server\t\t IP address or domain name of DNS server (resolver), where is the request send" << endl
         << "  -p port\t\t port number on which the program will expect queries (default: port 53)" << endl
         << "  -f filter_file\t name of the file with unwanted domains" << endl;

}

bool isNumber(const string& str){
  char* ptr;
  strtol(str.c_str(), &ptr, 10);
  return *ptr == '\0';
}


int main(int argc, char* argv[]){
    int c;
    char* server;
    int port = 53;
    char* filter_file;
    bool isServer = false;
    bool isFile = false;

    while((c = getopt(argc, argv, "s:p:f:h")) != -1){
        switch(c){
            case 's':
                isServer = true;
                server = optarg;
                if(!server){
                    print_usage();
                    exit(EXIT_FAILURE);
                }
                break;

            case 'p':
                try{
                    port = stoi(optarg);
                } catch(...){
                    error_exit("Port argument must be number between %d and %d!", MIN_PORT, MAX_PORT);
                } 
                if (port < MIN_PORT || port > MAX_PORT || !isNumber(optarg)){
                    error_exit("Port argument must be number between %d and %d!", MIN_PORT, MAX_PORT);
                }
                break;

            case 'f':
                isFile = true;
                filter_file = optarg;         
                break;

            case 'h':
                print_usage();
                exit(EXIT_SUCCESS);

            case '?':
                print_usage();
                exit(EXIT_FAILURE);
        }
    }

    if (!isServer || !isFile){
        print_usage();
        exit(EXIT_FAILURE);
    }

    string match = "247media.com";
    vector <string> v = parse_file(filter_file);
    for(auto & elem : v){
        if(regex_match(elem, regex("[^\n\r]*" + match))){
            cout << "Element " << match << " found" << endl;
            cout << endl;
            break;
        }
    }
    

    int s;
    int socket_fd;                      // socket
    int optval = 1;                     // socket options
    string port_str = to_string(port);
    struct sockaddr_in server_addr;
    struct addrinfo *info;
    struct sockaddr_in client_addr;     // server addr
    socklen_t server_addr_len;
    socklen_t client_addr_len;
    int nread;                          // number of bytes received
    char buf[BUFSIZE];                  // buffer
    char addrstr[100];

    if((s = getaddrinfo(server, port_str.c_str(), NULL, &info)) != 0){
        error_exit("getaddrinfo: %s", gai_strerror(s));
    }

    inet_ntop(info->ai_family, info->ai_addr->sa_data, addrstr, 100);

    switch (info->ai_family)
    {
        case AF_INET:
            inet_ntop(info->ai_family, &((struct sockaddr_in *) info->ai_addr)->sin_addr, addrstr, 100);
            break;
            
        case AF_INET6:
            inet_ntop(info->ai_family, &((struct sockaddr_in6 *) info->ai_addr)->sin6_addr, addrstr, 100);
            break;
    }


    printf ("IPv%d address: %s\n", info->ai_family == PF_INET6 ? 6 : 4, addrstr);

    socket_fd = socket(info->ai_family, SOCK_DGRAM, 0);
    if(socket_fd < 0){
        error_exit("Error opening socket.");
    }

    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR,
    (const void *)&optval, sizeof(int)) < 0){
        error_exit("Error while setting options on socket.");
    }

    bzero((char*) &client_addr, sizeof(client_addr));
    client_addr.sin_port = htons((unsigned short) port);
    
    if(bind(socket_fd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0){
        error_exit("Could not bind.");
    }
    client_addr.sin_family = info->ai_family;
    client_addr.sin_port = htons(port);

    inet_aton(addrstr, &(server_addr.sin_addr));
    
    int n;

    cout << "Listening on port " << port << "...\n" << endl;
    while(1){
        
        client_addr_len = sizeof(client_addr);
        nread = recvfrom(socket_fd, buf, BUFSIZE, 0, (struct sockaddr *) &client_addr, &client_addr_len);
        if (nread == -1){
            // ignore failed request
            continue;               
        }
        printf("Listening on server %s\n", inet_ntoa(client_addr.sin_addr));

        server_addr_len = sizeof(server_addr);

        n = sendto(socket_fd, &buf, nread, 0, (struct sockaddr *) &client_addr, client_addr_len);
        if (n != nread){
            error_exit("Error sending response: %s\n", strerror(errno));
        }

        sendto(socket_fd, &buf, nread, 0, (struct sockaddr *) &server_addr, server_addr_len);

        cout << "Query sent back to " << inet_ntoa(client_addr.sin_addr) << endl;
        cout << "Query sent to server " << inet_ntoa(server_addr.sin_addr) << "\n" << endl;
    }

    close(socket_fd);


    exit(EXIT_SUCCESS);
}