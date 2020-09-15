#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <direct.h> //getcwd -�Լ��� ������ ��밡���ѵ�
#include <Windows.h>
#include <Psapi.h> //process���� ���ϱ� ����
#include <processthreadsapi.h> //cpuload����.
#include <io.h> //ls�� ��� - ������ ���͸��� ������ ������ ���� �������� _finddata_t����ü ������ؼ� ���

#define MAX_PATH 260

void print_help();
void print_date();
char* change_dir(int argc, char** argv, char tmpDir[]);

void print_processes();
void print_cpuLoad();
DWORD subtract_times(FILETIME prev, FILETIME now);

void print_ls(char* path);

void cat(int argc, char** argv);
void my_mv(int argc, char** argv);
void my_rm(int argc, char** argv);
void execute(int argc, char** argv);

char* full_directory(char dir[]);

char homeDir[MAX_PATH] = { 0, };

int main()
{
	printf("######################## MY CLI ########################\n");

	char curDir[MAX_PATH] = { 0, };
	char tmpDir[MAX_PATH] = { 0, };
	char command[100]; // ��ɾ�
	int argc = 0; // ��ɾ�+�ɼ� ����

	_getcwd(homeDir, MAX_PATH);

	while (1)
	{
		argc = 0;
		_getcwd(curDir, MAX_PATH);

		printf("%s>", curDir);
		gets(command);

		//��ɾ� ���� ����
		int i = 0;
		if (command[0] != ' '&&command[0] != '\t')
			argc++;
		while (command[i])
		{
			if ((command[i] == ' ' || command[i] == '\t') && (command[i + 1] != ' ' && command[i + 1] != '\t'&&command[i + 1]))
				argc++;
			i++;
		}

		//argc���� ��ŭ �����Ҵ� �ޱ�
		char** argv = (char*)malloc(sizeof(char*)*argc);
		for (i = 0; i < argc; i++)
			argv[i] = (char*)malloc(sizeof(char) * 100); //cd "~"���⼭ ������������ؼ� ũ�⸦ 100���� ����

		 //argv�� ä���
		int j = 0, k = 0, tmp1, tmp2;
		if (command[0] == ' ' || command[0] == '\t')
			k = -1;
		for (i = 0; command[i]; i++)
		{
			if ((command[i] == ' ' || command[i] == '\t') && (command[i + 1] != ' ' && command[i + 1] != '\t'&&command[i + 1]))
			{
				argv[tmp1][tmp2] = '\0';
				j = 0;
				k++;
			}
			else if (command[i] != ' '&&command[i] != '\t')
			{
				argv[k][j] = command[i];
				j++;
				tmp1 = k;
				tmp2 = j;
			}
		}
		argv[tmp1][tmp2] = '\0';

		if (!strcmp(argv[0], "exit") || !strcmp(argv[0], "quit") || !strcmp(argv[0], "bye"))
			return 0;
		else if (!strcmp(argv[0], "help"))
		{
			if (argc > 1)
				printf("help�� ġ����.\n");
			else
				print_help();
		}
		else if (!strcmp(argv[0], "date"))
		{
			if (argc > 1)
				printf("help�� ġ����.\n");
			else
				print_date();
		}
		else if (!strcmp(argv[0], "pwd"))
		{
			if (argc > 1)
				printf("pwd�� ġ����.\n");
			else
				printf("%s\n", curDir);
		}
		else if (!strcmp(argv[0], "pro"))
		{
			if (argc > 1)
				printf("pro�� ġ����.\n");
			else
				print_processes();
		}
		else if (!strcmp(argv[0], "cpu"))
		{
			if (argc > 1)
				printf("cpu�� ġ����.\n");
			else
				print_cpuLoad();
		}
		else if (!strcmp(argv[0], "cd"))
		{
			if (argc > 2)
				printf("�ɼǰ����� 1���� �Է��ϼ���.\n");
			else
				strcpy(tmpDir, change_dir(argc, argv, tmpDir));
		}
		else if (!strcmp(argv[0], "ls"))
		{
			if (argc > 1)
				printf("ls�� ġ����.\n");
			else
				print_ls(curDir);
		}
		else if (!strcmp(argv[0], "cat"))
		{
			if (argc == 1)
				printf("����� ������ 1���̻� �Է��ϼ���.\n");
			else
				cat(argc, argv);
		}
		else if (!strcmp(argv[0], "mv"))
		{
			if (argc !=3)
				printf("�����̳� ���丮 ������ 2���� �ʿ��մϴ�.\n");
			else
				my_mv(argc, argv);
		}
		else if (!strcmp(argv[0], "rm"))
		{
			if (argc > 2)
				printf("������ 1���� �����մϴ�.\n");
			else
				my_rm(argc, argv);
		}
		else if (!strcmp(argv[0], "exe"))
		{
			if (argc > 2)
				printf("������ 1���� �����մϴ�.\n");
			else
				execute(argc, argv);
		}
		else
			printf("��ɾ �߸� �Է��ϼ̽��ϴ�.\n");

		//��ɾ� �����Ҵ� ����
		for (i = 0; i < argc; i++)
			free(argv[i]);
		free(argv);
	}
	return 0;
}
void print_help()
{
	printf("quit\t\t\t:quit my CLI program\n");
	printf("exit\t\t\t:quit my CLI program\n");
	printf("bye\t\t\t:quit my CLI program\n");

	printf("help\t\t\t:display all commands supported and their usage.\n");

	printf("date\t\t\t:display current time\n");
	printf("pwd\t\t\t:display the path of working directory\n");
	printf("pro\t\t\t:display the number of running processes.\n");
	printf("cpu\t\t\t:display current cpu load in the form of percentage.\n");

	printf("cd [path]\t\t:change a working directory to another. [��� ��밡��] : - , ~ , .. , .���� ���\n");
	printf("ls\t\t\t:displayall the files and directories in the working directory.\n");

	printf("cat [option] [file]\t:display the contents of a specified text file.[option] : -n\n");
	printf("mv [f/d] [f/d]\t\t:change the name of a file or directory to another.\n");
	printf("rm [f/d]\t\t:delete a specified file or dirextory.\n");
	printf("exe [��������]\t\t:execute a program.\n");

}
void print_date()
{
	/*���� 2���� ����� �� �״�� �����*/
	SYSTEMTIME local_time;
	GetLocalTime(&local_time);

	printf("%04d_%02d_%02d %02d:%02d:%02d\n", local_time.wYear, local_time.wMonth, local_time.wDay, local_time.wHour, local_time.wMinute, local_time.wSecond);
}
void print_ls(char* path)
{
	typedef struct _finddata_t FILE_SEARCH; //���� �Ӽ�, �̸�, ��¥ ũ����� ����������. 

	printf("List of files.\n");
	printf("----- --------- -----------------------------\n");

	long h_file;  //handle����
	char search_Path[100];

	FILE_SEARCH file_search;

	//��� ���� ã���� ����
	strcpy(search_Path, path);
	strcat(search_Path, "/*.*");

	//_findnext�� -1�� return�Ҷ����� �ݺ��� ���ǿ� �´� ��� ������ ã��.
	//findfirst�Լ��� ù��° �˻��� �ѵڿ� _findnext�Լ��� ���ǿ� �´� ���������� ã��.
	if ((h_file = _findfirst(search_Path, &file_search)) == -1L)
	{
		printf("No files in current directory!\n");
	}
	else
	{
		do
		{
			char buffer[30];
			printf((file_search.attrib & _A_RDONLY) ? "R" : "-");
			printf((file_search.attrib & _A_SYSTEM) ? "S" : "-");
			printf((file_search.attrib & _A_HIDDEN) ? "H" : "-");
			printf((file_search.attrib & _A_ARCH) ? "A" : "-");
			printf((file_search.attrib & _A_SUBDIR) ? "D" : "-");
			printf("%9ld", file_search.size);
			printf("\t%s\n", file_search.name);
		} while (_findnext(h_file, &file_search) == 0);
		_findclose(h_file);
	}

}
void print_processes()
{
	//2��° ���� �״�� ���
	DWORD aProcesses[1024], cbNeeded, Cprocesses;

	EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);
	Cprocesses = cbNeeded / sizeof(DWORD);

	printf("number of running processes:%4d\n", Cprocesses);
}
void print_cpuLoad()
{
	//2��° ���� �ڵ� �״�� �����
	FILETIME idle_time, kernel_time, user_time, prev_idle_time, prev_kernel_time, prev_user_time;
	DWORD idle, kernel, user, work;
	double cpu_usage_all;
	/*work = kernel + user*/
	/*5�� ���� ���� ��� �� ���� cpu ��뷮���� �Ѵ�.*/

	cpu_usage_all = 0X00f; /*5�� �� ���� ���ϱ� ���� ����(sum)*/

	GetSystemTimes(&prev_idle_time, &prev_kernel_time, &prev_user_time);
	Sleep(100);
	for (int i = 0; i < 5; i++)
	{
		GetSystemTimes(&idle_time, &kernel_time, &user_time);

		idle = subtract_times(prev_idle_time, idle_time);
		kernel = subtract_times(prev_kernel_time, kernel_time);
		user = subtract_times(prev_user_time, user_time);
		work = kernel + user;

		cpu_usage_all += (work - idle) * 100 / (double)work;

		prev_idle_time = idle_time;
		prev_user_time = user_time;
		prev_kernel_time = kernel_time;

		Sleep(100);
	}

	printf("Current CPU Load : %5.2lf\n", cpu_usage_all / 5.0);
}
DWORD subtract_times(FILETIME prev, FILETIME now)
{
	DWORD now_t = ((DWORDLONG)now.dwHighDateTime << 32 | now.dwLowDateTime);
	DWORD prev_t = ((DWORDLONG)prev.dwHighDateTime << 32 | prev.dwLowDateTime);

	return now_t - prev_t;
}
char* change_dir(int argc, char** argv, char tmpDir[])
{
	char curDir[MAX_PATH] = { 0, };
	char changeDir[MAX_PATH] = { 0, };
	int length, i;
	_getcwd(curDir, MAX_PATH);
	strcpy(changeDir, curDir);

	if (argc == 1)
		return curDir;
	else if (!strcmp(argv[1], "."))
		return curDir;
	else if (!strcmp(argv[1], "~"))
		strcpy(changeDir, homeDir);
	else if (!strcmp(argv[1], ".."))
	{
		for (length = 0; curDir[length]; length++); //���� ���丮 ���̱���
		for (i = length - 1; i >= 0; i--)
		{
			if (curDir[i] == '\\')
				break;
		}
		changeDir[i] = '\0';
	}
	else if (!strcmp(argv[1], "-"))
	{
		if (tmpDir[0] == 0)
			return curDir; //������丮 �״��
		strcpy(changeDir, tmpDir);
	}
	else if (argv[1][1] ==':')
	{
		/*for (length = 0; argv[1][length]; length++);
		for (i = 0; i < length - 2; i++)
			argv[1][i] = argv[1][i + 1];
		argv[1][i] = '\0'; : ��� �����ÿ� "C:\~"�̷������� ū����ǥ ���δٰ� �߸������ؼ� ���� �ڵ�*/
		strcpy(changeDir, argv[1]);
	}
	else
	{
		strcpy(changeDir,full_directory(argv[1]));
	}
	//./'���'�� /'���'�� �����غ�

	if (_chdir(changeDir))printf("��ΰ� �����ϴ�.\n");

	//���� ���丮 ����ؼ� -������ return���� �̷��� ����
	return curDir;
}
void cat(int argc, char** argv)
{	
	FILE* fp;
	char receiveStr[1000]; //���� ��½� ����ٰ� ��������
	char dir[MAX_PATH] = { 0, };
	int i = 1,j=0;
	
	if (!strcmp(argv[1], "-n"))
	{
		for (i=2; i < argc; i++)
		{
			fp = fopen(argv[i], "r");
			if (!fp)
			{
				printf("������ �����ϴ�.\n");
				return;
			}

			while (!feof(fp))
			{
				fgets(receiveStr, sizeof(receiveStr), fp);
				printf("%d. %s", j,receiveStr);
				j++;
			}
			printf("\n");
			fclose(fp);
		}
	}
	//���� ���� ��°���
	else {
		for (i = 1; i < argc; i++)
		{
			fp = fopen(argv[i], "r");
			if (!fp)
			{
				printf("������ �����ϴ�.\n");
				return;
			}

			while (!feof(fp))
			{
				fgets(receiveStr, sizeof(receiveStr), fp);
				printf("%s", receiveStr);
			}
			printf("\n");
			fclose(fp);
		}
	}

}
void my_mv(int argc, char** argv)
{
	for (int i = 1; i < argc; i++)
	{
		strcpy(argv[i], full_directory(argv[i]));
		printf("%s\n", argv[i]);
	}

	typedef struct _finddata_t FILE_SEARCH; //���� �Ӽ�, �̸�, ��¥ ũ����� ����������. 

	long h_file1,h_file;  //handle����
	char search_Path1[100];

	FILE_SEARCH file_search1;

	strcpy(search_Path1, argv[1]);

	if (((h_file1 = _findfirst(search_Path1, &file_search1))==-1L))
	{
		printf("No files in current directory!\n");
	}
	else
	{
		//���� directory���
		if ((file_search1.attrib & _A_SUBDIR) )
		{
			rename(argv[1], argv[2]);
		}
		else
		{
			//2�� ��� ���ٸ� ��������
			int length = strlen(argv[2]);
			int i;
			char tmp[MAX_PATH];
			strcpy(tmp, argv[2]);

			for (i = length - 1; i > 0; i--)
			{
				if (tmp[i] == '\\') break;
			}
			tmp[i] = '\0';

			_mkdir(tmp);

			FILE* one = fopen(argv[1], "r");
			FILE* two = fopen(argv[2], "wt");
			char receiveStr[1000];

			if (!one)
			{
				printf("������ ��γ��� �����ϴ�.\n");
				return;
			}
			if (!two)
			{
				printf("���������� X\n");
				return;
			}

			while (!feof(one))
			{
				fgets(receiveStr, sizeof(receiveStr), one);
				fputs(receiveStr, two);
			}

			fclose(one);
			remove(argv[1]);
			fclose(two);
		}
	}
}
void my_rm(int argc, char** argv)
{
	strcpy(argv[1], full_directory(argv[1]));
	
	int flag = 0; //���� �ȵȻ���
	if(!_rmdir(argv[1])) flag = 1; //���� �Ȼ���
	if(!remove(argv[1]))flag = 1;//�����Ȼ���
	if (!flag)
		printf("�ش� �����̳� ���丮�� �����ϴ�.\n");
}
void execute(int argc, char** argv)
{
	strcpy(argv[1], full_directory(argv[1]));

	system(argv[1]);
}

