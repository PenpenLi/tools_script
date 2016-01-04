//
//  Lightning.m
//  Maze Mouse
//
//  Created by Nader Eloshaiker on 15/05/13.
//  Copyright 2013 Nader Eloshaiker. All rights reserved.
//

#include "Lightning.h"
#include "Vertex.h"



void ccVertexTexLineToPolygon(Point *points, float stroke, ccV2F_T2F *vertices, NSUInteger offset, NSUInteger nuPoints) {
    
    nuPoints += offset;
    if(nuPoints<=1) return;
    
    stroke *= 0.5f;
    
    NSUInteger idx;
    NSUInteger nuPointsMinus = nuPoints-1;
    
    for(NSUInteger i = offset; i<nuPoints; i++) {
        idx = i*2;
        Point p1 = points[i];
        Point perpVector;
        
        if(i == 0)
            perpVector = ccpPerp(ccpNormalize(ccpSub(p1, points[i+1])));
        else if(i == nuPointsMinus)
            perpVector = ccpPerp(ccpNormalize(ccpSub(points[i-1], p1)));
        else {
            Point p2 = points[i+1];
            Point p0 = points[i-1];
            
            Point p2p1 = ccpNormalize(ccpSub(p2, p1));
            Point p0p1 = ccpNormalize(ccpSub(p0, p1));
            
            // Calculate angle between vectors
            float angle = acosf(ccpDot(p2p1, p0p1));
            
            if(angle < _DEGREES_TO_RADIANS(70))
                perpVector = ccpPerp(ccpNormalize(ccpMidpoint(p2p1, p0p1)));
            else if(angle < _DEGREES_TO_RADIANS(170))
                perpVector = ccpNormalize(ccpMidpoint(p2p1, p0p1));
            else
                perpVector = ccpPerp(ccpNormalize(ccpSub(p2, p0)));
        }
        
        perpVector = ccpMult(perpVector, stroke);
        
        vertices[idx].vertices = (ccVertex2F) {p1.x+perpVector.x, p1.y+perpVector.y};
        vertices[idx+1].vertices = (ccVertex2F) {p1.x-perpVector.x, p1.y-perpVector.y};
    }
    
    // Validate vertexes
    offset = (offset==0) ? 0 : offset-1;
    for(NSUInteger i = offset; i<nuPointsMinus; i++) {
        idx = i*2;
        const NSUInteger idx1 = idx+2;
        
        ccVertex2F p1 = vertices[idx].vertices;
        ccVertex2F p2 = vertices[idx+1].vertices;
        ccVertex2F p3 = vertices[idx1].vertices;
        ccVertex2F p4 = vertices[idx1+1].vertices;
        
        float s;
        //bool fixVertex = !ccpLineIntersect(ccp(p1.x, p1.y), ccp(p4.x, p4.y), ccp(p2.x, p2.y), ccp(p3.x, p3.y), &s, &t);
        bool fixVertex = !ccVertexLineIntersect(p1.x, p1.y, p4.x, p4.y, p2.x, p2.y, p3.x, p3.y, &s);
        
        if(!fixVertex)
            if (s<0.0f || s>1.0f)
                fixVertex = true;
        
        if(fixVertex) {
            vertices[idx1].vertices = p4;
            vertices[idx1+1].vertices = p3;
        }
    }
}


@implementation Lightning


@synthesize strikeSource = strikeSource_;
@synthesize strikeSplitDestination = strikeSplitDestination_;
@synthesize strikeDestination = strikeDestination_;
@synthesize displacement = displacement_;
@synthesize minDisplacement = minDisplacement_;
@synthesize lightningWidth = lightningWidth_;
@synthesize split = split_;
@synthesize fadeDuration = _fadeDuration;
@synthesize duration = _duration;
@synthesize autoRemoveFromParent = _autoRemoveFromParent;
@synthesize delegate = _delegate;
@synthesize cascadeColorEnabled = _cascadeColorEnabled;
@synthesize cascadeOpacityEnabled = _cascadeOpacityEnabled;


