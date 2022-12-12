#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    pid_t pid;
    int mynum = 5;

    pid = fork();

    /*
        This code demonstrates that two different processes are spawned and are run
        in two different memory spaces.
    */
    if (pid == 0){
        printf("I am a child. mynum is %d\n", mynum);
        getchar();
        // mynum = 10;
        printf("Stopping child. mynum is %d\n", mynum);
    }
    else{
        printf("I am the parent. Child pid is %d, and mynum is %d\n", pid, mynum);
        getchar();
        mynum = 20;
        printf("Stopping parent. mynum is %d\n", mynum);
    }
    return 0;
}