// Copyright 2018 Event Vision Library.
#include <iostream>
#include <thread>
#include <vector>

#include "unistd.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <evl/core/read_buffer.hpp>
#include <evl/core/store_buffer.hpp>
#include <evl/core/types.hpp>
#include <evl/imgproc/detection.hpp>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define CAM_WIDTH 240
#define CAM_HEIGHT 180

#define GL_WINDOW_WIDTH 1200
#define GL_WINDOW_HEIGHT 800

cv::Rect roi(50, 100, 150, 80);  // (x, y, w, h), initialized
cv::Point vertex(50, 100);

int lifetime = 10000;     // micro sec
int buffersize = 50000;
evl::EventBuffer buffer(buffersize);

void setup(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    // Make big points and wide lines.  (This may be commented out if desired.)
    glPointSize(3);
    glLineWidth(3);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);  // Make round (not square) points
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);   // Antialias the lines
}


void draw(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    std::vector<evl::EventTuple> v = evl::readBufferOnLifetime(&buffer, lifetime);
    evl::detect_rod_tip(v, &roi, &vertex);

    glColor3f(1.0, 0.0, 0.0);

    float x = (vertex.x / static_cast<float>(CAM_WIDTH) * 2.0 - 1.0);
    float y = -(vertex.y / static_cast<float>(CAM_HEIGHT) * 2.0 - 1.0);

    glBegin(GL_LINES);
    glVertex2f(x,  -1.0);
    glVertex2f(x, 1.0);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(-1.0, y);
    glVertex2f(1.0, y);
    glEnd();
    /*
    float d = 0.01;
    glBegin(GL_LINE_LOOP);
    glVertex2f( x+d, y+d);
    glVertex2f( x-d, y+d);
    glVertex2f( x-d, y-d);
    glVertex2f( x+d, y-d);
    glEnd();
    */
    glFlush();

    glutPostRedisplay();
}

int initGlutDisplay(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT);
    glutCreateWindow("display");
    // glutReshapeFunc(resize);
    glutDisplayFunc(draw);
    setup();

    glutMainLoop();
    return 0;
}

int main(int argc, char * argv[]) {
    std::thread t1(evl::storeBufferDAVIS, &buffer);
    initGlutDisplay(argc, argv);
    t1.join();
    return 1;
}
