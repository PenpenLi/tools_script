//
//  SeaLayer.cpp
//  ramboat
//
//  Created by li on 15/6/5.
//
//
#include "HelloWorldScene.h"
#include <math.h>
#include "GameScene.h"
#include "SeaLayer.h"

#define PI 3.14159265
USING_NS_CC;

WaterVertex WaterVertexMake (GLfloat x, GLfloat y, GLfloat z,
                             GLfloat u, GLfloat v,
                             unsigned char r, unsigned char g, unsigned char b, unsigned char a,
                             GLfloat topV)
{
    WaterVertex vertex;
    vertex.x = x;
    vertex.y = y;
    vertex.z = z;
    vertex.u = u;
    vertex.v = v;
    vertex.r = r;
    vertex.g = g;
    vertex.b = b;
    vertex.a = a;
    vertex.topV = topV;
    return vertex;
}

SeaLayer* SeaLayer::seaWithHeight(float height)
{
    SeaLayer* seaLayer = new SeaLayer();
    
    if (seaLayer && seaLayer->initWithHeight(height))
    {
        seaLayer->autorelease();
        
        return seaLayer;
    }

    return nullptr;
}

bool SeaLayer::initWithHeight(float height)
{
    if (Layer::init())
    {
        this->setContentSize(Director::getInstance()->getOpenGLView()->getDesignResolutionSize());
        //this->setContentSize(Director::getInstance()->getWinSize());
        
        _numNodes = 20;
        
        _minX = 0.0f;
        
        _maxX = this->getContentSize().width;
        
        _stepX = (_maxX - _minX) / (_numNodes - 1);
        
        _defaultHeight = height;
        
        _speed = 0.0f;
        
        _amplitude = 5.0f;
        
        _waterColor = Color3B(100, 100, 255);
        //_waterColor = Color3B::RED;
        
        _waterLineOpacity = 128;
        
        _numPolygonVertices = (_numNodes -1) * 6;
        
        _polygonVertices = new WaterVertex [_numPolygonVertices + 50];
        
        _numLineVertices = (_numNodes - 1) * 2;
        
        _lineVertices = new WaterVertex [_numLineVertices + 50];
        
        for (int i = 0; i < _numNodes; i++)
        {
            SeaNode *node = new SeaNode();
            
            node->position = Vec2(_maxX - (i * _stepX), _defaultHeight);
            
            node->frequency = (2.0f * i) / _numNodes;
            
            node->nextNode = NULL;
            
            if (!_nodes.empty())
            {
                _nodes.back()->nextNode = node;
            }
            
            _nodes.pushBack(node);
        }
        
        if (this->gameScene()->isRenderTextureEnabled())
        {
			_refractionTexture	=	TextureCache::getInstance()->addImage("refraction-hd.png");
			_refractionTexture->retain();
            Texture2D::TexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
            _refractionTexture->setTexParameters(params);
            _refractionTexture->retain();
            
            GLProgram *shaderWater = GLProgram::createWithFilenames("shaderWater.vert", "shaderWater.frag");
            
            shaderWater->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
            
            shaderWater->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
            
            shaderWater->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
            
            shaderWater->bindAttribLocation("a_topV", GLProgram::VERTEX_ATTRIB_MAX);
            
            shaderWater->link();
            
            shaderWater->updateUniforms();
            
            _refractionLocation = shaderWater->getUniformLocationForName("u_refraction");
            
            shaderWater->retain();
            
            this->setGLProgram(shaderWater);
        }
        else
        {
            this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
        }
        
        scheduleUpdate();
        return true;
    }
    
    return false;
}

SeaLayer::SeaLayer()
{
    //this->setContentSize(Size(960, 640/2));
}

SeaLayer::~SeaLayer()
{
	CC_SAFE_DELETE(_polygonVertices);
    _refractionTexture->release();
//	CC_SAFE_DELETE(_lineVertices);
    
}


GameScene *SeaLayer::gameScene()
{
    return static_cast<GameScene*>(GameScene::runningScene());
}


void SeaLayer::update(GLfloat dt)
{
    float frequency = fmaxf(0.5f, _speed * 0.007f);
    
    float incFrequency = dt * frequency;
    
    for (auto node : _nodes)
    {
        node->frequency -= incFrequency;
        
        float newY = _defaultHeight + sin(2.0f * PI * node->frequency) * _amplitude;
        
        node->position = cocos2d::Point(node->position.x, newY);
    }
}

