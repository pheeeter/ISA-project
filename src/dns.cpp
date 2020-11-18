// File: dns.cpp
// Description: Filtering DNS resolver
// Author: Peter Koprda <xkoprd00@stud.fit.vutbr.cz>

#include "dns.hpp"


/**
 * Prints usage of the program to the stderr.
 * */
void print_usage(){
    cerr << "Usage: ./dns -s server [-p port] -f filter_file\n" << endl
         << "Options:" << endl
         << "  -h\t\t\t prints this help and exit" << endl
         << "  -s server\t\t IP address or domain name of DNS server (resolver), where is the request send" << endl
         << "  -p port\t\t port number on which the program will expect queries (default: port 53)" << endl
         << "  -f filter_file\t name of the file with unwanted domains" << endl;

}


/**
 * Checks if parameter is valid integer number.
 * @param[in] str string that is checked
 * @return true if str is number
 * */
bool isNumber(const string& str){
  char* ptr;
  strtol(str.c_str(), &ptr, 10);
  return *ptr == '\0';
}

/**
 * Parsing arguments from console.
 * @param[in] argc count of arguments
 * @param[out] argv vector with arguments
 * @return structure with arguments
 * */
Args argParse(int argc, char* argv[]){
    int c;
    Args s;
    s.port = 53;
    bool isServer = false;
    bool isFile = false;


    while((c = getopt(argc, argv, "s:p:f:h")) != -1){
        switch(c){
            case 's':
                isServer = true;
                s.server = optarg;
                if(!s.server){
                    print_usage();
                    exit(EXIT_FAILURE);
                }
                break;

            case 'p':
                try{
                    s.port = stoi(optarg);
                } catch(...){
                    error_exit("Port argument must be number between %d and %d!", MIN_PORT, MAX_PORT);
                } 
                if (s.port < MIN_PORT || s.port > MAX_PORT || !isNumber(optarg)){
                    error_exit("Port argument must be number between %d and %d!", MIN_PORT, MAX_PORT);
                }
                break;

            case 'f':
                isFile = true;
                s.file_filter = optarg;         
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

    return s;
}

/**
 * Gets domain name from the question section.
 * @param buffer offset in header
 * @param domain_name domain name with dots
 * @return offset incremented by one
 * */
char* get_dns(char* buffer, char **domain_name){
    int8_t var;
    string tmp = "";

    while((int) *buffer != 0){
        var = *((int8_t *)(buffer));
        for (int8_t i = 0; i < var; ++i){
            tmp += buffer[i+1];
        }
        
        (++buffer) += var;
        if ((int) *buffer != 0){
            tmp += ".";
        }
    }

    *domain_name = strdup(tmp.c_str());
    return buffer + 1;
}

int main(int argc, char* argv[]){

    Args arguments = argParse(argc, argv);
    string port_str = to_string(arguments.port);

    vector <string> unwanted_domains = parse_file(arguments.file_filter);
    
    int socket_fd;                      // socket for local server
    int socket_DNS;                     // socket for DNS server
    int nread_dns;
                        
    struct sockaddr_in client_addr = localServer(arguments.port, &socket_fd);
    socklen_t client_addr_len = sizeof(client_addr);

    struct sockaddr_in server_addr;
    socklen_t server_addr_len;

    int nread;                          // number of bytes received
    dnsheader *dns = NULL;
    unsigned char *qname;
    char *qname_tmp;
    dnsquestion *qinfo; 
    unsigned char buf[BUFSIZE];
    char *domain_name;

    while(1){
        client_addr_len = sizeof(client_addr);
        nread = recvfrom(socket_fd, (char*) buf, MAX_LINE, 0, (struct sockaddr *) &client_addr, &client_addr_len);
        if (nread == -1){
            // ignore failed request
            continue;               
        }

        dns = (dnsheader *)buf;

        qname =(unsigned char*)&buf[sizeof(dnsheader)];
        qinfo = (dnsquestion*) &buf[sizeof(dnsheader) + (strlen((const char*)qname) + 1)];        

        domain_name = NULL;
        qinfo = (dnsquestion*) get_dns((char*) qname, &domain_name);
        qname_tmp = (char*) domain_name;
        
        if(ntohs(qinfo->qtype) != A || dns->z != 0){
            dns->rcode = RCODE_NOTIMP;
            if((sendto(socket_fd, (char*) dns, nread, 0, (struct sockaddr *) &client_addr, client_addr_len)) < 0){
                error_exit("Error while sending response: %s", strerror(errno));
            }
            continue;
        }

        string str(qname_tmp);
        if(isUnwantedDomain(qname_tmp, unwanted_domains)){
            dns->rcode = RCODE_REFUSED;
            if((sendto(socket_fd, (char*) dns, nread, 0, (struct sockaddr *) &client_addr, client_addr_len)) < 0){
                error_exit("Error while sending response: %s", strerror(errno));
            }
            continue;
        }
 

        server_addr = connectDNS(arguments, &socket_DNS);
        server_addr_len = sizeof(server_addr);
        dns->qr = 1;
        if((sendto(socket_DNS, (char*) dns, nread, 0, (struct sockaddr*) &server_addr, server_addr_len)) < 0){
            error_exit("Error while sending response: %s", strerror(errno));
        }

        dns->qr = 0;
        nread_dns = recvfrom(socket_DNS, (char*) dns, MAX_LINE, 0, (struct sockaddr*) &server_addr, &server_addr_len);
        if(nread_dns == -1){
            error_exit("%s\n", strerror(errno));
        }

        close(socket_DNS);

        dns->qr = 1;
        if((sendto(socket_fd, (char*) dns, nread, 0, (struct sockaddr *) &client_addr, client_addr_len)) < 0){
            error_exit("Error sending response: %s", strerror(errno));
        }
    }

    close(socket_fd);
    exit(EXIT_SUCCESS);
}