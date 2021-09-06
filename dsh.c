#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/wait.h>

#define version "0.1"

#include "config.h"

int getHistLen()
{
	// int to store history length
	int histLen = DEFAULTHISTLEN;

	// char to store history length env var
	char *histLenString = getenv(HISTLENVAR);

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
	char *prompt = getenv(PROMPTVAR);

	// use default prompt if doesn't exist
	if (!prompt)
	{
		prompt = DEFAULTPROMPT;
	}

	return prompt;
}

void exec(char *input)
{
	// int to loop through input string
	int index = 0;

	// array to store argument list
	char *argv[ARGLEN];

	// char to split input string at
	char delim[] = " ";

	// first split
	char *ptr = strtok(input, delim);

	// loop through the string
	while (ptr != NULL)
	{
		// add current split to array
		argv[index++] = ptr;

		// split it again
		ptr = strtok(NULL, delim);
	}

	// add null to end of arguments list
	argv[index++] = NULL;

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
	char filePath[DIRLEN];

	// get history file path
	strcat(strcpy(filePath, getenv("HOME")), "/" HISTFILENAME);

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
		char tempFilePath[DIRLEN] = TEMPFILEDIR "/" TEMPHISTFILENAME;

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
	char input[INPUTLEN];
 
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
