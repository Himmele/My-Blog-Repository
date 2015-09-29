#include <process.h>
#include <stdlib.h>

void* ThreadFunc(void* args) {}

int main(int argc, char* argv[], char* envp[])
{
	int pid;
	
	// don't mix fork() and threads
	
	/*
	* does not get a copy of the calling process's data *
	* vfork() borrows the calling process's memory and thread of control *
	* until a call to one of the exec*() functions is made. The calling process *
	* is suspended while the child is using its resources *
	*/
	pid = vfork();
	
	/*
	* use same I-space as parent process but seperate D-space --> only works in a MMU system *
	*/
	//pid = fork();
	if(pid < 0)
	{
		printf("unable to fork\n");
	}
	
	if(pid != 0)											// parent process
	{
		waitpid(-1, NULL, 0);							// suspend calling process until child terminates
	}
	else														// child process
	{
		/*
		* The execve() function replaces the current process image with a new *
		* process image specified by path. The new image is constructed from a *
		* regular, executable file called the new process image file. No return is *
		* made because the calling process image is replaced by the new process image. *
		*/
		execve("/sbin/Shell", argv, envp);
	}
	
	return 0;
}