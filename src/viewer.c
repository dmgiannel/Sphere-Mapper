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
 
#define OnWin 0
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#elif _WIN32
#include <GL/glew.h>
#include <GL/glut.h>
#undef OnWin
#define OnWin 1
#elif _WIN64
#include <GL/glew.h>
#include <GL/glut.h>
#undef OnWin
#define OnWin 1
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <png.h>
#include <string.h>
#include "tools.h"
#include "zpr.h"

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glClientActiveTexture(GL_TEXTURE0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex), vertices);
	glNormalPointer(GL_FLOAT, sizeof(struct vertex), normals);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct coord), texCoords);
	char info [500];
	getLatLong(info);
	glutSetWindowTitle(info);
	glDrawElements(GL_TRIANGLES, NTRI*3, GL_UNSIGNED_INT, triangles);
	glutSwapBuffers();
}

void keyPress(unsigned char key, int x, int y){
	//char ext[] = ".png";
	//char * title;
	char info[500];
	switch(key){
		case 'p': case 'P' :
#ifdef LIBPNG
			take_screenshot("img.png");
#else
			printf("no LIBPNG = no screenshot\n");
#endif //LIBPNG
			break;
		case 'l': case 'L' :
#ifdef LIBPNG
			getLatLong(info);
			take_screenshot(info);
#else
			printf("no LIBPNG = no screenshot\n");
#endif //LIBPNG
			break;
		case 'q' : case 'Q' :
			exit(0);
			break;
	}
}

void init(void)
{
	GLfloat mat_specular[] = {0, 0, 0, 0};
	GLfloat mat_shininess[] = {10.0};
 	GLfloat light_position[] = {0.0, 0.0, -10.0, 1.0};
 	GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
 	GLfloat lmodel_ambient[] = {0.05, 0.05, 0.05, 1.0};
 	glShadeModel(GL_SMOOTH);
 	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
 	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
 	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lmodel_ambient);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	char image[] = ("mars.png");
	char local[50];
	strcpy(local, "./maps/");
	strcat(local, image);
	int *width = malloc(sizeof(int));
	int *height = malloc(sizeof(int));
	textures[0] = loadTexture(local, width, height);
	add_tex_data();
	correct_tex_data();
	if(strcmp(image, "mars.png") == 0){
	char localheight[56];
	char imageheight[] = ("marsheight.png");
	strcpy(localheight, "./maps/");
	strcat(localheight, imageheight);
	int *hwidth = malloc(sizeof(int));
	int *hheight = malloc(sizeof(int));
	textures[1] = loadTexture(localheight, hwidth, hheight);
	do_height_data(textures[1], *hwidth, *hheight);
	apply_height_data();
	}
	if(light == 1){
	NNORM = NVERT;
	normals = malloc(sizeof(struct vertex)*NNORM);
	normIsSet = calloc(NNORM, sizeof(int));
	for(int i = 0; i<NTRI; i++)
		createNormals(i);
	}
	normNormals();
	FILE *fpo;
    fpo = fopen("normvsvert.txt", "a");
    fprintf(fpo,"#\n");
	for(int i=0;i<NTRI;i++){
    fprintf(fpo,"%d <%f, %f, %f>\n", i,vertices[triangles[i].i1].x, vertices[triangles[i].i1].y, vertices[triangles[i].i1].z);
	fprintf(fpo,"%d <%f, %f, %f>\n", i,vertices[triangles[i].i2].x, vertices[triangles[i].i2].y, vertices[triangles[i].i2].z);
	fprintf(fpo,"%d <%f, %f, %f>\n", i,vertices[triangles[i].i3].x, vertices[triangles[i].i3].y, vertices[triangles[i].i3].z);
    }
    fclose(fpo);
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hello");
	if(OnWin == 1)
	{
		GLenum err = glewInit();
		if(GLEW_OK != err)
		{
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}
	}
	zprInit();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}