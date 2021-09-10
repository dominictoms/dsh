void dshcd(int argc, char **argv)
{
	// no argument specified
	if (argc < 2)
	{
		printf("please specify a directory\n");
		return;
	}

	// too many arguments
	if (argc > 2)
	{
		printf("please only specify one directory\n");
		return;
	}

	// change the directory!
	chdir(argv[1]);
}
