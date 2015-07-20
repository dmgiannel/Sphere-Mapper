/*Lambert cylindrical equal area projection viewer 
 *Version 0.1
 *July 2015
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
#else
#include <GL/glut.h>
#endif //__APPLE__
#include <math.h>

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

struct triangle *triangles;
extern int NTRI;

struct vertex *vertices;
extern int NVERT; 

struct coord *texCoords;
extern int NCOORD;

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
GLuint loadTexture(char *filename);
void displayKey(unsigned char key, int x, int y);
void take_screenshot(char *filename);
struct latLongUp getLLInfo(struct vertex cpos, struct vertex cup);
//struct vertex getUpVector();
//struct vertex getCameraPos();
void getLatLong(char * info);
//char *printLatLong(struct vertex v);
