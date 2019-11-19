#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main()
{



	char cmd[50];
	int active = 0;
	int logging = 0;

	FILE * fc = fopen("command.log", "w+");
	FILE * fo = fopen("output.log", "w+");

	while(1)
	{
		printf("=> ");
		char * cmd_pipe[10] = {NULL};
		scanf("%[^\n]%*c", cmd);
		// printf("%s \n", cmd);

		int l = 0;
			cmd_pipe[l++] = strtok(cmd, "|");

		while(cmd_pipe[l-1] != NULL)
		{
			cmd_pipe[l++] = strtok(NULL, "|");
		}
		l -= 2;

		// printf("l %d \n", l);

		//int old_in = dup(0);
		//int old_out = dup(1);

		for(int j = 0; j <= l; j++)
		{
			// printf("lo \n");
			// printf("j is %d \n", j);
 
			char filename[50] = {'o', 'p', '_', '\0'};
			char prevfile[50] = {'o', 'p', '_', '\0'};
			// printf("%s \n", filename);
			char pipe_no[10];
			char prev_no[10];
			sprintf(pipe_no, "%d", j);
			sprintf(prev_no, "%d", j-1);
			// printf("%s sdjn\n", pipe_no);
			strcat(filename, pipe_no);
			strcat(prevfile, prev_no); 

			// printf("j is %d \n", j);
			if(l > 0 && j < l)
			{
				FILE * f1 = fopen(filename, "w+");
				int f1_desc = fileno(f1);
				
				dup2(f1_desc, 1);
				fclose(f1);
			}
			if(l > 0 && j > 0)
			{
				FILE * f2 = fopen(prevfile, "r");
				int f2_desc = fileno(f2);
				
				dup2(f2_desc, 0);
				fclose(f2);
			}
			// printf("here is %d \n", j);

			if((active == 0) && (strcmp(cmd, "entry") != 0))
			{
				printf("command line interpreter not started \n");
				continue;
			}
			else if((active == 0) && (strcmp(cmd, "entry") == 0))
			{
				active = 1;
				printf("command line interpreter started \n");
				continue;
			}
			else if((active == 1) && (strcmp(cmd, "exit") == 0))
			{
				active = 0;
				printf("command line interpreter exited \n");
				continue;
			}
			else
			{
				if(strcmp(cmd, "log") == 0)
				{
					logging = 1;
					continue;
				}
				if(strcmp(cmd, "unlog") == 0)
				{
					logging = 0;
					continue;
				}

				// if(strcmp(cmd, "changedir") == 0)
				// 	chdir()


				char * cmd_split[10] = {NULL};
				int i = 0;
				cmd_split[i++] = strtok(cmd_pipe[j], " ");

				// printf("entered %d\n", j);
				int n = 0;
		
				while(cmd_split[i-1] != NULL)
				{
					n++;
					cmd_split[i++] = strtok(NULL, " ");
				}
				// printf("%d \n", n);
		
				// i = 0;
				// while(cmd_split[i] != NULL)
				// 	printf("%s \n", cmd_split[i++]);

				if(strcmp(cmd_split[0], "changedir") == 0)
				{
					if(chdir(cmd_split[1]) == -1)
						printf("No such directory \n");
					continue;
				}

				// printf("creating child for exec \n");
				if(!fork())
				{
					
					char * v[n+1];
					// printf("in child for exec n is %d\n", n);

					for(int i = 0; i < n; i++)
					{
						v[i] =(char*) malloc(20*sizeof(char));
						sprintf(v[i], "%s", cmd_split[i]);	
					}
					v[n] = NULL;

					// int m = 0;
					// while(v[m] != NULL)
					// {
					// 	printf("%s ", v[m]);
					// 	m++;
					// }
					// printf("\n");

					// printf("in child for exec \n");

					if(execvp(v[0], v) == -1)
					{
						printf("kkk\n");
					}


					
					
				}
				wait(NULL);

			}
			// printf("pipe \n");
			//dup2(old_in, 0);
			//dup2(old_out, 1);
		}


		

		// dup2(old_in, 0);
		// dup2(old_out, 1);
		// printf("old_out \n");
		
	}

	fclose(fc);
	fclose(fo);

	return(0);
		
		
}