**Introduction**

This is my personal repository of 9front userspace utilities.  The goal here is to make updates to the tools Iuse on an everyday basis to make them more amenable to my work 
style and preferences.  Most of the time that has to do with things like color scheme changes or additional command line options.  When I do these simple things the expectation
is that the new commands will be drop in replacements for the ones already on the system.  If I need to totally alter the functionality of the tool I will give it a new name so
as not to clash with the old tool.

In addition to my own tools, I also keep other developer's code here.  I am not trying to steal their thunder or anything like that, it's just that the things I have put here make it 
easier to modify so that it conforms to my own conventions.   I have stuff in here from:

* [Stanley Lieber](http://plan9.stanleylieber.com/)
* [Cinap Lenrek](http://9front.org/cinap.html)
* [Aiju](http://aiju.de)

> Some of those links may be bad or down.  ;-)

**Description**

Most of the color scheme change were made to match Stanley Lieber's 'mono' [patches](http://plan9.stanleylieber.com/rio/) to rio and various utilities.  Why you may ask?  Well
I am not a fan of staring at glaring white backgrounds all day.  That much brightness takes it's toll on your eyes after a while.   [Why you should avoid white backgrounds!](http://presentationsforbusiness.com/professional-speaker-image/white-death-on-the-podium/)

[example screenshot](http://plan9.stanleylieber.com/rio/img/rio.mono.png)

* clock - this is the old clock program.  I just changed the background to black and the clock arms to green
* faces.b - black background (Stanley Lieber)
* mothra.black - changed the default white background to black (Stanley Lieber)
* rio.mono - this changes the pop-up menus to a black background with white text.  It also allows for using an image as a background using the '-B' option.  Borders are gray. (Stanley Lieber)
* stats.mono - makes the background black and changes the colors of the graphs to light-grey. (Stanley Lieber)
* statusmsg.mono - again makes the background black and text white.  This is a cool tool if your not aware of it.  [Read the man page](http://man.cat-v.org/9front/8/statusbar). (Stanley Lieber)
* winwatch - this is an updated version of the winwatch program.  I modified it with a black background and different highlight colors (magenta and blue)
* basicwinevt - this is an example skeleton of a simple resizeable window which handles keyboard, mouse, timer, menu and window resize events.  Is used for simple game and windowed application projects.
* screenlock - modified screenlock to redraw the window with a running digital clock overlaying the background image.

**Usage**

The easiest way to make use of the tools within this repo is to first install [9front](http://9front.org/).  The installation is surprising easily (once you know what your doing ;-) )
I am currently running 9front on QEMU/KVM on Debian 'stretch' 9.4.  I am using [virt-manager](https://virt-manager.org/) to manage my virtual machines to make it easy on myself. 

>*I have tweaked my KVM instance a bit to make it work more effectively.  One of the first things you will WANT to do is to use RAW disk partitions for KVM.  Not a lot of 
>tutorials out there for this, but if you know LVM2 on Linux and your familiar with virt-manager it is suprisingly easy.  [Here](https://johnsontravis.wordpress.com/2016/04/29/tutorial-6-configuring-lvm-storage-for-qemukvm-vms-using-virt-manager-on-centos-7/)
>is a decent resource for doing it with virt-manager.*

Once you have 9front installed you will need to pull down this repo.  A simple clone will do:

```
term% hg clone https://superbadmofo@bitbucket.org/superbadmofo/9front-user
```

Then all you have to do is cd into the 9front-user directory and run 'mk install'

Note: I will have to writeup my rational in more detail at a later date, but my 'mkfile' has been developed on some basic assumptions:

1. You have the following directories:
	* $home/bin/$objtype/bin
	* $home/bin/$objtype/bin/aux
	* $home/bin/$objtype/bin/games
	* $home/bin/$objtype/lib
	* $home/bin/$objtype/include
2. You are binding $home/bin/$objtype/bin BEFORE /$objtype/bin in your profile

I do this because I don't like to clobber the system wide binaries in '/bin'.  I like keeping things separate and clean and making use of 9front (Plan9) namespace modification
facilities to build up the environment I need on a per-user basis.  

Also, when doing system updates, I don't have to worry about overwriting any of the updates I have made.  

So my modifications and the core system are kept separated and clean.

	
**Administration**

This repository is maintained by [Tony Mendoza](mailto:tmendoza@superbadmofo.com)
