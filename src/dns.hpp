// File: dns.hpp
// Description: File with libraries, macros, etc.
// Author: Peter Koprda <xkoprd00@stud.fit.vutbr.cz>

// libraries
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <netinet/in.h>

#define MIN_PORT 0
#define MAX_PORT 65535
#define BUFSIZE 512
#define MAX_LINE 65536

// type A records
#define A 1

// rcode values
#define RCODE_NOTIMP 4
#define RCODE_REFUSED 5

using namespace std;

#define error_exit(message,...) \
    fprintf(stderr, message "\n", ##__VA_ARGS__); \
    exit(EXIT_FAILURE);

// structure of arguments
struct args{
    char* server;
    int port;
    char* file_filter;
};
typedef struct args Args;

//DNS header structure
struct DNS_HEADER{
    unsigned short id;          // identification number
 
    unsigned char rd :1;        // recursion desired
    unsigned char tc :1;        // truncated message
    unsigned char aa :1;        // authoritive answer
    unsigned char opcode :4;    // purpose of message
    unsigned char qr :1;        // query/response flag
 
    unsigned char rcode :4;     // response code
    unsigned char cd :1;        // checking disabled
    unsigned char ad :1;        // authenticated data
    unsigned char z :1;         // its z! reserved
    unsigned char ra :1;        // recursion available
 
    unsigned short q_count;     // number of question entries
    unsigned short ans_count;   // number of answer entries
    unsigned short auth_count;  // number of authority entries
    unsigned short add_count;   // number of resource entries
};
typedef struct DNS_HEADER dnsheader;

// DNS question structure
struct QUESTION{
    unsigned short qtype  :16;
    unsigned short qclass :16;
}; 
typedef struct QUESTION dnsquestion;


void print_usage(string message);
bool isNumber(const string& str);
Args argParse(int argc, char* argv[]);

vector<string> parse_file(char* filter_file);
bool isUnwantedDomain(string item, vector <string> domains);

struct sockaddr_in connectDNS(Args arguments, int *socket_DNS);
struct sockaddr_in localServer(int port, int* socket_fd);
char *get_dns(char* buffer, char **domain_name);
