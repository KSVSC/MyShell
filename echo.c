
void exec_echo(char ** cmd_input,int * cmd_count)
{
	char buf[100],buf1[100];
	int k=0;

	if(*cmd_count==1)
	{
		printf("least arguments passed to echo\n");
		return ;
	}
	sprintf(buf,"%c",cmd_input[1][0]);
	if(strcmp(buf,"\"")==0)
	{
		for(int i=1;cmd_input[i]!=NULL;i++)
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
		for(int i=1;cmd_input[i]!=NULL;i++)
	        printf("%s ",cmd_input[i]);
	               printf("\n");
	}
}
