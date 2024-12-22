#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND 32
#define MAX_INPUT 512

#define STDIN 0
#define STDOUT 1

#define TRUE 1

static void child_handler(int sig){
    pid_t pid;
    int status;

    while((pid = waitpid(-1, &status, WNOHANG)) > 0){}
}

char* add_space(char* command){
    int command_len = strlen(command);
    char *modified_command = (char*)malloc(3 * command_len); 
    int i = 0;
    int j = 0;
    for (i = 0; command[i] != '\0'; i++) {
        if (command[i] == '>') {
            if (j > 0 && modified_command[j - 1] != ' ') {
                modified_command[j++] = ' ';
            }
            modified_command[j++] = '>';
            modified_command[j++] = ' ';
        } else {
            modified_command[j++] = command[i];
        }
    }

    modified_command[j] = '\0';
    return modified_command; 
}

char* delete_space(char* command){
    char* end;
    while(isspace((unsigned char)*command)){
        command++;
    } 
    
    if(*command == 0){
        return command;
    }

    end = command + strlen(command) - 1;
    while(end > command && isspace((unsigned char)*end)) {
        end--;
    }

    end[1] = '\0';

    return command;

}

char* delete_quotes(char* arg){
    char* end;
    while((unsigned char)*arg == '"'){
        arg++;
    } 
    
    if(*arg == 0){
        return arg;
    }

    end = arg + strlen(arg) - 1;
    while(end > arg && (unsigned char)*end == '"') {
        end--;
    }

    end[1] = '\0';

    return arg;

}

void execute_command(char* command[], int input_fd, int output_fd, int background){
    
    char *modified_command = add_space(*command);
    char *arg[MAX_COMMAND];
    char *token_arg = strtok(modified_command, " ");
    int arg_count = 0;
    char *input_file = NULL;
    char *output_file = NULL;

    while(token_arg != NULL){
        if(strchr(token_arg, '<') != NULL){
            input_file = strtok(NULL, " ");
        } else if(strchr(token_arg, '>') != NULL){
           output_file = strtok(NULL, " ");
        } else {
            arg[arg_count++] = token_arg;
        }
        token_arg = strtok(NULL, " ");
    }
    arg[arg_count] = NULL;


    int i = 0;

    for(i = 0; i<arg_count; i++){
        arg[i] = delete_quotes(arg[i]);
    }

    pid_t pid;
    pid = fork();
    if (pid == 0) {
        if(input_file){
            input_fd = open(input_file, O_RDONLY);
            if(input_fd < 0){
                perror("ERROR: Error in opening input file: ");
                exit(EXIT_FAILURE);
            }
            if(dup2(input_fd, STDIN_FILENO) < 0){
                perror("ERROR: Error in read/write input file: ");
                exit(EXIT_FAILURE);
            }
            close(input_fd);
        }


        if(output_file){
            output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(output_fd < 0){
                perror("ERROR: Error in opening output file: ");
                exit(EXIT_FAILURE);
            }
            if(dup2(output_fd, STDOUT_FILENO) < 0){
                perror("ERROR: Error in read/write output file: ");
                exit(EXIT_FAILURE);
            }
            close(output_fd);
        }


        if (input_fd != STDIN_FILENO && !input_file) {
           if(dup2(input_fd, STDIN_FILENO) < 0){
                perror("ERROR: Error in dup2 input fd: ");
                exit(EXIT_FAILURE);
            }
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO && !output_file) {
            if(dup2(output_fd, STDOUT_FILENO) < 0){
                perror("ERROR: Error in dup2 output fd: ");
                exit(EXIT_FAILURE);
            }
            close(output_fd);
        }


        if(execvp(arg[0], arg) == -1){
            perror("ERROR: Error in execvp()");
            exit(EXIT_FAILURE);
        }

    } else if (pid > 0) {
        if(background == 0){
            waitpid(pid, NULL, 0);
        } 
    } else {
      perror("ERROR: Error in fork(): ");
      exit(EXIT_FAILURE);
    }

    return;
}

int parse_command(char* input, char* command[], int* background){
    
    *background = 0;

    if(input[strlen(input)-1] == '&'){
        *background = 1;
        input[strlen(input)-1] = '\0';
    }

    char *token = strtok(input, "|");
    int com_count = 0;
    int i = 0;

    while(token != NULL){
        command[com_count++] = token;
        token = strtok(NULL, "|");
    }
    command[com_count] = NULL;


    for(i = 0; i < com_count; i++) {
        if (i == 0) {  
            if(com_count > 1){
                if (strchr(command[i], '>') != NULL) {
                    printf("ERROR: '>' is only allowed in the last command: %s.\n", command[i]);
                    return -1;
                }
            }
        } 
        else if (i == com_count - 1) {  
            if(com_count > 1){
                if (strchr(command[i], '<') != NULL) {
                    printf("ERROR: '<' is only allowed in the first command: %s.\n", command[i]);
                    return -1;
                }
            }
        } 
        else {  
            if (strchr(command[i], '<') != NULL || strchr(command[i], '>') != NULL) {
                printf("ERROR: Redirection is only allowed in the first and last commands: %s.\n", command[i]);
                return -1;
            }
        }

        if(strstr(command[i], "<<") != NULL){
            printf("ERROR: Invalid << command: %s.\n", command[i]);
                return -1;
        }
    }
    return com_count;

}

void piping(int com_count, char* command[], int background){
    int pipefd[2];
    int res;
    int i = 0;

    int input_fd = STDIN_FILENO;
    int output_fd = STDOUT_FILENO;

    for(i = 0; i < com_count; i++){
        if(i < com_count - 1){
            res = pipe(pipefd);
            if (res != 0) {
                perror("ERROR: Error in pipe(): "); 
                exit(EXIT_FAILURE);
            }
        }

        if(i == com_count-1){
            output_fd = STDOUT_FILENO;
            execute_command(&command[i], input_fd, output_fd, background);
        } else{
            output_fd = pipefd[1];
            execute_command(&command[i], input_fd, output_fd, background); 
            close(output_fd); 
            if (input_fd != STDIN_FILENO){
                close(input_fd);
            }
            input_fd = pipefd[0]; 
        }
    }
    if (input_fd != STDIN_FILENO){
        close(input_fd);
    }
}


int main(int argc, char *argv[]){

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sa.sa_handler = child_handler;
    sigaction(SIGCHLD, &sa, NULL);

    char input[MAX_INPUT];

    char shell[] = "my_shell$ ";

    int no_prompt = 0;

    char *command[MAX_COMMAND];

    int com_count;

    int background = 0;

    while(TRUE){
        if(argc > 1 && strcmp(argv[1], "-n") == 0){
        no_prompt = 1;
        }

        if(no_prompt == 0){
            printf("%s", shell);
            fflush(stdout);
        }
        
        if(fgets(input, sizeof(input), stdin) == NULL){
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        com_count = parse_command(input, command, &background);

        if(com_count == 1){
            execute_command(&command[0], STDIN_FILENO, STDOUT_FILENO, background);
        } else if (com_count > 1){
            piping(com_count, command, background);
        } 
    }

    return 0;
}