float SeaLayer::tideAmplitude()
{
    return _amplitude;
}

void SeaLayer::setTideAmplitude(float amplitude)
{
    _amplitude = amplitude;
}

float SeaLayer::tideSpeed()
{
    return _speed;
}

void SeaLayer::setTideSpeed(float speed)
{
    _speed = speed;
}

float SeaLayer::seaHeightDefault()
{
    return _defaultHeight;
}

float SeaLayer::seaHeightWithX(float x)
{
    int index = (int)floor((_maxX - x) / _stepX);
    
    if (index < 0 || index >= (_nodes.size() - 1))
    {
        return _defaultHeight;
    }
    
    SeaNode *previousNode = _nodes.at(index);
    
    SeaNode *node = _nodes.at(index + 1);
    
    Point vector = previousNode->position - node->position;
    
    float angle = cocos2d::Point(1.0f, 0.0f).getAngle(vector);
    
    float b = x - node->position.x;
    
    float a = tan(angle)*b;
    
    return node->position.y + a;
}


float SeaLayer::seaAngleWithX(float x)
{
    int index = (int)floor((_maxX - x) / _stepX);
    
    if (index < 0 || index >= (_nodes.size() - 1))
    {
        return _defaultHeight;
    }
    
    SeaNode *previousNode = _nodes.at(index);
    
    SeaNode *node = _nodes.at(index + 1);
    
    Point vector = previousNode->position - node->position;
    
    return CC_RADIANS_TO_DEGREES(Point(1.0f, 0.0f).getAngle(vector));
}

void SeaLayer::setWaterColor(cocos2d::Color3B color)
{
    _waterColor = color;
}

void SeaLayer::setWaterLineOpacity(int opacity)
{
    _waterLineOpacity = opacity;
}

float SeaLayer::getPTMRatio()
{
    return 32.0f;
}

void SeaLayer::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(SeaLayer::onDraw, this,transform,flags);
    renderer->addCommand(&_customCommand);
}

