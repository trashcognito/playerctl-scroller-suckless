#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include "thread_helper.hpp"

void removeNL(char* arr) {
    if (arr[strlen(arr)-1] == '\n')
        arr[strlen(arr)-1] = '\0';
}

char* get_stdout(const char* command) {

    FILE* fp = popen(command, "r");

    if (fp == NULL){
        // fflush(stdout);
        fprintf(stderr, "Command did not run properly\n");
        pclose(fp);
        exit(1);
    }

    char* ret = (char*) malloc(1001*sizeof(char));
    fgets(ret, 1000, fp);
    pclose(fp);
    removeNL(ret);
    return ret;
}