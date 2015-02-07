#!/usr/bin/python
import pyglet
from anim3d import *

#3d projection setup func
def setup_gl(dims):
    global angle

    glMatrixMode(GL_PROJECTION)
    glPushMatrix()
    glLoadIdentity()
    gluPerspective(20, float(dims[0])/dims[1], 0.1, 100)
    glMatrixMode(GL_MODELVIEW)
    glPushMatrix()
    glLoadIdentity()
    gluLookAt(0, -20, 1, 0, 0, 0, 0, 1, 0)

#dummy timer func
def tfunc(dt):
    pass

#Texture related stuff
img = pyglet.image.load('bm_01.png')
tex = img.get_texture()

#model related stuff
model = Model('guy.txt')
obj = AnimObject()
obj.setModel(model)
obj.setAction("Idle")

#windowing/pyglet stuff
w = pyglet.window.Window(640, 480)
clock = pyglet.clock.schedule_interval(tfunc, 1/30.0)
fps_display = pyglet.clock.ClockDisplay()

#opengl init stuff
glClearColor(0.2, 0.2, 0.2, 1)
glEnable(GL_DEPTH_TEST)

@w.event
def on_draw():
    w.clear()
    setup_gl(w.get_size())
    glEnable(GL_TEXTURE_2D)
    glBindTexture(GL_TEXTURE_2D, tex.id)
    obj.renderObject()

    glPushMatrix();
    glTranslatef(-5, 2, 0);
    obj.renderObject();
    glTranslatef(10, 2, 0);
    obj.renderObject();
    glTranslatef(-5, 2, 0);
    obj.renderObject();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D)
    obj.cur_frame += 1
    glMatrixMode(GL_PROJECTION)
    glPopMatrix()
    glMatrixMode(GL_MODELVIEW)
    glPopMatrix()
    fps_display.draw()    
     
pyglet.app.run()
