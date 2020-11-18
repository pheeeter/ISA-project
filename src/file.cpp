// File: file.cpp
// Description: Implementation of parsing file with unwanted domains
// Author: Peter Koprda <xkoprd00@stud.fit.vutbr.cz>


#include "dns.hpp"

/**
 * Checks if line in file is empty or starts with #
 * @param str line in file
 * @return true is line is empty or starts with #
 * */
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


/**
 * Parses file into vector of strings line by line
 * @param filter_file file with unwanted domains
 * @return vector
 * */
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


/**
 * Checks is domain is in vector of unwanted domains.
 * @param item domain name
 * @param domains vector of strings with domains
 * @return true if item is in vector
 * */
bool isUnwantedDomain(string item, vector <string> domains){
    for(auto & elem : domains){
        if(elem == item){
            return true;
        }
    }
    return false;
}