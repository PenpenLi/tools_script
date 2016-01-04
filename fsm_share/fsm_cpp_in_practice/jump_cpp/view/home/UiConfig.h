#ifndef __HiFly__UiConfig__
#define __HiFly__UiConfig__

#define gap_role 213.0f

//zOrder
#define ZORDER_front 10
#define ZORDER_behind 0

//friend item size
#define SIZE_friendItem Size(510,86)

//tag for activity button
#define TAG_btnMail 201
#define TAG_btnTask 202
#define TAG_btnRank 203
#define TAG_btnActivity 204

//tag assets button
#define TAG_shopHpBtn 911
#define TAG_shopDiamondBtn 912
#define TAG_shopGoldBtn 913

//***tips***
#define COUNT_tips 17
#define TIME_tipsDelay 5.0f
#define TIME_tipsScheduleDelay 0.1f
#define SPEED_tipsScroll 5.0f

//*** role layer ***
//role image adjusting
#define SCALE_roleImgAdjust 0.7f

//*************************************************string************************************************

//role Frame img name
#define STR_roleName001 "img_roleBg_001"
#define STR_roleName002 "img_roleBg_002"
#define STR_roleName003 "img_roleBg_003"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~string~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//shop page type
enum ShopType
{
	TYPE_shopDiamond,
	TYPE_shopGold,
	TYPE_shopHp 
};

#endif //__HiFly__UiConfig__