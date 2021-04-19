#include <iostream>
#include <string>
#include <glut.h>
#include "atoms.h"
#include "Windows.h"
#include <time.h>
#include <glut.h>
#include <fstream>
#include <list>
using namespace std;
Atom ch[100];
float density; // density of plasma, higher number = less dense
float atomV;   // atom velocity
int sampleSize;
int iteration = 0;
int dens = 0;

ofstream angledata ("C:\\results\\angles.txt");
ofstream positiondata("C:\\results\\positions.txt");

//double endPoint[180];  // final position of atom bit
list <Atom> endPositionAtoms;

//why use void as argument?
void generateBase(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         //clear the color buffer (background)

    // draw a 1x1 Square centered at origin
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.95f, -0.95f);
    glVertex2f(-0.95f, 0.95f);
    glVertex2f(0.95f, 0.95f);
    glVertex2f(0.95f, -0.95f);
    glVertex2f(-0.95f, -0.95f);
    glEnd();

    // diffused plasma
    glPointSize(1);
    for (double j = -0.9; j < 0.9; j = j + density) {
        for (double k = -0.9; k < 0.9; k = k + density) {
            glBegin(GL_POINTS);
            glVertex2f(j, k);
            glEnd();
            dens++;
        }
    }

    for (Atom at : endPositionAtoms) {
        at.draw();
    }
}

void moveAtom(Atom* a) {
    a->x = a->x + cos(a->angle) * atomV;
    a->y = a->y + sin(a->angle) * atomV;
}

bool checkCollision(Atom a) {
    // check if collided with plasma
    for (double j = -0.9; j < 0.9; j = j + density) {
        for (double k = -0.9; k < 0.9; k = k + density) {
            double dx = a.x - j;
            double dy = a.y - k;
            double distance = sqrt(dx * dx + dy * dy);
            if (distance < 0.008) return true;
        }
    }

    // check if collided with wall
    double dx = a.x - 0.95f;
    double dy = a.y - 0.95f;
    if ((0.94 <= a.x && a.x <= 0.96) ||
        (-0.94 >= a.x && a.x >= -0.96) ||
        (0.94 <= a.y && a.y <= 0.96)) return true;

    return false;
}

bool checkSubstrateAtoms(Atom a1) {
    for (Atom a2 : endPositionAtoms) {
        double dx = a1.x - a2.x;
        double dy = a1.y - a2.y;
        double distance = sqrt(dx * dx + dy * dy);
        if (distance < 0.008) return true;
    }
    return false;
}

void display() {
    generateBase();
    for (int i = 0; i < 100; i++) {
        ch[i].draw();
    }
    glutSwapBuffers();
}

void iterate(int i) {
    glutPostRedisplay();
    glutTimerFunc(1, iterate, 0);
    for (int i = 0; i < 100; i++) {
        moveAtom(&ch[i]);

        if (checkCollision(ch[i])) {
            ch[i].bounceAngle();
            moveAtom(&ch[i]);
        }

        if (checkSubstrateAtoms(ch[i]) == true) {
            Atom newAtom;
            newAtom.x = ch[i].x;
            newAtom.y = ch[i].y + 0.01;
            endPositionAtoms.push_back(newAtom);
            angledata << ch[i].angle / (3.141593 / 180) << "\n";
            positiondata << ch[i].x << "\n";
            iteration++;
        }

        // atom at substrate
        if (-0.94 >= ch[i].y && ch[i].y >= -0.96) {
            // create atom for final position list
            Atom newAtom;
            newAtom.x = ch[i].x;
            newAtom.y = ch[i].y + 0.01;
            endPositionAtoms.push_back(newAtom);
            // collect final position data
            angledata << ch[i].angle / (3.141593 / 180) << "\n";
            positiondata << ch[i].x << "\n";

            // restart atom
            ch[i].generateAngle();
            ch[i].initialise();
            iteration++;
            if (iteration == sampleSize) while (true);
        }
    }
}

int main(int argc, char** argv) {

    density = 0.02;
    atomV = 0.01;
    sampleSize = 1000;

    for (int i = 0; i < 100; i++) {
        ch[i].initialise();
        ch[i].generateAngle();
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GL_DOUBLE);
    glutCreateWindow("Plasma Simulation");
    glutReshapeWindow(1000, 1000);
    glutDisplayFunc(display);
    glutTimerFunc(0, iterate, 0);
    glutMainLoop();  //enter the event-processing loop
    return 0;
}