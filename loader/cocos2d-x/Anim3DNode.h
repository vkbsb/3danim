//
//  3dAnimNode.h
//  3danim
//
//  Created by vamsi krishna on 25/05/13.
//
//

#ifndef ___danim___dAnimNode__
#define ___danim___dAnimNode__

#include <iostream>
#include "anim3d.h"
#include "cocos2d.h"

USING_NS_CC;

class Anim3DNodeListener
{
public:
    virtual void onAnimationFinished(std::string s){
        
    }
    
/*    void onTimeEvent(std::string s, float time){
        
    }*/
};

class Anim3DNode: public CCNode
{
    Obj m_AnimObject;
    bool m_LoopCurrentAnim;
    std::vector<Anim3DNodeListener*> m_ListenerList;
    
    CC_SYNTHESIZE_READONLY(std::string, m_CurAnimation, currentAnimation)
    CC_SYNTHESIZE(char, m_AnimRate, AnimRate)
    CC_SYNTHESIZE(CCTexture2D *, m_pTexture, Texture)
    
public:
    Anim3DNode();
    ~Anim3DNode();
    virtual bool initWithFile(char *fname);
    //provide the relative path to .txt file. The texture has to be a png with same name as the txt file.
    static Anim3DNode * createWithFile(char *fname);
    
    void addListener(Anim3DNodeListener *listener);
    void removeListener(Anim3DNodeListener *listener);
    
    void playAnimation(char *animName, bool loop = true, int offset = 0);
    virtual void draw();
};


#endif /* defined(___danim___dAnimNode__) */