Lightning* Lightning::lightningWithStrikePoint(Point source, Point destination, ccTime duration, ccTime fadeDuration,std::string texturename ) 
 {
    Texture2D *texture = TextureCache::sharedTextureCache()->addImage(texturename);
    return [[this->alloc() initWithStrikePoint:source strikePoint2:destination duration:duration fadeDuration:fadeDuration texture:texture] autorelease];
}
Lightning* Lightning::lightningWithStrikePoint(Point source, Point destination, ccTime duration, ccTime fadeDuration, Texture2D * texture ) 
 {
    return [[this->alloc() initWithStrikePoint:source strikePoint2:destination duration:duration fadeDuration:fadeDuration texture:texture] autorelease];
}
Lightning* Lightning::initWithStrikePoint(Point source, Point destination, ccTime duration, ccTime fadeDuration,std::string texturename ) 
 {
    Texture2D *texture = TextureCache::sharedTextureCache()->addImage(texturename);
    return this->initWithStrikePoint(source strikePoint2:destination, duration, fadeDuration, texture);
}
Lightning* Lightning::initWithStrikePoint(Point source, Point destination, ccTime duration, ccTime fadeDuration, Texture2D * texture ) 
 {
    if (this = super->init()) {
		this->shaderProgram = ShaderCache::sharedShaderCache()->programForKey(kShader_PositionTexture_uColor);
        _colorLocation = glGetUniformLocation( this->shaderProgram.program, "u_color");

        _displayedColor = _realColor = ccWHITE;
        _displayedOpacity = _realOpacity = 255;
        _cascadeOpacityEnabled = true;
        _cascadeColorEnabled = true;
        _autoRemoveFromParent = false;
        _delegate = NULL;

        _blendFunc = (ccBlendFunc) {_BLEND_SRC, _BLEND_DST};

        _texture = texture->retain();
        _opacityModifyRGB = false;
        
        strikeSource_ = source;
        strikeSplitDestination_ = Point(0, 0);
        strikeDestination_ = destination;
        
        _duration = duration;
        _fadeDuration = fadeDuration;
        
        split_ = false;
        
        displacement_ = 120;
        minDisplacement_ = 4;
        lightningWidth_ = 1.0f;
        
        this->ensureCapacity(16);
        

        glGenVertexArrays(1, &_lightningVAOName);
        ccGLBindVAO(_lightningVAOName);
        
        glGenBuffers(1, &_lightningBuffersVBO);
        glBindBuffer(GL_ARRAY_BUFFER, _lightningBuffersVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_T2F) * _lightningBuffersCapacity, _lightningVertices, GL_DYNAMIC_DRAW);
        
        glEnableVertexAttribArray(kVertexAttrib_Position);
        glVertexAttribPointer( kVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_T2F), (GLvoid *)offsetof(ccV2F_T2F, vertices) );
        
        glEnableVertexAttribArray(kVertexAttrib_TexCoords);
        glVertexAttribPointer( kVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_T2F), (GLvoid *)offsetof(ccV2F_T2F, texCoords) );
        
        ccGLBindVAO(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        CHECK_GL_ERROR();
        _wasCapacityIncreased = false;
    }
    
    return this;
}

bool Lightning:: ensureCapacity(NSUInteger count ) 
{
    if ( count > _lightningBuffersCapacity) {
        _lightningBuffersCapacity = MAX(_lightningBuffersCapacity, count);
        _lightningVertices = (ccV2F_T2F *)realloc( _lightningVertices, _lightningBuffersCapacity * sizeof(ccV2F_T2F) );
        _pointVertices = (Point *)realloc( _pointVertices, _lightningBuffersCapacity * 0.5f * sizeof(Point) );
        return true;
    }
    
    return false;
}

void Lightning::draw () 
{
    _NODE_DRAW_SETUP();
    
    // Arreglo, para que false se vean los rayos negros
    //ccGLBlendFunc( _blendFunc.src, _blendFunc.dst );
    ccGLBlendFunc(GL_SRC_ALPHA,GL_ONE);
    
    ccGLBindTexture2D( _texture->name() );
    
    ccColor4F floatColor = ccc4FFromccc3B(_displayedColor);
    floatColor.a = _realOpacity / 255.0f;
    [this->shaderProgram setUniformLocation:_colorLocation withF1:floatColor.r f2:floatColor.g f3:floatColor.b f4:floatColor.a];
    
#ifdef ANDROID
    //
    // Esto hay que hacerlo así en Android porque el VAO hace que el resto de sprites se vean oscuros
    //
    ccGLEnableVertexAttribs( kVertexAttribFlag_Position | kVertexAttribFlag_TexCoords );
    long offset = (long)_lightningVertices;
    // vertex
    NSInteger diff = offsetof( ccV2F_T2F, vertices);
    glVertexAttribPointer(kVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, sizeof(ccV2F_T2F), (void*) (offset + diff));
    // texCoods
    diff = offsetof( ccV2F_T2F, texCoords);
    glVertexAttribPointer(kVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_T2F), (void*) (offset + diff));
#else
    ccGLBindVAO( _lightningVAOName );
#endif
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)_noOfPoints * 2);
    CHECK_GL_ERROR_DEBUG();
    _INCREMENT_GL_DRAWS(1);
}

void Lightning::strikeRandom () 
{
    _seed = rand();
    this->strike();
}

void Lightning::strikeWithSeed(NSInteger seed ) 
{
    _seed = seed;
    this->strike();
}

