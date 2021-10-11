#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char *argv[]){
	/* fork a child process */
	pid_t pid;
	printf("Process start to fork\n");
    pid = fork();
    int EXIT_STATUS;
    //if the fork failed
    if (pid == -1){
        perror("fork");
        exit(1);
    }
    else{
        /*if the fork succeeded,
		 * child process executes the code in the blocks of "if" statement
		 */
        if (pid == 0){
            char *arg[argc];
            printf("I'm the Child process, my pid = %d\n", getpid());
            for (int i = 0; i < argc - 1; i++){
                arg[i] = argv[i+1];
            }
            arg[argc-1] = NULL;
            printf("Child process start to execute test program:\n");
            execve(arg[0], arg, NULL);
			perror("execve");
        }
        else {
            printf("I'm the Parent process, my pid = %d\n",getpid());
			// wait that child process terminates
			waitpid(pid, &EXIT_STATUS, 0);
            // waitpid(pid,&EXIT_STATUS,WUNTRACED);
            printf("Parent process receiving the SIGCHILD signal\n");
            /* 
			 * check child process'  termination status 
			 * If WIFEXITED(EXIT_STATUS) is nonzero, 
			 * it shows that process terminates normally.
			 */
            if (WIFEXITED(EXIT_STATUS)){ 
                printf("Normal termination with EXIT STATUS = %d\n", EXIT_STATUS);
            }
			/*
			 * If WIFSIGNALED(EXIT_STATUS) is nonzero,
			 * it shows that process terminates abnormally.
			 */
            else if (WIFSIGNALED(EXIT_STATUS)){
                char* SIGNAL, *STATUS;
                if (WTERMSIG(EXIT_STATUS) == 1){
                    SIGNAL = "SIGHUP";
                    STATUS = "hung up";
                }
                else if (WTERMSIG(EXIT_STATUS) == 2){
                    SIGNAL = "SIGINT";
                    STATUS = "interrupted";
                }
                else if (WTERMSIG(EXIT_STATUS) == 3){
                    SIGNAL = "SIGQUIT";
                    STATUS = "quit";
                }
                else if (WTERMSIG(EXIT_STATUS) == 4){
                    SIGNAL = "SIGILL";
                    STATUS = "failed because of illegal instruction";
                }
                else if (WTERMSIG(EXIT_STATUS) == 5){
                    SIGNAL = "SIGTRAP";
                    STATUS = "trapped";
                }
                else if (WTERMSIG(EXIT_STATUS) == 6){
                    SIGNAL = "SIGABRT";
                    STATUS = "aborted";
                }
                else if (WTERMSIG(EXIT_STATUS) == 7){
                    SIGNAL = "SIGBUS";
                    STATUS = "bus error";
                }
                else if (WTERMSIG(EXIT_STATUS) == 8){
                    SIGNAL = "SIGFPE";
                    STATUS = "floating point exception";
                }
                else if (WTERMSIG(EXIT_STATUS) == 9){
                    SIGNAL = "SIGKILL";
                    STATUS = "killed";
                }
                else if (WTERMSIG(EXIT_STATUS) == 11){
                    SIGNAL = "SIGSEGV";
                    STATUS = "segment fault";
                }
                else if (WTERMSIG(EXIT_STATUS) == 13){
                    SIGNAL = "SIGPIPE";
                    STATUS = "broken pipe";
                }
                else if (WTERMSIG(EXIT_STATUS) == 14){
                    SIGNAL = "SIGALARM";
                    STATUS = "alarmed";
                }
                else if (WTERMSIG(EXIT_STATUS) == 15){
                    SIGNAL = "SIGTERM";
                    STATUS = "terminated";
                }
                printf("child process get %s signal\n", SIGNAL);
                printf("child process is %s \n", STATUS);
            }
            else if (WIFSTOPPED(EXIT_STATUS)){
                char *SIGNAL, *STATUS;
                if (WSTOPSIG(EXIT_STATUS) == 19){
                    SIGNAL = "SIGSTOP";
                    STATUS = "stopped";
                }
                printf("child process get %s signal\n", SIGNAL);
                printf("child process %s\n", STATUS);
                printf("CHILD PROCESS STOPPED\n");
            }
        }
    }	
}
