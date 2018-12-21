#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <dirent.h>
#include<pwd.h>
#include <grp.h>
#include <time.h>
#include <langinfo.h>
#include <fcntl.h>
#include <signal.h>
#include <ctype.h>

#include "echo.c"
#include "redirect.c"
#include "ls.c"
#include "reminder.c"
#include "clock.c"
#include "pinfo.c"
#include "env.c"
#include "job.c"

#define delimiter " \t\r\n\a"
#define semi_delimiter ";"
#define back_delimiter "&"
#define pipe_delimiter "|"

void cyan_color() {
  printf("\033[1;36m");
}

void blue_color() {
  printf("\033[1;34m");
}

void reset_color() {
  printf("\033[0m");
}

void display_prompt();
char * get_input();
char ** parse(char *line,int * cmd_count);
char ** semi_parse(char *line,int * cmd_count);
char ** back_parse(char *line,int * cmd_count);
char ** is_pipe(char *line,int * cmd_count);
char ** redirect(char ** cmd_input,int * cmd_count, int * Pin, int * Pout, int * Pin_fd, int * Pout_fd, int * Ppos);
void execute(char **cmd_input,int * cmd_count);
void exec_cd(char **cmd_input,int * cmd_count);
void exec_pwd();
void exec_CTRLC(int sig_num);
void exec_CTRLZ(int sig_num);
int exec_fg(char **cmd_input,int * cmd_count);
int exec_bg(char **cmd_input,int * cmd_count);

int Shell_Pid;
int Child_Pid = -1;
jobL* list;
char * cur_proc;

char hostname[200];
char cwd[2000];
char currentDIR[2000];
char *user;
char *wd;

int main()
{
	list = (jobL *)malloc(sizeof(jobL));
	list->next = NULL;
	 Shell_Pid=getpid();
     user=getenv("USER");
     getcwd(currentDIR,2000);
     gethostname(hostname, 200);

     display_prompt();
     return 0;
}

