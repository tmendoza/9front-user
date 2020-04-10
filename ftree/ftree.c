#include <u.h>
#include <libc.h>
#include <stdio.h>

int debug = 0;

void 
usage(void) 
{
	fprint(2, "usage: ftree [-d] dir\n");
	exits("usage");
}

int 
hasmode(char *f, ulong m) 
{
	int r;
	Dir *dir;

	dir = dirstat(f);
	if (dir == nil)
		return 0;
	r = (dir->mode & m) != 0;
	free(dir);
	return r;
}

int 
isfile(char *f) 
{
	int r;
	Dir *dir;

	dir = dirstat(f);
	if (dir == nil)
		return 0;
	r = (dir->mode & DMDIR) == 0;
	free(dir);
	return r;
}

/*
 * Find function recursively descend directory heirarchy from 'root' directory
 * passed into this function.
 */
int
finder(char *dir)
{
	char *path;
	int ndents, fd, i;
	Dir*	d;
	Dir*	dents;

	d = dirstat(dir);

	if (d == nil) {
		werrstr("ftree: %r");
		return -1;
	}

	fd = open(dir, OREAD);

	if (fd < 0) {
		werrstr("ftree: %r");
		return -1;
	}

	for(;;) {

		ndents = dirread(fd, &dents);
		
		if (ndents == 0)
			break;

		for(i=0; i<ndents; i++) {

			path = (char *) malloc(strlen(dir)+strlen(dents[i].name)+2);
		
			if (!path)
				sysfatal("find: %r");

			sprintf(path, "%s/%s", dir, dents[i].name);

			if (isfile(path) != 0) {
				print("%s\n", path);
			} else {
				print("%s/\n", path);
				finder(path);
			}

			if (path)
				free(path);
		}

		if (dents)
			free(dents);
	}

	if (fd)
		close(fd);

	if (d)
		free(d);

	return 1;
}

void main(int argc, char *argv[]) {

	char *dirname;

	ARGBEGIN{
		case 'd':
			debug = 1;
			break;
		default:
			break;
	}ARGEND;

	if (argc < 1) {
		dirname = (char *) malloc(2);
		strcpy(dirname, ".");
	} else {
		dirname = (char *) malloc(strlen(argv[0])+1);
		strcpy(dirname, argv[0]);
	}

	switch(finder(dirname)) {
		case 1:
			exits(nil);
		default:
			sysfatal("ftree: %r");
	}
}

