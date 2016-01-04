//// LevelChooseLayer.h created by php script. chris.li
#ifndef __HiFly__LevelChooseLayer
#define __HiFly__LevelChooseLayer

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

class LevelChooseLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(LevelChooseLayer);
private:
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void handleTouchEvent(Ref *pSender, TouchEventType type);

	//get ui reference marked by ui_xxx 
	Button* m_returnBtn;

	Button* m_mailButton;

	Button* m_taskButton;

	Button* m_rankButton;

	Button* m_bbcButton;

	TextAtlas* m_starNum;

	Button* m_addHP;

	Text* m_hpState;

	Button* m_addDiamond;

	Button* m_addCoin;

	Text* m_coinNum;
	//
	Text* m_diamondNum;

	Button* m_battle_shell_1;

	Button* m_battle_shell_2;

	Button* m_battle_shell_3;

	Button* m_battle_shell_4;

	Button* m_battle_shell_5;

	Button* m_battle_shell_6;

	Button* m_battle_shell_7;

	Button* m_battle_shell_8;

	Button* m_battle_shell_9;

	Button* m_battle_shell_10;

	Button* m_battle_shell_11;

	Button* m_battle_shell_12;

	Button* m_battle_shell_13;

	Button* m_battle_shell_14;

	Button* m_battle_shell_15;

	Button* m_battle_shell_16;

	Button* m_battle_shell_17;

	Button* m_battle_shell_18;

	Button* m_battle_shell_19;

	Button* m_battle_shell_20;

	Button* m_battle_shell_21;

	Button* m_battle_shell_22;

	Button* m_battle_shell_23;

	Button* m_battle_shell_24;

	Button* m_battle_shell_25;

	Button* m_battle_shell_26;

	Button* m_battle_shell_27;

	Button* m_battle_shell_28;

	Button* m_battle_shell_29;

	Button* m_battle_shell_30;

	Button* m_battle_shell_31;

	Button* m_battle_shell_32;

	Button* m_battle_shell_33;

	Button* m_battle_shell_34;

	Button* m_battle_shell_35;

	Button* m_battle_shell_36;

	Button* m_battle_shell_37;

	Button* m_battle_shell_38;

	Button* m_battle_shell_39;

	Button* m_battle_shell_40;

	ImageView* m_battle_star_off_1;

	ImageView* m_battle_star_off_2;

	ImageView* m_battle_star_off_3;

	ImageView* m_battle_star_off_4;

	ImageView* m_battle_star_off_5;

	ImageView* m_battle_star_off_6;

	ImageView* m_battle_star_off_7;

	ImageView* m_battle_star_off_8;

	ImageView* m_battle_star_off_9;

	ImageView* m_battle_star_off_10;

	ImageView* m_battle_star_off_11;

	ImageView* m_battle_star_off_12;

	ImageView* m_battle_star_off_13;

	ImageView* m_battle_star_off_14;

	ImageView* m_battle_star_off_15;

	ImageView* m_battle_star_off_16;

	ImageView* m_battle_star_off_17;

	ImageView* m_battle_star_off_18;

	ImageView* m_battle_star_off_19;

	ImageView* m_battle_star_off_20;

	ImageView* m_battle_star_off_21;

	ImageView* m_battle_star_off_22;

	ImageView* m_battle_star_off_23;

	ImageView* m_battle_star_off_24;

	ImageView* m_battle_star_off_25;

	ImageView* m_battle_star_off_26;

	ImageView* m_battle_star_off_27;

	ImageView* m_battle_star_off_28;

	ImageView* m_battle_star_off_29;

	ImageView* m_battle_star_off_30;

	ImageView* m_battle_star_off_31;

	ImageView* m_battle_star_off_32;

	ImageView* m_battle_star_off_33;

	ImageView* m_battle_star_off_34;

	ImageView* m_battle_star_off_35;

	ImageView* m_battle_star_off_36;

	ImageView* m_battle_star_off_37;

	ImageView* m_battle_star_off_38;

	ImageView* m_battle_star_off_39;

	ImageView* m_battle_star_off_40;

	ImageView* m_battle_star_off_41;

	ImageView* m_battle_star_off_42;

	ImageView* m_battle_star_off_43;

	ImageView* m_battle_star_off_44;

	ImageView* m_battle_star_off_45;

	ImageView* m_battle_star_off_46;

	ImageView* m_battle_star_off_47;

	ImageView* m_battle_star_off_48;

	ImageView* m_battle_star_off_49;

	ImageView* m_battle_star_off_50;

	ImageView* m_battle_star_off_51;

	ImageView* m_battle_star_off_52;

	ImageView* m_battle_star_off_53;

	ImageView* m_battle_star_off_54;

	ImageView* m_battle_star_off_55;

	ImageView* m_battle_star_off_56;

	ImageView* m_battle_star_off_57;

	ImageView* m_battle_star_off_58;

	ImageView* m_battle_star_off_59;

	ImageView* m_battle_star_off_60;

	ImageView* m_battle_star_off_61;

	ImageView* m_battle_star_off_62;

	ImageView* m_battle_star_off_63;

	ImageView* m_battle_star_off_64;

	ImageView* m_battle_star_off_65;

	ImageView* m_battle_star_off_66;

	ImageView* m_battle_star_off_67;

	ImageView* m_battle_star_off_68;

	ImageView* m_battle_star_off_69;

	ImageView* m_battle_star_off_70;

	ImageView* m_battle_star_off_71;

	ImageView* m_battle_star_off_72;

	ImageView* m_battle_star_off_73;

	ImageView* m_battle_star_off_74;

	ImageView* m_battle_star_off_75;

	ImageView* m_battle_star_off_76;

	ImageView* m_battle_star_off_77;

	ImageView* m_battle_star_off_78;

	ImageView* m_battle_star_off_79;

	ImageView* m_battle_star_off_80;

	ImageView* m_battle_star_off_81;

	ImageView* m_battle_star_off_82;

	ImageView* m_battle_star_off_83;

	ImageView* m_battle_star_off_84;

	ImageView* m_battle_star_off_85;

	ImageView* m_battle_star_off_86;

	ImageView* m_battle_star_off_87;

	ImageView* m_battle_star_off_88;

	ImageView* m_battle_star_off_89;

	ImageView* m_battle_star_off_90;

	ImageView* m_battle_star_off_91;

	ImageView* m_battle_star_off_92;

	ImageView* m_battle_star_off_93;

	ImageView* m_battle_star_off_94;

	ImageView* m_battle_star_off_95;

	ImageView* m_battle_star_off_96;

	ImageView* m_battle_star_off_97;

	ImageView* m_battle_star_off_98;

	ImageView* m_battle_star_off_99;

	ImageView* m_battle_star_off_100;

	ImageView* m_battle_star_off_101;

	ImageView* m_battle_star_off_102;

	ImageView* m_battle_star_off_103;

	ImageView* m_battle_star_off_104;

	ImageView* m_battle_star_off_105;

	ImageView* m_battle_star_off_106;

	ImageView* m_battle_star_off_107;

	ImageView* m_battle_star_off_108;

	ImageView* m_battle_star_off_109;

	ImageView* m_battle_star_off_110;

	ImageView* m_battle_star_off_111;

	ImageView* m_battle_star_off_112;

	ImageView* m_battle_star_off_113;

	ImageView* m_battle_star_off_114;

	ImageView* m_battle_star_off_115;

	ImageView* m_battle_star_off_116;

	ImageView* m_battle_star_off_117;

	ImageView* m_battle_star_off_118;

	ImageView* m_battle_star_off_119;

	ImageView* m_battle_star_off_120;

	ImageView* m_battle_point_off_1;

	ImageView* m_battle_point_off_2;

	ImageView* m_battle_point_off_3;

	ImageView* m_battle_point_off_4;

	ImageView* m_battle_point_off_5;

	ImageView* m_battle_point_off_6;

	ImageView* m_battle_point_off_7;

	ImageView* m_battle_point_off_8;

	ImageView* m_battle_point_off_9;

	ImageView* m_battle_point_off_10;

	ImageView* m_battle_point_off_11;

	ImageView* m_battle_point_off_12;

	ImageView* m_battle_point_off_13;

	ImageView* m_battle_point_off_14;

	ImageView* m_battle_point_off_15;

	ImageView* m_battle_point_off_16;

	ImageView* m_battle_point_off_17;

	ImageView* m_battle_point_off_18;

	ImageView* m_battle_point_off_19;

	ImageView* m_battle_point_off_20;

	ImageView* m_battle_point_off_21;

	ImageView* m_battle_point_off_22;

	ImageView* m_battle_point_off_23;

	ImageView* m_battle_point_off_24;

	ImageView* m_battle_point_off_25;

	ImageView* m_battle_point_off_26;

	ImageView* m_battle_point_off_27;

	ImageView* m_battle_point_off_28;

	ImageView* m_battle_point_off_29;

	ImageView* m_battle_point_off_30;

	ImageView* m_battle_point_off_31;

	ImageView* m_battle_point_off_32;

	ImageView* m_battle_point_off_33;

	ImageView* m_battle_point_off_34;

	ImageView* m_battle_point_off_35;

	ImageView* m_battle_point_off_36;

	ImageView* m_battle_point_off_37;

	ImageView* m_battle_point_off_38;

	ImageView* m_battle_point_off_39;

	ImageView* m_battle_point_off_40;

	ImageView* m_battle_point_off_41;

	ImageView* m_battle_point_off_42;

	ImageView* m_battle_point_off_43;

	ImageView* m_battle_point_off_44;

	ImageView* m_battle_point_off_45;

	ImageView* m_battle_point_off_46;

	ImageView* m_battle_point_off_47;

	ImageView* m_battle_point_off_48;

	ImageView* m_battle_point_off_49;

	ImageView* m_battle_point_off_50;

	ImageView* m_battle_point_off_51;

	ImageView* m_battle_point_off_52;

	ImageView* m_battle_point_off_53;

	ImageView* m_battle_point_off_54;

	ImageView* m_battle_point_off_55;

	ImageView* m_battle_point_off_56;

	ImageView* m_battle_point_off_57;

	ImageView* m_battle_point_off_58;

	ImageView* m_battle_point_off_59;

	ImageView* m_battle_point_off_60;

	ImageView* m_battle_point_off_61;

	ImageView* m_battle_point_off_62;

	ImageView* m_battle_point_off_63;

	ImageView* m_battle_point_off_64;

	ImageView* m_battle_point_off_65;

	ImageView* m_battle_point_off_66;

	ImageView* m_battle_point_off_67;

	ImageView* m_battle_point_off_68;

	ImageView* m_battle_point_off_69;

	ImageView* m_battle_point_off_70;

	ImageView* m_battle_point_off_71;

	ImageView* m_battle_point_off_72;

	ImageView* m_battle_point_off_73;

	ImageView* m_battle_point_off_74;

	ImageView* m_battle_point_off_75;

	ImageView* m_battle_point_off_76;

	ImageView* m_battle_point_off_77;

	ImageView* m_battle_point_off_78;

	ImageView* m_battle_point_off_79;

	ImageView* m_battle_point_off_80;

	ImageView* m_battle_point_off_81;

	ImageView* m_battle_point_off_82;

	ImageView* m_battle_point_off_83;

	ImageView* m_battle_point_off_84;

	ImageView* m_battle_point_off_85;

	ImageView* m_battle_point_off_86;

	ImageView* m_battle_point_off_87;

	ImageView* m_battle_point_off_88;

	ImageView* m_battle_point_off_89;

	ImageView* m_battle_point_off_90;

	ImageView* m_battle_point_off_91;

	ImageView* m_battle_point_off_92;

	ImageView* m_battle_point_off_93;

	ImageView* m_battle_point_off_94;

	ImageView* m_battle_point_off_95;

	ImageView* m_battle_point_off_96;

	ImageView* m_battle_point_off_97;

	ImageView* m_battle_point_off_98;

	ImageView* m_battle_point_off_99;

	ImageView* m_battle_point_off_100;

	ImageView* m_battle_point_off_101;

	ImageView* m_battle_point_off_102;

	ImageView* m_battle_point_off_103;

	ImageView* m_battle_point_off_104;

	ImageView* m_battle_point_off_105;

	ImageView* m_battle_point_off_106;

	ImageView* m_battle_point_off_107;

	ImageView* m_battle_point_off_108;

	ImageView* m_battle_point_off_109;

	ImageView* m_battle_point_off_110;

	ImageView* m_battle_point_off_111;

	ImageView* m_battle_point_off_112;

	ImageView* m_battle_point_off_113;

	ImageView* m_battle_point_off_114;

	ImageView* m_battle_point_off_115;

	ImageView* m_battle_point_off_116;

	ImageView* m_battle_point_off_117;

	ImageView* m_level_limit_6;

	ImageView* m_level_limit_11;

	ImageView* m_level_limit_16;

	ImageView* m_level_limit_21;

	ImageView* m_level_limit_26;

	ImageView* m_level_limit_31;

	ImageView* m_level_limit_36;

	LoadingBar* m_hpState1;

	LoadingBar* m_hpState2;

	LoadingBar* m_hpState3;

	LoadingBar* m_hpState4;

	LoadingBar* m_hpState5;

	Text* m_mailNum;

	Text* m_taskNum;

	ImageView* m_battle_star_on_1;

	ImageView* m_battle_star_on_2;

	ImageView* m_battle_star_on_3;

	ImageView* m_battle_star_on_4;

	ImageView* m_battle_star_on_5;

	ImageView* m_battle_star_on_6;

	ImageView* m_battle_star_on_7;

	ImageView* m_battle_star_on_8;

	ImageView* m_battle_star_on_9;

	ImageView* m_battle_star_on_10;

	ImageView* m_battle_star_on_11;

	ImageView* m_battle_star_on_12;

	ImageView* m_battle_star_on_13;

	ImageView* m_battle_star_on_14;

	ImageView* m_battle_star_on_15;

	ImageView* m_battle_star_on_16;

	ImageView* m_battle_star_on_17;

	ImageView* m_battle_star_on_18;

	ImageView* m_battle_star_on_19;

	ImageView* m_battle_star_on_20;

	ImageView* m_battle_star_on_21;

	ImageView* m_battle_star_on_22;

	ImageView* m_battle_star_on_23;

	ImageView* m_battle_star_on_24;

	ImageView* m_battle_star_on_25;

	ImageView* m_battle_star_on_26;

	ImageView* m_battle_star_on_27;

	ImageView* m_battle_star_on_28;

	ImageView* m_battle_star_on_29;

	ImageView* m_battle_star_on_30;

	ImageView* m_battle_star_on_31;

	ImageView* m_battle_star_on_32;

	ImageView* m_battle_star_on_33;

	ImageView* m_battle_star_on_34;

	ImageView* m_battle_star_on_35;

	ImageView* m_battle_star_on_36;

	ImageView* m_battle_star_on_37;

	ImageView* m_battle_star_on_38;

	ImageView* m_battle_star_on_39;

	ImageView* m_battle_star_on_40;

	ImageView* m_battle_star_on_41;

	ImageView* m_battle_star_on_42;

	ImageView* m_battle_star_on_43;

	ImageView* m_battle_star_on_44;

	ImageView* m_battle_star_on_45;

	ImageView* m_battle_star_on_46;

	ImageView* m_battle_star_on_47;

	ImageView* m_battle_star_on_48;

	ImageView* m_battle_star_on_49;

	ImageView* m_battle_star_on_50;

	ImageView* m_battle_star_on_51;

	ImageView* m_battle_star_on_52;

	ImageView* m_battle_star_on_53;

	ImageView* m_battle_star_on_54;

	ImageView* m_battle_star_on_55;

	ImageView* m_battle_star_on_56;

	ImageView* m_battle_star_on_57;

	ImageView* m_battle_star_on_58;

	ImageView* m_battle_star_on_59;

	ImageView* m_battle_star_on_60;

	ImageView* m_battle_star_on_61;

	ImageView* m_battle_star_on_62;

	ImageView* m_battle_star_on_63;

	ImageView* m_battle_star_on_64;

	ImageView* m_battle_star_on_65;

	ImageView* m_battle_star_on_66;

	ImageView* m_battle_star_on_67;

	ImageView* m_battle_star_on_68;

	ImageView* m_battle_star_on_69;

	ImageView* m_battle_star_on_70;

	ImageView* m_battle_star_on_71;

	ImageView* m_battle_star_on_72;

	ImageView* m_battle_star_on_73;

	ImageView* m_battle_star_on_74;

	ImageView* m_battle_star_on_75;

	ImageView* m_battle_star_on_76;

	ImageView* m_battle_star_on_77;

	ImageView* m_battle_star_on_78;

	ImageView* m_battle_star_on_79;

	ImageView* m_battle_star_on_80;

	ImageView* m_battle_star_on_81;

	ImageView* m_battle_star_on_82;

	ImageView* m_battle_star_on_83;

	ImageView* m_battle_star_on_84;

	ImageView* m_battle_star_on_85;

	ImageView* m_battle_star_on_86;

	ImageView* m_battle_star_on_87;

	ImageView* m_battle_star_on_88;

	ImageView* m_battle_star_on_89;

	ImageView* m_battle_star_on_90;

	ImageView* m_battle_star_on_91;

	ImageView* m_battle_star_on_92;

	ImageView* m_battle_star_on_93;

	ImageView* m_battle_star_on_94;

	ImageView* m_battle_star_on_95;

	ImageView* m_battle_star_on_96;

	ImageView* m_battle_star_on_97;

	ImageView* m_battle_star_on_98;

	ImageView* m_battle_star_on_99;

	ImageView* m_battle_star_on_100;

	ImageView* m_battle_star_on_101;

	ImageView* m_battle_star_on_102;

	ImageView* m_battle_star_on_103;

	ImageView* m_battle_star_on_104;

	ImageView* m_battle_star_on_105;

	ImageView* m_battle_star_on_106;

	ImageView* m_battle_star_on_107;

	ImageView* m_battle_star_on_108;

	ImageView* m_battle_star_on_109;

	ImageView* m_battle_star_on_110;

	ImageView* m_battle_star_on_111;

	ImageView* m_battle_star_on_112;

	ImageView* m_battle_star_on_113;

	ImageView* m_battle_star_on_114;

	ImageView* m_battle_star_on_115;

	ImageView* m_battle_star_on_116;

	ImageView* m_battle_star_on_117;

	ImageView* m_battle_star_on_118;

	ImageView* m_battle_star_on_119;

	ImageView* m_battle_star_on_120;

	ImageView* m_battle_point_on_1;

	ImageView* m_battle_point_on_2;

	ImageView* m_battle_point_on_3;

	ImageView* m_battle_point_on_4;

	ImageView* m_battle_point_on_5;

	ImageView* m_battle_point_on_6;

	ImageView* m_battle_point_on_7;

	ImageView* m_battle_point_on_8;

	ImageView* m_battle_point_on_9;

	ImageView* m_battle_point_on_10;

	ImageView* m_battle_point_on_11;

	ImageView* m_battle_point_on_12;

	ImageView* m_battle_point_on_13;

	ImageView* m_battle_point_on_14;

	ImageView* m_battle_point_on_15;

	ImageView* m_battle_point_on_16;

	ImageView* m_battle_point_on_17;

	ImageView* m_battle_point_on_18;

	ImageView* m_battle_point_on_19;

	ImageView* m_battle_point_on_20;

	ImageView* m_battle_point_on_21;

	ImageView* m_battle_point_on_22;

	ImageView* m_battle_point_on_23;

	ImageView* m_battle_point_on_24;

	ImageView* m_battle_point_on_25;

	ImageView* m_battle_point_on_26;

	ImageView* m_battle_point_on_27;

	ImageView* m_battle_point_on_28;

	ImageView* m_battle_point_on_29;

	ImageView* m_battle_point_on_30;

	ImageView* m_battle_point_on_31;

	ImageView* m_battle_point_on_32;

	ImageView* m_battle_point_on_33;

	ImageView* m_battle_point_on_34;

	ImageView* m_battle_point_on_35;

	ImageView* m_battle_point_on_36;

	ImageView* m_battle_point_on_37;

	ImageView* m_battle_point_on_38;

	ImageView* m_battle_point_on_39;

	ImageView* m_battle_point_on_40;

	ImageView* m_battle_point_on_41;

	ImageView* m_battle_point_on_42;

	ImageView* m_battle_point_on_43;

	ImageView* m_battle_point_on_44;

	ImageView* m_battle_point_on_45;

	ImageView* m_battle_point_on_46;

	ImageView* m_battle_point_on_47;

	ImageView* m_battle_point_on_48;

	ImageView* m_battle_point_on_49;

	ImageView* m_battle_point_on_50;

	ImageView* m_battle_point_on_51;

	ImageView* m_battle_point_on_52;

	ImageView* m_battle_point_on_53;

	ImageView* m_battle_point_on_54;

	ImageView* m_battle_point_on_55;

	ImageView* m_battle_point_on_56;

	ImageView* m_battle_point_on_57;

	ImageView* m_battle_point_on_58;

	ImageView* m_battle_point_on_59;

	ImageView* m_battle_point_on_60;

	ImageView* m_battle_point_on_61;

	ImageView* m_battle_point_on_62;

	ImageView* m_battle_point_on_63;

	ImageView* m_battle_point_on_64;

	ImageView* m_battle_point_on_65;

	ImageView* m_battle_point_on_66;

	ImageView* m_battle_point_on_67;

	ImageView* m_battle_point_on_68;

	ImageView* m_battle_point_on_69;

	ImageView* m_battle_point_on_70;

	ImageView* m_battle_point_on_71;

	ImageView* m_battle_point_on_72;

	ImageView* m_battle_point_on_73;

	ImageView* m_battle_point_on_74;

	ImageView* m_battle_point_on_75;

	ImageView* m_battle_point_on_76;

	ImageView* m_battle_point_on_77;

	ImageView* m_battle_point_on_78;

	ImageView* m_battle_point_on_79;

	ImageView* m_battle_point_on_80;

	ImageView* m_battle_point_on_81;

	ImageView* m_battle_point_on_82;

	ImageView* m_battle_point_on_83;

	ImageView* m_battle_point_on_84;

	ImageView* m_battle_point_on_85;

	ImageView* m_battle_point_on_86;

	ImageView* m_battle_point_on_87;

	ImageView* m_battle_point_on_88;

	ImageView* m_battle_point_on_89;

	ImageView* m_battle_point_on_90;

	ImageView* m_battle_point_on_91;

	ImageView* m_battle_point_on_92;

	ImageView* m_battle_point_on_93;

	ImageView* m_battle_point_on_94;

	ImageView* m_battle_point_on_95;

	ImageView* m_battle_point_on_96;

	ImageView* m_battle_point_on_97;

	ImageView* m_battle_point_on_98;

	ImageView* m_battle_point_on_99;

	ImageView* m_battle_point_on_100;

	ImageView* m_battle_point_on_101;

	ImageView* m_battle_point_on_102;

	ImageView* m_battle_point_on_103;

	ImageView* m_battle_point_on_104;

	ImageView* m_battle_point_on_105;

	ImageView* m_battle_point_on_106;

	ImageView* m_battle_point_on_107;

	ImageView* m_battle_point_on_108;

	ImageView* m_battle_point_on_109;

	ImageView* m_battle_point_on_110;

	ImageView* m_battle_point_on_111;

	ImageView* m_battle_point_on_112;

	ImageView* m_battle_point_on_113;

	ImageView* m_battle_point_on_114;

	ImageView* m_battle_point_on_115;

	ImageView* m_battle_point_on_116;

	ImageView* m_battle_point_on_117;

	Button* m_limit_button_shell_6;
	Button* m_limit_button_shell_11;
	Button* m_limit_button_shell_16;
	Button* m_limit_button_shell_21;
	Button* m_limit_button_shell_26;
	Button* m_limit_button_shell_31;
	Button* m_limit_button_shell_36;

	//Coding Here
	Sprite* m_scrollViewBG;
	ScrollView* m_scrollView;
	Layout* m_rootNode;

	float m_scrollviewOffset;
	int m_totalStar;
	int m_newStage;
	//ScrollView 

	void scrollViewCallBack(Ref *pSender, TouchEventType type);

	void setStarNum(int);
	void setHP(int);
	void setDiamond(int);
	void setCoin(int);

	void setMailNum(int);
	void setTaskNum(int);

	void setLevelShell(int level, bool onOff);

	void setLevelStarOn(int level, int stars, bool onOff);
	void setLevelStarOff(int level, bool onOff);

	void setLevelLimit(int level, bool onOff);

	void startLevelRoute(int level, bool onOff);

	void startGame(int level);

	void startNewStage(int level);

	void startNewShell();

	void refreshUI();

	void openLimitLevel(int level);

	void startLimitLevel(int level);
	//////////////////////////////// CODING HERE /////////////////////////////////
	//schedule
	void scheduleHpTime(float delayTime);

	//refresh
	void refreshHp();
	void refreshDiamond();
	void refreshGold();
	void refreshMail();
	void refreshTask();

	/////////////////////////////////////member variable///////////
	//HP
	int m_HpNum;
	Vector<LoadingBar*>	m_hpLoadingBarV;

	//mail & task
	Text* m_mailNumText;
	Text* m_taskNumText;

};
#endif
