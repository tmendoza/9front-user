#include <u.h>
#include <libc.h>
#include <stdio.h>

void main(int argc, char* argv[]) {
	int pid;

	pid = getpid();
	printf("My PID is %d\n", pid);

	exits(nil);
}