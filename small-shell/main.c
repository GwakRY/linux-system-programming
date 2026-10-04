#include "smallsh.h"
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

char prompt[200] = "Command> "; // Global prompt string initialized to "Command> "

// Function to initialize the prompt based on the current working directory
void initializePrompt() {
    char currentDir[200];                         // Buffer to store the current working directory
    struct passwd *userInfo = getpwuid(getuid()); // Retrieve user information

    if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
        perror("getcwd error");
        return;
    }

    // Set up the prompt based on whether the current directory is within the home directory
    if (userInfo != NULL && strncmp(currentDir, userInfo->pw_dir, strlen(userInfo->pw_dir)) == 0 &&
        (currentDir[strlen(userInfo->pw_dir)] == '/' || currentDir[strlen(userInfo->pw_dir)] == '\0')) { // Inside home directory or subdirectory
        if (strcmp(userInfo->pw_dir, currentDir) == 0) {
            snprintf(prompt, sizeof(prompt), "~$ "); // If exactly home directory, show "~$ "
        } else {
            snprintf(prompt, sizeof(prompt), "~%.196s$ ",
                     currentDir + strlen(userInfo->pw_dir)); // Show "~" + relative path
        }
    } else {
        snprintf(prompt, sizeof(prompt), "%.197s$ ", currentDir); // Outside home directory, show full path
    }
}

void child_handler(int a){
    while(waitpid(-1, NULL, WNOHANG) >0){}
}
void background_handler(int a){ 
    printf("\n");
    return;
}

int main() {
    //1.좀비 프로세스 제거
    struct sigaction act;
    act.sa_handler = child_handler;
    act.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, NULL);
    
    //2. SIGINT 처리
    sigset_t set_int;
    sigemptyset(&set_int);
    sigaddset(&set_int, SIGINT);
    sigprocmask(SIG_BLOCK, &set_int, NULL);
    //signal(SIGINT, background_handler);
    
    // Initialize prompt
    initializePrompt();
    
    

    // ---------------------------------------------
    // Begin main loop to handle user commands
    // ---------------------------------------------

    // Main loop to continuously get user input and process commands
    while (userin(prompt) != EOF)
        procline(); // Process each line of user input

    return 0;
}