void SeaLayer::onDraw(const Mat4 &transform, uint32_t flags)
{
    float uStep = 1.0f / this->getContentSize().width;
    
    float vStep = 1.0f / this->getContentSize().height;
    
    const Color4B darkWater  = Color4B(_waterColor.r/2, _waterColor.g/2, _waterColor.b/2, /***GameScene::isRenderTextureEnabled()***/true ? 230 : 215);
    
    const Color4B lightWater = Color4B(_waterColor.r,  _waterColor.g,   _waterColor.b,   /***GameScene::isRenderTextureEnabled()***/true ? 150 : 35);
    
    int vertexIndex = 0;
    
    for (auto node : _nodes)
    {
        if (node->nextNode != NULL)
        {
            Point n1Pos = node->position;
            
            Point n2Pos = node->nextNode->position;
            
            _polygonVertices [vertexIndex++] = WaterVertexMake(n1Pos.x, n1Pos.y, 0.0f,
                                                               uStep*n1Pos.x, vStep*n1Pos.y,
                                                               lightWater.r, lightWater.g, lightWater.b, lightWater.a,
                                                               vStep*n1Pos.y);
            
            _polygonVertices [vertexIndex++] = WaterVertexMake( n1Pos.x, 0.0f, 0.0f,
                                                               uStep*n1Pos.x, vStep*0.0f,
                                                               darkWater.r, darkWater.g, darkWater.b, darkWater.a,
                                                               vStep*n1Pos.y);
            
            _polygonVertices [vertexIndex++] = WaterVertexMake( n2Pos.x, 0.0f, 0.0f,
                                                               uStep*n2Pos.x, vStep*0.0f,
                                                               darkWater.r, darkWater.g, darkWater.b, darkWater.a,
                                                               vStep*n2Pos.y);
            
            _polygonVertices [vertexIndex++] = WaterVertexMake( n2Pos.x, 0.0f, 0.0f,
                                                               uStep*n2Pos.x, vStep*0.0f,
                                                               darkWater.r, darkWater.g, darkWater.b, darkWater.a,
                                                               vStep*n2Pos.y);
            
            _polygonVertices [vertexIndex++] = WaterVertexMake( n2Pos.x, n2Pos.y, 0.0f,
                                                               uStep*n2Pos.x, vStep*n2Pos.y,
                                                               lightWater.r, lightWater.g, lightWater.b, lightWater.a,
                                                               vStep*n2Pos.y);
            
            _polygonVertices [vertexIndex++] = WaterVertexMake( n1Pos.x, n1Pos.y, 0.0f,
                                                               uStep*n1Pos.x, vStep*n1Pos.y,
                                                               lightWater.r, lightWater.g, lightWater.b, lightWater.a,
                                                               vStep*n1Pos.y);
        }
    }
    
    vertexIndex = 0;
    
    for (auto node : _nodes)
    {
        _lineVertices [vertexIndex++] = WaterVertexMake(node->position.x, node->position.y, 0.0f,
                                                        uStep*node->position.x, vStep*node->position.y,
                                                        lightWater.r, lightWater.g, lightWater.b, _waterLineOpacity,
                                                        vStep*node->position.y);
        if (node->nextNode)
        {
            _lineVertices [vertexIndex++] = WaterVertexMake(node->nextNode->position.x, node->nextNode->position.y, 0.0f,
                                                            uStep*node->nextNode->position.x, vStep*node->nextNode->position.y,
                                                            lightWater.r, lightWater.g, lightWater.b, _waterLineOpacity,
                                                            vStep*node->nextNode->position.y);
        }
    }
    
	

    CC_NODE_DRAW_SETUP();
//     getGLProgram()->use();
//     
//     getGLProgram()->setUniformsForBuiltins(transform);
    GL::blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
	

    char *texOffset = ((char*)_polygonVertices) + sizeof(GLfloat)*3;
    
    char *colorOffset = texOffset + sizeof(GLfloat)*2;
    
    char *topVOffset = colorOffset + sizeof(unsigned char)*4;

	// here is ok!

    if (this->gameScene()->isRenderTextureEnabled())
    {
        RenderTexture *rt = this->gameScene()->renderTexture();
        GL::bindTexture2D(rt->getSprite()->getTexture()->getName());

        GL::bindTexture2DN(1, _refractionTexture->getName());
        
        glUniform1i(_refractionLocation, 1);
        
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_MAX);
    }
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(WaterVertex), _polygonVertices);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_TRUE, sizeof(WaterVertex), texOffset);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(WaterVertex), colorOffset);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_MAX, 1, GL_FLOAT, GL_TRUE, sizeof(WaterVertex), topVOffset);
    
    glDrawArrays(GL_TRIANGLES, 0, _numPolygonVertices);
    
    Color4F colorf = Color4F(lightWater);
    
    GLProgram *lineShader = ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_U_COLOR);
    
    int colorLocation = glGetUniformLocation (lineShader->getProgram(), "u_color");
 
    lineShader->use();

    lineShader->setUniformsForBuiltins();
    
    lineShader->setUniformLocationWith4fv(colorLocation, (GLfloat*)&colorf.r, 1);

    float lineWidth = round(getPTMRatio() * 0.06f * CC_CONTENT_SCALE_FACTOR());
    
    glLineWidth(lineWidth);
    
    GL::enableVertexAttribs (GL::VERTEX_ATTRIB_FLAG_POSITION);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(WaterVertex), _lineVertices);

    glDrawArrays(GL_LINES, 0, _numLineVertices);

    glLineWidth(1.0f);
    
}


