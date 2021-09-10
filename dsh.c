#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/wait.h>

#define VERSION "dsh 0.1"

#include "config.h"
#include "cd.h"
#include "exit.h"
#include "unset.h"

// struct for calling shell  methods
const static struct
{
	const char *argv;
	void (*method)(int, char**);
}

// map of shell methods
methodMap[] =
{
	{"cd", dshcd},
	{"exit", dshexit},
	{"unset", dshunset},
};

int runShellMethod(int argc, char **argv)
{
	// loop through method map
	for (int i = 0; i < (sizeof(methodMap) / sizeof(methodMap[0])); i++)
	{
		// if method
		if (!strcmp(methodMap[i].argv, argv[0]) && methodMap[i].method)
		{
			// call method and return
	        methodMap[i].method(argc, argv);
			return 1;
		}
	}

	// otherwise return
	return 0;
}

int getHistLen()
{
	// int to store history length
	int histLen = DEFAULT_HIST_LEN;

	// char to store history length env var
	char *histLenString = getenv(HIST_LEN_VAR);

	// set length as env variable if it exists
	if (histLenString)
	{
		histLen = atoi(histLenString);
	}

	return histLen;
}

char *getPrompt()
{
	// store prompt from env variable
	char *prompt = getenv(PROMPT_VAR);

	// use default prompt if doesn't exist
	if (!prompt)
	{
		prompt = DEFAULT_PROMPT;
	}

	return prompt;
}

void exec(char *input)
{
	// int to loop through input string
	int argc = 0;

	// int to store total quotation marks
	int quotes = 0;

	// int to store total quoted strings
	int totalQuotes = 0;

	// array to store argument list
	char *argv[ARG_LEN];

	// array to store quoted arguments
	char argvConcat[ARG_LEN][ARG_LEN];

	// char to split input string at
	char delim[] = " ";

	// first split
	char *ptr = strtok(input, delim);

	// buffer to store concat strings
	char buffer[ARG_LEN] = "";

	// loop through the string
	while (ptr != NULL)
	{
		// if quotation marks
		for (int i = 0; i < strlen(ptr); i++)
		{
			// if char is quotation mark
			if (ptr[i] == '"')
			{
				// increase quote counter
				quotes++;

				// remove quote
				memmove(&ptr[i], &ptr[i + 1], strlen(ptr) - i);

				// decrease index
				i--;
			}
		}

		// open quote
		if (quotes == 1)
		{
			// add split to buffer
			strcat(buffer, ptr);

			// add space to buffer
			strcat(buffer, " ");

			// continue loop
			ptr = strtok(NULL, delim);
		}

		// closed quote
		else if (quotes == 2)
		{
			// add split to buffer
			strcat(buffer, ptr);

			strcpy(argvConcat[totalQuotes], buffer);

			// add quoted text to array
			argv[argc++] = argvConcat[totalQuotes];

			// reset quote counter
			quotes = 0;
			totalQuotes++;

			// clear the buffer
			strcpy(buffer, "");

			// split it again
			ptr = strtok(NULL, delim);
		}

		// no quotes
		else
		{
			// add current split to array
			argv[argc++] = ptr;

			// split it again
			ptr = strtok(NULL, delim);
		}
	}

	// check if env variable
	if (strchr(argv[0], '=') != NULL)
	{
		// array to store variable
		char *envvar[1];

		// get the env variable name
		char *token = strtok(argv[0], "=");
		envvar[0] = token;

		// get the env variable value
		token = strtok(NULL, "=");
		envvar[1] = token;

		// set env variable
		setenv(envvar[0], envvar[1], -1);

		return;	
	}

	// check for shell methods
	if (runShellMethod(argc, argv))
	{
		// run shell method instead
		return;
	}

	// add null to end of arguments list
	//strcpy(argv[argc++], NULL);
	argv[argc++] = NULL;

	// fork process
	pid_t pid = fork();

	// child process
	if (pid == 0)
	{
		// run the program
		execvp(argv[0], argv);

		// bye
		exit(127);
	}

	// wait for the program to end
	else
	{
		waitpid(pid, 0, 0);
	}
}

void updateHist(char *input)
{
	// variable to store history file
	FILE *histFile;

	// string to store history file path
	char filePath[DIR_LEN];

	// get history file path
	strcat(strcpy(filePath, getenv("HOME")), "/" HIST_FILE_NAME);

	// open the history file to read and append
	histFile = fopen(filePath, "a+");

	// append history to history file
	fprintf(histFile, "%s\n", input);

	// reset to beginning of history file
	rewind(histFile);

	// int to store total lines in history file
	int totalLines = 0;

	// int to store char in history file
	int currentChar;

	// get first char from history file
	currentChar = fgetc(histFile);

	// loop through history file
	while (currentChar != EOF)
	{
		// increase counter if linebreak
		if (currentChar == '\n')
		{
			totalLines++;
		}

		// go to next char in history file
		currentChar = fgetc(histFile);
	}

	// store history length as int
	int histLen = getHistLen();

	// if history file length above limit
	if (totalLines > histLen)
	{
		// variable to store temporary history file
		FILE *tempHistFile;

		// string to store temporary history file path
		char tempFilePath[DIR_LEN] = TEMP_FILE_DIR "/" TEMP_HIST_FILE_NAME;

		// open the temp history file to append
		tempHistFile = fopen(tempFilePath, "w");

		// int to store current line in history file
		int currentLine = 0;

		// reset to beginning of history file
		rewind(histFile);

		// get first char from history file
		currentChar = fgetc(histFile);

		// loop through history file again
		while (currentChar != EOF)
		{
			// add char if current line within limit
			if (currentLine >= totalLines - histLen)
			{
				fputc(currentChar, tempHistFile);
			}

			// increase counter if linebreak
			if (currentChar == '\n')
			{
				currentLine++;
			}

			// go to next char in file
			currentChar = fgetc(histFile);
		}

		// close both files
		fclose(histFile);
		fclose(tempHistFile);

		// overwrite original file with temp
		remove(filePath);
		rename(tempFilePath, filePath);
	}

	else
	{
		// just close the history file
		fclose(histFile);
	}
}

void getInput()
{
	// char array to store input
	char input[INPUT_LEN];
 
	// display prompt
	printf("%s", getPrompt());

	// read user input
	scanf("%[^\n]", input);

	// delete trailing linebreak
	fgetc(stdin);

	// update history
	updateHist(input);
	
	// lets gooo
	exec(input);
}

int main()
{
	// loop the prompt
	while(1)
	{
		getInput();
	}

	return 0;
}
