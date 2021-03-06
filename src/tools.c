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
#include <string.h>
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
int NHEIGHT = 0;
int NHmax = 0;
int NNORM = 0;
int NNmax = 0;
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
	if (i1==i2)	return -1;
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

int add_normal(struct vertex norm){
	while (NNmax<=NNORM){
       NNmax += 128;
       normals = realloc(normals,sizeof(struct vertex)*NNmax);
   }
   normals[NNORM] = norm;
   NNORM++;
   return NNORM -1;
}

void createNormals(int i){
	struct vertex norm1, norm2, norm3;
	int i1 = triangles[i].i1;
	int i2 = triangles[i].i2;
	int i3 = triangles[i].i3;
	struct vertex v1 = vertices[i1];
	struct vertex v2 = vertices[i2];
	struct vertex v3 = vertices[i3];
	struct vertex v12, v13, v23, v21, v32, v31;
	v12.x = v2.x - v1.x; v12.y = v2.y - v1.y; v12.z = v2.z - v1.z;
	v13.x = v3.x - v1.x; v13.y = v3.y - v1.y; v13.z = v3.z - v1.z;
	
	v21.x = v1.x - v2.x; v21.y = v1.y - v2.y; v21.z = v1.z - v2.z;
	v23.x = v3.x - v2.x; v23.y = v3.y - v2.y; v23.z = v3.z - v2.z;
	
	v31.x = v1.x - v3.x; v31.y = v1.y - v3.y; v31.z = v1.z - v3.z;
	v32.x = v2.x - v3.x; v32.y = v2.y - v3.y; v32.z = v2.z - v3.z;
	norm1.x = (v13.y)*(v12.z)-(v13.z)*(v12.y);
	norm1.y = (v13.x)*(v12.z)-(v13.z)*(v12.x);
	norm1.z = (v13.x)*(v12.y)-(v13.y)*(v12.x);
	norm2.x = (v21.y)*(v23.z)-(v21.z)*(v23.y);
	norm2.y = (v21.x)*(v23.z)-(v21.z)*(v23.x);
	norm2.z = (v21.x)*(v23.y)-(v21.y)*(v23.x);
	norm3.x = (v32.y)*(v31.z)-(v32.z)*(v31.y);
	norm3.y = (v32.x)*(v31.z)-(v32.z)*(v31.x);
	norm3.z = (v32.x)*(v31.y)-(v32.y)*(v31.x);
	if(normIsSet[i1] == 0){
		normals[i1] = norm1;
		normIsSet[i1] = 1;
	}
	else{
		normals[i1].x = (norm1.x + normals[i1].x);
		normals[i1].y = (norm1.y + normals[i1].y);
		normals[i1].z = (norm1.z + normals[i1].z);
	}
	if(normIsSet[i2] == 0){
		normals[i2] = norm2;
		normIsSet[i2] = 1;
	}
	else{
		normals[i2].x = (norm2.x + normals[i2].x);
		normals[i2].y = (norm2.y + normals[i2].y);
		normals[i2].z = (norm2.z + normals[i2].z);
	}
	if(normIsSet[i3] == 0){
		normals[i3] = norm3;
		normIsSet[i3] = 1;
	}
	else{
		normals[i3].x = (norm3.x + normals[i3].x);
		normals[i3].y = (norm3.y + normals[i3].y);
		normals[i3].z = (norm3.z + normals[i3].z);
	}
}

void normNormals(){
	for(int i = 0; i<NNORM; i++)
		normalize(&normals[i]);
}