void display_prompt()
{
     char  *line;                                                // the input line 
     int  *cmd_count1=(int*)malloc(sizeof(int));
     int  *cmd_count2=(int*)malloc(sizeof(int));
     int  *cmd_count3=(int*)malloc(sizeof(int));
     int  *cmd_count4=(int*)malloc(sizeof(int));                 
     int  *cmd_count5=(int*)malloc(sizeof(int));
     char ** cmd_input1,** cmd_input2,** cmd_input3,** cmd_input4,** cmd_input5;
     char *argv[100];
     int *Pin=(int*)malloc(sizeof(int));
   	 int *Pout=(int*)malloc(sizeof(int));
     int *Pin_fd=(int*)malloc(sizeof(int));
	 int *Pout_fd=(int*)malloc(sizeof(int));
     int bg=0,status,c=0,x,pip=0;
  	int in=0;
  	int out=0;
  	int in_fd=0;
  	int out_fd=1;

     signal(SIGTSTP, exec_CTRLZ);
     signal(SIGINT, exec_CTRLC); 
 	 while(1)
     {
     	Jobs_exited(list);
     	Child_Pid = -1;
 	    getcwd(cwd,2000);
          if(strstr(cwd,currentDIR) != NULL)
               wd = strstr(cwd, currentDIR) + strlen(currentDIR);
          else
               wd = cwd;

          cyan_color();
          printf("<%s@%s:~",user,hostname);
          blue_color();
          printf("%s>",wd);
     	  reset_color();
          
          line=get_input();
          if(strcmp(line,"\n")==0)
          {
          	  continue;
          }

          cmd_input1=semi_parse(line,cmd_count1);
          
          for(int i=0;i<*cmd_count1;i++)
          {
          	cmd_input2=is_pipe(cmd_input1[i],cmd_count2);

         	if(*cmd_count2==1)
          	{
          		pip=0;
          	}
          	else
          	{
          		pip=1;
          	}
//no pipes
          	if(pip==0)
          	{

          		cmd_input3=back_parse(cmd_input2[0],cmd_count3);

	          	if(*cmd_count3==1)
    	      	{
        	  		bg=0;
          		}
          		else
          		{
          			bg=1;
          		}

          		cmd_input4=parse(cmd_input3[0],cmd_count4);

				cmd_input5=redirect(cmd_input4,cmd_count4,Pin,Pout,Pin_fd,Pout_fd,cmd_count5);
          	
	          	in=*Pin;
	          	out=*Pout;
	          	in_fd=*Pin_fd;
	          	out_fd=*Pout_fd; 
	        
	        	if(cmd_input5==NULL)
	        		continue;

	          	if(strcmp(cmd_input5[0],"cd")==0)
	          	{
					execute(cmd_input5,cmd_count5);          		
	          	}

	          	if(strcmp(cmd_input5[0],"exit")==0 ||strcmp(cmd_input5[0],"quit")==0)
	          	{
					execute(cmd_input5,cmd_count5); 	        		
	          	}

	          	if(strcmp(cmd_input5[0],"pinfo")==0 ||strcmp(cmd_input5[0],"echo")==0 ||strcmp(cmd_input5[0],"ls")==0 ||strcmp(cmd_input5[0],"pwd")==0 ||strcmp(cmd_input5[0],"jobs")==0||strcmp(cmd_input5[0],"kjob")==0 ||strcmp(cmd_input5[0],"setenv")==0||strcmp(cmd_input5[0],"unsetenv")==0||strcmp(cmd_input5[0],"fg")==0||strcmp(cmd_input5[0],"bg")==0||strcmp(cmd_input5[0],"overkill")==0)
				{
					if(in)
						dup2(in_fd,0);
					if(out)
						dup2(out_fd,1);
					execute(cmd_input5,cmd_count5);
					
					if(in)
					{
						close(in_fd);
						freopen("/dev/tty","w",stdin);
					}
					if(out)
					{
						close(out_fd);
						freopen("/dev/tty","w",stdout);
					}
					continue;
				}

				int pid=fork();
				Child_Pid=pid;

				if(bg==0)
				{	
					if(pid==0)
		  			{	printf("in:%dout:%d",in,out);
				  				if(in)
									dup2(in_fd,0);
								if(out)
									dup2(out_fd,1);   			

		
						execute(cmd_input5,cmd_count5);	
				      			if(in)
									close(in_fd);
								if(out)
									close(out_fd);
				
	      			}
		      		else if(pid>0)
		      		{
						waitpid(-1,NULL,WUNTRACED);      			
		      		}

		      		else
					{	
						fprintf(stderr, "shell: Error Forking Child\n");
						exit(1);
					}

				}

				if(bg==1)
				{
					if(pid==0)
		  			{	

		  				if(setpgid(0,0) == 0)
					{


					}	

					else
					{
						fprintf(stderr, "shell: Unable to start as BG Process\n");
						exit(1);
					}
					
		  				 close(STDIN_FILENO);
		                 close(STDOUT_FILENO);
		                 close(STDERR_FILENO);
		                 x = open("/dev/null", O_RDWR);
		  		         dup(x);
		                 dup(x);   			
								if(in)
								{
									close(0);
									dup2(in_fd,0);
								}
								if(out)
								{
									close(1);
									dup2(out_fd,1);
								}
								if(in)
									close(in_fd);
								if(out)
									close(out_fd);

						execute(cmd_input5,cmd_count5);	

	      			}
		      		else if(pid>0)
		      		{
		      			cur_proc=cmd_input5[0];
		      			
  						add_proc(list, cur_proc, pid, 1);
		      			fprintf(stderr,"[+] %d Started\n",pid);
		      		}

		      		else
					{
						fprintf(stderr, "shell: Error Forking Child\n");
						exit(1);
					}

		    	}         		
          	}
//pipe execution
          	else
          	{          		
          		cmd_input3=back_parse(cmd_input2[0],cmd_count3);
          		
          		if(*cmd_count3>1)
    	      	{
        	  		fprintf(stderr, "bash: syntax error near unexpected token `|'\n");
        	  		continue;	
          		}
          		
          		int pipe_p1[2];
          		int pipe_p2[2];

	          	if(pipe(pipe_p1)!=0)
	          	{
	          		 perror("pipe failed");
	          	}

	          	if(pipe(pipe_p2)!=0)
	          	{
	          		 perror("pipe failed");
	          	}

	          	for(int j=0;j<*cmd_count2;j++)
	          	{

		          	cmd_input3=parse(cmd_input2[j],cmd_count3);
		        	
		        	cmd_input4=redirect(cmd_input3,cmd_count3,Pin,Pout,Pin_fd,Pout_fd,cmd_count4);
	
		          	in=*Pin;
		          	out=*Pout;
		          	in_fd=*Pin_fd;
		          	out_fd=*Pout_fd;

          			pid_t npid=fork();

		          	if(npid == -1) 
		          	{
				      perror("fork failed\n");
				    }

				    if(npid==0)
				    {
				    	if((j%2)==0)
						{
							if(j!=*cmd_count2-1)
								dup2(pipe_p1[1],1);
	
							if(j!=0)
								dup2(pipe_p2[0],0);
						}

						else
						{
							if(j!=*cmd_count2-1)
								dup2(pipe_p2[1],1);
	
							if(j!=0)
								dup2(pipe_p1[0],0);
						}

						if(strcmp(cmd_input4[0],"pinfo")==0 ||strcmp(cmd_input4[0],"echo")==0 ||strcmp(cmd_input4[0],"ls")==0 ||strcmp(cmd_input4[0],"pwd")==0 ||strcmp(cmd_input4[0],"jobs")==0||strcmp(cmd_input4[0],"kjob")==0||strcmp(cmd_input5[0],"setenv")==0||strcmp(cmd_input5[0],"unsetenv")==0||strcmp(cmd_input5[0],"fg")==0||strcmp(cmd_input5[0],"bg")==0||strcmp(cmd_input5[0],"overkill")==0)
						{
							if(in)
								dup2(in_fd,0);
							if(out)
								dup2(out_fd,1);
						
							execute(cmd_input4,cmd_count4);
							
							if(in)
							{
								close(in_fd);
								freopen("/dev/tty","w",stdin);
							}
							if(out)
							{
								close(out_fd);
								freopen("/dev/tty","w",stdout);
							}
							_exit(0);
							continue;
						}
			//////////////			//////////////
						else
						{
							if(in)
								dup2(in_fd,0);
							if(out)
								dup2(out_fd,1);
				    	
				    		if(in)
								close(in_fd);
							if(out)
								close(out_fd);
	
							execute(cmd_input4,cmd_count4);
						}
	
				    } 

					else
					{
						if((j%2)==0)
						{
							if(j!=*cmd_count2-1)
								close(pipe_p1[1]);
							if(j!=0)
								close(pipe_p2[0]);
						}
						
						else
						{
							if(j!=*cmd_count2-1)
								close(pipe_p1[0]);
							if(j!=0)
								close(pipe_p2[1]);
						}

						wait(NULL);
					}
		        }
          	}

     	 }
     }
}

