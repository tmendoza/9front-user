#include <u.h>
#include <libc.h>
#include <stdio.h>
#include <draw.h>
#include <event.h>
#include <cursor.h>

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

#define FRMRATE	30

/* Global Variables */
vlong starttime;
vlong currtime;

char *message = nil;
int count = 0;

Image		*backgrd;
Image   	*text;
Rectangle	rtext;

Mousestate ms;
Keystate ks;

void
initcolor(void){
	text = display->black;
	backgrd = allocimagemix(display, DPalegreen, DWhite);
}

void 
derror(Display *d, char *errmsg){
	printf("error occured: %s\n", errmsg);
}

void 
resetks(void){
	ks.last = ks.value;
	ks.value = -1;
}

void
update(void){
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

void
redraw(void)
{
 	Rectangle r;

	r = screen->r;

	draw(screen, r, backgrd, nil, ZP);
	string(screen, rtext.min, text, ZP, display->defaultfont, message);
	flushimage(display, 1);
}

void
eresized(int new)
{
	if(new && getwindow(display, Refnone) < 0)
		fprint(2,"can't reattach to window");

	rtext = screen->r;

	rtext.min.x += 20;
	rtext.min.y += 20;
	rtext.max.y = rtext.min.y + display->defaultfont->height;

	redraw();
}

void
usage(void)
{
	fprint(2, "usage: basicwinevt [-x size ] [ -y size ]\n");
	exits("usage");
}

void
procptr(Event *e, Menu *menu){
	Mouse m;
	Rectangle r;

	r = screen->r;
	m = e->mouse;

	ms.x = m.xy.x - r.min.x;
	ms.y = m.xy.y - r.min.y;
	ms.b = m.buttons;

	if(m.buttons & 4) {
		if(emenuhit(3, &m, menu) == 0)
			printf("Process Menuhit 'Open'\n");
		if(emenuhit(3, &m, menu) == 1)
			printf("Process Menuhit 'Save'\n");
		if(emenuhit(3, &m, menu) == 2)
			printf("Process Menuhit 'Reset'\n");
		if(emenuhit(3, &m, menu) == 3) {
			closedisplay(display);
			exits(0);
		}
	}
}

void
prockeybd(Event *e){
	ks.last = ks.value;
	ks.value = e->kbdc;	
	printf("Process keyboard Char Code: %d  Char: %c\n", ks.value, (char)ks.value);
}

//void
//proctimer(Event *event){
//	Event *e = event;
//	//printf("Timer fired.\n");	
//}

void inittimer(void){
	starttime = nsec();
	currtime = starttime;
}

int getticks(void){
	currtime = nsec();
	return (currtime - starttime) / 1000000;
}

void framewait(void){
	inittimer();
	if(getticks() < (1000 / FRMRATE))
		sleep((1000/FRMRATE) - getticks());
}

void main(int argc, char **argv){
	Event   e;
	Menu menu;

	char *dx = nil;
    char *dy = nil;
	char winargs[512];

	char *mstr[] = {"open", "save", "reset", "exit", 0};

	int		t;
	ulong	key;

	printf("Num args: %d\n", argc);

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

	if(newwindow(winargs) < 0){
		fprint(2, "testwin: can't open new window: %r\n");
		exits("newwindow");
	}

	if(initdraw(derror, nil, "testwin") < 0){
		fprint(2, "testwin: can't open display: r\n");
		exits("initdraw");
	}
	
	if((message = (char *) malloc(2048)) == nil)
		sysfatal("malloc: %r");
	memset(message, 0, 2048);

	/* Allocate some images for display */
	initcolor();

	eresized(0);
	
	einit(Emouse | Ekeyboard);
	t = etimer(0, 1000/60);

	menu.item = mstr;
	menu.lasthit = 0;

	for(;;){
		key = event(&e);
		if(key == Emouse)
			procptr(&e, &menu);
		else if(key == Ekeyboard)
			prockeybd(&e);
		else if(key == t)
			//proctimer(&e);
			update();
			redraw();
	}
}