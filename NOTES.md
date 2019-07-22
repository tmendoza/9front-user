## Introduction

This is just a bunch of notes for configuring various things within Plan9

* [Stanley Lieber](http://plan9.stanleylieber.com/)
* [Cinap Lenrek](http://9front.org/cinap.html)
* [Aiju](http://aiju.de)

## Desktop Configuration

### Display Settings

### Get a listing of VGA screen sizes

```
@{rfork n; aux/realemu; aux/vga -p}
```

### Resize the screen size (resolution)

```
@{rfork n; aux/realemu; aux/vga -m -l 1280x1024x32}
```

### Setup my default font

Add the following to your $home/lib/profile

```
font=/lib/font/bit/fixed/unicode.9x15.font
```

### Setup some default rio windows at login

```
#!/bin/rc
window 0,0,161,117 stats -lmisce
window -r 32 130 1236 994
window -r 83 54 1273 948 'acme -l $home/acme.dump'
window -r 3 772 578 1023 'cat /dev/kprint'
window -r 3 241 105 643 winwatch
```

### Set rio's window/terminal background to black

```
rio -b -s -i riostart
```

## Networking Configuration 

### Networking Startup
Add the following to your $home/lib/profile

```
ip/ipconfig
ndb/cs
ndb/dns
```
### Networking Commands

#### To list the networking configuration
```
% cat /net/ndb
```

### SSH Setup
First you will need to generate an SSH private key

```
mkdir -p $home/lib/ssh
auth/rsagen -t 'service=ssh' > $home/lib/ssh/rsa-private.key
auth/rsa2ssh $home/lib/ssh/rsa-public.key
cat $home/lib/ssh/rsa-private.key >/mnt/factotum/ctl
```

Now that your newly generated public/private key pair has been generated and added to factotum, you will need to configure the
remote hosts to accept you public key (replace 'unix' with a proper IP or hostname)

```
cat $home/lib/ssh/rsa-public.key | ssh unix 'cat >> ~/.ssh/authorized_keys'
ssh unix

```

### GitHub access using native 'git' command line tools
First, you must have SSH configured properly on your Plan9/9front desktop.  You can clone Ori Bernstein's git9 cli client as 
follows

```
hg clone https://bitbucket.org/oridb/git9
```

You should be able to 'cd' into the cloned repo dir and run

```
mk install
```

This will install the binaries in /bin/git/

Examples
--------

Some usage examples:

	git/clone git://git.eigenstate.org/ori/mc.git
	git/log
	cd subdir/name
	git/add foo.c
	diff bar.c /mnt/git/HEAD/
	git/commit
	git/push

Some suggestions:

* Use git+ssh style URLs if you want to be able to push to you git repo.  For example:

```
term% git/clone git+ssh://git@github.com:tmendoza/9front-user
```

Is the command format you will want to use when clone your repo.  This, of course, assumes you have SSH setup properly on your end
and that your public key has been uploaded to your github repo account.

Using this GitHub URL format will allow you to properly push changes up to your repo.

I Can't tell you how awesome this is.  ;-)

### HTTP Proxy Configuration 

Pretty much all of the tools that access the web using HTTP go thru [webfs](http://man.9front.org/4/webfs).  If your working for a company not smart enough to be using [Plan9](https://en.wikipedia.org/wiki/Plan_9_from_Bell_Labs) and their systems are not secure enough to exist out on the internet by themselves, then your probably behind some type of firewall.  Assuming that they have setup a proxy for use of those other inferior operating systems you can make use of said proxy.  You will need to do the following:

* Create a local password database (or use secstore)

Normally you will want to use secstored to store secrets, but if your running a self-contained 9front temrinal you can create a password database and manually load this into factotum.  You will need to create
as password database.  This can be a simple text file.  I normally store this file in $home/lib/pwddb

Within this file enter a key entry.  A key entry is made up of key=value pairs:

```
key proto=pass realm=ADHCSCINT server=bcilproxy.hcscint.net user=I269363 service=http !password=<enter pass here>
```

The **realm** would be equal the HTTP Basic Auth Realm name.  This is used as part of the [basic auth scheme](https://developer.mozilla.org/en-US/docs/Web/HTTP/Authentication#WWW-Authenticate_and_Proxy-Authenticate_headers)

The **server** key is the name of the proxy server.   This is the same FQDN within the environment variable setting.  

The **user** key is just your username used to authenticate thru a proxy.  In my case, we use Active Directory SSO, so I use my SSO user id and password.  
 
The **!password** key is your actual password.  DO NOT QUOTE THIS!

* Setup Environment Variables 

This should be pretty straightforward.  Within your $home/lib/profile you can add the entry:

```
httpproxy='http://bcilproxy.hcscint.net:8080'
```

* Configure factotum
Now, from within your $home/lib/profile you can load your password database into factotum:

```
cat /usr/$home/lib/pwddb > /mnt/factotum/ctl
```
Assuming your using [acme](http://man.9front.org/1/acme) (if your not using acme, get with the program... seriously!) save all of your sessions and then do a 'fshalt -r' in a terminal to reboot.

At this point, if you open a mothra session, you should be using a proxy for http requests.  You can also test out hget from a terminal to see if that works too.  For the uninitiated, [hget](http://man.9front.org/1/hget) is similar to [wget](https://www.gnu.org/software/wget/) or [curl](https://curl.haxx.se/).

