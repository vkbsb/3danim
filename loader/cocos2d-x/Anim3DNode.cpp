//
//  3dAnimNode.cpp
//  3danim
//
//  Created by vamsi krishna on 25/05/13.
//
//

#include "Anim3DNode.h"
#include <cocos2d.h>
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

Anim3DNode::Anim3DNode()
:CCNode(),
m_AnimRate(1),
m_CurAnimation(""),
m_pTexture(NULL),
m_LoopCurrentAnim(true)
{
    
}

Anim3DNode* Anim3DNode::createWithFile(char *fname)
{
    CCLog("++Anim3DNode::createWithFile");
    Anim3DNode *ptr = new Anim3DNode();
    CCAssert(ptr != NULL, "Could not allocate memory for animNode");
    if(ptr->initWithFile(fname)){
        ptr->autorelease();
    CCLog("--Anim3DNode::createWithFile");
        return ptr;
    }else{
        delete ptr;
    CCLog("--Anim3DNode::createWithFile");
        return NULL;
    }
}

bool Anim3DNode::initWithFile(char *fname)
{
    CCLog("++Anim3DNode::initWithFile");
    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
    CCLog("++set shader program");
    
    setAnchorPoint(ccp(0.5, 0));

    std::string texname = fname;
    CCLog("+finding tex Name");
    if (std::string(".txt").compare(&fname[strlen(fname)-4]) == 0){
        texname.replace(strlen(fname)-3, 3, std::string("png"));
    }else{
        texname += ".png";
    }
    CCLog("-finding tex Name");

    CCLog("+Loading  TexName: %s", texname.c_str());
    m_pTexture = CCTextureCache::sharedTextureCache()->addImage(texname.c_str());
    CCAssert(m_pTexture!=NULL,"Texture should not be NULL");
    
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(fname);
    unsigned long size = 0;
    unsigned char * pBytes = CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "rb", &size);
    m_AnimObject.model  = loadModelFromCharArray((char*)pBytes);

    m_pTexture->retain();
    
    CCLog("--Anim3DNode::initWithFile");
    return true;
}

void Anim3DNode::playAnimation(char *animName, bool loop, int offset)
{
    CCLog("+playing animation %s",animName);
    m_CurAnimation = animName;
    m_AnimObject.cur_frame = offset;
    CCLog("++Setting action");
    setAction(&m_AnimObject, animName);
    CCLog("--Setting action");
    float x1, y1, x2, y2;
    CCLog("+getBounds");
    getBounds(&m_AnimObject, &x1, &y1, &x2, &y2);
    CCLog("-getBounds");
    setContentSize(CCSize(x2-x1, y2-y1));
    m_LoopCurrentAnim = loop;
    CCLog("-playing animation %s",animName);
}

Anim3DNode::~Anim3DNode()
{
    m_pTexture->release();
    if(m_AnimObject.model){
        freeModel(m_AnimObject.model);
    }
}


void Anim3DNode::addListener(Anim3DNodeListener *listener)
{
    std::vector<Anim3DNodeListener*>::iterator it = m_ListenerList.begin();
    while(it != m_ListenerList.end()){
        if(*it == listener){
            return;
        }
        ++it;
    }
    m_ListenerList.push_back(listener);
}

void Anim3DNode::removeListener(Anim3DNodeListener *listener){
    std::vector<Anim3DNodeListener*>::iterator it = m_ListenerList.begin();
    while(it != m_ListenerList.end()){
        if(*it == listener){
            m_ListenerList.erase(it);
            return;
        }
        ++it;
    }
}

void Anim3DNode::draw()
{
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );

    CC_NODE_DRAW_SETUP();
    
	getShaderProgram()->use();
    if(m_CurAnimation.empty()){
        return;
    }
    Obj *obj = &m_AnimObject;
    int index = 0;
    int n_index = 1;
    
    
    m_AnimObject.cur_frame += m_AnimRate;
    while(obj->cur_frame > obj->act->kf[n_index].frame_num)
    {
        index++;
        n_index++;
        if(n_index == obj->act->num_frames)
        {
            for(int i = 0; i < m_ListenerList.size(); i++){
                m_ListenerList[i]->onAnimationFinished(m_CurAnimation);
            }
            if(m_LoopCurrentAnim){
                index = 0;
                n_index = 1;
                obj->cur_frame = obj->act->kf[index].frame_num;
                break;
            }else{
                obj->cur_frame -= m_AnimRate;
                index--;
                n_index--;
                break;
            }
        }
    }

    
    if(m_pTexture){
        ccGLBindTexture2D( m_pTexture->getName() );
    }else{
        ccGLBindTexture2D(0);
    }
    glEnable(GL_DEPTH_TEST);
    ccGLBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    
    //draw the 3danim object
    renderObject(&m_AnimObject, index, n_index);
    
    glDisable(GL_DEPTH_TEST);
    
    CC_INCREMENT_GL_DRAWS(1);
}