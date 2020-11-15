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

#define MIN_PORT 0
#define MAX_PORT 65535
#define BUFSIZE 512

using namespace std;

#define error_exit(message,...) \
    fprintf(stderr, message "\n", ##__VA_ARGS__); \
    exit(EXIT_FAILURE);

void print_usage(string message);
bool isNumber(const string& str);
vector<string> parse_file(char* filter_file);
