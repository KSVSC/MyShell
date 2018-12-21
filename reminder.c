

void exec_remind(char ** cmd_input,int * cmd_count)
{
	if(* cmd_count<3)
	{
		perror("least arguments passed");
	}
	else 
	{
		pid_t pid;

		struct timeval start, end;
		gettimeofday(&start, NULL);
		double delta;

		int background=1,i; // default back ground on
		int lapse= atoi(cmd_input[1]);
	
		char buf[100],buf1[100];

		if(strcmp(cmd_input[*cmd_count-1], "&") == 0)   /* if & is included process running in background*/
		{
	        cmd_input[*cmd_count-1] = NULL;
	        background = 1;
	    }

		pid=fork();

		if(pid < 0)
		{
			printf("%s\n","child process failed" );
			exit(1);
		}

		else if(pid==0 && background==1)
		{
			do
			{
				gettimeofday(&end, NULL);
				
   				delta = ((end.tv_sec-start.tv_sec) * 1000000u +end.tv_usec - start.tv_usec) / 1.e6;

			}while(delta< lapse);

			printf("\nREMINDER:");
			sprintf(buf,"%c",cmd_input[2][0]);
			if(strcmp(buf,"\"")==0)
			{
				for(int i=2;cmd_input[i]!=NULL;i++)
				{
					for(int j=0;j<strlen(cmd_input[i]);j++)
					{
						sprintf(buf1,"%c",cmd_input[i][j]);
						if(!strcmp(buf1,"\"")==0)
						{
							printf("%s",buf1);
						}
					}
					printf(" ");
				}
				printf("\n");
				
			}
			else
			{
				for(int i=2;cmd_input[i]!=NULL;i++)
			        printf("%s ",cmd_input[i]);
			               printf("\n");
				
			}
			exit(1);

		}

		else
		{
			exit(1);
		}
/*		else if (!background)
		{
      		wait(NULL);
      		exit(1);
		}
*/

	}
}