void Lightning::setInitialPoints(Point * initialPoints, NSUInteger noOfInitialPoints ) 
 {
    _noOfInitialPoints = noOfInitialPoints;
    _wasCapacityIncreased = this->ensureCapacity(noOfInitialPoints * 2);

    for (NSUInteger i = 0; i < _noOfInitialPoints; i++) {
        _pointVertices[i] = initialPoints[i];
    }
}

void Lightning::strike
() 
{
    this->visible = false;
    FiniteTimeAction *action, *fadeAction;
    
    if (_opacityModifyRGB) {
        fadeAction = FadeTo::create(_fadeDuration, 0);
    } else {
        TintTo *tintTo = TintTo::create(_fadeDuration, 0, 0, 0);
        FadeTo *fadeTo = FadeTo::create(_fadeDuration, 0);
        fadeAction = Spawn::actionOne(tintTo, fadeTo);
    }
    
    action = Sequence::create([Show action),, :create(_duration),
              fadeAction,
              NULL];
    
    if (_delegate) {
        action = Sequence::create([CallFuncN, _delegate, @selector(actionLightingWillStrike)),
                  action,
                  CallFuncN::actionWithTarget(_delegate, @selector(actionLightingDidStrike)),
                  NULL];
    }

    if (_autoRemoveFromParent) {
        action = Sequence::actionOne(action two:[CallFuncND, this, @selector(removeFromParentAndCleanup:), (void*)TRUE]);
    } else {
        action = [Sequence actions:action,
                  Hide::action(),, :create(0.0f opacity:_realOpacity),, :create(0.0f red:_realColor.r green:_realColor.g, _realColor.b),
                  NULL];
    }
        
    this->runAction(action);
    
    srand((int)_seed);
    NSInteger noOfLines = this->computeNumberOfLines(strikeSource_, strikeDestination_, displacement_, minDisplacement_);
    _noOfPoints = _noOfInitialPoints + noOfLines + 1;
    _wasCapacityIncreased = this->ensureCapacity(_noOfPoints * 2) || _wasCapacityIncreased;
    _noOfPoints = _noOfInitialPoints;
    srand((int)_seed);
    Point mid = this->addLightning(strikeSource_, strikeDestination_, displacement_, minDisplacement_);
    ccVertexTexLineToPolygon(_pointVertices, lightningWidth_, _lightningVertices, 0, _noOfPoints);
    float texDelta = 1.0f / _noOfPoints;

    for (NSUInteger i = 0; i < _noOfPoints; i++ ) {
        _lightningVertices[i * 2].texCoords = (ccTex2F) {0, texDelta * i};
        _lightningVertices[i * 2 + 1].texCoords = (ccTex2F) {1, texDelta * i};
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, _lightningBuffersVBO );

    if (_wasCapacityIncreased) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_T2F) * _noOfPoints * 2, _lightningVertices, GL_DYNAMIC_DRAW);
    } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ccV2F_T2F) * _noOfPoints * 2, _lightningVertices);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    if (split_) {
        
        if (_splitLightning == 0) {
            _splitLightning = Lightning::alloc()->initWithStrikePoint(mid strikePoint2:strikeSplitDestination_, _duration, _fadeDuration, _texture);
            this->addChild(_splitLightning, -1);
        }
        
        _splitLightning.strikeSource = mid;
        _splitLightning.strikeDestination = strikeSplitDestination_;
        _splitLightning.minDisplacement = minDisplacement_;
        _splitLightning.displacement = displacement_ * 0.5f;
        _splitLightning.lightningWidth = lightningWidth_;
        _splitLightning.color = _realColor;
       _splitLightning->setOpacity(_realOpacity);
        _splitLightning.duration = _duration;
        _splitLightning.fadeDuration = _fadeDuration;
        _splitLightning->setOpacityModifyRGB(_opacityModifyRGB);
        _splitLightning->setInitialPoints(_pointVertices, _noOfPoints * 0.5f + 1);
        _splitLightning->strikeWithSeed(_seed);
    }
    
    _wasCapacityIncreased = false;
}

Point Lightning::addLightning(Point pt1, Point pt2, NSInteger displace, NSInteger minDisplace ) 
 {
    Point mid = ccpMult(ccpAdd(pt1,pt2), 0.5f);
    
    if (displace < minDisplace) {
        
        if (_noOfPoints == 0) {
            _pointVertices[0] = pt1;
            _noOfPoints++;
        }
        
        _pointVertices[_noOfPoints] = pt2;
        _noOfPoints++;
        
    } else {
        
        mid.x += ( (rand() % 101) / 100.0f - 0.5f ) * displace;
        mid.y += ( (rand() % 101) / 100.0f - 0.5f ) * displace;
        
        this->addLightning(pt1, mid, displace * 0.5f, minDisplace);
        this->addLightning(mid, pt2, displace * 0.5f, minDisplace);
    }
    
    return mid;
}

