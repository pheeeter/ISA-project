#include "dns.hpp"

bool isEmpty(const char *str)
{
    char ch;

    ch = *(str++);
    if(ch == '#'){
        return true;
    }
    ch = *(str--);

    do
    {
        ch = *(str++);

        // Check non whitespace character
        if(ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' && ch != '\0' )
            return false;

    } while (ch != '\0');

    return true;
}



vector<string> parse_file(char* filter_file){
    char str[256];
    char *pos;
    vector <string> v;

    FILE* fp = fopen(filter_file, "r");
    if(!fp){
        error_exit("Could not open file \"%s\"!", filter_file);
    }

    while(fgets(str, sizeof(str), fp) != NULL){
        if(isEmpty(str)){
            continue;
        }
        
        if ((pos=strchr(str, '\n')) != NULL){
            *pos = '\0';
            
        }
        v.push_back(str);
    }

    if(fclose(fp) == EOF){
        error_exit("Could not close file \"%s\"!", filter_file);
    }

    return v;
}