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
#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
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
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_DOUBLE, sizeof(struct vertex), vertices);
	glTexCoordPointer(2, GL_DOUBLE, sizeof(struct coord), texCoords);
	char info [500];
	getLatLong(info);
	glutSetWindowTitle(info);
	glDrawArrays(GL_TRIANGLES, 0, NVERT);
	glutSwapBuffers();
}

void keyPress(unsigned char key, int x, int y){
	//char ext[] = ".png";
	//char * title;
	char info[500];
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
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	char image[] = ("mars.png");
	char local[50];
	strcpy(local, "./maps/");
	strcat(local, image);
	texture = loadTexture(local);
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