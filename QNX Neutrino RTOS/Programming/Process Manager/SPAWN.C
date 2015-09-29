#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char* argv[], char* envp[])
{
	int pid;
	
	/* more efficient than a fork() followed by an exec(), because there is  */
	/* no need to duplicate address spaces as in a fork() */
	if((pid = spawn("/sbin/Shell", NULL, NULL, NULL, argv, envp)) == -1)			// create and execute a child process
		printf("cannot create process\n");
	
	waitpid(pid, NULL, WEXITED);																		// wait for child process to exit
	
	return 0;
}