#include <iostream>
#include <gL/glew.h>
#include <gL/freeglut.h>
#include <gL/glext.h>
#include "imageloader.h"
#pragma comment(lib, "glew32.lib")

using namespace std;

class Planet{
public:
	float radius, distance, orbit, orbitSpeed, axisTilt, axisAni;
	Planet(float _radius, float _distance, float _orbit, float _orbitSpeed, float _axisTilt, float _axisAni){
		radius = _radius;
		distance = _distance;
		orbit = _orbit;
		orbitSpeed = _orbitSpeed;
		axisTilt = _axisTilt;
		axisAni = _axisAni;
	}
	// لرسم مسار الدوران 
	void drawSmallOrbit(void){
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(90.0, 0.0, 0.0, 0.0);
		glutWireTorus(0.001, distance, 100.0, 100.0);
		glPopMatrix();
	}

	
};

//
Planet sun(3.5, 0, 0, 0, 0, 0);				//atom
Planet mer(0.5, 16, 0, 2.50, 02.11, 0);		//electron0
Planet ven(0.5, 16, 0, 2.70, 177.0, 0);		//electron1
Planet ear(0.5, 16, 0, 2.98, 23.44, 0);		//electron2
Planet mar(1.2, 21, 0, 2.41, 25.00, 0);		//electron3

int isAnimate = 0;
int bigOrbitActive = 1;
int smallOrbitActive = 1;
int moonsActive = 1;
int changeCamera = 0;
int frameCount = 0;
int labelsActive = 0;
int zoom = 50;
int logoScene = 1;

float lightPos[] = { 0.0, 0.0, -75.0, 1.0 }; // Spotlight position.
static float spotAngle = 40; // Spotlight cone half-angle.
float spotDirection[] = { 1.0, 0.0, 0.0 }; // Spotlight direction.
static float spotExponent = 1.0; // Spotlight exponent = attenuation factor.

GLuint loadTexture(Image* image) {
	//http://www.codeincodeblock.com/2012/05/simple-method-for-texture-mapping-on.html
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}

GLuint sunTexture, merTexture, venTexture, earTexture, marTexture, jupTexture, satTexture, uraTexture, nepTexture, pluTexture, staTexture, logTexture;

void writeBitmapString(void *font, char *string)
{
	char *c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void setup(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	//TEXUTRING SETUP
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	Image* sta = loadBMP("stars.bmp");		staTexture = loadTexture(sta);		delete sta;
	Image* sun = loadBMP("atom.bmp");		sunTexture = loadTexture(sun);		delete sun;
	Image* mer = loadBMP("elec.bmp");	merTexture = loadTexture(mer);		delete mer;
	Image* ven = loadBMP("elec.bmp");		venTexture = loadTexture(ven);		delete ven;
	Image* ear = loadBMP("elec.bmp");		earTexture = loadTexture(ear);		delete ear;
	Image* mar = loadBMP("elec.bmp");		marTexture = loadTexture(mar);		delete mar;
	

	//LIGHTING SETUP
	glEnable(GL_LIGHTING);
	float lightAmb[] = { 0.0, 1.0, 1.0, 1.0 };
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float globAmb[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotAngle);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);
}

void orbitalTrails(void){
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	
	glRotatef(45.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.001, ear.distance, 100.0, 100.0);
	glRotatef(45.0, 1.0, 0.0, 0.0);
	glRotatef(45.0, 1.0, 0.0, 0.0);

	glutWireTorus(0.001, ear.distance, 100.0, 100.0);
	glRotatef(45.0, 1.0, 0.0, 0.0);
	glRotatef(45.0, 1.0, 0.0, 0.0);
	glRotatef(45.0, 1.0, 0.0, 0.0);
	
	glutWireTorus(0.001, ven.distance, 100.0, 100.0);

	glPopMatrix();
}

void drawLogoScene(void){
	
}

