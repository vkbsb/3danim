#ifndef __NEWFORMAT_H__
#define __NEWFORMAT_H__

#ifdef DEBUG
#include <stdio.h>
#define DEBUG_PRINT fprintf
#else
#define DEBUG_PRINT(...)
#endif


/*struct Face //asssuming triangles
{
    int indices[3];
    float uv[3 * 2];
};*/

/*each key frame is defined by the frame number and
 * the vertex coords assosicated with it.*/
struct KeyFrame
{
    int frame_num;
    float *verts;
};

/*each action has a name and keyframes associated with it.*/
struct Action
{
    char name[256];
    int num_frames;
    struct KeyFrame *kf;
};

struct Model
{
    int num_faces;
    int num_vert;
    int num_acts;
    struct Action *acts;
    //Faces of the model stored just as indices instead of faces.
    //This helps speeding up the rendering process
    int *indices;
    float *uv;
};

struct Obj
{
    struct Model *model;
    struct Action *act;
    int cur_frame;
};

struct Model * loadModel(char *filename);
void freeModel(struct Model*);
void setAction(struct Obj *obj, char *actname);
void renderObject(struct Obj *obj);
#endif //
