#define _CRT_NONSTDC_NO_WARNINGS 1
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>

#define MAX_COMMAND 30

typedef struct COMMAND_STRUCT {
	int				BankID;
	char			*strBankID;
	int				CommandType;
	double			Money;
} CommandStruct;

void comand_line_to_strings(char** command_strings, char* command_line, int position)
{
	int i = 0;
	char* temp_char = NULL; command_strings[position] = NULL;
	int copy_length = 0;
	if (command_line[strlen(command_line) - 1] == '\n')
		command_line[strlen(command_line) - 1] = '\0';

	temp_char = strchr(command_line, ' ');
	if (temp_char != NULL)
	{
		copy_length = strlen(command_line) - strlen(temp_char);
		command_strings[position] = (char*)malloc(sizeof(char) * (copy_length + 1));
		for (i = 0; i < copy_length; ++i)
		{
			command_strings[position][i] = command_line[i];
		}
		command_strings[position][copy_length] = '\0';
		comand_line_to_strings(command_strings, temp_char + 1, position + 1);
	}
	else
	{
		copy_length = strlen(command_line);
		command_strings[position] = (char*)malloc(sizeof(char) * (copy_length + 1));
		for (i = 0; i < copy_length; ++i)
		{
			command_strings[position][i] = command_line[i];
		}
		command_strings[position][copy_length] = '\0';
	}
}

CommandStruct* command_to_struct(FILE *infile)
{
	char *possibleCommands[5] = { "CreateAccount", "CloseAccoung", "PrintBalances", "Deposit", "Withdrawal" };
	char *command_line = NULL, *ptr = NULL;
	char** command_strings = NULL;
	int CommandType = 0;
	CommandStruct* command = (CommandStruct*)malloc(sizeof(CommandStruct));
	command->strBankID = NULL;
	command_line = (char*)malloc(sizeof(char) * MAX_COMMAND);
	command_strings = (char**)malloc(sizeof(char*) * 3);
	if (command_line == NULL || command_strings == NULL)
		exit(1);

	if (fgets(command_line, MAX_COMMAND, infile) == NULL)
		return NULL;
	comand_line_to_strings(command_strings, command_line, 0);

	for (CommandType = 0; CommandType < 5; ++CommandType)
	{
		if (strstr(command_strings[0], possibleCommands[CommandType]) != NULL)
			break;
	}

	switch (CommandType)
	{
	case 0:
	case 3:
	case 4:
		command->strBankID = (char*)malloc(sizeof(char)* strlen(command_strings[0] + 1));
		strcpy(command->strBankID, command_strings[0]);
		command->BankID = strtol(command_strings[1],&ptr,10);
		command->CommandType = CommandType;
		command->Money = atof(command_strings[2]);
		break;

	case 1:
		command->strBankID = (char*)malloc(sizeof(char)* strlen(command_strings[0] + 1));
		strcpy(command->strBankID, command_strings[0]);
		command->BankID = strtol(command_strings[1], &ptr, 10);
		command->CommandType = CommandType;
		command->Money = 0;
		break;

	case 2:
		command->strBankID = (char*)malloc(sizeof(char)* strlen(command_strings[0] + 1));
		strcpy(command->strBankID, command_strings[0]);
		command->BankID = 0;
		command->CommandType = CommandType;
		command->Money = 0;
		break;

	deafult:
		command->BankID = 0;
		command->CommandType = -1;
		command->Money = 0;
	}
	
	return command;
}

HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE StartAddress,
	LPVOID ParameterPtr,
	LPDWORD ThreadIdPtr)
{
	return CreateThread(
		NULL,            /*  default security attributes */
		0,                /*  use default stack size */
		StartAddress,    /*  thread function */
		ParameterPtr,    /*  argument to thread function */
		0,                /*  use default creation flags */
		ThreadIdPtr);    /*  returns the thread identifier */
}

void MainThreadsManagment(FILE* infile)
{
	//HANDLE				ThreadHandles[NUM_OF_THREADS];   /* An array of thread handles */
	//DWORD				ThreadIDs[NUM_OF_THREADS];        /* An array of threadIDs */
	//DWORD				exitcode;
	int					i = 0;
	char*				output_text = NULL;
	CommandStruct		*command;

	while ((command = command_to_struct(infile)) != NULL)
	{
		printf("account num is: %d\n", command->BankID);
		printf("command is: %d\n", command->CommandType);
		printf("command string is: %s\n", command->strBankID);
		printf("command money is: %f\n", command->Money);
	}

	/*ThreadHandles[0] = CreateThreadSimple(
		(LPTHREAD_START_ROUTINE)check_five_letters,
		output,
		&ThreadIDs[0]);

	ThreadHandles[1] = CreateThreadSimple(
		(LPTHREAD_START_ROUTINE)getExt,
		output,
		&ThreadIDs[1]);

	ThreadHandles[2] = CreateThreadSimple(
		(LPTHREAD_START_ROUTINE)getSize,
		output,
		&ThreadIDs[2]);

	ThreadHandles[3] = CreateThreadSimple(
		(LPTHREAD_START_ROUTINE)getFileTiming,
		output,
		&ThreadIDs[3]);


	WaitForMultipleObjects(
		NUM_OF_THREADS,
		ThreadHandles,
		TRUE,
		INFINITE);

	output_text = Create_output_text(output_text, output);
	while (output_text[i] != '\0') { // writing to the output file
		fputc(output_text[i], files->outfile);
		i++;
	}

	fclose(files->outfile);

	for (i = 0; i < NUM_OF_THREADS; i++)
	{
		GetExitCodeThread(ThreadHandles[i], &exitcode);
		printf("Thread number %d returned exit code %d\n", i, exitcode);
		CloseHandle(ThreadHandles[i]);
	}

	free(output_text);
	Free_Infiles(files);
	Free_Trdout(output);*/
}

void main(int argc, char* argv[])
{
	HANDLE				MainThreadHandle;   /* An array of thread handles */
	DWORD				MainThreadID;        /* An array of threadIDs */
	DWORD				exitcode;
	FILE				*infile = NULL, *outfile = NULL;
	char				*output_file_name = NULL;
	char				*text = NULL;

	if (argc != 2) {
		printf("not enough input arguments\n");
		exit(1);
	}

	infile = fopen(argv[1], "r");

	if (infile == NULL)
	{
		printf("Unable to read input file\n");
		exit(1);
	} 
	MainThreadHandle = CreateThreadSimple(
		(LPTHREAD_START_ROUTINE)MainThreadsManagment,  /*  thread function */
		infile,                            /*  argument to thread function */
		&MainThreadID);                    /*  returns the thread identifier */

	WaitForSingleObject(MainThreadHandle, INFINITE);
	Sleep(10);

	GetExitCodeThread(MainThreadHandle, &exitcode);
	printf("Main Thread returned exit code %d\n", exitcode);
	CloseHandle(MainThreadHandle);

}