void drawScene(void){
	frameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (changeCamera == 0)gluLookAt(0.0, zoom, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if (changeCamera == 1)gluLookAt(0.0, 0.0, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if (changeCamera == 2)gluLookAt(0.0, zoom, 0.00001, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	if (bigOrbitActive == 1) orbitalTrails();

	GLUquadric *quadric;
	quadric = gluNewQuadric();

	//atom
	glPushMatrix();
	glRotatef(sun.orbit, 0.0, 1.0, 0.0);
	glTranslatef(sun.distance, 0.0, 0.0);
	if (labelsActive == 1){
		glRasterPos3f(-1.2, 7.0, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "atom");
	}
	glPushMatrix();
	glRotatef(sun.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(sun.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sunTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, sun.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//electorn0
	glPushMatrix();
	glRotatef(mer.orbit, 1.0, 1.0, 0.0);
	glTranslatef(0, 0.0, mer.distance);
	if (labelsActive == 1){
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "electron");
	}
	glPushMatrix();
	glRotatef(mer.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(mer.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, merTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, mer.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//electorn1
	glPushMatrix();
	glRotatef(ven.orbit, 1.0, 1.0, 0.0);
	glTranslatef(0, 0.0, ven.distance);
	if (labelsActive == 1){
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "electron");
	}
	glPushMatrix();
	glRotatef(ven.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(ven.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, venTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ear.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//electorn2
	glPushMatrix();
	glRotatef(ear.orbit, 1.0, -1.0, 0.0);
	glTranslatef(0, 0.0, ear.distance);
	if (labelsActive == 1){
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "electron");
	}
	glPushMatrix();
	glRotatef(ear.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(ear.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, earTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ear.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();


	//electorn3
	glPushMatrix();
	glRotatef(mar.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ear.distance, 0.0, 0.0);
	if (labelsActive == 1){
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "electron");
	}
	glPushMatrix();
	glRotatef(mar.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(mar.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, marTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ear.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1){
		
	}
	if (moonsActive == 1){

	}
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, staTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBegin(GL_POLYGON);
	glTexCoord2f(-1.0, 0.0); glVertex3f(-200, -200, -100);
	glTexCoord2f(2.0, 0.0); glVertex3f(200, -200, -100);
	glTexCoord2f(2.0, 2.0); glVertex3f(200, 200, -100);
	glTexCoord2f(-1.0, 2.0); glVertex3f(-200, 200, -100);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, staTexture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-200, -83, 200);
	glTexCoord2f(8.0, 0.0); glVertex3f(200, -83, 200);
	glTexCoord2f(8.0, 8.0); glVertex3f(200, -83, -200);
	glTexCoord2f(0.0, 8.0); glVertex3f(-200, -83, -200);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glutSwapBuffers();
}

void drawScenesInOrder(void){
	if (logoScene == 1){
		drawScene();
	}
	else {
		drawScene();
	}
}

void resize(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

void animate(int n){
	if (isAnimate){
		mer.orbit += mer.orbitSpeed;
		ven.orbit += ven.orbitSpeed;
		ear.orbit += ear.orbitSpeed;
		mar.orbit += mar.orbitSpeed;
		if (mer, ven, ear, mar.orbit > 360.0){
			mer, ven, ear, mar.orbit -= 360.0;
		}
		mer.axisAni += 10.0;
		ven.axisAni += 10.0;
		ear.axisAni += 10.0;
		mar.axisAni += 10.0;
		if (mer, ven, ear, mar.axisAni > 360.0){
			mer, ven, ear, mar.axisAni -= 360.0;
		}
		glutPostRedisplay();
		glutTimerFunc(30, animate, 1);
	}
}

void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		if (logoScene) logoScene = 0;

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) exit(0);
	glutPostRedisplay();
}

void mouseWheel(int wheel, int direction, int x, int y)
{
	if (direction > 0 && zoom < 100) zoom++;
	if (direction < 0 && zoom > -75) zoom--;
	glutPostRedisplay();
}

void keyInput(unsigned char key, int x, int y){
	switch (key){
	case 27: exit(0); break;
	case ' ': if (isAnimate) isAnimate = 0; else{ isAnimate = 1; animate(1); } break;
	case 'o': if (smallOrbitActive) smallOrbitActive = 0; else smallOrbitActive = 1; glutPostRedisplay(); break;
	case 'O': if (bigOrbitActive) bigOrbitActive = 0; else bigOrbitActive = 1; glutPostRedisplay(); break;
	case 'm': if (moonsActive) moonsActive = 0; else moonsActive = 1; glutPostRedisplay(); break;
	case 'M': if (moonsActive) moonsActive = 0; else moonsActive = 1; glutPostRedisplay(); break;
	case 'l': if (labelsActive) labelsActive = 0; else labelsActive = 1; glutPostRedisplay(); break;
	case 'L': if (labelsActive) labelsActive = 0; else labelsActive = 1; glutPostRedisplay(); break;
	case '1': changeCamera = 0; glutPostRedisplay(); break;
	case '2': changeCamera = 1; glutPostRedisplay(); break;
	case '3': changeCamera = 2; glutPostRedisplay(); break;
	}
}

void intructions(void){
	
}

int main(int argc, char **argv){
	intructions();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(500, 0);
	glutCreateWindow("Atom simulation");
	glutDisplayFunc(drawScenesInOrder);
	glutReshapeFunc(resize);
	glutMouseFunc(mouseControl);
	glutKeyboardFunc(keyInput);
	glutMouseWheelFunc(mouseWheel);
	glewExperimental = GL_TRUE;
	glewInit();

	setup();
	glutMainLoop();
}