#include <u.h>
#include <libc.h>

void main(int argc, char* argv[]) {
	char *homedir;
	char *username;
	char *sysname;
	int pid;

	pid = getpid();

	homedir = getenv("home");
	if (homedir == nil)
		sysfatal("envvar home error: %r");

	username = getenv("user");
	if (username == nil) 
		sysfatal("envvar user error: %r");

	sysname = getenv("sysname");
	if (sysname == nil)
		sysfatal("envvar sysname error: %r");
		
	print("\n-------------------------\n");
	print("Name: %s\n", username);
	print("Home Directory: %s\n", homedir);
	print("System Name: %s\n", sysname);
	print("Process ID: %d\n", pid);

	exits(nil);
}