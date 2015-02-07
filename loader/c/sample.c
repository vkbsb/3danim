#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "anim3d.h"
#include <stdio.h>

struct Model *m; //global model
struct Obj obj;
float angle = 0;

void timer(int dt)
{
    obj.cur_frame += 2; 
    glutTimerFunc(60, timer, 0);
    glutPostRedisplay();
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderObject(&obj);
    glFlush();
    glutSwapBuffers();
}
#include <stdio.h>
#include <stdlib.h>
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40, (float)w/(float)h, 0.1f, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, -20, 10, 0, 0, 0, 0, 1, 0);
}

void procEvents(int id)
{
    if(id < 0)
    {
        freeModel(m);
        exit(0);
    } 
    else //asuming this is only called when other menu items are pressed.
    {
        setAction(&obj, m->acts[id].name);
    }
}

/*create a menu with different animations present in the model*/
void createMenu()
{
    int i = 0;
    int sub_menu = glutCreateMenu(procEvents); 
    for(i = 0; i < m->num_acts; i++)
    {
        glutAddMenuEntry(m->acts[i].name, i); 
    }
    glutCreateMenu(procEvents);
    glutAddSubMenu("Animations", sub_menu);
    glutAddMenuEntry("Exit", -1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Usage: ./output modelfile.txt\n");
        exit(0);
    }
    else
    {
        m = obj.model = loadModel(argv[1]);
        setAction(&obj, m->acts[0].name);
        obj.cur_frame = 2; //all the animations created start with the first frame as 1 so add 1 to that.
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Anim Loader");
    createMenu();
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutTimerFunc(100, timer, 0);
    //glutIdleFunc(display);
    glutReshapeFunc(resize);
    glutMainLoop();
    freeModel(m);

	return 0;
}
