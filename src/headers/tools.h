/*Lambert cylindrical equal area projection viewer 
 *Version 0.2
 *June 2017
 *
 *David Giannella, School of Arts and Sciences
 *University of Rochester
 *dgiannel@u.rochester.edu
 *
 *Alice C. Quillen, School of Arts and Science
 *University of Rochester
 *alice.quillen@gmail.com
 *
 *These files may be redistributed and used in accordance with the 
 *GNU General Public License, which has been provided with Sphere-Mapper.
 */
 
#include <stdlib.h>
#include <stdio.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#elif _WIN32
#include <GL/glew.h>
#include <GL/glut.h>
#elif _WIN64
#include <GL/glew.h>
#include <GL/glut.h>
#else
#include <GL/glut.h>
#endif //__APPLE__
#include <math.h>
#include <png.h>

struct coord{
	double x;
	double y;
};

struct vertex {
	double x;
	double y;
	double z;
};

struct triangle{
	int i1;
	int i2;
	int i3;
};

GLuint textures[2];
GLuint framebuffer;

struct triangle *triangles;
extern int NTRI;

struct vertex *vertices;
extern int NVERT; 

struct coord *texCoords;
extern int NCOORD;

double *vertHeights;
extern int NHEIGHT;

struct latLongUp{
	double lat;
	double longi;
	double up;
};

int add_vertex(struct vertex);
int add_face(int i1, int i2, int i3);
int add_coord(double x, double y);
void add_tex_data();
void subdivide(struct vertex v1, struct vertex v2, struct vertex v3, int devh);
void normalize (struct vertex *v);
void create_sphere(int devh, double radius);
void correct_tex_data();
GLuint loadTexture(char *filename, int *width, int *height);
void keyPress(unsigned char key, int x, int y);
void take_screenshot(char *filename);
struct latLongUp getLLInfo(struct vertex cpos, struct vertex cup);
void getLatLong(char * info);
void changeR (struct vertex *v, double dr);
void do_height_data(GLuint texture, int width, int height);
void store_height_data(GLubyte *pixel);
int add_height(double dr);
void apply_height_data();
