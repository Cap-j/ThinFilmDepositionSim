#pragma once
#define _USE_MATH_DEFINES
#include <glut.h>
#include <math.h>
#include <random>

using namespace std;

class Atom {
public:
	double x, y; //position, mass, velocity
	double angle;

	void initialise() { // randomising starting position and later velocity
		random_device rd; // obtain a random number from hardware
		mt19937 gen(rd()); // seed the generator
		uniform_int_distribution<> distr(10, 170);

		double j = distr(gen);
		double k = 0.94f;
		double a = (j - 90) / 100;
		double b = k;
		x = a;
		y = b;
		return;
	}

	void bounceAngle() {
		double a = 180 * (M_PI / 180) - (360 * (M_PI / 180) - this->angle);
		this->angle = 360 * (M_PI / 180) - (180 * (M_PI / 180) - (360 * (M_PI / 180) - this->angle));
	}

	void generateAngle() {
		random_device rd; // obtain a random number from hardware
		mt19937 gen(rd()); // seed the generator
		
		normal_distribution<double> distr(2700, 200);

		double ang = distr(gen);
		ang = ang/10;
		this->angle  = ang* (M_PI / 180);
	}

	void draw() {
		glPointSize(2.5);
		glColor3f(0, 255, 0);
		glBegin(GL_POINTS);
		glVertex2f(this->x, this->y);
		glEnd();
	}
};
