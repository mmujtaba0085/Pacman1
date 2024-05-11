#include <GL/glut.h>

#include<unistd.h>
#include <math.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <png.h>
#include <stdlib.h>
#include <vector>
#include <fstream>

pthread_mutex_t lock;
//headers
#include "Texture.h"
#include "maze.h"
#include "pac.h"
#include "ghost.h"

Pac pac;

pthread_t thr;

void display() {
//pthread_mutex_lock(&lock);
glClear(GL_COLOR_BUFFER_BIT);
drawMaze();
pac.draw();

glutSwapBuffers();
glutPostRedisplay();
//pthread_mutex_unlock(&lock);
}

void keyboard(int key, int xx, int yy) {
pthread_mutex_lock(&lock);
switch (key) {
case GLUT_KEY_RIGHT:
pac.nexDirect=RIGHT;
break;
case GLUT_KEY_LEFT:
pac.nexDirect=LEFT;
break;
case GLUT_KEY_UP:
pac.nexDirect=UPWARD;
break;
case GLUT_KEY_DOWN:
pac.nexDirect=DOWNWARD;
break;
}
//glutPostRedisplay();
pthread_mutex_unlock(&lock);
}
void initOpenGL() {
pthread_mutex_lock(&lock);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0, 300, 0, 300);
loadAndBindTextures();      
pthread_mutex_unlock(&lock);
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(512, 512);
    
    glutCreateWindow("OpenGL Setup Test");
    
    
    

    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);


    initOpenGL();
    pthread_create(&thr,NULL,Pacmove,(void *)&pac);
    glutMainLoop();
return 0;
}