#include <u.h>
#include <libc.h>
#include <stdio.h>
#include <json.h>
#include "dict.h"
#include "olist.h"

char *webfsroot = "/mnt/web";
char *apiurl = "https://api.todoist.com/rest/v1/projects";
char *clonefile = "clone";
char *apitokhdrpfx = "Authorization: Bearer ";
int debug = 0;

void showdict(Dict);
void dumptables(JSON *);
Dict convjel2d(JSONEl *table);

/*
 *		"id": 1810201490,
 *		"order": 20,
 *		"color": 48,
 *		"name": "Fitness",
 *		"comment_count": 0,
 *		"shared": false
 */
void
showdict(Dict d)
{
	if (d == nil)
		return;

	print("=========== Todoist Project Dictionary ===========\n");
	print("ID: %s\n", dictsearch(d, "id"));
	print("Order: %s\n", dictsearch(d, "order"));
	print("Color: %s\n", dictsearch(d, "color"));
	print("Name: %s\n", dictsearch(d, "name"));
	print("Comment Count: %s\n", dictsearch(d, "comment_count"));
	print("Shared: %s\n\n", dictsearch(d, "shared"));
}

void
dumptables(JSON *j)
{
	if (j->first == nil)
		return;

	JSONEl *table;
	Dict d;

	for(table = j->first; table != nil; table = table->next) {
		if (table) {
			d = convjel2d(table);
			if (d != nil) {
				showdict(d);
				dictdestroy(d);
			}
		}
	}
}

Dict 
convjel2d(JSONEl *table)
{
	Dict d;
	JSONEl *obj;

	if (table == nil)
		return nil;

	d = dictcreate();

	for (obj = table->val->first; obj != nil; obj = obj->next) {

		if (obj->val->t == JSONBool || obj->val->t == JSONNumber) {
			dictinsert(d, obj->name, smprint("%.2f", obj->val->n));
		} else {
			dictinsert(d, obj->name, smprint("%s", jsonstr(obj->val)));
		}		
	}	
	return d;
}

void 
usage(void) 
{
	fprint(2, "usage: todoist [-d] [-a apitoken] dir\n");
	exits("usage");
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



int 
getconn(void)
{
	int fd, nr;
	int conn;
	char rbuf[16];
	char *clone;

	clone = (char *) malloc(strlen(webfsroot)+strlen(clonefile)+2);

	if (clone == nil) {
		werrstr("getconn: %r");
		return -1;
	}

	sprintf(clone, "%s/%s", webfsroot, clonefile);

	if ((fd = open(clone, OREAD)) < 0) {
		werrstr("getconn: %r");
		return -1;
	}
	
	nr = read(fd, rbuf, sizeof(rbuf));

	if (nr <= 0) {
		werrstr("getconn: could not clone file.");
		return -1;
	}
	
	conn = atoi(rbuf);
	
	if (clone)
		free(clone);

	if (fd)
		close(fd);

	return conn;	
}

int 
sendurl(int fd, char *u)
{
	int br;
	char *data;

	data = (char *) malloc(strlen(u)+5);

	sprintf(data, "url %s", u);

	br = write(fd, data, strlen(data));

	free(data);
	return br;
}

int 
sendheader(int fd, char *h)
{
	int br;
	char *data;

	data = (char *) malloc(strlen(h)+9);

	sprintf(data, "headers %s", h);

	br = write(fd, data, strlen(data));

	free(data);
	return br;
}

void
main(int argc, char *argv[])
{
	JSON *j;
	char *apitok;
	char *apitokhdr;
	char *ctlfile, *bodyfile, *lenfile;
	int connfd;
	int fd, bd, ld, n;
	int br, lr;
	char lbuf[64];
	char bbuf[1024];
	//char *fbuf;
	char fbuf[65532];

	/* Check the environment variable for the API token */
	/* 
	 * apitok = getenv('todoist_api_tok');
	 */

	ARGBEGIN{
		case 'd':
			debug = 1;
			break;
		case 'a':
			apitok = EARGF(usage());
		default:
			break;
	}ARGEND;
	
	if (apitok == nil) {
		usage();
	}

	connfd = getconn();

	if (connfd < 0)
		sysfatal("main: %r");

	ctlfile = (char *) malloc(strlen(webfsroot)+7);
	bodyfile = (char *) malloc(strlen(webfsroot)+9);
	lenfile = (char *) malloc(strlen(webfsroot)+18);

	if (ctlfile == nil)
		sysfatal("main: no memory left.");

	if (bodyfile == nil)
		sysfatal("main: no memory left.");

	if (lenfile == nil)
		sysfatal("main: no memory left.");

	sprintf(ctlfile, "%s/%d/ctl", webfsroot, connfd);
	sprintf(bodyfile, "%s/%d/body", webfsroot, connfd);	
	sprintf(lenfile, "%s/%d/contenttype", webfsroot, connfd); 

	fd = open(ctlfile, ORDWR);

	if (fd < 0)
		sysfatal("main: %r");

	n = sendurl(fd, apiurl);

	apitokhdr = (char *) malloc(strlen(apitokhdrpfx)+strlen(apitok));

	if (apitokhdr == nil)
		sysfatal("main: no memory left.");

	sprintf(apitokhdr, "%s%s", apitokhdrpfx, apitok);

	n = sendheader(fd, apitokhdr);

	/* Open body file */
	bd = open(bodyfile, OREAD);

	if (bd < 0)
		sysfatal("main: %r");

	/* Read content length */
	ld = open(lenfile, OREAD);

	if (ld < 0)
		sysfatal("todoist: %r");

	lr = read(ld, lbuf, sizeof(lbuf));

	if (lr <= 0)
		sysfatal("main: could not read Content-Length %r");


	for(;;) {
		br = read(bd, bbuf, sizeof(bbuf));
		if (br <= 0)
			break;	 
		strncat(fbuf, bbuf, br);
	}

	j = jsonparse(fbuf);

	if (j == nil)
		sysfatal("main: json: could not parse %r");

	dumptables(j);

	if (j)
		jsonfree(j);

	if (bodyfile)
		free(bodyfile);

	if (lenfile)
		free(lenfile);

	if (ctlfile)
		free(ctlfile);

	if (ld)
		close(ld);

	if (bd)
		close(bd);

	if (fd)
		close(fd);

	exits(nil);
}