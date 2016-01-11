//
//  SeaLayer.h
//  ramboat
//
//  Created by li on 15/6/5.
//
//

#ifndef __ramboat__SeaLayer__
#define __ramboat__SeaLayer__

#include "cocos2d.h"

USING_NS_CC;

class GameScene;

struct WaterVertex
{
    GLfloat x,y,z;
    
    GLfloat u,v;
    
    unsigned char r,g,b,a;
    
    GLfloat topV;
};


/*
typedef GLfloat CGFloat;
struct CGPoint
{
    CGFloat x;
    CGFloat y;
};
*/
class SeaNode : public Ref
{
public:
    //struct CGPoint position;
    cocos2d::Point position;
    
    float frequency;
    
    SeaNode *nextNode;
};

class HelloWorld;

class SeaLayer : public Layer
{
public:
    static SeaLayer* seaWithHeight(float height);
    
    SeaLayer();
    
    ~SeaLayer();
    
    void update(GLfloat dt);
    
    float tideAmplitude();
    
    void setTideAmplitude(float amplitude);
    
    float tideSpeed();
    
    void setTideSpeed(float speed);
    
    float seaHeightWithX(float x);
    
    float seaAngleWithX(float x);
    
    void setWaterColor(Color3B color);
    
    void setWaterLineOpacity(int opacity);
    
    float seaHeightDefault();
    
    HelloWorld *_testLayer;
    
	void onDraw2(const Mat4 &transform, uint32_t flags);
    
private:
    bool initWithHeight(float height);
    
    GameScene* gameScene();
    
    float getPTMRatio();
    
    void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
    void onDraw(const Mat4 &transform, uint32_t flags);
    //void draw();
    
    int _numNodes;
    
    float _minX;
    
    float _maxX;
    
    float _stepX;
    
    float _defaultHeight;
    
    float _speed;
    
    float _amplitude;
    
    cocos2d::Vector<SeaNode*> _nodes;
    
    Color3B _waterColor;
    
    int _waterLineOpacity;
    
	int _numPolygonVertices;

	WaterVertex *_polygonVertices;
    
	int _numLineVertices;

	WaterVertex *_lineVertices;
    
    Texture2D *_refractionTexture;
    
    GLint _refractionLocation;
    
    RenderTexture *_rt;
    
    Sprite *_sprite;
    
    CustomCommand _customCommand;
};

#endif /* defined(__ramboat__SeaLayer__) */
