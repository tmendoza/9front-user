	  #include <u.h>
	  #include <libc.h>
	  #include <thread.h>
	  #include <draw.h>
	  #include <mouse.h>
	  #include <keyboard.h>
	  #include <control.h>
	  Controlset *cs;
	  int ctldeletequits = 1;
	  void
	  resizecontrolset(Controlset*)
	  {
	      int i;
	      Rectangle	r, r1, r2, r3;
	      if(getwindow(display, Refnone) < 0)
		  sysfatal("resize failed: %r");
	      r	= insetrect(screen->r, 10);
	      r1 = r;
	      r2 = r;
		  r3 = r;

	      r1.max.y = r1.min.y+1+font->height+1;
	      r2.min.y = r1.max.y+10;
	      r2.max.y = r2.min.y+1+font->height+1;

		  r3.min.x = 200;
		  r3.min.y = 200;
		  r3.max.y = 400;
		  r3.max.y = 400;

	      chanprint(cs->ctl, "top rect %R\ntop show", r1);
	      chanprint(cs->ctl, "bot rect %R\nbot show", r2);
		  chanprint(cs->ctl, "button rect %R\nbutton show", r3);
          //chanprint(cs->ctl, "menu rect %R\nmenu window", r3);
	  }
	  void
	  threadmain(int argc, char *argv[])
	  {
	      char *s, *args[3];
	      Channel *c;
	      Control *top, *bot, *menu, *button;
	      int n;
	      initdraw(0, 0, "example");
	      initcontrols();
	      cs = newcontrolset(screen, nil, nil, nil);
	      cs->clicktotype =	1;

		  button = createbutton(cs, "button");
		  chanprint(cs->ctl, "button image red");
		  chanprint(cs->ctl, "button light paleyellow");
	      chanprint(cs->ctl, "button border 1");

		  menu = createmenu(cs, "menu");
		  chanprint(cs->ctl, "menu add Open");
		  chanprint(cs->ctl, "menu add Close");
		  chanprint(cs->ctl, "menu add Exit");
		  chanprint(cs->ctl, "menu reveal");

	      top = createentry(cs, "top");
	      chanprint(cs->ctl, "top image paleyellow");
	      chanprint(cs->ctl, "top border 1");

	      bot = createslider(cs, "bot");
	      chanprint(cs->ctl, "bot border 1");
	      chanprint(cs->ctl, "bot image paleyellow");
	      chanprint(cs->ctl, "bot indicatorcolor red");
	      chanprint(cs->ctl, "bot max 100");
	      chanprint(cs->ctl, "bot clamp low	1");
	      chanprint(cs->ctl, "bot orient horizontal");

	      c	= chancreate(sizeof(char*), 0);
	      controlwire(top, "event",	c);
	      controlwire(bot, "event",	c);
		  controlwire(menu, "event", c);
		  controlwire(button, "event", c);

	      activate(top);
	      activate(bot);
		  activate(menu);
		  activate(button);

	      resizecontrolset(cs);
	      for(;;){
		  s = recvp(c);
		  n = tokenize(s, args,	nelem(args));
		  if(n==3 && strcmp(args[1], "value")==0){
		      if(strcmp(args[0], "top:") == 0)
			  	chanprint(cs->ctl, "bot value	%q", args[2]);
		      else
			  	chanprint(cs->ctl, "top value	%q", args[2]);

			  if(strcmp(args[0], "button:") == 0)
				chanprint(cs->ctl, "menu window 1", args[2]);
		  }
	      }
	      threadexitsall(nil);
	  }
