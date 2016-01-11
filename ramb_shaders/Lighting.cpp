//
//  Lighting.cpp
//  C2DX_V3.2_Ramboat_WIN
//
//  Created by li on 15/6/23.
//
//

#include "Lighting.h"

static void ccVertexTexLineToPolygon(cocos2d::Point *points, float stroke, ccV2F_T2F *vertices, unsigned int offset, unsigned int nuPoints)
{
    nuPoints += offset;
    if(nuPoints<=1) return;
    
    stroke *= 0.5f;
    
    unsigned int idx;
    unsigned int nuPointsMinus = nuPoints-1;
    
    for(unsigned int i = offset; i<nuPoints; i++)
    {
        idx = i*2;
        Point p1 = points[i];
        Point perpVector;
        
        if(i == 0)
        {
            perpVector = ccpPerp(ccpNormalize(ccpSub(p1, points[i+1])));
        }
        else if(i == nuPointsMinus)
        {
            perpVector = ccpPerp(ccpNormalize(ccpSub(points[i-1], p1)));
        }
        else
        {
            Point p2 = points[i+1];
            Point p0 = points[i-1];
            
            Point p2p1 = ccpNormalize(ccpSub(p2, p1));
            Point p0p1 = ccpNormalize(ccpSub(p0, p1));
            
                // Calculate angle between vectors
            float angle = acosf(ccpDot(p2p1, p0p1));
            
            if(angle < CC_DEGREES_TO_RADIANS(70))
            {
                perpVector = ccpPerp(ccpNormalize(ccpMidpoint(p2p1, p0p1)));
            }
            else if(angle < CC_DEGREES_TO_RADIANS(170))
            {
                perpVector = ccpNormalize(ccpMidpoint(p2p1, p0p1));
            }
            else
            {
                perpVector = ccpPerp(ccpNormalize(ccpSub(p2, p0)));
            }
        }
        
        perpVector = ccpMult(perpVector, stroke);
        
        vertices[idx].vertices = {p1.x+perpVector.x, p1.y+perpVector.y};
        vertices[idx+1].vertices = {p1.x-perpVector.x, p1.y-perpVector.y};
    }
    
        // Validate vertexes
    offset = (offset==0) ? 0 : offset-1;
    for(unsigned int i = offset; i<nuPointsMinus; i++)
    {
        idx = i*2;
        const unsigned int idx1 = idx+2;
        
        ccVertex2F p1 = vertices[idx].vertices;
        ccVertex2F p2 = vertices[idx+1].vertices;
        ccVertex2F p3 = vertices[idx1].vertices;
        ccVertex2F p4 = vertices[idx1+1].vertices;
        
        float s;
        //bool fixVertex = !ccpLineIntersect(ccp(p1.x, p1.y), ccp(p4.x, p4.y), ccp(p2.x, p2.y), ccp(p3.x, p3.y), &s, &t);
        bool fixVertex = !ccVertexLineIntersect(p1.x, p1.y, p4.x, p4.y, p2.x, p2.y, p3.x, p3.y, &s);
        
        if(!fixVertex)
        {
            if (s<0.0f || s>1.0f)
            {
                fixVertex = true;
            }
        }
        
        if(fixVertex)
        {
            vertices[idx1].vertices = p4;
            vertices[idx1+1].vertices = p3;
        }
    }
}



Lightning* Lightning::lightningWithStrikePoint(cocos2d::Point source, cocos2d::Point destination, float duration, float fadeDuration,std::string texturename )
{
    Texture2D *texture = TextureCache::sharedTextureCache()->addImage(texturename);
    auto lighting = new Lightning();
    if (lighting->initWithStrikePoint(source, destination, duration, fadeDuration, texture))
    {
        lighting->autorelease();
        return lighting;
    }
    return nullptr;
}

Lightning* Lightning::lightningWithStrikePoint(cocos2d::Point source, cocos2d::Point destination, float duration, float fadeDuration, Texture2D * texture )
{
     auto lighting = new Lightning();
     if (lighting->initWithStrikePoint(source, destination, duration, fadeDuration, texture))
     {
         lighting->autorelease();
         return lighting;
     }
     return nullptr;
}

bool Lightning::initWithStrikePoint(cocos2d::Point source, cocos2d::Point destination, float duration, float fadeDuration, std::string texturename)
{
    Texture2D *texture = TextureCache::getInstance()->addImage(texturename);
    return this->initWithStrikePoint(source, destination, duration, fadeDuration, texture);
}

