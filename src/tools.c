#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <png.h>
#include "zpr.h"
#include "tools.h"
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif //M_PI

int NTmax = 0;
int NVmax = 0;
int NTRI = 0;
int NVERT = 0;
int NCmax = 0;
int NCOORD = 0;
unsigned char* 	image = NULL;

//add vertex to the list, expand list if necessary
int add_vertex(struct vertex v){
	while (NVmax<=NVERT){
       NVmax += 128;
       vertices = realloc(vertices,sizeof(struct vertex)*NVmax);
   }
   vertices[NVERT] = v;
   NVERT++;
   return NVERT -1;
}

//makes a recording of three indices which define a triangle
//and adds it to a list
int add_face(int i1, int i2, int i3){
	if (i1==i2) return -1;
	if (i2==i3) return -1;
	if (i3==i1) return -1;
	struct triangle tt;
		tt.i1 = i1;
		tt.i2 = i2;
		tt.i3 = i3;
	while (NTmax<=NTRI){
		NTmax += 128;
		triangles = realloc(triangles,sizeof(struct triangle)*NTmax);
	}
	triangles[NTRI] = tt;
	NTRI++;
	return NTRI -1;
}

int add_coord(double x, double y){
	struct coord c;
		c.x = x;
		c.y = y;
	while (NCmax<=NCOORD){
		NCmax += 128;
		texCoords = realloc(texCoords,sizeof(struct coord)*NCmax);
	}
	texCoords[NCOORD] = c;
	NCOORD++;
	return NCOORD -1;
}


// creates a sphere of requested radius, subdivided from an octahedron via devh
// sphere will be in vertices in [imin, N-1]
// where imin is number of vertices before routine is called
// return half of mininum distance between vertices
// set radii of vertices to this value
// masses of all vertices set to 1
// velocities and accelerations zeroed
// triangular faces are created as part of the subdivide routine
void create_sphere(int devh, double radius){
   struct vertex v0, v1, v2, v3, v4, v5;

// make an octahedron
		v0.x = 1; v0.y = 0; v0.z = 0; add_vertex(v0);//i=0 
		v1.x =-1; v1.y = 0; v1.z = 0; add_vertex(v1);//i=1
		v2.x = 0; v2.y = 1; v2.z = 0; add_vertex(v2);//i=2
		v3.x = 0; v3.y =-1; v3.z = 0; add_vertex(v3);//i=3
		v4.x = 0; v4.y = 0; v4.z = 1; add_vertex(v4);//i=4
		v5.x = 0; v5.y = 0; v5.z =-1; add_vertex(v5);//i=5
    // there are 6 vertices, but 8 faces
		subdivide(v4, v2, v0, devh);
		subdivide(v4, v0, v3, devh);
		subdivide(v4, v3, v1, devh);
		subdivide(v4, v1, v2, devh);
		subdivide(v5, v0, v2, devh);
		subdivide(v5, v3, v0, devh);
		subdivide(v5, v1, v3, devh);
		subdivide(v5, v2, v1, devh);

   // sphere has now been created with radius 1

   // rescale sphere to its desired radius
   for (int i=0;i<NVERT;i++)  {
		vertices[i].x *= radius; 
		vertices[i].y *= radius; 
		vertices[i].z *= radius; 
   }

/* for checking triangles
   FILE *fpo;
   fpo = fopen("tri.txt", "a");
   fprintf(fpo,"#\n");
   for(int i=itri;i<NTRI;i++){
     fprintf(fpo,"%d %d %d %d\n",i,triangles[i].i1, triangles[i].i2,triangles[i].i3);
   }
   fclose(fpo);
*/

}

