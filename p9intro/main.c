#include <u.h>
#include <libc.h>
#include <stdio.h>

void usage(void) {
	fprint(2, "usage: %s [-d] -s string -c count\n", argv0);
	exits("usage");
}

void dolist(char *s, int count) {
	for (int i=0; i<count; i++)
		printf("Text: %s\n", s);
}

void main(int argc, char* argv[]) {
	char *outstr;
	char *counter;
	char *homedir;
	char *username;
	int icount, debug = 0;

	if (argc < 4)
		sysfatal("invalid number of args.");	

	ARGBEGIN{
		case 'd':
			debug = 1;
			break;
		case 's':
			outstr = EARGF(usage());
			break;
		case 'c':
			counter = EARGF(usage());
		default:
			break;
	}ARGEND;

	if (outstr == nil)
		sysfatal("String to output is invalid.");

	if ((icount = atoi(counter)) == 0)
		sysfatal("Counter input is not a valid number.");

	if (debug)
		printf("Getting some environment variables.\n");

	username = getenv("user");

	if (username == nil)
		sysfatal("We are nameless");

	printf("Looks like our name is %s\n", username);

	homedir = getenv("home");

	if (homedir == nil)
		sysfatal("We are homeless!");

	printf("Home directory is %s\n", homedir);

	dolist(outstr, icount);

	exits(nil);
}

