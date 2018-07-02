**Introduction**

This is my personal repository of 9front userspace utilities.  The goal here is to make updates to the tools Iuse on an everyday basis to make them more amenable to my work 
style and preferences.  Most of the time that has to do with things like color scheme changes or additional command line options.  When I do these simple things the expectation
is that the new commands will be drop in replacements for the ones already on the system.  If I need to totally alter the functionality of the tool I will give it a new name so
as not to clash with the old tool.

**Usage**

The easiest way to make use of the tools within this repo is to first install [9front](http://9front.org/).  The installation is surprising easily (once you know what your doing ;-) )
I am currently running 9front on QEMU/KVM on Debian 'stretch' 9.4.  I am using [virt-manager](https://virt-manager.org/) to manage my virtual machines to make it easy on myself. 

>*I have tweaked my KVM instance a bit to make it work more effectively.  One of the first things you will WANT to do is to use RAW disk partitions for KVM.  Not a lot of 
>tutorials out there for this, but if you know LVM2 on Linux and your familiar with virt-manager it is suprisingly easy.  [Here](https://johnsontravis.wordpress.com/2016/04/29/tutorial-6-configuring-lvm-storage-for-qemukvm-vms-using-virt-manager-on-centos-7/)
>is a decent resource for doing it with virt-manager.*

Once you have 9front installed you will need to pull down this repo.  A simple clone will do:

>*term% hg clone https://superbadmofo@bitbucket.org/superbadmofo/9front-user*

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
