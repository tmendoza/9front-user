#include <u.h>
#include <libc.h>
#include <stdio.h>
#include <draw.h>
#include <event.h>
#include <cursor.h>

char pic[] = "/usr/glenda/GitHub-Mark-Light-64px.bit";

int fd, dx, dy;
Point pt;
Rectangle rect;

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
Image		*colorred;
Image		*colormedblue;
Image		*colordarkyellow;
Image		*colorgreygreen;
Image		*diskimage;

Rectangle	rtext;

Point		sp;
Point 		ep;

Mousestate ms;
Keystate ks;

Point squarepts[5];
Point squarepolypts[5];
Point ellipsecenter;

void
initdiskimage(void)
{
	if((fd = open(pic, OREAD)) >= 0){
		if((diskimage = readimage(display, fd, 0)) != nil){
 			rect = screen->r;
			pt = Pt(rect.max.x / 2, rect.max.y * 2 / 3); 
			dx = (Dx(screen->r) - Dx(diskimage->r)) / 2;
			rect.min.x += dx;
			rect.max.x -= dx;
			dy = (Dy(screen->r) - Dy(diskimage->r)) / 2;
			rect.min.y += dy;
			rect.max.y -= dy;
		}
		close(fd);
	}
}

void
initcolor(void){
	text = display->black;

	colorred = allocimage(display, Rect(0,0,1,1), screen->chan, 1, DRed);
	colormedblue = allocimage(display, Rect(0,0,1,1), screen->chan, 1, DMedblue);
	colordarkyellow = allocimage(display, Rect(0,0,1,1), screen->chan, 1, DDarkyellow);
	colorgreygreen = allocimage(display, Rect(0,0,1,1), screen->chan, 1, DGreygreen);

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

	squarepts[0].x = screen->r.min.x + ms.x;
	squarepts[0].y = screen->r.min.y + ms.y;
	squarepts[1].x = screen->r.min.x + ms.x + 200;
	squarepts[1].y = screen->r.min.y + ms.y;	
	squarepts[2].x = screen->r.min.x + ms.x + 200;
	squarepts[2].y = screen->r.min.y + ms.y + 200;
	squarepts[3].x = screen->r.min.x + ms.x;
	squarepts[3].y = screen->r.min.y + ms.y + 200;
	squarepts[4].x = screen->r.min.x + ms.x;
	squarepts[4].y = screen->r.min.y + ms.y;

	squarepolypts[0].x = screen->r.min.x + 100;
	squarepolypts[0].y = screen->r.min.y + 100;
	squarepolypts[1].x = screen->r.min.x + 200;
	squarepolypts[1].y = screen->r.min.y + 100;	
	squarepolypts[2].x = screen->r.min.x + 200;
	squarepolypts[2].y = screen->r.min.y + 200;
	squarepolypts[3].x = screen->r.min.x + 100;
	squarepolypts[3].y = screen->r.min.y + 200;
	squarepolypts[4].x = screen->r.min.x + 100;
	squarepolypts[4].y = screen->r.min.y + 100;

	ellipsecenter.x = screen->r.min.x + 400;
	ellipsecenter.y = screen->r.min.y + 400;
}


void initellipse(void)
{
	ellipsecenter.x = screen->r.min.x + 400;
	ellipsecenter.y = screen->r.min.y + 400;
}

void initsquares(void)
{
	Point sqrpt1, sqrpt2, sqrpt3, sqrpt4, sqrpt5;

	sqrpt1.x = screen->r.min.x + 100;
	sqrpt1.y = screen->r.min.y + 100;

	sqrpt2.x = screen->r.min.x + 200;
	sqrpt2.y = screen->r.min.y + 100;

	sqrpt3.x = screen->r.min.x + 200;
	sqrpt3.y = screen->r.min.y + 200;

	sqrpt4.x = screen->r.min.x + 100;
	sqrpt4.y = screen->r.min.y + 200;

	sqrpt5.x = screen->r.min.x + 100;
	sqrpt5.y = screen->r.min.y + 100;

	squarepts[0] = sqrpt1;
	squarepts[1] = sqrpt2;
	squarepts[2] = sqrpt3;
	squarepts[3] = sqrpt4;
	squarepts[4] = sqrpt5;

	return;

}

