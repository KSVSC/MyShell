

void exec_clock(char ** cmd_input,int * cmd_count)
{
	if(* cmd_count<5)
	{
		perror("least arguments passed");
	}

	else 
	{
		pid_t pid;
		int background=0,i;// initally no back ground
		int lapse= atoi(cmd_input[2]);
		int duration=atoi(cmd_input[4]);
		int status;
		struct timeval start, end;
		gettimeofday(&start, NULL);
		double delta;

		time_t now = time( NULL);
		struct tm now_tm = *localtime( &now);
		struct tm *t;
		char text[100]; // for time display

		if(strcmp(cmd_input[*cmd_count-1], "&") == 0)
		{ /* if & is included process running in background*/

	        cmd_input[*cmd_count-1] = NULL;

	        background = 0; // since this dosnt care aout back ground
	    }

	    pid=fork();
	    if(pid == 0 && background != 1) // execute normally
            {
              	for( i=0;delta<duration;i=lapse)
              	{
              		now_tm.tm_sec+=i;
              		mktime( &now_tm);
              		t=&now_tm;
              		strftime(text, sizeof(text)-1, "%d %m %Y %H:%M:%S",t);
              		printf("%s\n",text );
              		gettimeofday(&end, NULL);
              		delta = ((end.tv_sec-start.tv_sec) * 1000000u +end.tv_usec - start.tv_usec) / 1.e6;
              		sleep(2);
              	}
				exit(1);
            }

		else if (!background)
		{
       		waitpid(pid, &status, WUNTRACED);
       		exit(1);
	    }


		
	}

}
