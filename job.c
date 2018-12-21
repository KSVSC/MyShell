
struct node
{
	char name[1000];
	pid_t pid;
	int stat;
	struct node *next;
};

typedef struct node jobL;

jobL* getjob(jobL* list, int pid);
int kill_proc(jobL* list, pid_t pid, int sig);

void add_proc(jobL* list, char name[], pid_t pid , int stat)
{
	jobL* head = list;
	jobL* temp;
	while(head)
	{
		temp = head->next;
		if(temp == NULL)
		{
			temp  = (jobL*)malloc(sizeof(jobL));
			strcpy(temp->name, name);
			temp->pid = pid;
			temp->next = NULL;
			temp->stat = stat;
			head->next = temp;
			break;
		}
		head = temp;
	}
}

void Jobs_exited(jobL* list)
{
	jobL* head = list;
	jobL* temp;
	while(head)
	{
		temp = head->next;
		if(temp != NULL)
		{
			pid_t return_pid = waitpid(temp->pid, NULL, WNOHANG);
			if(return_pid == temp->pid)
			{
				fprintf(stderr,"[-] Done %d %s\n", temp->pid, temp->name);
				head-> next = temp->next;
				free(temp);
			}
		}
		head = head->next;
	}
}

void print_jobs(jobL* list)
{
	jobL* temp = list->next;
	int count  = 0;
	while(temp)
	{
		count += 1;
		printf("[%d] \t %s \t %s \t [%d]\n",count, (temp->stat == 0)?"Stopped":"Running", temp->name, temp->pid);
		temp = temp->next;
	}
	if(count==0)
		printf("no jobs found\n");
}

int exec_kjob(char **cmd_input,int * cmd_count,jobL * list)
{
	if(*cmd_count >3)
			fprintf(stderr,"kjob: Too many arguments\n");
	else if(*cmd_count < 3)
			fprintf(stderr,"kjob: Too few arguments\n\n");
	else
	{

		int Jno = atoi(cmd_input[1]);
		int sig = atoi(cmd_input[2]);
		jobL* job_node = getjob(list, Jno);
		if(job_node !=NULL)
		{
			int pid = job_node -> pid;
			kill(pid,sig);
			if(sig!=9){
				job_node -> stat = 0;
			}
		}
		else
		{
			fprintf(stderr,"kjob: No such pid exists\n" );
		}
		sleep(1);
	}
	return 1;
}

jobL* getjob(jobL* list, int pid)
{
	jobL* head = list;
	jobL* temp;
	int count = 0;
	while(head)
	{
		temp = head->next;
		count += 1;
		if(temp!= NULL)
		{
			if(count == pid)
			{
				return temp;
			}
		}
		head = temp;
	}
}

void remove_proc(jobL* list, pid_t pid)
{
	jobL* head = list;
	jobL* temp;
	while(head)
	{
		temp = head->next;
		if(temp != NULL)
		{
			if(temp->pid == pid)
			{
				head-> next = temp->next;
				free(temp);
				break;
			}
		}
		head = temp;
	}
}

void change_list(jobL* list, pid_t pid, int stat)
{
	jobL* head = list;
	jobL* temp;
	int count = 0;
	while(head)
	{
		temp = head->next;
		count += 1;
		if(temp != NULL)
		{
			if(temp->pid == pid)
			{
				temp->stat = stat;
				break;
			}
		}
		head = temp;
	}
}

void exec_kALL(jobL* list)
{
	jobL* head = list->next;
	int copy;
	while(head)
	{	
		copy = head->pid;
		head = head->next;
		kill_proc(list, copy, SIGINT);
	}
	sleep(1);
}

int kill_proc(jobL* list, pid_t pid, int sig)
{
	jobL* head = list;
	jobL* temp;
	while(head)
	{
		temp = head->next;
		if(temp != NULL)
		{
			if(temp->pid == pid)
			{
				head-> next = temp->next;
				kill(pid, 17);
				kill(pid, sig);
				fprintf(stderr,"Killed process -[%d]\n" , pid);
				free(temp);
				return 1;
				break;
			}
		}
		head = temp;
	}
	return 0;
}
