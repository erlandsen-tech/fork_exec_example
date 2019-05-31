#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main (){

	char *argvs[] = {"cut", "-f1", "-d",":", "/etc/passwd", NULL};
	char printBuf[4];

	int p[2];
	if (pipe(p) == -1)
		printf("Piping failed..\n");

	pid_t pid;

	for(int i = 1; i <= 7; i++){
		pid = fork();
		if(pid == -1)
			printf("Fork error..\n");
		if (pid == 0) {
			close(p[0]);
			dup2(p[1], 1);
			sprintf(printBuf, "-f%d", i);
			argvs[1] = printBuf;
			execvp(argvs[0], argvs);
			printf("%s", printBuf);
			fprintf(stderr, "Execution Failed\n");
		}
		else
		{
			close(p[1]);
			char buf[1];
			while(read(p[0], buf, 1) > 0) {
				write(1, buf, 1);
			}
			sleep(1);
		}
	}
}
