GRRLIB for Nintendo Gamecube
======


Table of Contents
-----------------

- [Introduction](#introduction)
 - ...What is it?
- [Developing for the Gamecube](#developing-for-the-wii)
 - ...How do I even start?
- [Downloading GRRLIB](#downloading-grrlib)
 - ...Where do I get it from?
- [Installing GRRLIB](#installing-grrlib)
 - ...How do I get it to a useable state?
- [Using GRRLIB](#using-grrlib)
 - ...What essentials do I need to know to get going?
- [Gamecube and Wii incompatability](#gamecube-and-wii-incompatability)
 - ...I upgraded and now my programs won't compile properly!?
- [Using GitHub](#using-github)
 - ...What is this Git thing that the L337 devs keep talking about?
- [Credits](#credits)
 - ...Who do I thank for all this free stuff?
- [Licence](#licence)
 - ...When you say "free" do you actually mean something else?


Introduction
------------

GRRLIB is a C/C++ 2D/3D graphics library for Wii application developers.  It is
essentially a wrapper which presents a friendly interface to the Nintendo GX
core.

This document is written to be viewed with equal clarity in either a web browser
or a text editor.

As of v4.1.0, GRRLIB is supplied as a standard C/C++ library (aka. archive)
called 'libgrrlib'.  Because GRRLIB processes JPEG and PNG images, it requires
the installation of the 'libjpeg' and 'libpngu' libraries.  'libpngu', in turn,
requires 'libpng' and 'libpng' requires 'libz'.  GRRLIB has FileIO functions
to allow real-time loading and saving of graphical data, and thus requires
'libfat'.  GRRLIB also has the possibility to use TrueType fonts, so
'libfreetype' is also required.

```
libgrrlib          <- 2D/3D graphics library
+-- libfat         <- File I/O
+-- libjpeg        <- JPEG image processor
+-- libpngu        <- Wii wrapper for libpng
    +-- libpng     <- PNG image processor
        +-- libz   <- Zip (lossless) compression (for PNG compression)
+-- libfreetype    <- TrueType font processor
```


Developing for the Gamecube
----------------------

Do not progress until you have installed and configured devkitPro.  Guides are
and assistance are available at http://forums.devkitpro.org

If you have just performed a clean (re)install on your Computer, be sure to
reboot before you continue.


Downloading GRRLIB for Gamecube
------------------

You are invited to use "the latest Git master version" of this fork at all times.

This document will presume that you have downloaded "the latest Git master
version" to a folder called grr, located on your desktop.

In order to add Gamecube support to GRRLIB, you'll need to use the latest Git master version of GRRLIB for Gamecube. Don't worry, you're there right now.

Installing GRRLIB for Gamecube
-----------------

This guide has been updated for Linux, Mac OS X, and Windows support.

GRRLIB      is supplied as source code
libjpeg     is supplied as source code
libpngu     is supplied as source code
libpng      is supplied as source code
libz        is supplied as source code
libfreetype is supplied as source code
libfat      is supplied with devkitpro (Ie. preinstalled)

The easy way is to install GRRLIB and all the required libraries in a single
command. 
On Windows, open cmd, by typing cmd in the windows search box and launching the first result. 
On Linux, launch your favorite terminal app. 
On Mac OS X, you'll find the Terminal app in /Applications/Utilities/

On Windows type the following, and press enter at the end of each line:
```
  cd Desktop\grr\GRRLIB
  make clean all install
```

On Linux/Mac OS X:
```
  cd ~/Desktop/grr/GRRLIB
  sudo make clean all install
```

This process may take some time depending on the speed of your Computer.

If you used the method above the following steps are not required, GRRLIB is
installed and you are ready to start developing Wii homebrew games.

If you want, you could install the libz, libpng, libpngu, libjpeg and
libfreetype libraries in a single command.
On Windows:
```
  cd Desktop\grr\GRRLIB\lib 
  make clean all install
```
On Linux/Mac OS X:
```
  cd ~/Desktop/grr/GRRLIB/lib 
  sudo make clean all install
```

Using GRRLIB
------------

After everything is installed, simply put
```c
    #include <grrlib.h>
```
at the top of your .c/.cpp file and use the functions as required

You will also need to add
```make
  -lgrrlib -lfreetype -lfat -ljpeg -lpngu -lpng -lz
```
to the libs line in your makefile

...Remember the order of the libraries is critical.  You may (need to) insert
other libraries in the middle of the list, you may need to add others to the
start, or even the end - but do _not_ change the order of the libraries shown
here.

You do NOT need to place /anything/ in the folder there your project source code sits

If you would like to see a working example of this, you can look at the example
found in: ~/Desktop/grr/examples/template/source


Gamecube and Wii Incompatability
----------------------------------------------------
Due to this modified version of GRRLIB being patched throughout in order to support the gamecube, having both the gamecube and wii version installed on the same computer has not been tested and is not supported. The external libraries, such as freetype, jpeg, png, etc that come with this version of GRRLIB are built with all of the Wii options stripped out, and as such they may or may not be functional on Wii.


Using GitHub
------------

GitHub allows the developers to submit changes to the code in such a way that
these changes can be easily monitored, quickly merged together with other
changes. and (if necessary) reverted.

It also allows the power-users to gain access to the latest (often "in-test")
features.

To obtain the "cutting edge" codebase for Gamecube GRRLIB go to https://github.com/capz/GRRLIB/tree/master

 1. Create a folder to hold the code (e.g. Desktop/grr)
 2. Click the *Download ZIP* button if you don't have the proper tools installed on your PC or Clone the following URL https://github.com/capz/GRRLIB.git

All official (Wii) releases can be found here https://github.com/GRRLIB/GRRLIB/releases
Unofficial Gamecube releases can be found here https://github.com/capz/GRRLIB/releases


Credits
-------
Official Wii GRRLIB: https://github.com/GRRLIB/GRRLIB#credits
Gamecube-patched fork: capz


Licence
-------

GRRLIB is released under [the MIT Licence](https://github.com/GRRLIB/GRRLIB/blob/master/LICENCE.TXT).
If we had chosen the GPL licence you would be +forced+ (legally required) to release your source code.
But in the spirit of "free as in FREE" we have left you with the +option+ to release your
source code.

GRRLIB do +request+ that you tell others about GRRLIB by naming it in
the credits of your game/application.  And, if you +choose+ to do that, we
encourage you to use the logo to achieve it; You can find the logo here:
http://grrlib.santo.fr/wiki/images/logo.png
