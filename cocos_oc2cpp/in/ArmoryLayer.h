//
//  ArmoryLayer.h
//  WaterBox2D
//
//  Created by Juan María Clavería Rosario on 02/04/14.
//  Copyright (c) 2014 Level App Studios. All rights reserved.
//

#import "CCLayer.h"
#import "ShopCoinsLayer.h"
#import "ShopSpecialCoinsLayer.h"
#import "CashNode.h"

@interface ArmoryLayer : CCLayer <ShopCoinsDelegate,ShopSpecialCoinsLayerDelegate,CashNodeDelegate>
+(id)layerWithTime:(ccTime)time numPlayer:(int)numPlayer;
-(void)enableMenu:(BOOL)enable;
@end
