void dshcd(int argc, char **argv)
{
	// invalid arguments
	if (argc != 2)
	{
		printf("%s: invalid arguments\n", argv[0]);
		return;
	}

	// if first char is tilde
	if (argv[1][0] == '~')
	{
		// remove the tilde
		memmove(&argv[1][0], &argv[1][0 + 1], strlen(argv[1]) - 0);

		// get the full dir
		char dir[ARG_LEN];
		strcat(dir, getenv("HOME"));
		strcat(dir, argv[1]);

		// update the argument
		argv[1] = dir;
	}

	// change the directory!
	if (chdir(argv[1]))
	{
		// directory doesn't exist
		printf("%s: directory not found\n", argv[1]);
	}
}
