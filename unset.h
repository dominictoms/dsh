void dshunset(int argc, char **argv)
{
	// no argument specified
	if (argc < 2)
	{
		printf("please specify a variable\n");
		return;
	}

	// too many arguments
	if (argc > 2)
	{
		printf("please only specify one variable\n");
		return;
	}

	// unset the env variable
	unsetenv(argv[1]);
}