NSUInteger Lightning::computeNumberOfLines(Point pt1, Point pt2, NSInteger displace, NSInteger minDisplace ) 
 {
    Point mid = ccpMult(ccpAdd(pt1,pt2), 0.5f);
    
    if (displace < minDisplace) {
        return 1;
    }
    
    mid.x += ( (rand() % 101) / 100.0f - 0.5f ) * displace;
    mid.y += ( (rand() % 101) / 100.0f - 0.5f ) * displace;
    
    return
    this->computeNumberOfLines(pt1, mid, displace * 0.5f, minDisplace) +
    this->computeNumberOfLines(mid, pt2, displace * 0.5f, minDisplace);
}

void Lightning::updateBlendFunc () 
{
    if ( !_texture || !_texture->hasPremultipliedAlpha() ) {
        
        _blendFunc.src = GL_SRC_ALPHA;
        _blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
        this->setOpacityModifyRGB(false);
        
    } else {
        
        _blendFunc.src = _BLEND_SRC;
        _blendFunc.dst = _BLEND_DST;
    }
}

void Lightning::setTexture(Texture2D * texture ) 
{
    if ( _texture == texture )
        return;
    
    if (_texture) {
        _texture->release();
        _texture = NULL;
    }
    
    _texture = _texture->retain();
    this->updateBlendFunc();

}

Texture2D * Lightning::texture () 
{
    return _texture;
}

GLubyte Lightning:: opacity () 
{
    return _realOpacity;
}

void Lightning::setOpacity(GLubyte opacity ) 
{
    _displayedOpacity = _realOpacity = opacity;
    
	if( _cascadeOpacityEnabled ) {
		GLubyte parentOpacity = 255;
        
		if( _parent->conformsToProtocol(@protocol(RGBAProtocol)] && [(id<RGBAProtocol>)_parent isCascadeOpacityEnabled) ) {
            
			parentOpacity = [(id<RGBAProtocol>)_parent displayedOpacity];
        }
        
		this->updateDisplayedOpacity(parentOpacity);
	}

    
    // special opacity for premultiplied textures
    if ( _opacityModifyRGB ) {
        this->setColor(_realColor);
    }
}

ccColor3B Lightning::color () 
{
    if (_opacityModifyRGB) {
        return _realColor;
    }
    
    return _displayedColor;
}

void Lightning::setColor(ccColor3B color3 ) 
{
    _displayedColor = _realColor = color3;
    
    if ( _opacityModifyRGB ) {
        _displayedColor.r = color3.r * _realOpacity / 255.0f;
        _displayedColor.g = color3.g * _realOpacity / 255.0f;
        _displayedColor.b = color3.b * _realOpacity / 255.0f;
    }
    
    if( _cascadeColorEnabled ) {
		ccColor3B parentColor = ccWHITE;
		if( _parent->conformsToProtocol(@protocol(RGBAProtocol)] && [(id<RGBAProtocol>)_parent isCascadeColorEnabled) )
			parentColor = [(id<RGBAProtocol>)_parent displayedColor];
		this->updateDisplayedColor(parentColor);
	}
}

void Lightning::setOpacityModifyRGB(bool modify ) 
{
    ccColor3B oldColor = this->color;
    _opacityModifyRGB = modify;
    this->color = oldColor;
}

bool Lightning:: doesOpacityModifyRGB () 
{
    return _opacityModifyRGB;
}

/** recursive method that updates display color */
void Lightning::updateDisplayedColor(ccColor3B parentColor ) 
{
	_displayedColor.r = _realColor.r * parentColor.r/255.0;
	_displayedColor.g = _realColor.g * parentColor.g/255.0;
	_displayedColor.b = _realColor.b * parentColor.b/255.0;
    
	Sprite *item;
	ARRAY_FOREACH(_children, item) {
		item->updateDisplayedColor(_displayedColor);
	}
}

/** recursive method that updates the displayed opacity */
void Lightning::updateDisplayedOpacity(GLubyte parentOpacity ) 
{
	_displayedOpacity = _realOpacity * parentOpacity/255.0;
    
	Sprite *item;
	ARRAY_FOREACH(_children, item) {
		item->updateDisplayedOpacity(_displayedOpacity);
	}
}

ccColor3B Lightning:: displayedColor () 
{
	return _displayedColor;
}

GLubyte Lightning:: displayedOpacity () 
{
	return _displayedOpacity;
}

void Lightning::dealloc () 
{
    free(_lightningVertices);
    free(_pointVertices);
    glDeleteBuffers(0, &_lightningBuffersVBO);
    glDeleteBuffers(0, &_lightningVAOName);
    
    if (_delegate) {
        _delegate->release();
        _delegate = NULL;
    }
    
    super->dealloc();
}




