#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
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

char* get_stdout_multiline(const char* command) {

    FILE* fp = popen(command, "r");

    if (fp == NULL){
        // fflush(stdout);
        fprintf(stderr, "Command did not run properly\n");
        pclose(fp);
        exit(1);
    }

    char* ret = (char*) malloc(10001*sizeof(char));
    fread(ret,sizeof(char), 10000, fp);
    pclose(fp);
    removeNL(ret);
    return ret;
}

char *funnel_command(const char *command, const char *input) {
    int inpipes[2];
    int outpipes[2];

    pipe(inpipes);
    pipe(outpipes);
    
    write(inpipes[1], input, strlen(input));

    if (fork()) {
        //main
        close(inpipes[0]);
        close(outpipes[1]);

        
        close(inpipes[1]);

        int cursize = 1024;
        char *ret = (char *)malloc(cursize);
        
        int delta = 0;
        int curpos = 0;
        while ((delta = read(outpipes[0], &ret[curpos], 1023-curpos))) {
            curpos += delta;
        };
        
        close(outpipes[0]);
        removeNL(ret);
        return ret;

    } else {
        //child
        close(inpipes[1]);
        close(outpipes[0]);
        dup2(inpipes[0], STDIN_FILENO);
        dup2(outpipes[1], STDOUT_FILENO);
        system(command);
        exit(0);
    }
}