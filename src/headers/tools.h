#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
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
GLuint loadTexture(char *filename, int width, int height);
void displayKey(unsigned char key, int x, int y);
void take_screenshot(char *filename);
struct latLongUp getLLInfo(struct vertex cpos, struct vertex cup);
//struct vertex getUpVector();
//struct vertex getCameraPos();
void getLatLong(char * info);
//char *printLatLong(struct vertex v);