// subdivide a triangular face
// i1, i2, i3 are particle indices in particle list, making a triangle
// indices are offset by imin
// if devh is zero add a triangle to index list otherwise subdivide
// recursive call to self
// devh should always be positive
// for sphere starting at imin, indexing for i's are done with respect to imin
// but vertices are stored with indexing j's including offset imin
void subdivide(struct vertex v1, struct vertex v2, struct vertex v3, int devh){
   if (devh==0){ // add triangles to index list
	   add_face(add_vertex(v1), add_vertex(v2), add_vertex(v3));
       return;
   }
   // compute midpoints of edges
   struct vertex v12, v23, v31;
   v12.x = (v1.x + v2.x);
   v23.x = (v2.x + v3.x);
   v31.x = (v3.x + v1.x);
   v12.y = (v1.y + v2.y);
   v23.y = (v2.y + v3.y);
   v31.y = (v3.y + v1.y);
   v12.z = (v1.z + v2.z);
   v23.z = (v2.z + v3.z);
   v31.z = (v3.z + v1.z);
   normalize(&v12);
   normalize(&v23);
   normalize(&v31);

// add the vertices to the particle list
// add_vertex returns index of added vertex
   subdivide(v1,v12,v31, devh-1); // recursive call to self
   subdivide(v2,v23,v12, devh-1);
   subdivide(v3,v31,v23, devh-1);
   subdivide(v12,v23,v31,devh-1);
}

// normalize vector (coordinates of particle)
void normalize (struct vertex *v){
  double r = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
  v->x /= r; v->y /= r; v->z /= r;
}

//import texture from a png image
GLuint loadTexture(char *filename, int width, int height) 
 {
	GLuint ERROR = 0;
	//header for testing if it is a png
	png_byte header[8];
   
   //open file as binary
   FILE *fp = fopen(filename, "rb");
   if (!fp) {
	  printf("hi159");
     return ERROR;
   }
   
   //read the header
   fread(header, 1, 8, fp);
   
   //test if png
   int is_png = !png_sig_cmp(header, 0, 8);
   if (!is_png) {
     fclose(fp);
	 printf("hi170");
     return ERROR;
   }
   
   //create png struct
   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
       NULL, NULL);
   if (!png_ptr) {
     fclose(fp);
	 printf("hi179");
     return (ERROR);
   }
   
   //create png info struct
   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr) {
     png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
     fclose(fp);
	 printf("hi188");
     return (ERROR);
   }
 
   //create png info struct
   png_infop end_info = png_create_info_struct(png_ptr);
   if (!end_info) {
     png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
     fclose(fp);
	 printf("hi197");
     return (ERROR);
   }
 
   //png error stuff, not sure libpng man suggests this.
   if (setjmp(png_jmpbuf(png_ptr))) {
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
	 printf("hi205");
     return (ERROR);
   }
 
   //init png reading
   png_init_io(png_ptr, fp);
   
   //let libpng know you already read the first 8 bytes
   png_set_sig_bytes(png_ptr, 8);
 
   // read all the info up to the image data
   png_read_info(png_ptr, info_ptr);
 
   //variables to pass to get info
   int bit_depth, color_type;
   png_uint_32 twidth, theight;
 
   // get info about png
   png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
       NULL, NULL, NULL);
 
   //update width and height based on png info
   width = twidth;
   height = theight;
 
   // Update the png info struct.
   png_read_update_info(png_ptr, info_ptr);
 
   // Row size in bytes.
   int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
 
   // Allocate the image_data as a big block, to be given to opengl
   png_byte image_data[rowbytes * height]; 
   if (!image_data) {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
	 printf("hi242");
     return ERROR;
   }
 
   //row_pointers is for pointing to image_data for reading the png with libpng
   png_bytep row_pointers[height];
   if (!row_pointers) {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     free(&image_data);
     fclose(fp);
	 printf("hi253");
     return ERROR;
   }
   // set the individual row_pointers to point at the correct offsets of image_data
   for (int i = 0; i < height; ++i)
     row_pointers[height - 1 - i] = image_data + i * rowbytes;
 
   //read the png into image_data through row_pointers
   png_read_image(png_ptr, row_pointers);
 
   //Now generate the OpenGL texture object
   GLuint texture;
   glGenTextures(1, &texture);
   glBindTexture(GL_TEXTURE_2D, texture);
   glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, width, height, 0,
       GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) image_data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
   //clean up memory and close stuff
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
   //free(image_data);
   //free(row_pointers);
   fclose(fp);
   return texture;
 }
 
 //create uv coordinates for each vertex
 void add_tex_data(){
	 double x;
	 double y;
	for(int i = 0; i < NVERT; i++){
		double x1 = vertices[i].x;
		double y1 = vertices[i].y;
		double z1 = vertices[i].z;
		x = 1 - (double)(M_PI - atan2(z1, x1))/(double)(2.0*M_PI);
		y = 1.0 - acos(y1/sqrt(x1*x1+y1*y1+z1*z1))/M_PI;
		
		add_coord(x, y);
	}
 }
 
 //correct uv coordinates for each vertex
 void correct_tex_data(){
	for(int i = 0; i < NTRI; i++){
		double x1 = texCoords[triangles[i].i1].x;
		double y1 = texCoords[triangles[i].i1].y;
		double x2 = texCoords[triangles[i].i2].x;
		double y2 = texCoords[triangles[i].i2].y;
		double x3 = texCoords[triangles[i].i3].x;
		double y3 = texCoords[triangles[i].i3].y;
		if(fabs(x1 - x2) > .5){
			if(x1 > .8)
				x2 += 1;
			else
				x1 += 1;
		}
		if(fabs(x1 - x3) > .5){
			if(x1 > .8)
				x3 += 1;
			else
				x1 += 1;
		}
		if(fabs(x2 - x3) > .5){
			if(x3 >.8)
				x2 += 1;
			else
				x3 += 1;
		}
		if(fabs(y1 - y2) > .5){
			if(y1 > .8)
				y2 += 1;
			else
				y1 += 1;
		}
		if(fabs(y1 - y3) > .5){
			if(y1 > .8)
				y3 += 1;
			else
				y1 += 1;
		}
		if(fabs(y2 - y3) > .5){
			if(y3 >.8)
				y2 += 1;
			else
				y3 += 1;
		}
		
		struct coord c1, c2, c3;
		c1.x = x1;
		c1.y = y1;
		c2.x = x2;
		c2.y = y2;
		c3.x = x3;
		c3.y = y3;
		
		texCoords[triangles[i].i1] = c1;
		texCoords[triangles[i].i2] = c2;
		texCoords[triangles[i].i3] = c3;
	}
}

