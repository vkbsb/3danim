bl_info = {
    "name": "Export 3DAnim Format(.txt)",
    "author": "V.Vamsi Krishna(vkrishna)",
    "version": (1, 0),
    "blender": (2, 64, 0),
    "api": 40000,
    "location": "File > Export > 3DAnim (.txt)",
    "description": "Export 3dAnim (.txt)",
    "warning": "",
    "category": "Import-Export"}

import bpy
from bpy.props import *
from bpy_extras.io_utils import ExportHelper
import math

def getFrameNumbers(act):
    """
    Function to return the keyframes given the
    action object. We assume and use only one fcurve.
    """
    fc = act.fcurves[0]
    kf = []
    for key in fc.keyframe_points:
        kf.append(math.ceil(key.co[0]))
    return kf

def call_back(operator, context, filename):
    if not(filename.endswith(".txt")):
        filename = filename + ".txt"    
    fp = open(filename, "w")
    mesh_objs =  context.selected_objects
    mesh_obj = mesh_objs[0]
    modifier = mesh_obj.modifiers[0] #get modifier associated with object
    armature_obj = context.scene.objects[modifier.name]
    
    mesh = mesh_obj.to_mesh(context.scene, True, 'RENDER')
    num_verts = len(mesh.vertices)
    num_faces = len(mesh.polygons)
    actions = bpy.data.actions #get all actions from scene. Assume one obj per scene.
    uv_layer = mesh.uv_layers.active.data
    #start writing the model faces 
    print (num_verts, num_faces, len(actions))
    fp.write("%d %d %d\n" % (num_verts, num_faces, len(actions)))

    for (i,face) in enumerate(mesh.polygons):
        buff = ""
        for v in face.vertices:
            print (v, )
            buff += v.__str__() + " "
        print ("\n",)
        fp.write(buff+"\n")
        for loop_index in range(face.loop_start, face.loop_start + face.loop_total):
            uv = uv_layer[loop_index].uv
            fp.write('%.3f %.3f\n' % (uv[0], uv[1]))
    #remove the mesh created above.
    bpy.data.meshes.remove(mesh)                

    #get the actions devined on the armature
    #get their keyframes and print the vertex coords
    #do a linear interpolation of the vertieces
    for act in actions:
        armature_obj.animation_data.action = act
        keyframes = getFrameNumbers(act)
        print (act.name, len(keyframes))
        fp.write(act.name + " %d" % len(keyframes) + "\n")
        for frame in keyframes:
            context.scene.frame_set(frame)
            print (frame)
            fp.write("%d\n" % frame)
            mesh = mesh_obj.to_mesh(context.scene, True, 'RENDER')
            for vert in mesh.vertices:
                fp.write('%.3f %.3f %.3f\n' % (vert.co[0], vert.co[1], vert.co[2]))
            bpy.data.meshes.remove(mesh)                
    #finished writing animation data             
    fp.close() #close file
    return {'FINISHED'}

class Export3DAnimModel(bpy.types.Operator, ExportHelper):
    bl_idname = "filename.txt"
    bl_label = "3DAnim Model (.txt)"
    filename_ext = ".txt"
    
    def execute(self, context):
        if not self.filepath:
            raise Exception("Filepath not set")
        return call_back(self, context, self.filepath)
    
    def invoke(self, context, event):
        if not self.filepath:
            self.filepath = bpy.path.ensure_ext(bpy.data.filepath, ".txt")
        wm = context.window_manager
        wm.fileselect_add(self)
        return {'RUNNING_MODAL'}           
    
def menu_func(self, context):
    self.layout.operator(Export3DAnimModel.bl_idname)
    
def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)
    
def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)
    
if __name__ == "__main__":
    register()

