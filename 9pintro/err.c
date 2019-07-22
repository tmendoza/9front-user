#include <u.h>
#include <libc.h>

void main(int argc, char* []) {
	char errorstr[1024];

	if (chdir("magic") < 0) {
		rerrstr(errorstr, sizeof(errorstr));
		//sysfatal("chdir failed: %r");
		print("%s\n", errorstr);
		exits(errorstr);
	}

	exits(nil);
}