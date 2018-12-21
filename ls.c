
void exec_ls(char ** cmd_input,int * cmd_count)
{
	char cd[1000];
	struct passwd *pwd;
	struct group   *grp;
	struct tm      *time;
	char  datestring[256];

	DIR *dir;
	struct dirent *files;
	struct stat fStat;

	char buf[1000];

	if(* cmd_count==1)
	{
		getcwd(cd,1000);
		dir=opendir(cd);
		while((files=readdir(dir))!=NULL)
		{
			if(!(strcmp(files->d_name,".")==0) && !(strcmp(files->d_name,"..")==0))
				printf("%s\t",files->d_name);
		}
		printf("\n");
		closedir(dir);
	}
	else if(strcmp(cmd_input[1],"-l")==0)
	{
		getcwd(cd,1000);
		if(*cmd_count==3)
		{
			strcat(cd,"/");
			strcat(cd,cmd_input[2]);			
		}

		dir=opendir(cd);
		if(dir==NULL)
		{
			printf("cannot access %s :no such directory\n",cmd_input[2]);
		}
		else
		{	
			while((files=readdir(dir))!=NULL)
			{
				if(!(strcmp(files->d_name,".")==0) && !(strcmp(files->d_name,"..")==0))
				{
				if (stat(files->d_name, &fStat) == -1)
		                        continue;
					printf( (S_ISDIR(fStat.st_mode)) ? "d" : "-");
					printf( (fStat.st_mode & S_IRUSR) ? "r" : "-");
					printf( (fStat.st_mode & S_IWUSR) ? "w" : "-");
					printf( (fStat.st_mode & S_IXUSR) ? "x" : "-");
					printf( (fStat.st_mode & S_IRGRP) ? "r" : "-");
					printf( (fStat.st_mode & S_IWGRP) ? "w" : "-");
					printf( (fStat.st_mode & S_IXGRP) ? "x" : "-");
					printf( (fStat.st_mode & S_IROTH) ? "r" : "-");
					printf( (fStat.st_mode & S_IWOTH) ? "w" : "-");
					printf( (fStat.st_mode & S_IXOTH) ? "x" : "-");
					    printf(" %ld",fStat.st_nlink);
					pwd = getpwuid(fStat.st_uid);
					if(pwd!=NULL)
					{
					    printf(" %s", pwd->pw_name);
					}
					else
				    printf(" %d", fStat.st_uid);
				if ((grp = getgrgid(fStat.st_gid)) != NULL)
				    printf(" %s", grp->gr_name);
				else
				   printf(" %d", fStat.st_gid);
				printf(" %ld",fStat.st_size);

				time = localtime(&fStat.st_mtime);
				strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), time);
					printf(" %s",datestring);

					printf(" %s \n",files->d_name);
				}
			}
		}
	}

	else if(strcmp(cmd_input[1],"-a") == 0)
			{
                getcwd(cd,1000);
                if(*cmd_count==3)
				{
					strcat(cd,"/");
					strcat(cd,cmd_input[2]);			
				}
				dir = opendir(cd);
				if(dir==NULL)
				{
					printf("cannot access %s :no such directory\n",cmd_input[2]);
				}
				else
				{
					while((files = readdir(dir)) != NULL)
					{
							printf("%s\t",files->d_name);
					}
					printf("\n");
				}
				closedir(dir);
           	}
	else if(strcmp(cmd_input[1],"-al")==0 || strcmp(cmd_input[1],"-la")==0 || strcmp(cmd_input[1],"-l-a")==0 || strcmp(cmd_input[1],"-a-l")==0)
	{
		getcwd(cd,1000);
		if(*cmd_count==3)
		{
			strcat(cd,"/");
			strcat(cd,cmd_input[2]);			
		}
		dir=opendir(cd);
		if(dir==NULL)
		{
			printf("cannot access %s :no such directory\n",cmd_input[2]);
		}
		else
		{
			while((files=readdir(dir))!=NULL)
			{
		        if (stat(files->d_name, &fStat) == -1)
	                            continue;
				printf( (S_ISDIR(fStat.st_mode)) ? "d" : "-");
				printf( (fStat.st_mode & S_IRUSR) ? "r" : "-");
				printf( (fStat.st_mode & S_IWUSR) ? "w" : "-");
				printf( (fStat.st_mode & S_IXUSR) ? "x" : "-");
				printf( (fStat.st_mode & S_IRGRP) ? "r" : "-");
				printf( (fStat.st_mode & S_IWGRP) ? "w" : "-");
				printf( (fStat.st_mode & S_IXGRP) ? "x" : "-");
				printf( (fStat.st_mode & S_IROTH) ? "r" : "-");
				printf( (fStat.st_mode & S_IWOTH) ? "w" : "-");
				printf( (fStat.st_mode & S_IXOTH) ? "x" : "-");
				    printf(" %ld",fStat.st_nlink);
				pwd = getpwuid(fStat.st_uid);
				if(pwd!=NULL)
				{
				    printf(" %s", pwd->pw_name);
				}
				else
		            printf(" %d", fStat.st_uid);
		        if ((grp = getgrgid(fStat.st_gid)) != NULL)
		            printf(" %s", grp->gr_name);
		        else
		           printf(" %d", fStat.st_gid);
		        printf(" %ld",fStat.st_size);

		        time = localtime(&fStat.st_mtime);
		        strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), time);
				printf(" %s",datestring);
		        
				printf(" %s \n",files->d_name);
			}
		}
	}

	else
	{
		getcwd(cd,1000);
		strcat(cd,"/");
		strcat(cd,cmd_input[1]);
		dir=opendir(cd);
		if(dir==NULL)
		{
			printf("cannot access %s :no such directory\n",cmd_input[2]);
		}
		else
		{
			while((files=readdir(dir))!=NULL)
			{
				if(!(strcmp(files->d_name,".")==0) && !(strcmp(files->d_name,"..")==0))
					printf("%s\t",files->d_name);
			}
			printf("\n");
		}
		closedir(dir);
	}

}