char * get_input()
{
     unsigned long buf_size = 0;
     char *buff;
     getline(&buff, &buf_size, stdin);
     return buff;
}

char ** parse(char *line,int * cmd_count)
{
	int pos=0;
	char *arg;
	char **args=malloc(sizeof(char*)*(50));
	
	arg=strtok(line,delimiter);
	while(arg!=NULL)
	{
		args[pos++]=arg;
		arg=strtok(NULL,delimiter);
	}
	args[pos]=NULL;
	*cmd_count=pos;

	return args;
}

char ** semi_parse(char *line,int * cmd_count)
{
	int pos=0;
	char *arg;
	char **args=malloc(sizeof(char*)*(50));
	
	arg=strtok(line,semi_delimiter);
	while(arg!=NULL)
	{
		args[pos++]=arg;
		arg=strtok(NULL,semi_delimiter);
	}
	args[pos]=NULL;
	*cmd_count=pos;

	return args;
}

char ** back_parse(char *line,int * cmd_count)
{
	int pos=0;
	char *arg;
	char **args=malloc(sizeof(char*)*(50));
	
	arg=strtok(line,back_delimiter);
	while(arg!=NULL)
	{
		args[pos++]=arg;
		arg=strtok(NULL,back_delimiter);
	}
	args[pos]=NULL;
	*cmd_count=pos;

	return args;
}

char ** is_pipe(char *line,int * cmd_count)
{
	int pos=0;
	char *arg;
	char **args=malloc(sizeof(char*)*(50));
	
	arg=strtok(line,pipe_delimiter);
	while(arg!=NULL)
	{
		args[pos++]=arg;
		arg=strtok(NULL,pipe_delimiter);
	}
	args[pos]=NULL;
	*cmd_count=pos;

	return args;
}

