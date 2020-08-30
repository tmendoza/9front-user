#include <u.h>
#include <libc.h>
#include <stdio.h>
#include <thread.h>
#include <draw.h>
#include <mouse.h>
#include <keyboard.h>

typedef enum _ArrowKey{
	ARROW_LEFT 	= 	61457,
	ARROW_UP 	= 	61454,
	ARROW_RIGHT	=	61458,
	ARROW_DOWN	=	63488,
} ArrowKey;

typedef struct _Mousestate{
	int x;
	int y;
	int b;
} Mousestate;

typedef struct _Keystate{
	int last;
	int value;
} Keystate;

#define FRMRATE 30
/* Global Variables */
vlong starttime;
vlong currtime;

int running = 0;

Mousestate ms;
Keystate ks;

Channel *gamewinc;
Channel	 *cexit;

char *message = nil;
int count = 0;

Image		*backgrd;
Image   	*text;
Rectangle	rtext;

void usage(void);
void blank(void);
void update(void);
void resetks(void);
void drawgamewin(uint val);

void blank(void)
{
	draw(screen, screen->r, display->black, nil, ZP);
	flushimage(display, 1);
}

void update(void)
{
	/* Update data structure for Animations, Sprites, Explosions, etc. */
	switch(ks.value){
		case ARROW_LEFT:
			printf("Left Arrow\n");
			break;
		case ARROW_UP:
			printf("Up Arrow\n");
			break;
		case ARROW_DOWN:
			printf("Down Arrow\n");
			break;
		case ARROW_RIGHT:
			printf("Right Arrow\n");
			break;
	}
	resetks();

	/* Set Scoreboard String */
	sprintf(message, "Mouse - X:%d Y:%d", ms.x, ms.y);
	count++;
}

void resizethread(void *arg)
{
	Mousectl *mctl = arg;

	for(;;)
	{
		recvul(mctl->resizec);
		print("Resizing.\n");
		if (getwindow(display, Refbackup) < 0)
			sysfatal("getwindow: %r");
		
		rtext = screen->r;

		rtext.min.x += 20;
		rtext.min.y += 20;
		rtext.max.y = rtext.min.y + display->defaultfont->height;

		update();
		sendul(gamewinc, 0);
	}
}

void resetks(void){
	ks.last = ks.value;
	ks.value = -1;
}


void mousethread(void *arg)
{
	Mousectl *mctl = arg;
	Mouse m;
	Menu menu;
	Rectangle r;
	
	r = screen->r;

	// Menu Items
	char *mstr[] = {"open", "save", "reset", "exit", 0};

	menu.item = mstr;
	menu.lasthit = 0;

	for(;;)
	{
		recv(mctl->c, &m);

		ms.x = m.xy.x - r.min.x;
		ms.y = m.xy.y - r.min.y;
		ms.b = m.buttons;

		if (m.buttons & 1)
		{
			printf("Left button\n");
		}

		if (m.buttons & 2)
		{
			printf("Middle button\n");
		}

		if (m.buttons & 4)
		{
			if(m.buttons & 4) {
				if(menuhit(3, mctl, &menu, _screen) == 0)
					printf("Process Menuhit 'Open'\n");
				if(menuhit(3, mctl, &menu, _screen) == 1)
					printf("Process Menuhit 'Save'\n");
				if(menuhit(3, mctl, &menu, _screen) == 2)
					printf("Process Menuhit 'Reset'\n");
				if(menuhit(3, mctl, &menu, _screen) == 3) {
					sendul(cexit, 0);
				}
			}
		}
	}
}

void initcolor(void)
{
	text = display->black;
	backgrd = allocimagemix(display, DPalegreen, DWhite);
}

void inittimer(void)
{
	starttime = nsec();
	currtime = starttime;
}

int getticks(void)
{
	currtime = nsec();
	return (currtime - starttime) / 1000000;
}

void framewait(void)
{
	inittimer();
	if(getticks() < (1000 / FRMRATE))
		sleep((1000/FRMRATE) - getticks());
}

void gamewinthread(void *arg)
{
	Channel *gwt = arg;

	uint val, nval;

	val = ~0;

	update();
	drawgamewin(val);

	for(;;)
	{
		recvul(gwt);
		update();
		drawgamewin(val);
	}
}

void framewaitthread(void *arg)
{
	Channel *gwt = arg;

	for(;;)
	{
		//framewait();
		sendul(gwt, 0);
		print("Sending.\n");
	}
}

void drawgamewin(uint val)
{
 	Rectangle r;

	r = screen->r;

	draw(screen, r, backgrd, nil, ZP);
	string(screen, rtext.min, text, ZP, display->defaultfont, message);
	flushimage(display, 1);
}

void usage(void)
{
	fprint(2, "usage: guiapptest [-x size ] [ -y size ]\n");
	exits("usage");
}


void threadmain(int argc, char **argv)
{
	Mousectl *mctl;
	//Mouse	m;

	char *dx = nil;
    char *dy = nil;
	char	winargs[512];

	running = 1;

	if(argc<4)
		usage();

	ARGBEGIN{
	case 'x':
		dx = ARGF();
		break;
	case 'y':
		dy = ARGF();
		break;
	default:
		usage();
	}ARGEND

	sprintf(winargs, "-dx %s -dy %s", dx, dy);
	printf("Win Args: %s\n", winargs);

	if (newwindow(winargs) < 0)
		sysfatal("newwindow: %r");
	
	mctl = initmouse(nil, nil);
	if (mctl == nil)
		sysfatal("initmouse: %r");

	if (initdraw(nil, nil, argv[0]) < 0)
		sysfatal("initdraw: %r");

	if((message = (char *) malloc(2048)) == nil)
		sysfatal("malloc: %r");

	memset(message, 0, 2048);

	initcolor();
	
	gamewinc = 	chancreate(sizeof(ulong), 0);
	cexit	 =	chancreate(sizeof(ulong), 0);

	threadcreate(mousethread, mctl, 8*1024);
	threadcreate(resizethread, mctl, 8*1024);
	threadcreate(gamewinthread, cexit, 8*1024);

	print("Ending program.\n");
	threadexits(nil);
}