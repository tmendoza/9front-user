#include <u.h>
#include <libc.h>

void main(int argc, char* argv[]) {
	switch(fork()) {
		case -1:
			sysfatal("fork failed %r");
		case 0:
			execl("/bin/ls", "ls", nil);
			break;
		default:
			print("ls started\n");
	}
	exits(nil);
}