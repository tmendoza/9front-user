</$objtype/mkfile


DIRS=`{echo */mkfile | sed 's,/mkfile *,\n,g'}

none:VQ:
	echo usage: mk dirs, all, install, clean, nuke

all:V:	dirs

dirs:V:
	for(i in $DIRS) @{
		cd $i
		echo mk $i
		mk $MKFLAGS all
	}

%.directories:V:
	for(i in $DIRS) @{
		cd $i
		echo mk $i
		mk $MKFLAGS $stem
	}

clean:V:	clean.directories

nuke:V:		nuke.directories

directories:V:	install.directories

install:V:
	mk $MKFLAGS directories

