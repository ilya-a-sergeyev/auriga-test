#include "auriga_test.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char out_filename[MAX_FILENAME];
char in_filename[MAX_FILENAME];

enum
{
	IN_FILE_OPTION = 0,
	OUT_FILE_OPTION = 1,
	THE_END
};

const char *mount_opts[] =
{
        "infile",
        "outfile",
        NULL
};

int parse_av(int argc, char **argv)
{
	int opt;

	memset(in_filename, 0, MAX_FILENAME);
	memset(out_filename, 0, MAX_FILENAME);

	while ((opt = getopt (argc, argv, "i:o:")) != -1)
		switch (opt)
		{
		case 'i':
			strncpy(in_filename, optarg, MAX_FILENAME);
			break;
		case 'o':
			strncpy(out_filename, optarg, MAX_FILENAME);
			break;
		default:
		abort ();
	}

	if (!in_filename[0] || !out_filename[0]) {
		printf("USAGE: %s i<input_filename> o<output_filename>\n", argv[0]);
		return -1;
	}

	/*
	printf("INP: %s\n", infile);
	printf("OUT: %s\n", outfile);
	*/

	return 0;
}
