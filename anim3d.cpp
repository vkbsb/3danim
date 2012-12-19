#include <cocos2d.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "anim3d.h"

void setAction(struct Obj *obj, const char *actname)
{
    int i = 0;
    obj->act = NULL;
    for(i = 0; i < obj->model->num_acts; i++)
    {
        struct Action *a = obj->model->acts+i;
        if(strcmp(actname, a->name) == 0)
        {
            obj->act = a;
            obj->cur_frame = a->kf[0].frame_num;
            break;
        }
    } 
    if(obj->act == NULL)
    {   
        obj->act = obj->model->acts;
    }
}

void renderObject(struct Obj *obj)
{
    int i = 0;
    int n_index = 0; 
    int index = 0;
    float *verts = NULL; 
    float *verts1 = NULL;
    float t = 0; //0 is first frame will be drawn.. 1 the second.. (keyframes)
    
    //3 coords per vertex and 3 vertices per face.
    float *drawv = (float*)malloc(sizeof(float) * obj->model->num_faces * 3 * 3);
    if(drawv == NULL)
    {
        DEBUG_PRINT(stderr, "Could not allocate memory");
        return;
    }
    
    int *indx = obj->model->indices;
    float *uv = obj->model->uv;
    
    index = 0;
    n_index = 1;
    
    while(obj->cur_frame > obj->act->kf[n_index].frame_num)
    {
        index++;
        n_index++;
        if(n_index == obj->act->num_frames)
        {
            index = 0;
            n_index = 1;
            obj->cur_frame = obj->act->kf[index].frame_num;
            break;
        }
    }


    verts = obj->act->kf[index].verts; //get the vertex from the first key frame
    verts1 = obj->act->kf[n_index].verts; //get the vertex from the first key frame

    t = (obj->cur_frame - obj->act->kf[index].frame_num)/(float)(obj->act->kf[n_index].frame_num - obj->act->kf[index].frame_num);//obj->cur_frame); 
    //DEBUG_PRINT(stderr, "%d %d %d %f\n", obj->cur_frame, index, n_index, t);
    
    int num_indices = obj->model->num_faces * 3; //3 indices per face.
    for(i = 0; i < num_indices; i++)
    {
        int v_index = indx[i];
        
        drawv[i*3 + 0] = verts[v_index*3 + 0] + t*(verts1[v_index*3 + 0]-verts[v_index*3 + 0]);
        
        drawv[i*3 + 1] = verts[v_index*3 + 1] + t*(verts1[v_index*3 + 1]-verts[v_index*3 + 1]);
        
        drawv[i*3 + 2] = verts[v_index*3 + 2] + t*(verts1[v_index*3 + 2]-verts[v_index*3 + 2]);
    }
    
    
    glVertexAttribPointer(cocos2d::kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, drawv);    
    glVertexAttribPointer(cocos2d::kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, uv);
	glDrawArrays(GL_TRIANGLES, 0, obj->model->num_faces*3);
    
    free(drawv);
}


void freeModel(struct Model *m)
{
    int i, j;
    //free all the face information (indices, uv)
    free(m->indices);
    free(m->uv);
    
    for(i = 0; i < m->num_acts; i++)
    {
        for(j = 0; j < m->acts[i].num_frames; j++)
        {
            free(m->acts[i].kf[j].verts);
        }
        free(m->acts[i].kf);
    }
    free(m->acts);
}

struct Model * loadModel(const char *filename)
{
    FILE *fp = NULL;
    int face_offset = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    fp = fopen(filename, "r");
    if(fp == NULL)
        exit(1);
    struct Model *m = (struct Model*) malloc(sizeof(struct Model));
    fscanf(fp, "%d", &(m->num_vert));
    fscanf(fp, "%d", &(m->num_faces));
    fscanf(fp, "%d", &(m->num_acts));
    //allocate memory for the faces. i.e, allocate memory for the indices
    m->indices = (int *) malloc(sizeof(int) * 3 * m->num_faces);    //3 indices per triangle
    m->uv = (float*) malloc(sizeof(float) * 2 * 3 * m->num_faces);  //2 uv per vertex(index) per triangle
    DEBUG_PRINT(stderr, "%d %d %d\n", m->num_vert, m->num_faces, m->num_acts);
    for(i = 0; i < m->num_faces; i++)
    {
        fscanf(fp, "%d %d %d", &m->indices[i*3+0], &m->indices[i*3+1], &m->indices[i*3+2]);
        DEBUG_PRINT(stderr, "%d %d %d\n", m->indices[i*3+0], m->indices[i*3+1], m->indices[i*3+2]);
        for(face_offset = i * 6, j = 0; j < 3; j++)
        {
            fscanf(fp, "%f %f", &m->uv[face_offset+j*2], &m->uv[face_offset+j*2+1]);
            DEBUG_PRINT(stderr, "%.3f %.3f\n", m->uv[face_offset+j*2], m->uv[face_offset+j*2+1]);
        }
    }
    //allocate memory for the actions
    m->acts = (struct Action*) malloc(sizeof(struct Action) * m->num_acts);
    for(i = 0; i < m->num_acts; i++)
    {
        fscanf(fp, "%s %d", m->acts[i].name, &m->acts[i].num_frames);    
        DEBUG_PRINT(stderr, "%s %d\n", m->acts[i].name, m->acts[i].num_frames);    
        ///allocate memory for the keyframes
        m->acts[i].kf = (struct KeyFrame *) malloc(sizeof(struct KeyFrame) * m->acts[i].num_frames);
        for(j = 0; j < m->acts[i].num_frames; j++)
        {
            float * verts = (float *) malloc(sizeof(float) * 3 * m->num_vert); 
            fscanf(fp, "%d", &m->acts[i].kf[j].frame_num);
            DEBUG_PRINT(stderr, "%d\n", m->acts[i].kf[j].frame_num);
            //m.acts[i].kf[j].verts //
            for(k = 0; k < m->num_vert; k++)
            {
                fscanf(fp, "%f %f %f", verts+k*3, verts+k*3+1, verts+k*3+2);   
                DEBUG_PRINT(stderr, "%.3f %.3f %.3f\n", verts[k*3], verts[k*3+1], verts[k*3+2]);   
            }
            m->acts[i].kf[j].verts = verts;
        }
        
    }
    fclose(fp);    
    return m;
}
