//------------------------------------------------------------------------
//
//	Layer3D - Use cocos2d-x to render simple 3D objects.
//
//	Licensed under the BSD license, see LICENSE in root for details.
// 
//	Copyright (c) 2012 James Hui (a.k.a. Dr.Watson)
// 
//	For latest updates, please visit http://cn.cocos2d-x.org/bbs
//
//------------------------------------------------------------------------

#include "Layer3D.h"


Layer3D::Layer3D()
{
    object.cur_frame = 0;
}


Layer3D::~Layer3D()
{
    if(object.model){
        freeModel(object.model);
    }
}


// on "init" you need to initialize your instance
bool Layer3D::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

		mTexture = CCTextureCache::sharedTextureCache()->addImage("cubetex.jpg");
        std::string str =  CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("puzzlecube.txt");
        //std::string str =  CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("guy.txt");
        
		CCGLProgram* program = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture);
		setShaderProgram(program);

		CCScheduler *s = CCDirector::sharedDirector()->getScheduler();
		s->scheduleUpdateForTarget(this, 0, false);

        mTimer = 0;
		mSpeed = 80;

		mYaw = 0;
		mPitch = 90.0f;
		mRoll = 0;

        bRet = true;
        
        CCLog(str.c_str());
        object.model = loadModel(str.c_str());
        if(!object.model){
            bRet = false;
            break;
        }
        //set current animation
        setAction(&object, "Idle");

    } while (0);

    return bRet;
}

float t = 0;
void Layer3D::update(float dt)
{
	if (dt > 0.0167f)
		dt = 0.0167f;

	mYaw += dt*mSpeed;
	mPitch += dt*mSpeed;
	mRoll += dt*mSpeed;

	mTimer += dt;
/*    if(mTimer > t){
        object.cur_frame +=1;
        t = mTimer + 0.05;
    }*/
    object.cur_frame +=1;
}


void Layer3D::draw()
{
	CCDirector::sharedDirector()->setDepthTest(true);

	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );

	getShaderProgram()->use();

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	//float scale = size.width/480.0f;

	kmMat4 matrixP;
    kmMat4 matrixMV;
    kmMat4 matrixMVP;

    kmGLGetMatrix(KM_GL_PROJECTION, &matrixP );
//    kmScalar fovY, kmScalar aspect, kmScalar zNear, kmScalar zFar
    kmMat4PerspectiveProjection(&matrixP, 10.0f, size.width/size.height, 0.1f, 100.0f);
    
    kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV );

	kmQuaternion quat;
	kmQuaternionRotationYawPitchRoll(&quat, mYaw, mPitch, mRoll);

	kmMat3 rotation;
	kmMat3RotationQuaternion(&rotation, &quat);

	kmVec3 translation;
	kmVec3Fill(&translation, size.width/2, size.height/2.0f, 200);

	kmMat4 rotationAndMove;
	kmMat4RotationTranslation(&rotationAndMove, &rotation, &translation);

    kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
	kmMat4Multiply(&matrixMVP, &matrixMVP, &rotationAndMove);				// apply rotation and translation to the matrix

	GLuint matrixId = glGetUniformLocation(getShaderProgram()->getProgram(), "u_MVPMatrix");
    getShaderProgram()->setUniformLocationwithMatrix4fv(matrixId, matrixMVP.mat, 1);

	// texture for the box	
    ccGLBindTexture2D( mTexture->getName() );

    //draw the 3danim object
    renderObject(&object);
    
    CC_INCREMENT_GL_DRAWS(1);
}


