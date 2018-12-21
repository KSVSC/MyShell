
void exec_pinfo(char ** cmd_input,int * cmd_count)
{
	FILE *fd1,*fd2;
    int flag= 0;
    char state;
    int pid;
    long unsigned int vmsize;
    char exec_stat[200],buf1[200],buf2[200],buf3[200];

	if(* cmd_count==1)
	{
		printf("%d",getpid());
	     sprintf(buf1,"/proc/%d/stat",getpid());
	     sprintf(buf2,"/proc/%d/statm",getpid());
	     sprintf(buf3,"/proc/%d/exe",getpid());
 	}
 	else
 	{
	     sprintf(buf1,"/proc/%s/stat",cmd_input[1]);
	     sprintf(buf2,"/proc/%s/statm",cmd_input[1]);
	     sprintf(buf3,"/proc/%s/exe",cmd_input[1]);
	}

    fd1 = fopen(buf1,"r");
	if(fd1 != NULL)
	{
		flag= 1;
		fscanf(fd1,"%d %*s %c",&pid,&state);
		fclose(fd1);
		fd2 = fopen(buf2,"r");
		fscanf(fd2,"%lu",&vmsize);
		fclose(fd2);
	}

	if(flag)
	{
		printf("Pid: %d\n",pid);
		printf("Process State : %c\n",state);
		printf("VirtualMemory:  %lukB\n",vmsize);

		readlink(buf3,exec_stat,256);
		printf("Executable Path: %s\n",exec_stat);
	}

    else
	{
	    fprintf(stderr,"No such process with given pid\n");
	}

}