//take a png screenshot, output is "<filename>"
void take_screenshot(char *filename){
	//create file
	FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
       return;
    }
	//create struct
	png_structp png_ptr = png_create_write_struct
       (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    
	if (!png_ptr)
       return;
	
	//create info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
       png_destroy_write_struct(&png_ptr,
         (png_infopp)NULL);
       return;
    }
	//init output
	png_init_io(png_ptr, fp);
	
	GLint info[4];
	glGetIntegerv(GL_VIEWPORT, info);
	int w = info[2];
	int h = info[3];
	glReadBuffer(GL_BACK);
	if(image == NULL){
		image = calloc(w*h*3, sizeof(unsigned char));
	}
	png_byte* row_pointers[h];
	for (int i = 0; i < h; i++) {
		row_pointers[h-i-1] = (png_bytep) &image[w*3*i];
	}
	
	glReadPixels(0,0,w,h,GL_RGB,GL_UNSIGNED_BYTE,image);
	
	png_set_IHDR(png_ptr, info_ptr, w, h, 8, 
	PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	
	png_write_info(png_ptr, info_ptr);
	png_write_image(png_ptr, row_pointers);
	png_write_end(png_ptr, info_ptr);
	
	png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

	//close
	fclose(fp);
	
}

//Returns string indicating latitude,
//longitude and forward azimuth (using 
//up vector from center of image)
//fazi eqn: http://www.movable-type.co.uk/scripts/latlong.html
void getLatLong(char * info){
	GLdouble mod[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mod);
	double x = -mod[2];
	double y = -mod[6];
	double z = -mod[10];
	double longi = -atan2(z, x);
	double lat = -(atan2(sqrt(x*x+z*z),y) - M_PI/2);
	double ux = mod[1];
	double uy = mod[5];
	double uz =	mod[9];
	double longi2 = -atan2(uz, ux);
	double lat2 = -(atan2(sqrt(ux*ux+uz*uz),uy) - M_PI/2);
	double fazi = atan2(sin(longi2-longi)*cos(lat2) , cos(lat)*sin(lat2)
						-sin(lat)*cos(lat2)*cos(longi2-longi));
	fazi *= -180/M_PI;
	fazi = (int)(fazi+360)%360;
	sprintf(info, "lat%dlong%dfazi%d.png", (int)(lat*180/M_PI), (int)(longi*180/M_PI), (int)fazi);
	return;
}