//Add a uv coordinate, expanding list if necessary
int add_coord(float x, float y){
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

int add_height(float dr){
	while (NHmax<=NHEIGHT){
		NHmax += 128;
		vertHeights = realloc(vertHeights,sizeof(float)*NHmax);
	}
	vertHeights[NHEIGHT] = dr;
	NHEIGHT++;
	return NHEIGHT-1;
}


// creates a sphere of requested radius, subdivided from an octahedron via devh
// sphere will be in vertices in [imin, N-1]
// where imin is number of vertices before routine is called
// return half of mininum distance between vertices
// set radii of vertices to this value
// masses of all vertices set to 1
// velocities and accelerations zeroed
// triangular faces are created as part of the subdivide routine
void create_sphere(int devh, float radius){
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
   for(int i=0;i<NTRI;i++){
     fprintf(fpo,"%d\n %d\n %d\n %d\n",i,triangles[i].i1, triangles[i].i2, triangles[i].i3);
   }
   for(int i=0;i<NVERT;i++){
     fprintf(fpo,"%d\n %f\n %f\n %f\n",i,vertices[i].x, vertices[i].y, vertices[i].z);
   }
   fclose(fpo);*/


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
	   add_face(backCheckVertex(v1), backCheckVertex(v2), backCheckVertex(v3));
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

int backCheckVertex(struct vertex v){
	int i = NVERT - 1;
	for(i; i >= 0; i--)
		if(v.x == vertices[i].x && v.y == vertices[i].y && v.z == vertices[i].z)
			return i;
	return add_vertex(v);
}

// normalize vector (coordinates of particle)
void normalize (struct vertex *v){
  float r = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
  v->x /= r; v->y /= r; v->z /= r;
}

//change radial position of a vertex v by dr
//seemingly messed up ordering of x,y,z to accomodate opengl 3d coords
void changeR (struct vertex *v, float dr){
	float z = v->x;
	float x = v->y;
	float y = v->z;
	float r = sqrt(x*x + y*y + z*z);
	float theta = atan2(sqrt(x*x+y*y), z);
	float phi = atan2(y, x);
	r += dr;
	v->z = r*sin(theta)*cos(phi);
	v->x = r*sin(theta)*sin(phi);
	v->y = r*cos(theta);
}

//import texture from a png image and save width and height for potential later usage
//https://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures
GLuint loadTexture(char *filename, int *width, int *height) 
 {
	GLuint error = 0;
	//header for testing if it is a png
	png_byte header[8];
   
   //open file as binary
   FILE *fp = fopen(filename, "rb");
   if (!fp) {
	  printf("hi159");
     return error;
   }
   
   //read the header
   fread(header, 1, 8, fp);
   
   //test if png
   int is_png = !png_sig_cmp(header, 0, 8);
   if (!is_png) {
     fclose(fp);
	 printf("hi170");
     return error;
   }
   
   //create png struct
   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
       NULL, NULL);
   if (!png_ptr) {
     fclose(fp);
	 printf("hi179");
     return (error);
   }
   
   //create png info struct
   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr) {
     png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
     fclose(fp);
	 printf("hi188");
     return (error);
   }
 
   //create png info struct
   png_infop end_info = png_create_info_struct(png_ptr);
   if (!end_info) {
     png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
     fclose(fp);
	 printf("hi197");
     return (error);
   }
 
   //png error stuff, not sure libpng man suggests this.
   if (setjmp(png_jmpbuf(png_ptr))) {
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
	 printf("hi205");
     return (error);
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
   
   //get width and height for return
   *width = png_get_image_width(png_ptr, info_ptr);
   *height = png_get_image_height(png_ptr, info_ptr);
 
   // get info about png
   png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
       NULL, NULL, NULL);
	   
   // Update the png info struct.
   png_read_update_info(png_ptr, info_ptr);
 
   // Row size in bytes.
   int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
 
   // Allocate the image_data as a big block, to be given to opengl
	png_byte *image_data = NULL;
	image_data = malloc(sizeof(png_byte)*rowbytes*theight);
	int test = sizeof(png_byte)*rowbytes*theight;
	if (!image_data) {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
	 printf("hi242");
     return error;
	}
 
   //row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers;
	row_pointers = (png_bytep *) malloc(sizeof(png_bytep)*theight);
	if (!row_pointers) {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     free(image_data);
     fclose(fp);
	 printf("hi253");
     return error;
	}
   // set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < theight; ++i)
		row_pointers[theight - 1 - i] = image_data + i * rowbytes;
 
   //read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);
 
	//Now generate the OpenGL texture object
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
    if(color_type == 2)
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, twidth, theight, 0,
       GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) image_data);
	else
	 glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, twidth, theight, 0,
       GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
   //clean up memory and close stuff
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
   free(image_data);
   free(row_pointers);
   fclose(fp);
   return texture;
 }
 
 //create uv coordinates for each vertex
 void add_tex_data(){
	 float x;
	 float y;
	for(int i = 0; i < NVERT; i++){
		float x1 = vertices[i].x;
		float y1 = vertices[i].y;
		float z1 = vertices[i].z;
		x = 1 - (float)(M_PI - atan2(z1, x1))/(float)(2.0*M_PI);
		y = 1.0 - acos(y1/sqrt(x1*x1+y1*y1+z1*z1))/M_PI;
		add_coord(x, y);
	}
 }
 
 //correct uv coordinates for each vertex
 void correct_tex_data(){
	for(int i = 0; i < NTRI; i++){
	int fix = 0;
	float x1 = texCoords[triangles[i].i1].x;
 	float y1 = texCoords[triangles[i].i1].y;
 	float x2 = texCoords[triangles[i].i2].x;
 	float y2 = texCoords[triangles[i].i2].y;
 	float x3 = texCoords[triangles[i].i3].x;
 	float y3 = texCoords[triangles[i].i3].y;
 		if(fabs(x1 - x2) > .5){
 			if(x1 > .8)
 				x2 += 1;
 			else
 				x1 += 1;
			fix = 1;
 		}
 		if(fabs(x1 - x3) > .5){
 			if(x1 > .8)
 				x3 += 1;
 			else
 				x1 += 1;
			fix = 1;
 		}
 		if(fabs(x2 - x3) > .5){
 			if(x3 >.8)
 				x2 += 1;
 			else
 				x3 += 1;
			fix = 1;
 		}
 		if(fabs(y1 - y2) > .5){
 			if(y1 > .8)
 				y2 += 1;
 			else
 				y1 += 1;
			fix = 1;
 		}
 		if(fabs(y1 - y3) > .5){
 			if(y1 > .8)
 				y3 += 1;
 			else
 				y1 += 1;
			fix = 1;
 		}
 		if(fabs(y2 - y3) > .5){
 			if(y3 >.8)
 				y2 += 1;
 			else
 				y3 += 1;
			fix = 1;
 		}
 		
		if(fix == 1)
		{
 		struct coord c1, c2, c3;
 		c1.x = x1;
 		c1.y = y1;
 		c2.x = x2;
 		c2.y = y2;
 		c3.x = x3;
 		c3.y = y3;
 		struct vertex v1, v2, v3;
		v1.x = vertices[triangles[i].i1].x; v1.y = vertices[triangles[i].i1].y; v1.z = vertices[triangles[i].i1].z;
		v2.x = vertices[triangles[i].i2].x; v2.y = vertices[triangles[i].i2].y; v2.z = vertices[triangles[i].i2].z;
		v3.x = vertices[triangles[i].i3].x; v3.y = vertices[triangles[i].i3].y; v3.z = vertices[triangles[i].i3].z;
		triangles[i].i1 = add_vertex(v1);
		triangles[i].i2 = add_vertex(v2);
		triangles[i].i3 = add_vertex(v3);
 		add_coord(c1.x, c1.y);
		add_coord(c2.x, c2.y);
		add_coord(c3.x, c3.y);
		}
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
//longitude and forward azimuth fazi 
//(using up vector from center of image)
//fazi eqn: http://www.movable-type.co.uk/scripts/latlong.html
void getLatLong(char * info){
	float mod[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mod);
	float x = -mod[2];
	float y = -mod[6];
	float z = -mod[10];
	float longi = -atan2(z, x);
	float lat = -(atan2(sqrt(x*x+z*z),y) - M_PI/2);
	float ux = mod[1];
	float uy = mod[5];
	float uz =	mod[9];
	float longi2 = -atan2(uz, ux);
	float lat2 = -(atan2(sqrt(ux*ux+uz*uz),uy) - M_PI/2);
	float fazi = atan2(sin(longi2-longi)*cos(lat2) , cos(lat)*sin(lat2)
						-sin(lat)*cos(lat2)*cos(longi2-longi));
	fazi *= -180/M_PI;
	fazi = (int)(fazi+360)%360;
	sprintf(info, "lat%dlong%dfazi%d.png", (int)(lat*180/M_PI), (int)(longi*180/M_PI), (int)fazi);
	return;
}

void do_height_data(GLuint texture, int width, int height){
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	for(int i = 0; i<NCOORD; i++){
	GLubyte* pixel = malloc(sizeof(GLubyte) * 3);
	float xUnShift = texCoords[i].x;
	float yUnShift = texCoords[i].y;
	if(xUnShift > 1)
		xUnShift -= 1;
	if(yUnShift > 1)
		yUnShift -= 1;
	int xP = xUnShift*width;
	int yP = yUnShift*height;
	if(xP >= width)
		xP -= 1;
	if(yP >= height)
		yP -= 1;
	glReadPixels(xP, yP, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	store_height_data(pixel, h_mult);
	}
	GLsizei attach[1] = {GL_COLOR_ATTACHMENT0};
	GLuint toDel[1] = {1};
	glDeleteFramebuffers(1, toDel);
	//glInvalidateFramebuffer(GL_FRAMEBUFFER, 1, &attach[0]);
}

void store_height_data(GLubyte *pixel, int m){
	float maxIncr = .00867; //based on distance between highest and lowest points on surface
	float dr = (float)pixel[0]/255.0 * maxIncr - .00241;
	add_height(m*dr);
}

void apply_height_data(){
	for(int i = 0; i < NHEIGHT; i++)
		changeR(&vertices[i], vertHeights[i]);
}