void execute(char ** cmd_input,int * cmd_count)
{
	if(strcmp(cmd_input[0],"exit")==0 ||strcmp(cmd_input[0],"quit")==0)
	{
		printf("Bye!!");
		exit(0);
	}

	else if(strcmp(cmd_input[0],"cd")==0)
	{
		exec_cd(cmd_input,cmd_count);
	}

	else if(strcmp(cmd_input[0],"pwd")==0)
	{
		exec_pwd();
	}

	else if(strcmp(cmd_input[0],"echo")==0)
	{
		exec_echo(cmd_input,cmd_count);
	}

	else if(strcmp(cmd_input[0],"ls")==0)
	{
		exec_ls(cmd_input,cmd_count);
	}

	else if(strcmp(cmd_input[0],"remindme")==0)
	{
		exec_remind(cmd_input,cmd_count);
	}

	else if(strcmp(cmd_input[0],"clock")==0)
	{
		exec_clock(cmd_input,cmd_count);
	}

	else if(strcmp(cmd_input[0],"pinfo")==0)
	{
		exec_pinfo(cmd_input,cmd_count);
	}
	else if(strcmp(cmd_input[0],"setenv")==0)
	{
		exec_setenv(cmd_input,cmd_count);
	}
	else if(strcmp(cmd_input[0],"unsetenv")==0)
	{
		exec_unsetenv(cmd_input,cmd_count);
	}
	else if(strcmp(cmd_input[0],"jobs")==0)
	{
		print_jobs(list);
	}
	else if(strcmp(cmd_input[0],"kjob")==0)
	{
		exec_kjob(cmd_input,cmd_count,list);
	}
	else if(strcmp(cmd_input[0],"fg")==0)
	{
		exec_fg(cmd_input,cmd_count);
	}
	else if(strcmp(cmd_input[0],"bg")==0)
	{
		exec_bg(cmd_input,cmd_count);
	}
	else if(strcmp(cmd_input[0],"overkill")==0)
	{
		exec_kALL(list);
	}
	else
	{
		int e=execvp(cmd_input[0],cmd_input);

		if(e==-1)
		{
			perror("shell");
			exit(0);
		}	
	}
}

void exec_cd(char ** cmd_input,int * cmd_count)
{		
	if(*cmd_count==1)
		printf("least arguments passed to cd\n");
	else if(strcmp(cmd_input[1],"~")==0)
		chdir(currentDIR);
	else if(!strcmp(cmd_input[1],".")==0)
		if(chdir(cmd_input[1])!=0)
			perror("shell");
}

void exec_pwd()
{
   printf("%s\n",cwd);
}

void exec_CTRLC(int sig_num) 
{ 
	if(getpid() != Shell_Pid)
		return;
	if(Child_Pid != -1)
	{
		kill(Child_Pid, SIGINT);
	}
     signal(SIGINT, exec_CTRLC);
}

void exec_CTRLZ(int sig_num) 
{
	if(getpid() != Shell_Pid)
		return;
	if(Child_Pid != -1)
	{	
		kill(Child_Pid, SIGTTIN);	
		kill(Child_Pid, SIGTSTP);
		add_proc(list, cur_proc , Child_Pid, 0);		
		fprintf(stderr,"[+] %d \n" , Child_Pid);		
	}
    signal(SIGTSTP, exec_CTRLZ); 
} 

int exec_fg(char **cmd_input,int * cmd_count)
{
	if(*cmd_count >2)
			fprintf(stderr,"fg: Too many arguments\n");
	else if(*cmd_count < 2)
			fprintf(stderr,"fg: Too few arguments\n");
	else
	{

		int Jno = atoi(cmd_input[1]);
		jobL* job_node = getjob(list, Jno);
		if(job_node !=NULL)
		{
			if(job_node->stat)
			{
				int pid = job_node -> pid;
				kill(pid, SIGCONT);
				Child_Pid = pid;	
				strcpy(cur_proc, job_node->name);
				remove_proc(list, pid);
				waitpid(-1,NULL,WUNTRACED);
			}
			else
				fprintf(stderr,"fg: No job with such pid Running\n");		
		}
		else
		{
			fprintf(stderr,"fg: No such pid exists\n");
		}
	}

	return 1;
}

int exec_bg(char **cmd_input,int * cmd_count)
{
	if(*cmd_count >2)
			fprintf(stderr,"bg: Too many arguments\n");
	else if(*cmd_count <2)
			fprintf(stderr,"bg: Too few arguments\n");
	else
	{

		int Jno = atoi(cmd_input[1]);
		jobL* job_node = getjob(list, Jno);
		if(job_node !=NULL)
		{
			if(job_node->stat)
				fprintf(stderr, "job Running already\n");
			else
			{
				int pid = job_node -> pid;
				kill(pid, SIGTTIN);
				kill(pid, SIGCONT);
				change_list(list,pid,1);
			}
		}
		else
		{
			fprintf(stderr,"bg: No such pid exists\n");
		}

	}

	return 1;
}