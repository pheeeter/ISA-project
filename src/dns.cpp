#include "dns.hpp"


void print_usage(string message){
    cerr << "ERROR: " << message << endl
         << "\nUsage: ./dns -s server [-p port] -f filter_file" << endl
         << "Options:" << endl
         << "  -s server\t\t IP address or domain name of DNS server (resolver), where should be the request send" << endl
         << "  -p port\t\t port number on which the program will expect queries, default port is port 53" << endl
         << "  -f filter_file\t name of the file with unwanted domains" << endl;

    exit(EXIT_FAILURE);
}

bool isNumber(const string& str){
  char* ptr;
  strtol(str.c_str(), &ptr, 10);
  return *ptr == '\0';
}

int main(int argc, char* argv[]){
    int c;
    string server;
    int port = 53;
    char* filter_file;
    bool isServer = false;
    bool isFile = false;

    while((c = getopt(argc, argv, "s:p:f:")) != -1){
        switch(c){
            case 's':
                isServer = true;
                server = optarg;
                if(server == ""){
                    print_usage("Option s requires argument!");
                }
                break;

            case 'p':
                try{
                    port = stoi(optarg);
                } catch(...){
                    print_usage("Port argument must be number between 0 and 65 535!");
                } 
                if (port < MIN_PORT || port > MAX_PORT || !isNumber(optarg)){
                    print_usage("Port argument must be number between 0 and 65 535!");
                }
                
                break;

            case 'f':
                isFile = true;
                filter_file = optarg;         
                break;

            case '?':
                string message = "Unknown  option '";
                message = message + char(optopt) + "'";
                print_usage(message);
        }
    }

    if(!isServer){ print_usage("Option s is required!"); }
    if(!isFile)  { print_usage("Option f is required!"); }

    FILE* fp = fopen(filter_file, "r");
    if(!fp){
        error_exit("Could not open file \"%s\"!", filter_file);
    }
    cout << "server: "     <<    server   << "" << endl
         << "filter_file: "<< filter_file << "" << endl
         << "port: "       <<    port     << "" << endl;

    if(fclose(fp) == EOF){
        error_exit("Could not close file \"%s\"!", filter_file);
    }
    exit(EXIT_SUCCESS);
}