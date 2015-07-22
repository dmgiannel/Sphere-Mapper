

Sphere-Mapper - A more natural way to view maps
------------------------------------------------
.. image:: http://i.imgur.com/94XUZ3O.png 
.. image:: http://i.imgur.com/YeN7AwE.png 
.. image:: http://i.imgur.com/m5C0KII.png

GENERAL INFO
------------------------------------------------
Sphere-Mapper is a small program in C that takes maps in Lambert Cylindrical Equal-Area format and "unprojects" them so that the user can view the more natural looking spherical, 3D map. The user may rotate around using the mouse, zoom in and take pictures with either no data or coordinate data to identify the location and direction from which it was taken. At all times, the viewer will be locked onto the center of the celestial body being viewed.

PREREQUISITES
------------------------------------------------
To run Sphere-mapper, you will first need to have the ability to compile programs in the X Windows format. On Windows, an example that should suffice for this is Cygwin (includes an Xserver in installation if you choose it during setup). On Macs, termainal with Xcode should be capable of this. In addition to a compiler, you will need a number of libraries to cover the functions within Sphere-Mapper.These are:

Zlib (required for libpng, so install it first): http://www.zlib.net/

Libpng: http://www.libpng.org/pub/png/libpng.html

GLUT libraries - see below

OPENGL AND GLUT VIEWER
------------------------------------------------
Obviously, to display anything some sort of graphics library is necessary, and Sphere-Mapper uses OpenGL. You will need the OpenGL Utility Library (GLU) to ensure recognition of the OpenGL functions in the program. Furthermore, you will need to have the GLUT library as well, to recognize the window frawing functions. On Windows, one could acquire this through the old official GLUT libraries available in a number of places, but it is recommended to instead install freeglut, which is an open source, up to date version of the GLUT libraries. It can be found here: http://freeglut.sourceforge.net/. Once you have installed these prerequisites through your compiler, you should be ready to go.

USING SPHERE-MAPPER
------------------------------------------------
Once you have set up the compiler and prereqs, you can start converting maps (Lambert Cylindrical equal area projections only - for now? Who knows!) to spherical, orthographic representations. First you will of course need a map to use, though. I have placed examples in the maps folder in the main directory of the program, which includes, among other maps, the moon, mars and enceladus. As long as you use pngs and LCEA (Lambert cylindrical equal area) projections, they should be mapped correctly. Be careful with these kinds of maps for earth, though, as the poles are often cut out, which causes incorrect mapping. To change maps, at the moment you must go into the viewer.c file, go to the init function, and change the image character array (char image[] = <your map here.png>) to the deisre map. Do not worry about the directory, the next few lines of code make sure it looks in the maps folder. Then you can save the file and compile the program and view whatever map you like, so long as you do not exceed the stack size. I will make it a priority to prompt the user to enter this information at prgoram startup so they don't have to fish around in the code in later versions but this is sufficient for now. 

When ready, make sure you are in the Sphere-mapper-master folder (or whatever you may have renamed it to) in your command line, and then enter "make" to compile. Once this completes, if there are no errors you can enter "./viewer" to start the .exe. Upon startup, you should see your planet in the window, centered on Lat0Long90Fazi0 (These coordinates update live in the window title). You can drag the mouse with the left button depressed to rotate around the body, and pressing both mouse buttons and dragging will zoom in and out (don't go too far though, it's somewhat buggy. You can back out though and it will be fine again.). Pressing 'p' will take a screenshot with no coordinate data and output it as img.png. PRessing 'l' will output an image whose title will be the latittude, longitude and foward azimuth information so that you can get a bearing on where you were when you took the shot. When you are finished, you can close the window by pressing q or Q. And that should be it!

Credits
------------------------------------------------
The Sphere-Mapper has been developed by David Giannella and Professor Alice C. Quillen at the University of Rochester. The rotation and other viewing capabilities is made possible by gltzpr (zpr.c and zpr.h), developed by Nigel Stewart at RMIT University. 