void initpolyfill(void)
{
	Point sqrpt1, sqrpt2, sqrpt3, sqrpt4, sqrpt5;

	sqrpt1.x = screen->r.min.x + 100;
	sqrpt1.y = screen->r.min.y + 100;

	sqrpt2.x = screen->r.min.x + 200;
	sqrpt2.y = screen->r.min.y + 100;

	sqrpt3.x = screen->r.min.x + 200;
	sqrpt3.y = screen->r.min.y + 200;

	sqrpt4.x = screen->r.min.x + 100;
	sqrpt4.y = screen->r.min.y + 200;

	sqrpt5.x = screen->r.min.x + 100;
	sqrpt5.y = screen->r.min.y + 100;

	squarepolypts[0] = sqrpt1;
	squarepolypts[1] = sqrpt2;
	squarepolypts[2] = sqrpt3;
	squarepolypts[3] = sqrpt4;
	squarepolypts[4] = sqrpt5;

	return;

}

void
redraw(void)
{
 	Rectangle r;

	r = screen->r;

	// Clear and Draw the background
	draw(screen, r, backgrd, nil, ZP);

	// Draw some text to the screen
	string(screen, rtext.min, text, ZP, display->defaultfont, message);

	// Draw a blueline on the screen
	line(screen, sp, ep, Endsquare, Endsquare, 1, colorred, sp);

	// Draw a Red polygon on the screen
	poly(screen, squarepts, 5, Endsquare, Endsquare, 1, colormedblue, squarepts[0]);

	// Draw a polygon filled with Dark Yellow
	fillpoly(screen, squarepolypts, 5, 0, colordarkyellow, squarepolypts[0]);

	// Draw an ellipse
	ellipse(screen, ellipsecenter, 100, 100, 1, colorgreygreen, ellipsecenter);


	// Draw arcs
	for(int i=0; i<4; i++)
	{
		for (int j=0; j<3; j++)
		{
			int radius = 20;
			int startangle = 0;
			int endangle = (int) (3.14159 + (3.14159 * j)/2) * (180/3.14159);
			Point p;

			p.x = screen->r.min.x + (400 + j * 50);
			p.y = screen->r.min.y + (400 + i * 50);

			if (i>1)
			{
				arc(screen, p, radius, radius, 2, colorred, p, startangle, endangle);
			}
			else
			{
				fillarc(screen, p, radius, radius, colorred, p, startangle, endangle);
			}
		}
	}

	// Draw disk image
 	rect = screen->r;
	pt = Pt(rect.max.x / 2, rect.max.y * 2 / 3); 

	dx = (Dx(screen->r) - Dx(diskimage->r)) / 2;
	rect.min.x += dx;
	rect.max.x -= dx;

	dy = (Dy(screen->r) - Dy(diskimage->r)) / 2;
	rect.min.y += dy;
	rect.max.y -= dy;

	draw(screen, rect, diskimage, nil, diskimage->r.min);

	// Flush the Display buffer
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

	// Setup line start and stop points
	sp.x = rtext.min.x + 200;
	sp.y = rtext.min.y + 200;

	ep.x = rtext.min.x + 400;
	ep.y = rtext.min.y + 400;

	redraw();
}

void
usage(void)
{
	fprint(2, "usage: drawtests [-x size ] [ -y size ]\n");
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

	if(initdraw(derror, nil, "drawtest") < 0){
		fprint(2, "testwin: can't open display: r\n");
		exits("initdraw");
	}
	
	if((message = (char *) malloc(2048)) == nil)
		sysfatal("malloc: %r");
	memset(message, 0, 2048);

	/* Allocate some images for display */
	initcolor();
	initsquares();
	initpolyfill();
	initdiskimage();

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