bool Lightning::initWithStrikePoint(cocos2d::Point source, cocos2d::Point destination, float duration, float fadeDuration, Texture2D * texture )
{
    if (Node::init())
    {
		this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_U_COLOR));
        _colorLocation = this->getGLProgram()->getUniformLocation("u_color");

        _displayedColor = _realColor = Color3B::WHITE;
        _displayedOpacity = _realOpacity = 255;
        _cascadeOpacityEnabled = true;
        _cascadeColorEnabled = true;
        _autoRemoveFromParent = false;
        _lightningBuffersCapacity = 0;
        _lightningVertices = NULL;
        _pointVertices = NULL;
        _blendFunc = {CC_BLEND_SRC, CC_BLEND_DST};

        texture->retain();
        _texture = texture;
        _opacityModifyRGB = false;
        
        strikeSource = source;
        strikeSplitDestination = Point(0, 0);
        strikeDestination = destination;
        
        _duration = duration;
        _fadeDuration = fadeDuration;
        
        split = false;
        
        displacement = 120;
        minDisplacement = 4;
        lightningWidth = 1.0f;
        _noOfInitialPoints = 0;
        _noOfPoints = 0;
        
        
        this->ensureCapacity(16);
        

        glGenVertexArrays(1, &_lightningVAOName);
        GL::bindVAO(_lightningVAOName);
        
        glGenBuffers(1, &_lightningBuffersVBO);
        glBindBuffer(GL_ARRAY_BUFFER, _lightningBuffersVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_T2F) * _lightningBuffersCapacity, _lightningVertices, GL_DYNAMIC_DRAW);
        
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
        glVertexAttribPointer( GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_T2F), (GLvoid *)offsetof(ccV2F_T2F, vertices));
        
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
        glVertexAttribPointer( GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_T2F), (GLvoid *)offsetof(ccV2F_T2F, texCoords));
        
        GL::bindVAO(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        CHECK_GL_ERROR_DEBUG();
        _wasCapacityIncreased = false;
        
        return true;
    }
    
    return false;
}

bool Lightning:: ensureCapacity(unsigned long count)
{
    if ( count > _lightningBuffersCapacity)
    {
//        _lightningBuffersCapacity = MAX(_lightningBuffersCapacity, count);
//        if(_lightningVertices){free(_lightningVertices);_lightningVertices=nullptr;}
//        if (_pointVertices){free(_pointVertices);_pointVertices=nullptr;}
//        _lightningVertices = (ccV2F_T2F *)malloc(_lightningBuffersCapacity * sizeof(ccV2F_T2F));
//        _pointVertices = (cocos2d::Point *)malloc(_lightningBuffersCapacity * 0.5f * sizeof(cocos2d::Point));
        _lightningBuffersCapacity = MAX(_lightningBuffersCapacity, count);
        CCLOG("[Lightning] realloc memeory %d", _lightningBuffersCapacity * sizeof(ccV2F_T2F));
        _lightningVertices = (ccV2F_T2F *)realloc( _lightningVertices, _lightningBuffersCapacity * sizeof(ccV2F_T2F));
        _pointVertices = (cocos2d::Point *)realloc( _pointVertices, _lightningBuffersCapacity * 0.5f * sizeof(cocos2d::Point));
        return true;
    }
    
    return false;
}

void Lightning::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    
    _customCommand.func = CC_CALLBACK_0(Lightning::onDraw, this, transform, flags);
    
    renderer->addCommand(&_customCommand);
}

void Lightning::onDraw(const Mat4 &transform, uint32_t flags)
{
#if 1
    CC_NODE_DRAW_SETUP();
    
    GL::blendFunc(GL_SRC_ALPHA,GL_ONE);
    
    GL::bindTexture2D( _texture->getName());
    
    Color4F floatColor = Color4F(_displayedColor);
    floatColor.a = _realOpacity / 255.0f;
    
    this->getGLProgram()->setUniformLocationWith4f(_colorLocation, floatColor.r, floatColor.g, floatColor.b, floatColor.a);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );
    long offset = (long)_lightningVertices;
    // vertex
    int diff = offsetof( ccV2F_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, sizeof(ccV2F_T2F), (void*) (offset + diff));
    // texCoods
    diff = offsetof( ccV2F_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_T2F), (void*) (offset + diff));
#else
    GL::bindVAO(_lightningVAOName);
#endif

    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)_noOfPoints * 2);
    CHECK_GL_ERROR_DEBUG();
    CC_INCREMENT_GL_DRAWS(1);
#endif
}


void Lightning::strikeRandom () 
{
    _seed = rand();
    this->strike();
}

void Lightning::strikeWithSeed(unsigned long seed )
{
    _seed = seed;
    this->strike();
}

