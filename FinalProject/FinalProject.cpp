/*
Miguel Santos
final project: Lithium atom
cs368
Dr. Xiang
Dec/2018
********************************+
User controls:					|
********************************+
	'a  key:	zoom in.
	'd' key:	zoom out.
	'p' key:	suspend and continue animation.
	'r' key:	toggle rotation, does not effect eletrons!

*/

#include "pch.h"
#include<iostream>
#include <gl/glut.h>
#include <math.h>
#include <cstdlib>

const double pi = 3.14159262;
const double orbitDistance = 1.7;
bool pauseAnimation = false;
bool pauseRotation = false;

GLdouble nuc_radius = .3;	// neutron and proton radii
GLdouble electron_radii = .1;	//electron radii

	//colors for object mats
GLfloat a_greenColor[] = { 0.0, .85, 0.1, 1.0 }; //Green 
GLfloat a_white[] = { 1.0, 1.0, 1.0, 1.0 }; //White 
GLfloat a_red[] = { 1.0, 0.0, 0.0, 1.0 }; //red 
GLfloat a_blue[] = { .179, .179, .718, 0.0 }; //blue

	// Set lighting intensity and color
GLfloat a_ambientLight[] = { .7f, .7f, 1.0f, 1.0f };
GLfloat a_diffusedLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat a_SpecularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat a_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
GLfloat SHININESS = 300.f;

// Start light and declare source position
GLfloat emitLight[] = { 0.9, 0.1, 0.9, 0.5 };
GLfloat lightPosition[] = { 5, 5, 3.5 };

static double zMovement = -5; //zooming in and out
static double orbits = 0.0; // rate of orbit of electrons

//methods used
void display(void);
void reshape(int x, int y);
void keyboard(unsigned char key, int x, int y);
void initLighting();
void nuc_neutron();
void nucleus();
void electron_orbit();
void orbitFunc(void);


int main(int argc, char **argv){
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(400, 300);
		glutCreateWindow("Lithium Atom Model");
		initLighting();
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutKeyboardFunc(keyboard);
		glutIdleFunc(orbitFunc); //orbit of electrons
		glutMainLoop();
		return 0;
}

void display(void){
		glMatrixMode(GL_MODELVIEW);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glEnable(GL_DEPTH_TEST);

		// manipulate 'zooming' 
		glTranslatef(0.0, 0.0, zMovement);

		if(pauseRotation != true) // toggle rotation, not electron orbit
			glRotatef(orbits*10, 0, 1, 0); // rotates at 10x orbit speed of electrons

		glPushMatrix();
		glRotatef(orbits, 0, 1, 0);
		electron_orbit();
		nucleus();
		glPopMatrix();

		glFlush();
		glutSwapBuffers();
}

void nuc_neutron() {
		// Set material properties
		glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, a_blue);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, a_greenColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, a_white);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, SHININESS);
		glutSolidSphere(nuc_radius, 60, 60);
		glPopMatrix();
}

void nuc_proton() {
		// Set material properties
		glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, a_red);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, a_white);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, a_white);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, SHININESS);
		glutSolidSphere(nuc_radius, 60, 60);
		glPopMatrix();

}

void electron() {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, a_greenColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, a_red);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, a_white);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, SHININESS);
		glutSolidSphere(electron_radii, 60, 60);
}

// create nucleus (proton and neutrons)
void nucleus() {
		nuc_neutron();

		glTranslatef(.3, -.25, .25);
		nuc_proton();

		glTranslatef(-.3, 0, -.3);
		nuc_neutron();

		glTranslatef(.25, .25, .25);
		nuc_proton();

		glTranslatef(0, -.3, -.3);
		nuc_neutron();
}

void electron_orbit() {
		glPushMatrix();
		glTranslatef(orbitDistance* cos(orbits*pi), 0, orbitDistance*sin(orbits*pi));
		electron();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-orbitDistance*cos((orbits+30)*pi), orbitDistance*cos((orbits+30)*pi), orbitDistance*sin(orbits*pi));
		electron();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-orbitDistance * sin((orbits + 30)*pi), orbitDistance*cos((orbits + 30)*pi), orbitDistance*sin(orbits*pi));
		electron();
		glPopMatrix();
}

void orbitFunc(void) {
	if (pauseAnimation != true) {
		orbits += .0025;
		if (orbits > 360)
			orbits -= 360;
		glutPostRedisplay();
	}
}

void reshape(int x, int y){
		if (y == 0 || x == 0) 
			return;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(39.0, (GLdouble)x / (GLdouble)y, 0.6, 21.0);
		glMatrixMode(GL_MODELVIEW);
		glViewport(0, 0, x, y);  //Use the whole window for rendering
}

void initLighting() {
		// Enable lighting
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		// Set lighting intensity and color
		glLightfv(GL_LIGHT0, GL_AMBIENT, a_ambientLight);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, a_diffusedLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, a_SpecularLight);

		// Set the light position
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void keyboard(unsigned char key, int x, int y) {
		switch (key) {
		case 'a':
			if (zMovement < 1)
				zMovement += .1;
			break;
		case 'd':
			if (zMovement > -10)
				zMovement -= .1;
			break;
		case 'p':
			pauseAnimation = !pauseAnimation; // toggles animation
			break;
		case 'r':
			pauseRotation = !pauseRotation;//
		}
}
