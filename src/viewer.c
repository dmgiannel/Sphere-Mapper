#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __APPLE__
#include <GLUT.glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <png.h>
#include <string.h>
#include "tools.h"
#include "zpr.h"
#define depth 6
#define TEXTURES 0
#define NUM_BUFFERS 1
#define LIBPNG 1

GLuint texture;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture);
	glClientActiveTexture(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_DOUBLE, sizeof(struct vertex), vertices);
	//glIndexPointer(GL_UNSIGNED_INT, sizeof(struct triangle), triangles);
	//glEnableClientState(GL_INDEX_ARRAY);
	glNormalPointer(GL_DOUBLE, sizeof(struct vertex), vertices);
	glTexCoordPointer(2, GL_DOUBLE, sizeof(struct coord), texCoords);
	char * info = malloc(sizeof(int)*20);
	getLatLong(info);
	glutSetWindowTitle(info);
	glDrawArrays(GL_TRIANGLES, 0, NVERT);
	glutSwapBuffers();
	
}

void keyPress(unsigned char key, int x, int y){
	//char ext[] = ".png";
	//char * title;
	char * info = malloc(sizeof(int)*20);
	switch(key){
		case 'p':
#ifdef LIBPNG
			take_screenshot("img.png");
#else
			printf("no LIBPNG = no screenshot\n");
#endif //LIBPNG
			break;
		case 'l':
#ifdef LIBPNG
			getLatLong(info);
			take_screenshot(info);
#else
			printf("no LIBPNG = no screenshot\n");
#endif //LIBPNG
			break;
	}
}

void init(void)
{
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};
	GLfloat light_position[] = {0.0, 0.0, -4.0, 1.0};
	GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat lmodel_ambient[] = {0.1, 0.1, 0.1, 1.0};
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	texture = loadTexture("./maps/mars.png", 2048, 1024);
	add_tex_data();
	correct_tex_data();
	glutKeyboardFunc(keyPress);
}

void reshape(int w, int h) 
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(90, (GLfloat)w/(GLfloat)h, 0.0, 0.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main(int argc, char** argv)
{
	create_sphere(depth, 1);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hello");
	zprInit();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}