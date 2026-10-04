#include "smallsh.h"
#include <pwd.h>
static char inpbuf[MAXBUF];     // Buffer to store the user's input line
static char tokbuf[2 * MAXBUF]; // Buffer to store tokens created from the input
static char *ptr = inpbuf;      // Pointer to traverse inpbuf
static char *tok = tokbuf;      // Pointer to store tokens in tokbuf

static char special[] = {' ', '\t', '&', ';', '\n', '\0'};

int userin(char *p) {
    int c, count;
    ptr = inpbuf; // Reset ptr to the start of inpbuf
    tok = tokbuf; // Reset tok to the start of tokbuf
    printf("%s", p);
    count = 0;

    while (1) {
        if ((c = getchar()) == EOF)
            return EOF;
        if (count < MAXBUF)
            inpbuf[count++] = c;
        if (c == '\n' && count < MAXBUF) {
            inpbuf[count] = '\0';
            return count;
        }
        if (c == '\n' || count >= MAXBUF) {
            printf("smallsh: input line too long\n");
            count = 0;
            printf("%s", p);
        }
    }
}

int gettok(char **outptr) {
    int type;
    *outptr = tok; // Set 'outptr' to point to the current 'tok' position in 'tokbuf'

    while (*ptr == ' ' || *ptr == '\t') // Skip leading spaces and tabs in 'inpbuf'
        ptr++;

    *tok++ = *ptr;    // Copy the current character from 'ptr' to 'tok' and increment 'tok'
    switch (*ptr++) { // Determine the type of the token based on the current character
    case '\n':
        type = EOL;
        break;
    case '&':
        type = AMPERSAND;
        break;
    case ';':
        type = SEMICOLON;
        break;
    case '|':
        type = PIPE;
        break;
    default:
        type = ARG;
        while (inarg(*ptr)) // While the character is not in 'special' characters, continue adding to the token
            *tok++ = *ptr++;
    }
    *tok++ = '\0';
    return type;
}

int inarg(char c) {
    char *wrk;

    for (wrk = special; *wrk; wrk++) { // Check if character 'c' is in the 'special' characters array
        if (c == *wrk)
            return 0;
    }
    return 1;
}


    int pipefd[2];//fds
    int prev_fd =-1;
    
void procline() {
    char *arg[MAXARG + 1];
    int toktype, type;
    int narg = 0;
    
    
    for (;;) {                                  // Infinite loop to process tokens until end of line (EOL)
        switch (toktype = gettok(&arg[narg])) { // Get the next token and assign it to 'toktype'
        case ARG:
            if (narg < MAXARG)
                narg++;
            break;
        case PIPE:
            type = FOREGROUND;
            if (narg != 0) {
                arg[narg] = NULL;
                runcommand(arg, type);
            }
            //pipe 생성
            if(pipe(pipefd)==-1){
              perror("pipe");
              return;
            }
            if (prev_fd != -1) {
                dup2(prev_fd, STDOUT_FILENO);
                close(prev_fd);
            }
            prev_fd = pipefd[1];
            narg = 0;
            break;
        case EOL:
        case SEMICOLON:
        case AMPERSAND:
            if (toktype == AMPERSAND)
                type = BACKGROUND;
            else
                type = FOREGROUND;
            if (narg != 0) {
                arg[narg] = NULL;
                runcommand(arg, type);
            }
            if (toktype == EOL)
                return;
            narg = 0; // Reset argument count for the next command
            break;
        }
    }
}



int runcommand(char **cline, int where) {
    pid_t pid;
    int status;

    
    // procline() has already parsed this command; do not consume another token.

    // ---------------------------------------------
    // Exit command: terminates the shell if 'exit' is entered
    // ---------------------------------------------

    if (strcmp(*cline, "exit") == 0)
        exit(0);

    // ---------------------------------------------
    // Handle 'cd' command with argument parsing
    // ---------------------------------------------
    if (strcmp(*cline, "cd") == 0) 
        return handle_cd_command(cline);
    

    // ---------------------------------------------
    // Command execution: fork a new process and execute command
    // ---------------------------------------------
    
   

    //--------------------------------------
    switch (pid = fork()) {
    case -1:
        perror("smallsh");
        return -1;
    case 0: // Code executed by the child process  
           sigset_t set_int;
            sigemptyset(&set_int);
            sigaddset(&set_int, SIGINT);
        if(where == BACKGROUND){//background는 SIGINT blcok 하여 계속 수행
            sigprocmask(SIG_BLOCK, &set_int, NULL);
            //signal(SIGINT, SIG_IGN);
        }
        else{//foreground는 SIGINT 받아들여 종료
            //signal(SIGINT,SIG_DFL);
            sigprocmask(SIG_UNBLOCK, &set_int, NULL);
        }
        
        
       
         
        
        
        
        execvp(*cline, cline);
        perror(*cline);
        exit(1);
    
    }
    ///////////////////
    

    /* following is the code of parent */
    
    if (where == BACKGROUND) {
        printf("[Process id] %d\n", pid);
        return 0;
    }
    if (waitpid(pid, &status, 0) == -1)//여기서 signal
        return -1;
    else
        return status;
}

// Function to handle 'cd' command
int handle_cd_command(char **cline) {
    struct passwd *userInfo = getpwuid(getuid());
    char *target = cline[1];
    char *expanded = NULL;

    if (cline[1] != NULL && cline[2] != NULL) {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }
    if (target == NULL || target[0] == '~') {
        if (userInfo == NULL) {
            fprintf(stderr, "cd: cannot find home directory\n");
            return 1;
        }
        const char *suffix = target == NULL ? "" : target + 1;
        expanded = malloc(strlen(userInfo->pw_dir) + strlen(suffix) + 1);
        if (expanded == NULL) {
            perror("malloc");
            return 1;
        }
        strcpy(expanded, userInfo->pw_dir);
        strcat(expanded, suffix);
        target = expanded;
    }
    int result = chdir(target);
    free(expanded);
    if (result != 0) {
        perror("cd");
        return 1;
    }
    // Refresh the existing main loop rather than enter a nested input loop.
    initializePrompt();
    return 0;
}