void SeaLayer::onDraw2(const Mat4 &transform, uint32_t flags)
{
    float uStep = 1.0f / this->getContentSize().width;
    
    float vStep = 1.0f / this->getContentSize().height;
    
    const Color4B darkWater  = Color4B(_waterColor.r/2, _waterColor.g/2, _waterColor.b/2, /***GameScene::isRenderTextureEnabled()***/true ? 230 : 215);
    
    const Color4B lightWater = Color4B(_waterColor.r,  _waterColor.g,   _waterColor.b,   /***GameScene::isRenderTextureEnabled()***/true ? 150 : 35);
    
    int vertexIndex = 0;
    
    for (auto node : _nodes)
    {
        if (node->nextNode != NULL)
        {
            Point n1Pos = node->position;
            
            Point n2Pos = node->nextNode->position;
            
            _polygonVertices [vertexIndex++] = WaterVertexMake(n1Pos.x, n1Pos.y, 0.0f,
                                                               uStep*n1Pos.x, vStep*n1Pos.y,
                                                               lightWater.r, lightWater.g, lightWater.b, lightWater.a,
                                                               vStep*n1Pos.y);
            
            _polygonVertices [vertexIndex++] = WaterVertexMake( n1Pos.x, 0.0f, 0.0f,
                                                               uStep*n1Pos.x, vStep*0.0f,
                                                               darkWater.r, darkWater.g, darkWater.b, darkWater.a,
                                                               vStep*n1Pos.y);
            
            _polygonVertices [vertexIndex++] = WaterVertexMake( n2Pos.x, 0.0f, 0.0f,
                                                               uStep*n2Pos.x, vStep*0.0f,
                                                               darkWater.r, darkWater.g, darkWater.b, darkWater.a,
                                                               vStep*n2Pos.y);
            
            _polygonVertices [vertexIndex++] = WaterVertexMake( n2Pos.x, 0.0f, 0.0f,
                                                               uStep*n2Pos.x, vStep*0.0f,
                                                               darkWater.r, darkWater.g, darkWater.b, darkWater.a,
                                                               vStep*n2Pos.y);
            
            _polygonVertices [vertexIndex++] = WaterVertexMake( n2Pos.x, n2Pos.y, 0.0f,
                                                               uStep*n2Pos.x, vStep*n2Pos.y,
                                                               lightWater.r, lightWater.g, lightWater.b, lightWater.a,
                                                               vStep*n2Pos.y);
            
            _polygonVertices [vertexIndex++] = WaterVertexMake( n1Pos.x, n1Pos.y, 0.0f,
                                                               uStep*n1Pos.x, vStep*n1Pos.y,
                                                               lightWater.r, lightWater.g, lightWater.b, lightWater.a,
                                                               vStep*n1Pos.y);
        }
    }
    
    vertexIndex = 0;
    
    for (auto node : _nodes)
    {
        _lineVertices [vertexIndex++] = WaterVertexMake(node->position.x, node->position.y, 0.0f,
                                                        uStep*node->position.x, vStep*node->position.y,
                                                        lightWater.r, lightWater.g, lightWater.b, _waterLineOpacity,
                                                        vStep*node->position.y);
        if (node->nextNode)
        {
            _lineVertices [vertexIndex++] = WaterVertexMake(node->nextNode->position.x, node->nextNode->position.y, 0.0f,
                                                            uStep*node->nextNode->position.x, vStep*node->nextNode->position.y,
                                                            lightWater.r, lightWater.g, lightWater.b, _waterLineOpacity,
                                                            vStep*node->nextNode->position.y);
        }
    }
    
    CC_NODE_DRAW_SETUP();
//     getGLProgram()->use();
//     
//     getGLProgram()->setUniformsForBuiltins(transform);
    GL::blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    char *texOffset = ((char*)_polygonVertices) + sizeof(GLfloat)*3;
    
    char *colorOffset = texOffset + sizeof(GLfloat)*2;
    
    char *topVOffset = colorOffset + sizeof(unsigned char)*4;



    if (this->gameScene()->isRenderTextureEnabled())
    {
        RenderTexture *rt = this->gameScene()->renderTexture();
        GL::bindTexture2D(rt->getSprite()->getTexture()->getName());
        GL::bindTexture2DN(1, _refractionTexture->getName());
        
        glUniform1i(_refractionLocation, 1);
        
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_MAX);
    }
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(WaterVertex), _polygonVertices);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_TRUE, sizeof(WaterVertex), texOffset);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(WaterVertex), colorOffset);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_MAX, 1, GL_FLOAT, GL_TRUE, sizeof(WaterVertex), topVOffset);
    
    glDrawArrays(GL_TRIANGLES, 0, _numPolygonVertices);
    
    Color4F colorf = Color4F(lightWater);
    
    GLProgram *lineShader = ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_U_COLOR);
    
    int colorLocation = glGetUniformLocation (lineShader->getProgram(), "u_color");
 
    lineShader->use();

    lineShader->setUniformsForBuiltins();
    
    lineShader->setUniformLocationWith4fv(colorLocation, (GLfloat*)&colorf.r, 1);

    float lineWidth = round(getPTMRatio() * 0.06f * CC_CONTENT_SCALE_FACTOR());
    
    glLineWidth(lineWidth);
    
    GL::enableVertexAttribs (GL::VERTEX_ATTRIB_FLAG_POSITION);
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(WaterVertex), _lineVertices);

    glDrawArrays(GL_LINES, 0, _numLineVertices);

    glLineWidth(1.0f);
    
}

