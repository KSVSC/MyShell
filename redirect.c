

char ** redirect(char ** cmd_input,int * cmd_count, int * Pin, int * Pout, int * Pin_fd, int * Pout_fd, int * Ppos)
{
	int in=0,out=0,in_fd=0,out_fd=1,pos=0;
	char **args=malloc(sizeof(char*)*(50));
	
	for(int i=0;i<*cmd_count;i++)
	{
			if(strcmp(cmd_input[i],"<")==0)
			{
				in=1;
				if(*cmd_count>i+1)
				{
					in_fd=open(cmd_input[++i],O_RDONLY);
					if(in_fd==-1)
					{
						fprintf(stderr,"No such file exists\n");
						return NULL;
					}
				}
				else
				{
					fprintf(stderr,"syntax error :Please give an input file \n");
						return NULL;
				}
			}

			else if(strcmp(cmd_input[i], ">") == 0)
			{
				out = 1;
				if(*cmd_count>i+1)
				{
					out_fd = open(cmd_input[++i], O_WRONLY | O_TRUNC | O_CREAT, 0644);
					if(out_fd==-1)
					{
						fprintf(stderr,"error in opening the file\n");
						return NULL;
					}
				}
				else
				{
					fprintf(stderr,"syntax error :Please give an output file\n");
						return NULL;
				}				
			}

			else if(strcmp(cmd_input[i], ">>") == 0)
			{
				out = 1;
				if(*cmd_count > i + 1)
				{
					out_fd = open(cmd_input[++i], O_WRONLY | O_APPEND | O_CREAT, 0644);
					if(out_fd==-1)
					{
						fprintf(stderr,"syntax error :error in opening the file\n");
						return NULL;
					}
				}
				else
				{
					fprintf(stderr,"Please give an output file\n");
						return NULL;
				}						
			}

			else
			{
				args[pos]=cmd_input[i];
				pos=pos+1;
			}
			
	}
		
	*Pin = in;
	*Pout =out;
	*Pin_fd = in_fd;
	*Pout_fd = out_fd;
	*Ppos = pos;

	return args;
}

