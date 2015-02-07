from pyglet.gl import *
import math

class Face:
    def __init__(self, ind, uv):
        self.indices = ind 
        self.uv = uv

class KeyFrame:
    def __init__(self, fp, nv):
        self.verts = []
        self.frame_num = int(fp.readline())
        for i in xrange(nv):
            (v1, v2, v3) = fp.readline().split(' ')
            self.verts.append(float(v1)) 
            self.verts.append(float(v2)) 
            self.verts.append(float(v3)) 

class Action:
    def __init__(self, fp, nv):
        (self.name, self.num_frames) = fp.readline().split(' ')
        self.num_frames = int(self.num_frames)
        self.kf = []
        for j in xrange(self.num_frames):
            k = KeyFrame(fp, nv)
            self.kf.append(k)
    
#function to initialize values in a list.
def init_uv(v):
    return -1

class Model:
    def __init__(self, fname):
        fp = open(fname)
        (self.nv, self.nf, self.na) = fp.readline().split(' ')
        self.nv = int(self.nv)
        self.nf = int(self.nf)
        self.na = int(self.na)
        self.indices = ()
        self.uv = range(0, 2*self.nv)
        self.uv = map(init_uv, self.uv)
        self.repeats = ()
        #load the model data
        for i in xrange(self.nf):
            line = fp.readline().rstrip(' \n')
            indx = map(int, line.split(' '))
            for j in xrange(3):
                (s, t) = map(float, fp.readline().split(' '))
                if(self.uv[int(indx[j])*2] < 0):
                    self.uv[int(indx[j])*2] = s
                    self.uv[int(indx[j])*2+1] = t
                elif math.fabs(self.uv[int(indx[j])*2]-s) < 0.0001 and math.fabs(self.uv[int(indx[j])*2+1]-t) < 0.0001:
                    self.repeats += (int(indx[j]),) 
                    self.uv += [s, t]
                    indx[j] = self.nv + (len(self.repeats)-1)
            self.indices += (indx[0], indx[1], indx[2])
        #load the animation data
        self.acts = []
        for i in xrange(self.na):
            act = Action(fp, self.nv)
            self.acts.append(act)
        fp.close()

        print "Model: ", fname
        print "Faces: %d Anims: %d" % (self.nf, len(self.acts))
        for act in self.acts:
            print act.name

def linear_interop(v, v1, t):
    return v + t*(v1-v)
    
class AnimObject:
    def __init__(self):
        self.model = None
        self.act = None
        self.cur_frame = 0

    def loadModel(self, fname):
        pass
   
    def setModel(self, model):
        self.model = model

    def setAction(self, actname):
        self.act = None
        for i in xrange(self.model.na):
            a = self.model.acts[i]
            if(actname == a.name):
                self.act = a
                self.cur_frame = a.kf[0].frame_num
        if(self.act == None):
            self.act = self.obj.model.acts[0]

    def renderObject(self):
        if(self.cur_frame > self.act.kf[self.act.num_frames-1].frame_num):
            self.cur_frame = self.act.kf[0].frame_num+1

        n_index = 0
        while (self.cur_frame > self.act.kf[n_index].frame_num):
            n_index += 1
        index = n_index - 1

        v = self.act.kf[index].verts
        v1 = self.act.kf[n_index].verts
        t = (self.cur_frame - self.act.kf[index].frame_num)/float(self.act.kf[n_index].frame_num - self.act.kf[index].frame_num)
       
        dv = map(linear_interop, v, v1, (t, ) * len(v))

        for re in self.model.repeats:
            dv += (dv[re*3], dv[re*3+1], dv[re*3+2])

        pyglet.graphics.draw_indexed(self.model.nv + len(self.model.repeats), pyglet.gl.GL_TRIANGLES, self.model.indices, ('v3f', dv), ('t2f', self.model.uv))            
        