void Lightning::setInitialPoints(cocos2d::Point * initialPoints, unsigned int noOfInitialPoints)
 {
    _noOfInitialPoints = noOfInitialPoints;
    _wasCapacityIncreased = this->ensureCapacity(noOfInitialPoints * 2);

    for (unsigned int i = 0; i < _noOfInitialPoints; i++)
    {
        _pointVertices[i] = initialPoints[i];
    }
}

void Lightning::strike()
{
#if 1
    this->setVisible(false);
    FiniteTimeAction *action, *fadeAction;
    
    if (_opacityModifyRGB)
    {
        fadeAction = FadeTo::create(_fadeDuration, 0);
    }
    else
    {
        TintTo *tintTo = TintTo::create(_fadeDuration, 0, 0, 0);
        FadeTo *fadeTo = FadeTo::create(_fadeDuration, 0);
        fadeAction = Spawn::create(tintTo, fadeTo, NULL);
    }
    
    action = Sequence::create(Show::create(), DelayTime::create(_duration), fadeAction, NULL);
    
    if (actionLightingWillStrike)
    {
        action = Sequence::create(CallFunc::create(actionLightingWillStrike), action, CallFunc::create(actionLightingDidStrike), NULL);
    }

    if (_autoRemoveFromParent)
    {
        action = Sequence::create(action, RemoveSelf::create(), NULL);
    }
    else
    {
        action = Sequence::create(action, Hide::create(), FadeTo::create(0.0f, _realOpacity), TintTo::create(0.0f, _realColor.r, _realColor.g, _realColor.b), NULL);
    }
    
    this->runAction(action);
    
    int noOfLines = this->computeNumberOfLines(strikeSource, strikeDestination, displacement, minDisplacement);
    _noOfPoints = _noOfInitialPoints + noOfLines + 1;
    _wasCapacityIncreased = this->ensureCapacity(_noOfPoints * 2) || _wasCapacityIncreased;
    _noOfPoints = _noOfInitialPoints;
    cocos2d::Point mid = this->addLightning(strikeSource, strikeDestination, displacement, minDisplacement);
    ccVertexTexLineToPolygon(_pointVertices, lightningWidth, _lightningVertices, 0, (unsigned int)_noOfPoints);
    float texDelta = 1.0f / _noOfPoints;

    for (int i = 0; i < _noOfPoints; i++)
    {
        _lightningVertices[i * 2].texCoords = Tex2F(0, texDelta * i);
        _lightningVertices[i * 2 + 1].texCoords = Tex2F(1, texDelta * i);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, _lightningBuffersVBO );

    if (_wasCapacityIncreased)
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_T2F) * _noOfPoints * 2, _lightningVertices, GL_DYNAMIC_DRAW);
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ccV2F_T2F) * _noOfPoints * 2, _lightningVertices);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    if (split)
    {
        if (_splitLightning == nullptr)
        {
            _splitLightning = Lightning::lightningWithStrikePoint(mid, strikeSplitDestination, _duration, _fadeDuration, _texture);
            this->addChild(_splitLightning, -1);
        }
        
        _splitLightning->strikeSource = mid;
        _splitLightning->strikeDestination = this->strikeSplitDestination;
        _splitLightning->minDisplacement = this->minDisplacement;
        _splitLightning->displacement = this->displacement * 0.5f;
        _splitLightning->lightningWidth = this->lightningWidth;
        _splitLightning->color = this->_realColor;
        _splitLightning->setOpacity(this->_realOpacity);
        _splitLightning->duration = this->_duration;
        _splitLightning->fadeDuration = this->_fadeDuration;
        _splitLightning->setOpacityModifyRGB(this->_opacityModifyRGB);
        _splitLightning->setInitialPoints(this->_pointVertices, this->_noOfPoints * 0.5f + 1);
        _splitLightning->strikeWithSeed(this->_seed);
    }
    
    _wasCapacityIncreased = false;
#endif
}

cocos2d::Point Lightning::addLightning(cocos2d::Point pt1, cocos2d::Point pt2, int displace, int minDisplace)
{
     cocos2d::Point mid = ccpMult(ccpAdd(pt1,pt2), 0.5f);
    
     if (displace < minDisplace)
     {
        if (_noOfPoints == 0)
        {
            if (_pointVertices) _pointVertices[0] = pt1;
            _noOfPoints++;
        }
        
        if (_pointVertices) _pointVertices[_noOfPoints] = pt2;
        _noOfPoints++;
        
     }
     else
     {
        mid.x += ( (rand() % 101) / 100.0f - 0.5f ) * displace;
        mid.y += ( (rand() % 101) / 100.0f - 0.5f ) * displace;
        
        this->addLightning(pt1, mid, displace * 0.5f, minDisplace);
        this->addLightning(mid, pt2, displace * 0.5f, minDisplace);
     }
    
     return mid;
}

