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
#include<chrono>


auto startTime = std::chrono::steady_clock::now();

auto animation = std::chrono::steady_clock::now();

pthread_mutex_t lock;
//headers
#include "Texture.h"
#include "maze.h"
#include "pac.h"
#include "ghost.h"



Pac pac;
Ghost red(initalRed_X,initailRed_Y,RED,0.1);
Ghost blue(initalBlue_X,initailBlue_Y,BLUE,0.1);
Ghost pink(initialPink_X,initialPink_Y,PINK,0.1);
Ghost yellow(initialYellow_X,initialYellow_Y,YELLOW,0.1);
pthread_t pacThread;
pthread_t RGhostThread;
pthread_t BGhostThread;
pthread_t PGhostThread;
pthread_t YBGhostThread;

void display() {
//pthread_mutex_lock(&lock);
glClear(GL_COLOR_BUFFER_BIT);
drawMaze();
if(lives>0)
{
pac.draw();
red.draw();
blue.draw();
yellow.draw();
pink.draw();
}
else
{
    glPushMatrix();
    translateBottomLeft();
    translateToMazeCoords(11.4f, 13.5f); // Translate to the correct area of the screen ready for drawing
    drawTex(gameover_tex,40, 8, 0);
    glPopMatrix();
}
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
    pthread_create(&pacThread,NULL,Pacmove,(void *)&pac);
    pthread_create(&RGhostThread,NULL,GnrlMov,(void*)&red);
    pthread_create(&BGhostThread,NULL,GnrlMov,(void*)&blue);
    pthread_create(&RGhostThread,NULL,GnrlMov,(void*)&pink);
    pthread_create(&BGhostThread,NULL,GnrlMov,(void*)&yellow);
    glutMainLoop();
return 0;
}