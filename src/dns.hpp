#include <iostream>
#include <string>
#include <stdlib.h>
#include <getopt.h>

#define MIN_PORT 0
#define MAX_PORT 65535

using namespace std;

#define error_exit(message,...) \
    fprintf(stderr, "ERROR: " message "\n", ##__VA_ARGS__); \
    exit(EXIT_FAILURE);

void print_usage(string message);
bool isNumber(const string& str);
