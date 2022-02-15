#include <windows.h>
#include <glut.h>
#include<gl.h>
#include<iostream>

#define t_body 0
#define t_head 1
#define t_ear 2
#define t_tail 3


GLdouble rot[3];
GLfloat sc = 0.5;
POINT mouseDownPt;
int drag;
GLuint tex[4];
const char* texFiles[4] = { "asset/pokemon_body.bmp","asset/pokemon_head.bmp","asset/pokemon_ear.bmp" ,"asset/pokemon_tail.bmp" };


void reshape(int w, int h) {
	GLfloat fAspect;
	
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	fAspect = (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(35.0, fAspect, 1.0, 90.0);
	glEnable(GL_DEPTH_TEST);              // so the renderer considers depth
}

void LoadBMP()
{
	FILE* file;
	unsigned int dataPos;
	unsigned int size;
	unsigned int width, height;
	

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(4, tex);
	for (int i = 0; i < 4; i++) {
		GLubyte* data;
		unsigned char header[54];

		file = fopen(texFiles[i], "rb");

		fread(header, 1, 54, file);

		dataPos = *(int*)&(header[0x0A]);
		size = *(int*)&(header[0x22]);
		width = *(int*)&(header[0x12]);
		height = *(int*)&(header[0x16]);

		if (size == NULL)
			size = width * height * 3;
		if (dataPos == NULL)
			dataPos = 54;

		data = new unsigned char[size];

		fread(data, 1, size, file);

		fclose(file);
		
		glBindTexture(GL_TEXTURE_2D, tex[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

		delete []data;
	}
}

void display(void) {
	GLUquadricObj* pObj;

	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	gluLookAt(0.0, 0.0, -5.0, 0.0, 0.0, 0.0, 0, 1, 0);

	glRotatef(rot[0], 1.0, 0.0, 0.0);
	glRotatef(rot[1], 0.0, 1.0, 0.0);
	glRotatef(rot[2], 0.0, 0.0, 1.0);
	glScalef(sc, sc, sc);
	glRotatef(-90, 1.0, 0.0, 0.0);

	LoadBMP();


	glPushMatrix();
		pObj = gluNewQuadric(); 
		gluQuadricNormals(pObj, GLU_SMOOTH);
		gluQuadricDrawStyle(pObj, GLU_FILL);
		gluQuadricTexture(pObj, TRUE);
		glTranslatef(0.0, 0.0,-1.0);
		

		//머리
		glBindTexture(GL_TEXTURE_2D, tex[t_head]);
		glPushMatrix();
			glRotatef(180.0, 0.0, 0.0, 1.0);
			glTranslatef(0.0, 0.0, 1.0);
			glScalef(1.16, 1.16, 1.0);
			gluSphere(pObj, 0.58, 26, 13);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, tex[t_body]);
		//몸통
		glPushMatrix();
			gluCylinder(pObj,0.6,0.4,0.75,26,13);
			gluDisk(pObj, 0.0, 0.6, 26, 13);
		glPopMatrix();

		//발
		glPushMatrix();
			glTranslatef(-0.25, 0.0, -0.2);
			glScalef(1.0, 1.5, 1.0);
			gluCylinder(pObj, 0.25, 0.1, 0.2, 26, 13);

			gluDisk(pObj, 0.0, 0.24, 26, 13);

			glTranslatef(0.0, 0.0, 0.2);
			gluDisk(pObj, 0.0, 0.1, 26, 13);

			glTranslatef(0.5, 0.0, -0.2);
			gluCylinder(pObj, 0.25, 0.1, 0.2, 26, 13);

			gluDisk(pObj, 0.0, 0.24, 26, 13);
			
			glTranslatef(0.0, 0.0, 0.2);
			gluDisk(pObj, 0.0, 0.1, 26, 13);
		glPopMatrix();

		//팔1
		glPushMatrix();
			glTranslatef(0.45, 0.3, 0.4);
			glRotatef(70.0, 0.0, 0.0, 1.0);
			glScalef(4.0, 1.0, 1.0);
			gluSphere(pObj, 0.15, 26, 13);
		glPopMatrix();

		//팔2
		glPushMatrix();
			glTranslatef(-0.45, 0.3, 0.4);
			glRotatef(-70.0, 0.0, 0.0, 1.0);
			glScalef(4.0, 1.0, 1.0);
			gluSphere(pObj, 0.15, 26, 13);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, tex[t_tail]);
		//꼬리1
		glPushMatrix();
			glRotatef(180.0, 0.0, 1.0, 0.0);
			glTranslatef(0.0, -0.5, -0.2);	
			glRotatef(30.0, 1.0, 0.0, 0.0);
			glScalef(1.0, 3.0, 1.0);
			glutSolidCube(0.15);
		glPopMatrix();

		//꼬리2
		glPushMatrix();
		glRotatef(180.0, 0.0, 1.0, 0.0);
		glTranslatef(0.0, -0.73, -0.26);
		glRotatef(-25.0, 1.0, 0.0, 0.0);
		glScalef(1.0, 1.5, 1.0);
		glutSolidCube(0.15);
		glPopMatrix();

		//꼬리3
		glPushMatrix();
		glRotatef(180.0, 0.0, 1.0, 0.0);
		glTranslatef(0.0, -0.9, -0.3);
		glRotatef(-25.0, 1.0, 0.0, 0.0);
		glScalef(1.0, 2.0, 1.0);
		glutSolidCube(0.15);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, tex[t_body]);
		//꼬리4
		glPushMatrix();
		glRotatef(180.0, 0.0, 1.0, 0.0);
		glTranslatef(0.0, -1.35, -0.35);
		glRotatef(65.0, 1.0, 0.0, 0.0);
		glScalef(1.0, 2.5, 5.0);
		glutSolidCube(0.15);
		glPopMatrix();

		//귀1
		glBindTexture(GL_TEXTURE_2D, tex[t_ear]);
		glPushMatrix();
			glTranslatef(0.3, 0.0, 1.5); 
			glRotatef(30.0, 0.0, 1.0, 0.0);
			glRotatef(180.0, 0.0, 0.0, 1.0);
			glScalef(1.0, 1.0, 4.5); 
			gluSphere(pObj, 0.10, 26, 13);
		glPopMatrix();

		//귀2
		glPushMatrix();
			glTranslatef(-0.3, 0.0, 1.5);
			glRotatef(-30.0, 0.0, 1.0, 0.0);
			glRotatef(180.0, 0.0, 0.0, 1.0);
			glScalef(1.0, 1.0, 4.5);
			gluSphere(pObj, 0.1, 26, 13); 
		glPopMatrix();

	glPopMatrix();
	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			drag = 1;
			mouseDownPt.y = y;
			mouseDownPt.x = x;
		}
		else
			drag = 0;
	}
}

void mouse_move(int x, int y) {
	if (drag) {
		rot[0] += (y - mouseDownPt.y) / 3.6;
		rot[1] += (x - mouseDownPt.x) / 3.6;
		glutPostRedisplay();
		mouseDownPt.y = y;
		mouseDownPt.x = x;
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'a':
			if(sc<4.0)
				sc += 0.05;
		break;
		case 'z':
			if(sc>0.1)
				sc -= 0.05;
		break;
	}
	glutPostRedisplay();
}
void setupLight() {
	GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat yellow[] = { 1.0, 1.0, 0.5, 1.0 };
	GLfloat direction[] = { 1.0, 1.0, 1.0, 0.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 70);

	glLightfv(GL_LIGHT0, GL_AMBIENT, black);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_POSITION, direction);

	glEnable(GL_LIGHTING);  
	glEnable(GL_LIGHT0);    
	glEnable(GL_DEPTH_TEST);     
}

void deletetex() {
	glDeleteTextures(4, tex);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("project_20182602");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_move);
	glutKeyboardFunc(keyboard);
	setupLight();
	glutMainLoop();
	deletetex();
	return 0;
}