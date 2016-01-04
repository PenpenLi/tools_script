local define_guard_cmds = {
    ["1"] = {
        ["name"] = "pickcard_dialog_jitan",
        ["dialog"] = "跟我去祭坛，召唤一个英雄",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["2"] = {
        ["name"] = "pickcard_dialog_jitan0",
        ["dialog"] = "被打败了，我们要重新集结所有的英雄，再次战斗",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["3"] = {
        ["name"] = "pickcard_click_jitanbtn",
        ["textdialog"] = "快来组建你的战队，开始新的旅程吧",
        ["tip"] = "step2:提示点击祭坛"
    },
    ["4"] = {
        ["name"] = "pickcard_click_pickcardbtn",
        ["textdialog"] = "在这里迎接你的第一个小伙伴，来看看会是谁呢？",
        ["tip"] = "step3:提示点击左边的抽卡"
    },
    --[[["5"] = {
        ["name"] = "pickcard_dialog_cardfunction",
        ["dialog"] = "运气太好了，是个红色英雄，最高级品质。",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step4:"
    },--]]
    ["6"] = {
        ["name"] = "pickcard_click_surebtn",
        ["tip"] = "step5:提示点击确定"
    },

    ["7"] = {
        ["name"] = "pickcard_click_closebtn",
        ["tip"] = "step6:提示关闭对话框"
    },

    ["8"] = {
        ["name"] = "pve_dialog_battle",
        ["dialog"] = "现在，带着英雄去战斗吧。",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step4:"
    },

    ["9"] = {
        ["name"] = "pve_click_battlebtn",
        ["textdialog"] = "从这里开始你新的冒险吧",
        ["tip"] = "step1:点击战役按钮"
    },

    ["10"] = {
        ["name"] = "pve_click_level1btn",
        ["textdialog"] = "进入目标关卡，迎接你的战斗",
        ["tip"] = "step2:点击第一关"
    },

    ["11"] = {
        ["name"] = "pve_click_goonbtn",
        ["tip"] = "step3:点击继续按钮"
    },

    ["12"] = {
        ["name"] = "pve_click_hero1btn",
        ["textdialog"] = "点击把小伙伴都派上阵",
        ["tip"] = "step4:点击上阵英雄1"
    },

    ["13"] = {
        ["name"] = "pve_click_hero2btn",
        ["textdialog"] = "点击把小伙伴都派上阵",
        ["tip"] = "step5:点击上阵英雄2"
    },

    ["14"] = {
        ["name"] = "pve_dialog_skill",
        ["dialog"] = "现在怒气满了，英雄可以放大招了",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step6:提醒放技能"
    },


    ["15"] = {
        ["name"] = "pve_click_fireskillbtn",
        ["textdialog"] = "英雄怒气满的时候，点击图标可释放英雄技能",
        ["tip"] = "fire big skill"
    },

    ["16"] = {
        ["name"] = "pve_dialog_jxskill",
        ["dialog"] = "右边的技能，都是觉醒技。",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step7:提醒放技能"
    },

    ["17"] = {
        ["name"] = "pve_dialog_jxskill2",
        ["dialog"] = "特定组合的英雄，一起战斗时，才会发动这种技能",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step7:提醒放技能"
    },

    ["18"] = {
        ["name"] = "pve_dialog_jxskill3",
        ["dialog"] = "觉醒技，是比刚才的大招，更厉害的存在",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step7:提醒放技能"
    },

    ["19"] = {
        ["name"] = "pve_click_fireJXbtn",
        ["textdialog"] = "巧妙地使用觉醒技，可以快速改变战斗格局",
        ["tip"] = "fire 觉醒技能"
    },

    ["20"] = {
        ["name"] = "pve_click_fightbtn",
        ["tip"] = "step6:点击开始战斗按钮"
    },

    ["21"] = {
        ["name"] = "pve_dialog_log1",
        ["dialog"] = "赢的很轻松嘛",
        ["direction"] = 1,
        ["person"] = 2,
        ["tip"] = "step7:对话1"
    },

    ["22"] = {
        ["name"] = "pve_dialog_log2",
        ["dialog"] = "是的，但是我们只有2个英雄，是不够的，还需要更多的帮手",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step7:对话2"
    },

    ["23"] = {
        ["name"] = "pve_click_surebtn",
        ["tip"] = "sure button"
    },

    ["24"] = {
        ["name"] = "pve_click_closepvebtn",
        ["tip"] = "step7：关闭PVE"
    },

    ["25"] = {
        ["name"] = "qianghua_click_tiejiangpu",
        ["textdialog"] = "铁匠铺可以强化你的装备，为你带来更多的战斗力",
        ["tip"] = "step1:点击主城铁匠铺"
    },

    ["26"] = {
        ["name"] = "qianghua_click_dazhaobtn",
        ["textdialog"] = "选择需要强化的装备",
        ["tip"] = "step2:点击在装备武器列表的第一个装备的打造按钮的位置"
    },

    ["27"] = {
        ["name"] = "qianghua_click_qianghuabtn",
        ["textdialog"] = "点击强化你的装备",
        ["tip"] = "step3:点击在强化界面上“强化”的按钮的位置"
    },

    ["28"] = {
        ["name"] = "qianghua_click_returnbtn",
        ["tip"] = "step4:返回"
    },

    ["29"] = {
        ["name"] = "qianghua_click_closebtn",
        ["tip"] = "step5:关闭"
    },

    ["30"] = {
        ["name"] = "jinjie_dialog_jinjie",
        ["dialog"] = "进阶的装备，让你的英雄更强力",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["31"] = {
        ["name"] = "jinjie_click_tiejiangpu",
        ["textdialog"] = "使用稀有材料可进阶装备，进阶将大幅提高装备自身属性",
        ["tip"] = "step2:点击城镇上的铁匠铺"
    },

    ["32"] = {
        ["name"] = "jinjie_click_dazaobtn",
        ["textdialog"] = "选择需要进阶的装备",
        ["tip"] = "step3:点击装备武器列表第一个装备“打造”按钮的位置"
    },

    ["33"] = {
        ["name"] = "jinjie_click_jinjiebtn",
        ["textdialog"] = "进入装备进阶面板",
        ["tip"] = "step4:点击“进阶”按钮"
    },

    ["34"] = {
        ["name"] = "jinjie_click_jinjiebtn2",
        ["textdialog"] = "点击进阶装备品质",
        ["tip"] = "step5:点击“进阶”按钮"
    },

    ["35"] = {
        ["name"] = "jinjie_click_returnbtn",
        ["tip"] = "step6:返回"
    },

    ["36"] = {
        ["name"] = "jinjie_click_closebtn",
        ["tip"] = "step7:关闭"
    },

    ["37"] = {
        ["name"] = "duobao_dialog_huodong",
        ["dialog"] = "现在可以开启夺宝啦",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["38"] = {
        ["name"] = "duobao_click_huodongbtn",
        ["textdialog"] = "每日参加活动可获得丰富的资源",
        ["tip"] = "step2:点击活动按钮"
    },

    ["39"] = {
        ["name"] = "duobao_click_duobaobtn",
        ["textdialog"] = "参加夺宝可获得稀有的技能药水",
        ["tip"] = "step3:点击夺宝按钮"
    },

    --[[["40"] = {
        ["name"] = "duobao_click_feiting",
        ["tip"] = "step4:点击飞艇图标"
    },

    ["41"] = {
        ["name"] = "duobao_click_zhanbtn",
        ["tip"] = "step5:点击战斗按钮"
    },

    ["42"] = {
        ["name"] = "duobao_click_fomationzhanbtn",
        ["tip"] = "step6:点击布阵面板战斗按钮"
    },--]]

    ["43"] = {
        ["name"] = "jingyanyaoshui_dialog_hero",
        ["dialog"] = "去给英雄升级吧",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["44"] = {
        ["name"] = "jingyanyaoshui_click_herobtn",
        ["textdialog"] = "通过使用战斗掉落的经验药水可快速提升英雄等级",
        ["tip"] = "step2:点击城镇英雄按钮"
    },

    ["45"] = {
        ["name"] = "jingyanyaoshui_click_heroicon",
        ["textdialog"] = "选择需要提升等级的英雄",
        ["tip"] = "step3:点击英雄图标"
    },

    ["46"] = {
        ["name"] = "jingyanyaoshui_click_heroshengjibtn",
        ["textdialog"] = "进入英雄升级界面",
        ["tip"] = "step4:点击英雄升级按钮"
    },

    ["47"] = {
        ["name"] = "jingyanyaoshui_click_shengjibtn",
        ["textdialog"] = "点击提升英雄等级",
        ["tip"] = "step5:点击升级按钮"
    },

    ["48"] = {
        ["name"] = "sport_dialog_sport",
        ["dialog"] = "，去试试你的实力吧",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["49"] = {
        ["name"] = "sport_click_sportbtn",
        ["textdialog"] = "挑战竞技场，来证明你的实力吧",
        ["tip"] = "step2:点击城镇竞技场"
    },

    ["50"] = {
        ["name"] = "sport_click_challenge",
        ["textdialog"] = "选择一个对手，祝你挑战成功",
        ["tip"] = "step3:点击竞技场内战斗按钮"
    },

    --[[["51"] = {
        ["name"] = "sport_click_fomationicon1",
        ["tip"] = "step4:点击布阵内英雄图标1"
    },

    ["52"] = {
        ["name"] = "sport_click_fomationicon2",
        ["tip"] = "step5:点击布阵内英雄图标2"
    },

    ["53"] = {
        ["name"] = "sport_click_fomationfight",
        ["tip"] = "step6:点击布阵内战斗按钮"
    },--]]

    ["54"] = {
        ["name"] = "jinengyaoshui_dialog_hero",
        ["dialog"] = "去获得更强力的技能吧",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["55"] = {
        ["name"] = "jinengyaoshui_click_herobtn",
        ["textdialog"] = "使用副本掉落的技能药水可提高英雄的技能等级",
        ["tip"] = "step2:点击城镇英雄按钮"
    },

    ["56"] = {
        ["name"] = "jinengyaoshui_click_heroicon",
        ["textdialog"] = "选择需要提升技能等级的英雄",
        ["tip"] = "step3:点击英雄图标"
    },

    ["57"] = {
        ["name"] = "jinengyaoshui_click_jinengbtn",
        ["textdialog"] = "点击进入技能提升面板",
        ["tip"] = "step4:点击英雄技能升级按钮"
    },

    ["58"] = {
        ["name"] = "jinengyaoshui_click_shengjibtn",
        ["textdialog"] = "点击按钮提升你所需要的技能",
        ["tip"] = "step5:点击升级按钮"
    },

    ["59"] = {
        ["name"] = "herojinjie_dialog_hero",
        ["dialog"] = "英雄可以进阶啦",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["60"] = {
        ["name"] = "herojinjie_click_herobtn",
        ["tip"] = "step2:点击城镇英雄按钮"
    },

    ["61"] = {
        ["name"] = "herojinjie_click_heroicon",
        ["tip"] = "step3:点击英雄图标"
    },

    ["62"] = {
        ["name"] = "herojinjie_click_jinjiebtn",
        ["tip"] = "step4:点击英雄进阶按钮"
    },

    ["63"] = {
        ["name"] = "herowear_dialog_hero",
        ["dialog"] = "你的英雄貌似获得了新装备哦",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["64"] = {
        ["name"] = "herowear_click_herobtn",
        ["textdialog"] = "穿戴副本掉落的装备可提高英雄战斗力",
        ["tip"] = "step2:点击城镇英雄按钮"
    },

    ["65"] = {
        ["name"] = "herowear_click_heroicon",
        ["textdialog"] = "选择需要穿戴装备的英雄",
        ["tip"] = "step3:点击英雄图标"
    },

    ["66"] = {
        ["name"] = "herowear_click_weaponbtn",
        ["textdialog"] = "显示绿色加号的表明有可使用的多余装备",
        ["tip"] = "step4:点击武器icon"
    },

    ["67"] = {
        ["name"] = "herowear_click_wearbtn",
        ["textdialog"] = "点击按钮给英雄穿戴装备",
        ["tip"] = "step5:点击装备按钮"
    },

    ["68"] = {
        ["name"] = "herowear_click_backbtn",
        ["tip"] = "step6:返回英雄界面"
    },

    ["69"] = {
        ["name"] = "herowear_click_closebtn",
        ["tip"] = "step6:关闭英雄界面"
    },

    ["70"] = {
        ["name"] = "task_dialog_task",
        ["dialog"] = "你是第一天来我们的世界，我要送你一些礼物。",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["71"] = {
        ["name"] = "task_click_taskbtn",
        ["textdialog"] = "你是第一天来我们的世界，我要送你一些礼物",
        ["tip"] = "step2:点击城镇任务按钮"
    },

    ["72"] = {
        ["name"] = "task_click_loadbtn",
        ["textdialog"] = "记得每天上游戏领取登陆领奖！",
        ["tip"] = "step3:点击累计登录"
    },

    ["73"] = {
        ["name"] = "task_click_award",
        ["textdialog"] = "点击领取奖励，获得你的第一份礼物",
        ["tip"] = "step4:点击累计登录第一个奖励"
    },

    ["74"] = {
        ["name"] = "task_click_closetask",
        ["tip"] = "step5:退出任务界面"
    },

    ["75"] = {
        ["name"] = "task2_dialog_task",
        ["dialog"] = "刚才战斗，表现的不错，去看看还有什么奖励",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["76"] = {
        ["name"] = "task2_click_taskbtn",
        ["textdialog"] = "刚才表现不错，看看能获得什么奖励",
        ["tip"] = "step2:点击城镇任务按钮"
    },

    ["77"] = {
        ["name"] = "task2_click_levelupBtn",
        ["textdialog"] = "成功升级可领取丰富的奖励",
        ["tip"] = "step3:点击升级赢钻石"
    },

    ["78"] = {
        ["name"] = "task2_click_award",
        ["textdialog"] = "点击领取第三个英雄的碎片",
        ["tip"] = "step4:点击升级赢钻石第一个奖励"
    },

    ["79"] = {
        ["name"] = "task2_click_closetask",
        ["tip"] = "step5:退出任务界面"
    },

    ["80"] = {
        ["name"] = "task22_dialog_hero",
        ["dialog"] = "现在有了足够的碎片，我们可以合成新英雄了。",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["81"] = {
        ["name"] = "task22_click_herobtn",
        ["textdialog"] = "我们先来召唤你的新英雄",
        ["tip"] = "step1:点击城镇英雄按钮"
    },

    ["82"] = {
        ["name"] = "task22_click_heroicon",
        ["finger"] = "1",
        ["tip"] = "step2:点击英雄图标"
    },

    ["83"] = {
        ["name"] = "task22_click_fixbtn",
        ["textdialog"] = "点击召唤英雄",
        ["tip"] = "step3:点击英雄合成按钮"
    },
    
    ["84"] = {
        ["name"] = "task22_click_fixbtn2",
        ["textdialog"] = "你还有更多碎片可提升英雄品质",
        ["tip"] = "step3:再次点击英雄合成按钮"
    },
    
    ["85"] = {
        ["name"] = "task22_click_fixbtn3",
        ["tip"] = "step3:再次点击英雄合成按钮"
    },

    ["86"] = {
        ["name"] = "task22_click_backbtn",
        ["tip"] = "step4:返回"
    },

    ["87"] = {
        ["name"] = "task22_click_closebtn",
        ["tip"] = "step5:退出"
    },

    ["88"] = {
        ["name"] = "herofix_dialog_hero",
        ["dialog"] = "刚才获得了怀特迈恩的英雄碎片，这些碎片，足够合成一个英雄了",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["89"] = {
        ["name"] = "herofix_dialog_hero2",
        ["dialog"] = "去合成新的英雄吧。",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["90"] = {
        ["name"] = "herofix_click_herobtn",
        ["textdialog"] = "我们现在去召唤第二个英雄吧",
        ["tip"] = "step1:点击城镇英雄按钮"
    },

    ["91"] = {
        ["name"] = "herofix_click_heroicon",
        ["textdialog"] = "下方选择怀特图标",
        ["finger"] = "1",
        ["tip"] = "step2:点击英雄图标"
    },

    ["92"] = {
        ["name"] = "herofix_click_fixbtn",
        ["textdialog"] = "点击合成召唤英雄",
        ["tip"] = "step3:点击英雄合成按钮"
    },

    ["93"] = {
        ["name"] = "herofix_dialog_hero3",
        ["dialog"] = "还有多余的碎片，可以提高她的品质。",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["94"] = {
        ["name"] = "herofix_dialog_hero4",
        ["dialog"] = "品质越高，英雄越强大！",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },
    
    ["95"] = {
        ["name"] = "herofix_click_fixbtn2",
        ["textdialog"] = "你还有更多碎片，可以提高她的品质",
        ["tip"] = "step3:再次点击英雄合成按钮"
    },
    
   --[[ ["96"] = {
        ["name"] = "herofix_click_fixbtn3",
        ["tip"] = "step4:再次点击英雄合成按钮"
    },--]]

    ["97"] = {
        ["name"] = "herofix_click_backbtn",
        ["tip"] = "step5:返回"
    },

    ["98"] = {
        ["name"] = "herofix_click_closebtn",
        ["tip"] = "step6:退出"
    },

    ["99"] = {
        ["name"] = "pve2_dialog_battle",
        ["dialog"] = "现在有3个英雄了，再次去战斗吧",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step1:"
    },

    ["100"] = {
        ["name"] = "pve2_click_battlebtn",
        ["textdialog"] = "带上你的新伙伴再次战斗吧",
        ["tip"] = "step1:点击战役按钮"
    },

    ["101"] = {
        ["name"] = "pve2_click_level2btn",
        ["tip"] = "step2:点击第二关"
    },

    ["102"] = {
        ["name"] = "pve2_click_goonbtn",
        ["tip"] = "step3:点击继续按钮"
    },

    ["103"] = {
        ["name"] = "pve2_click_hero3btn",
        ["textdialog"] = "阿尔萨斯是一个非常耐揍的英雄",
        ["tip"] = "step3:选择第三个英雄"
    },

    ["104"] = {
        ["name"] = "pve2_click_fightbtn",
        ["tip"] = "step6:点击开始战斗按钮"
    },

    ["105"] = {
        ["name"] = "pve2_click_fireskillbtn",
        ["tip"] = "fire big skill"
    },

    ["106"] = {
        ["name"] = "pve2_click_fireJXbtn",
        ["tip"] = "fire 觉醒技能"
    },

    ["107"] = {
        ["name"] = "pve2_click_surebtn",
        ["tip"] = "sure button"
    },

    ["108"] = {
        ["name"] = "pve2_click_closepvebtn",
        ["tip"] = "step9：关闭PVE"
    },

    ["109"] = {
        ["name"] = "pickcard_click_armaturebtn",
        ["tip"] = "pickcard_click_armaturebtn"
    },
    
    --[[["110"] = {
        ["name"] = "pve3_click_battlebtn",
        ["tip"] = "step1:点击战役按钮"
    },

    ["111"] = {
        ["name"] = "pve3_click_level3btn",
        ["tip"] = "step2:点击第三关"
    },

    ["112"] = {
        ["name"] = "pve3_click_goonbtn",
        ["tip"] = "step3:点击继续按钮"
    },

    ["113"] = {
        ["name"] = "pve3_click_fightbtn",
        ["tip"] = "step4:点击开始战斗按钮"
    },

    ["114"] = {
        ["name"] = "pve3_click_surebtn",
        ["tip"] = "sure button"
    },--]]

    ["115"] = {
        ["name"] = "pve3_click_closepvebtn",
        ["tip"] = "step5：关闭PVE"
    },
    
    --[[["116"] = {
        ["name"] = "pve4_click_battlebtn",
        ["tip"] = "step1:点击战役按钮"
    },

    ["117"] = {
        ["name"] = "pve4_click_level4btn",
        ["tip"] = "step2:点击第四关"
    },

    ["118"] = {
        ["name"] = "pve4_click_goonbtn",
        ["tip"] = "step3:点击继续按钮"
    },

    ["119"] = {
        ["name"] = "pve4_click_fightbtn",
        ["tip"] = "step4:点击开始战斗按钮"
    },

    ["120"] = {
        ["name"] = "pve4_click_surebtn",
        ["tip"] = "sure button"
    },--]]

    ["121"] = {
        ["name"] = "pve4_click_closepvebtn",
        ["tip"] = "step5：关闭PVE"
    },
    
   --[[ ["122"] = {
        ["name"] = "pve5_click_battlebtn",
        ["tip"] = "step1:点击战役按钮"
    },

    ["123"] = {
        ["name"] = "pve5_click_level5btn",
        ["tip"] = "step2:点击第五关"
    },

    ["124"] = {
        ["name"] = "pve5_click_goonbtn",
        ["tip"] = "step3:点击继续按钮"
    },

    ["125"] = {
        ["name"] = "pve5_click_fightbtn",
        ["tip"] = "step4:点击开始战斗按钮"
    },

    ["126"] = {
        ["name"] = "pve5_click_surebtn",
        ["tip"] = "sure button"
    },--]]

    ["127"] = {
        ["name"] = "pve5_click_closepvebtn",
        ["tip"] = "step5：关闭PVE"
    },
    
    ["128"] = {
        ["name"] = "pve6_click_battlebtn",
        ["tip"] = "step1:点击战役按钮"
    },

    ["129"] = {
        ["name"] = "pve6_click_level6btn",
        ["tip"] = "step2:点击第六关"
    },

    ["130"] = {
        ["name"] = "pve6_click_goonbtn",
        ["tip"] = "step3:点击继续按钮"
    },

    ["131"] = {
        ["name"] = "pve6_click_fightbtn",
        ["tip"] = "step4:点击开始战斗按钮"
    },

    ["132"] = {
        ["name"] = "pve6_click_surebtn",
        ["tip"] = "sure button"
    },

    ["133"] = {
        ["name"] = "pve6_click_closepvebtn",
        ["tip"] = "step5：关闭PVE"
    },
    
    ["134"] = {
        ["name"] = "jingyanyaoshui_click_closebtn",
        ["tip"] = "step6：关闭英雄界面"
    },
    
    ["135"] = {
        ["name"] = "jinengyaoshui_click_closebtn",
        ["tip"] = "step6：关闭英雄界面"
    },
    
    ["136"] = {
        ["name"] = "jingyanyaoshui_click_back1btn",
        ["tip"] = "step6：返回1"
    },
    
    ["137"] = {
        ["name"] = "jingyanyaoshui_click_backbtn",
        ["tip"] = "step6：返回2"
    },
    
    ["138"] = {
        ["name"] = "jinengyaoshui_click_back2btn",
        ["tip"] = "step6：返回1"
    },

    ["139"] = {
        ["name"] = "jinengyaoshui_click_backbtn",
        ["tip"] = "step6：返回2"
    },
    
   --[[ ["140"] = {
        ["name"] = "moni_dialog_black1",
        ["dialog"] = "我在哪里？这里好像已经不是。。。",
        ["direction"] = 1,
        ["person"] = 2,
        ["tip"] = "step1:"
    },
    
    ["141"] = {
        ["name"] = "moni_dialog_black2",
        ["dialog"] = "命运之子你终于来了，这个世界等你的到来已经很久很久了",
        ["direction"] = 1,
        ["person"] = 4,
        ["tip"] = "step2:"
    },
    
    ["142"] = {
        ["name"] = "moni_dialog_black3",
        ["dialog"] = "这个世界的混乱需要你来拯救",
        ["direction"] = -1,
        ["person"] = 4,
        ["tip"] = "step3:"
    },
    
    ["143"] = {
        ["name"] = "moni_dialog_black4",
        ["dialog"] = "我要怎么拯救这个世界？咦，前方好像爆发了战斗",
        ["direction"] = 1,
        ["person"] = 2,
        ["tip"] = "step4:"
    },
    
    ["144"] = {
        ["name"] = "moni_dialog_black5",
        ["dialog"] = "我要怎么拯救这个世界？咦，前方好像爆发了战斗",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step4:"
    },--]]
    
    ["145"] = {
        ["name"] = "moni_dialog_bt1",
        ["dialog"] = "伊利丹，上！帮我消灭这帮小喽啰",
        ["direction"] = -1,
        ["person"] = 3,
        ["tip"] = "step4:"
    },
    
    ["146"] = {
        ["name"] = "moni_dialog_bt2",
        ["dialog"] = "魔神降临了这个世界，需要你来引导英雄释放他的能量",
        ["direction"] = 1,
        ["person"] = 1,
        ["tip"] = "step4:"
    },
    
    ["147"] = {
        ["name"] = "moni_click_skillBtn",
        ["tip"] = "step1:点击释放技能"
    },
    
    ["148"] = {
        ["name"] = "moni_click_skillBtn2",
        ["tip"] = "step1:点击释放技能"
    },
    
    ["149"] = {
        ["name"] = "moni_click_skillBtn3",
        ["tip"] = "step1:点击释放技能"
    },
    
    ["150"] = {
        ["name"] = "moni_click_skillBtn4",
        ["tip"] = "step1:点击释放技能"
    },
    
    ["151"] = {
        ["name"] = "moni_click_skillBtn5",
        ["tip"] = "step1:点击释放技能"
    },
    
    ["152"] = {
        ["name"] = "moni_click_skillBtn6",
        ["tip"] = "step1:点击释放觉醒"
    },
    
    ["153"] = {
        ["name"] = "moni_dialog_bt3",
        ["dialog"] = "我魔神手下可不会这么容易失败",
        ["direction"] = -1,
        ["person"] = 3,
        ["tip"] = "step4:"
    },
    
    ["154"] = {
        ["name"] = "herowear_click_closepvebtn",
        ["tip"] = "step:关闭PVE"
    },
    
    ["155"] = {
        ["name"] = "jinengyaoshui_click_closepvebtn",
        ["tip"] = "step:关闭PVE"
    },
    
    ["156"] = {
        ["name"] = "qianghua_click_closepvebtn",
        ["tip"] = "step:关闭PVE"
    },
    
    ["157"] = {
        ["name"] = "jinjie_click_closepvebtn",
        ["tip"] = "step:关闭PVE"
    },
    
    ["158"] = {
        ["name"] = "duobao_click_closepvebtn",
        ["tip"] = "step:关闭PVE"
    },
    
    ["159"] = {
        ["name"] = "sport_click_closepvebtn",
        ["tip"] = "step:关闭PVE"
    },
    
    ["160"] = {
        ["name"] = "pickcard2_click_jitanbtn",
        ["textdialog"] = "",
        ["tip"] = "step2:提示点击祭坛"
    },
    ["161"] = {
        ["name"] = "pickcard2_click_pickcardbtn",
        ["textdialog"] = "",
        ["tip"] = "step3:提示点击左边的抽卡"
    },
    
    ["162"] = {
        ["name"] = "pickcard2_click_surebtn",
        ["tip"] = "step5:提示点击确定"
    },

    ["163"] = {
        ["name"] = "pickcard2_click_closebtn",
        ["tip"] = "step6:提示关闭对话框"
    }
}

return define_guard_cmds