// 'route' ./'route' /'route' ~/'route' �̷������� �Ǿ��ִ� ��� ��ġ��
char* full_directory(char dir[])
{
	char fullDir[MAX_PATH] = { 0, };
	int length = strlen(dir);

	_getcwd(fullDir, MAX_PATH);
	if (!strcmp(dir, "..") || !strcmp(dir, "-"))
		return dir;
	if (!strcmp(dir, "."))
		return fullDir;
	if (dir[1] == ':')
		return dir;
	if (!strcmp(dir, "~"))
		return homeDir;

	if (dir[0] == '\\')
		strcat(fullDir, dir);
	else if (dir[0] == '.'&&dir[1] == '\\')
	{
		dir = strtok(dir, ".");
		strcat(fullDir, dir);
	}
	else if (dir[0] == '~'&&dir[1] == '\\')
	{
		dir = strtok(dir, "~");
		strcpy(fullDir, homeDir);
		strcat(fullDir, dir);
	}
	else
	{
		strcat(fullDir, "\\");
		strcat(fullDir, dir);
	}
	return fullDir;
}

/*
ls

���ϸ�� ������ ����
real ���� : https://mobilecv.tistory.com/73
ó���� ���� : https://sks3297.tistory.com/entry/C%EC%96%B8%EC%96%B4-%ED%8C%8C%EC%9D%BC%EB%AA%A9%EB%A1%9D-%EA%B0%80%EC%A0%B8%EC%98%A4%EA%B8%B0
*/