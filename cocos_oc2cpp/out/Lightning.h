//
//  Lightning.h
//  Maze Mouse
//
//  Created by Nader Eloshaiker on 15/05/13.
//  Copyright 2013 Nader Eloshaiker. All rights reserved.
//

#include <Foundation/Foundation.h>
#include "cocos2d.h"
#include "LightningStrikeDelegate.h"



typedef struct _ccV2F_T2F
{
    //! vertices (2F)
    ccVertex2F		vertices;
    //! tex coords (2F)
    ccTex2F			texCoords;
} ccV2F_T2F;



class Lightning : public CCNode {public:<CCRGBAProtocol, CCTextureProtocol> {
    ccV2F_T2F *_lightningVertices;
    GLuint _lightningVAOName;
    GLuint _lightningBuffersVBO;
    NSUInteger _lightningBuffersCapacity;
    Point *_pointVertices;
    NSUInteger _noOfPoints;
    NSUInteger _noOfInitialPoints;
    
    NSUInteger _seed;
    
    GLuint _colorLocation;
    ccColor3B _realColor;
    GLubyte _realOpacity;
    
    ccColor3B _displayedColor;
    GLubyte _displayedOpacity;
    bool _cascadeColorEnabled;
    bool _cascadeOpacityEnabled;
    
    CCTexture2D *_texture;
    ccBlendFunc _blendFunc;
    bool _opacityModifyRGB;
    bool _wasCapacityIncreased;
    
    Lightning *_splitLightning;
    ccTime _duration;
    ccTime _fadeDuration;
    bool _autoRemoveFromParent;
    
    id<LightningStrikeDelegate> _delegate;
}


@property(nonatomic, readwrite) Point strikeSource;
@property(nonatomic, readwrite) Point strikeSplitDestination;
@property(nonatomic, readwrite) Point strikeDestination;

@property(nonatomic, readwrite) ccColor3B color;
@property(nonatomic, readwrite) GLubyte opacity;

@property(nonatomic, readonly) ccColor3B displayedColor;
@property(nonatomic, readonly) GLubyte displayedOpacity;
@property(nonatomic, getter = isCascadeColorEnabled) bool cascadeColorEnabled;
@property(nonatomic, getter = isCascadeOpacityEnabled) bool cascadeOpacityEnabled;
@property(nonatomic, getter = isAutoRemoveFromParent) bool autoRemoveFromParent;
@property(nonatomic, readwrite, retain) id<LightningStrikeDelegate> delegate;


@property(nonatomic, readwrite) bool split;
@property(nonatomic, readwrite) NSInteger displacement;
@property(nonatomic, readwrite) NSInteger minDisplacement;
@property(nonatomic, readwrite) float lightningWidth;
@property(nonatomic, readwrite) ccTime fadeDuration;
@property(nonatomic, readwrite) ccTime duration;
@property(nonatomic, readwrite) ccBlendFunc blendFunc;

   static Lightning* lightningWithStrikePoint(Point source, Point destination, ccTime duration, ccTime fadeDuration,std::string texturename);
   static Lightning* lightningWithStrikePoint(Point source, Point destination, ccTime duration, ccTime fadeDuration, CCTexture2D* texture);
   static Lightning* initWithStrikePoint(Point source, Point destination, ccTime duration, ccTime fadeDuration,std::string texturename);
   static Lightning* initWithStrikePoint(Point source, Point destination, ccTime duration, ccTime fadeDuration, CCTexture2D* texture);

    void strikeRandom();
    void strikeWithSeed(NSInteger seed);
    void strike();
    void setInitialPoints(Point * initialPoints, NSUInteger noOfInitialPoints);
    void setOpacityModifyRGB(bool modify);
};