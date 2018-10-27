#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	int in[2]; // in[0] : read, in[1]: write

	pipe(in);
	
	if (fork()) // papa
	{
		close(in[0]);
		dup2(in[1], STDOUT_FILENO);
		execlp("./calculator", "./calculator", (char *) NULL);
	}
	else 
	{
		close(in[1]);		
		if (dup2(in[0], STDIN_FILENO) != STDIN_FILENO) fprintf(stderr, "in failed\n");
				
		execlp("sage", "sage",(char *) NULL);
	}
	
	
	return 0;
}
