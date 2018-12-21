
void exec_setenv(char **cmd_input,int * cmd_count)
{
	if(*cmd_count>3)
	{
		fprintf(stderr, "%s\n","setenv:too many arguments passed" );
	}
	else if(*cmd_count<2)
	{
		fprintf(stderr, "%s\n","setenv:too few arguments passed" );
	}
	else if(*cmd_count==2)
	{
		if(setenv(cmd_input[1], "", 1) == -1)
		{
			perror("Shell");
		}
	}
	else
	{
		if(setenv(cmd_input[1], cmd_input[2], 1) == -1)
		{
			perror("Shell");
		}	
	}

}


void exec_unsetenv(char **cmd_input,int * cmd_count)
{
	if(*cmd_count>2)
	{
		fprintf(stderr, "%s\n","setenv:too many arguments passed" );
	}
	else if(*cmd_count<2)
	{
		fprintf(stderr, "%s\n","setenv:too few arguments passed" );
	}
	else
	{
		if(unsetenv(cmd_input[1]) == -1)
		{
			perror("Shell");
		}
	}
}