//
//  Lighting.h
//  C2DX_V3.2_Ramboat_WIN
//
//  Created by li on 15/6/23.
//
//

#ifndef __C2DX_V3_2_Ramboat_WIN__Lighting__
#define __C2DX_V3_2_Ramboat_WIN__Lighting__

#include "cocos2d.h"

USING_NS_CC;

typedef struct _ccV2F_T2F
{
    ccVertex2F vertices;
    ccTex2F texCoords;
} ccV2F_T2F;

class Lightning : public Node, public TextureProtocol
{
public:
    static Lightning* lightningWithStrikePoint(cocos2d::Point source, cocos2d::Point destination, float duration, float fadeDuration, std::string texturename);
    static Lightning* lightningWithStrikePoint(cocos2d::Point source, cocos2d::Point destination, float duration, float fadeDuration, Texture2D* texture);
    bool initWithStrikePoint(cocos2d::Point source, cocos2d::Point destination, float duration, float fadeDuration, std::string texturename);
    bool initWithStrikePoint(cocos2d::Point source, cocos2d::Point destination, float duration, float fadeDuration, Texture2D* texture);
    
    void strikeRandom();
    void strikeWithSeed(unsigned long seed);
    void strike();
    void setInitialPoints(cocos2d::Point * initialPoints, unsigned int noOfInitialPoints);
    void setOpacityModifyRGB(bool modify);
    
    virtual void setTexture(Texture2D * texture);
    virtual Texture2D * getTexture()const;
    virtual void setBlendFunc(const BlendFunc &blendFunc);
    virtual const BlendFunc& getBlendFunc()const;
    
    cocos2d::Point strikeSource;
    cocos2d::Point strikeSplitDestination;
    cocos2d::Point strikeDestination;
    
    Color3B color;
    unsigned int opacity;
    
    Color3B displayedColor;
    unsigned displayedOpacity;
    bool cascadeColorEnabled;
    bool cascadeOpacityEnabled;
    bool autoRemoveFromParent;
    
    bool split;
    int displacement;
    int minDisplacement;
    float lightningWidth;
    float fadeDuration;
    float duration;
    BlendFunc blendFunc;
    
    // callback function
    std::function<void(void)> actionLightingWillStrike;
    std::function<void(void)> actionLightingDidStrike;
    
private:
    Lightning();
    ~Lightning();
    bool ensureCapacity(unsigned long count);
    void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
    void onDraw(const Mat4 &transform, uint32_t flags);
    unsigned int computeNumberOfLines(cocos2d::Point pt1, cocos2d::Point pt2, int displace, int minDisplace);
    cocos2d::Point addLightning(cocos2d::Point pt1, cocos2d::Point pt2, int displace, int minDisplace);
    void updateBlendFunc();

    unsigned int getOpacity();
    void setOpacity(unsigned int opacity);
    Color3B getColor();
    void setColor(Color3B color3);
    bool doesOpacityModifyRGB();
    void updateDisplayedColor(Color3B parentColor);
    void updateDisplayedOpacity(unsigned int parentOpacity);
    Color3B getDisplayedColor();
    unsigned int getDisplayedOpacity();
    
    CustomCommand _customCommand;
    ccV2F_T2F *_lightningVertices;
    unsigned int _lightningVAOName;
    unsigned int _lightningBuffersVBO;
    unsigned long _lightningBuffersCapacity;
    cocos2d::Point *_pointVertices;
    unsigned long _noOfPoints;
    unsigned long _noOfInitialPoints;
    
    unsigned long _seed;
    
    unsigned int _colorLocation;
    Color3B _realColor;
    unsigned int _realOpacity;
    
    Color3B _displayedColor;
    unsigned int _displayedOpacity;
    bool _cascadeColorEnabled;
    bool _cascadeOpacityEnabled;
    
    Texture2D *_texture;
    BlendFunc _blendFunc;
    bool _opacityModifyRGB;
    bool _wasCapacityIncreased;
    
    Lightning *_splitLightning;
    float _duration;
    float _fadeDuration;
    bool _autoRemoveFromParent;
};

#endif /* defined(__C2DX_V3_2_Ramboat_WIN__Lighting__) */