unsigned int Lightning::computeNumberOfLines(cocos2d::Point pt1, cocos2d::Point pt2, int displace, int minDisplace)
{
    cocos2d::Point mid = ccpMult(ccpAdd(pt1, pt2), 0.5f);
    
    if (displace < minDisplace)
    {
        return 1;
    }
    
    mid.x += ( (rand() % 101) / 100.0f - 0.5f ) * displace;
    mid.y += ( (rand() % 101) / 100.0f - 0.5f ) * displace;
    
    return this->computeNumberOfLines(pt1, mid, displace * 0.5f, minDisplace) +
        this->computeNumberOfLines(mid, pt2, displace * 0.5f, minDisplace);
}

void Lightning::updateBlendFunc () 
{
    if ( !_texture || !_texture->hasPremultipliedAlpha())
    {
        
        _blendFunc.src = GL_SRC_ALPHA;
        _blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
        this->setOpacityModifyRGB(false);
        
    }
    else
    {
        
        _blendFunc.src = CC_BLEND_SRC;
        _blendFunc.dst = CC_BLEND_DST;
    }
}

void Lightning::setTexture(Texture2D * texture ) 
{
    if ( _texture == texture )
    {
        return;
    }
    
    if (_texture)
    {
        _texture->release();
        _texture = NULL;
    }
    
    _texture->retain();
    _texture = texture;
    this->updateBlendFunc();
}

Texture2D * Lightning::getTexture() const
{
    return _texture;
}

unsigned int Lightning::getOpacity()
{
    return _realOpacity;
}

void Lightning::setOpacity(unsigned int opacity)
{
    _displayedOpacity = _realOpacity = opacity;
    
	if( _cascadeOpacityEnabled )
    {
		unsigned int parentOpacity = 255;
        
        if (this->getParent())
        {
            parentOpacity = this->getParent()->getDisplayedOpacity();
        }
		this->updateDisplayedOpacity(parentOpacity);
	}

    
    // special opacity for premultiplied textures
    if ( _opacityModifyRGB ) {
        this->setColor(_realColor);
    }
}

Color3B Lightning::getColor()
{
    if (_opacityModifyRGB) {
        return _realColor;
    }
    
    return _displayedColor;
}

void Lightning::setColor(Color3B color3) 
{
    _displayedColor = _realColor = color3;
    
    if ( _opacityModifyRGB )
    {
        _displayedColor.r = color3.r * _realOpacity / 255.0f;
        _displayedColor.g = color3.g * _realOpacity / 255.0f;
        _displayedColor.b = color3.b * _realOpacity / 255.0f;
    }
    
    if( _cascadeColorEnabled )
    {
        Color3B parentColor = Color3B::WHITE;
        if (this->getParent())
        {
            parentColor = this->getParent()->getDisplayedColor();
        }
		this->updateDisplayedColor(parentColor);
	}
}

void Lightning::setOpacityModifyRGB(bool modify)
{
    Color3B oldColor = this->getColor();
    _opacityModifyRGB = modify;
    this->color = oldColor;
}

bool Lightning:: doesOpacityModifyRGB()
{
    return _opacityModifyRGB;
}

/** recursive method that updates display color */
void Lightning::updateDisplayedColor(Color3B parentColor) 
{
	_displayedColor.r = _realColor.r * parentColor.r/255.0;
	_displayedColor.g = _realColor.g * parentColor.g/255.0;
	_displayedColor.b = _realColor.b * parentColor.b/255.0;
    
    for(auto item : _children)
    {
        item->updateDisplayedColor(_displayedColor);
    }
}

/** recursive method that updates the displayed opacity */
void Lightning::updateDisplayedOpacity(unsigned int parentOpacity)
{
	_displayedOpacity = _realOpacity * parentOpacity/255.0;
    
    for(auto item : _children)
    {
        item->updateDisplayedOpacity(_displayedOpacity);
    }
}

Color3B Lightning:: getDisplayedColor()
{
	return _displayedColor;
}

unsigned int Lightning:: getDisplayedOpacity()
{
	return _displayedOpacity;
}

Lightning::Lightning()
{
    
}

Lightning::~Lightning()
{
    free(_lightningVertices);
    free(_pointVertices);
    glDeleteBuffers(0, &_lightningBuffersVBO);
    glDeleteBuffers(0, &_lightningVAOName);
}

void Lightning::setBlendFunc(const BlendFunc &blendFunc)
{
    this->blendFunc = blendFunc;
}

const BlendFunc& Lightning::getBlendFunc() const
{
    return this->blendFunc;
}


