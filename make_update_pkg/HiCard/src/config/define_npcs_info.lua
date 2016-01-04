--key is npcId
local define_npcs_info = {
    ["10000"] = {
        ["name"] = "Captain",
        ["scaleFactor"] = 0.7,
        ["attackRange"] = 1,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 550,
        ["attackInterval"] = 2,
        ["smallSkillInterval"] = 3,
        ["bigSkillInterval"] = 8,
        ["firePosition"] = {x=0,y=40},
        ["hurtPosition"] = {x=5,y=60},
        ["dieDuration"] = 1.583,
        ["normalAttckDuration"] = 0.05,
        ["normalAttackReadyTime"] = 1.0,
        ["smallSkillDuration"] = 0.2,
        ["smallSkillReadyTime"] = 0.86,
        ["bigSkillDuration"] = 0.8,
        ["bigSkillReadyTime"] = 1.0,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/daojian.mp3",
        ["smallMusic"] ="sound/skill/kunkka1.mp3",
        ["bigMusic"] ="sound/skill/kunkka2.mp3",
        ["hpPosition"] = {x = 0,y =173}
    },
    
    ["10001"] = {
        ["name"] = "DuanGuZhe",
        ["scaleFactor"] = 0.7,
        ["attackRange"] = 1,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 5,
        ["bigSkillInterval"] = 8,
        ["firePosition"] = {x=0,y=0},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 0.708,
        ["normalAttckDuration"] = 0.15,
        ["normalAttackReadyTime"] = 0.56,
        ["smallSkillDuration"] = 0.2,
        ["smallSkillReadyTime"] = 0.85,
        ["bigSkillDuration"] = 0.6,
        ["bigSkillReadyTime"] = 2.0,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/chuizi.mp3",
        ["smallMusic"] ="sound/skill/shieduanguzhe1.mp3",
        ["bigMusic"] ="sound/skill/shieduanguzhe2.mp3",
        ["hpPosition"] = {x = 0,y =160}
    },
    
    ["10002"] = {
        ["name"] = "FanKeLiFu",
        ["scaleFactor"] = 0.7,
        ["attackRange"] = 1,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 5,
        ["bigSkillInterval"] = 8,
        ["firePosition"] = {x=0,y=0},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 1,
        ["normalAttckDuration"] = 0.4,
        ["normalAttackReadyTime"] = 0.6,
        ["smallSkillDuration"] = 0.4,
        ["smallSkillReadyTime"] = 1.0,
        ["bigSkillDuration"] = 1.0,
        ["bigSkillReadyTime"] = 0.8,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/daojian.mp3",
        ["smallMusic"] ="sound/skill/fankelifu1.mp3",
        ["bigMusic"] ="sound/skill/fankelifu2.mp3",
        ["hpPosition"] = {x =5,y =155}
        
    },
    
    ["10003"] = {
        ["name"] = "FengXing",
        ["scaleFactor"] = 0.7,
        ["attackRange"] = 5,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 5,
        ["bigSkillInterval"] = 8,
        ["firePosition"] = {x=0,y=30},
        ["hurtPosition"] = {x=0,y=45},
        ["dieDuration"] = 1,
        ["normalAttckDuration"] = 0.6,
        ["normalAttackReadyTime"] = 1.2,
        ["smallSkillDuration"] = 0.8,
        ["smallSkillReadyTime"] = 1.5,
        ["bigSkillDuration"] = 0.6,
        ["bigSkillReadyTime"] = 2.1,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/yuancheng.mp3",
        ["smallMusic"] ="sound/skill/fengxingzhe1.mp3",
        ["bigMusic"] ="sound/skill/fengxingzhe2.mp3",
        ["hpPosition"] = {x = 0,y =153}
    },
    
    ["10004"] = {
        ["name"] = "HuaiTe",
        ["scaleFactor"] = 0.7,
        ["attackRange"] = 3,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 5,
        ["bigSkillInterval"] = 8,
        ["firePosition"] = {x=5,y=50},
        ["hurtPosition"] = {x=0,y=45},
        ["dieDuration"] = 1,
        ["normalAttckDuration"] = 0.5,
        ["normalAttackReadyTime"] = 0.375,
        ["smallSkillDuration"] = 0.3,
        ["smallSkillReadyTime"] = 0.2,
        ["bigSkillDuration"] = 0.4,
        ["bigSkillReadyTime"] = 1.0,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/huaite1.mp3",
        ["bigMusic"] ="sound/skill/huaite2.mp3",
        ["hpPosition"] = {x = 0,y =150}
    },
    
    ["10005"] = {
        ["name"] = "KuLouWang",
        ["scaleFactor"] = 0.7,
        ["attackRange"] = 1,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 5,
        ["bigSkillInterval"] = 8,
        ["firePosition"] = {x=40,y=40},
        ["hurtPosition"] = {x=0,y=65},
        ["dieDuration"] = 1,
        ["normalAttckDuration"] = 0.05,
        ["normalAttackReadyTime"] = 1.25,
        ["smallSkillDuration"] = 0.05,
        ["smallSkillReadyTime"] = 2.85,
        ["bigSkillDuration"] = 1.4,
        ["bigSkillReadyTime"] = 0.6,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/daojian.mp3",
        ["smallMusic"] ="sound/skill/kulouwang1.mp3",
        ["bigMusic"] ="sound/skill/kulouwang2.mp3",
        ["hpPosition"] = {x = 0,y =175}
    },
    
    ["10006"] = {
        ["name"] = "LaZuoKeLao",
        ["scaleFactor"] = 0.7,
        ["attackRange"] = 1,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 5,
        ["bigSkillInterval"] = 8,
        ["firePosition"] = {x=0,y=0},
        ["hurtPosition"] = {x=0,y=70},
        ["dieDuration"] = 1,
        ["normalAttckDuration"] = 0.2,
        ["normalAttackReadyTime"] = 0.8,
        ["smallSkillDuration"] = 0.3,
        ["smallSkillReadyTime"] = 1.2,
        ["bigSkillDuration"] = 0.4,
        ["bigSkillReadyTime"] = 1.8,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/daojian.mp3",
        ["smallMusic"] ="sound/skill/lazuokelao1.mp3",
        ["bigMusic"] ="sound/skill/lazuokelao2.mp3",
        ["hpPosition"] = {x = 20,y =172}
    },
    
    ["10007"] = {
        ["name"] = "Lina",
        ["scaleFactor"] = 0.7,
        ["attackRange"] = 3,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 500,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 5,
        ["bigSkillInterval"] = 8,
        ["firePosition"] = {x=48,y=80},
        ["hurtPosition"] = {x=5,y=70},
        ["dieDuration"] = 1,
        ["normalAttckDuration"] = 0.5,
        ["normalAttackReadyTime"] = 0.8,
        ["smallSkillDuration"] = 0.4,
        ["smallSkillReadyTime"] = 0.8,
        ["bigSkillDuration"] = 1.0,
        ["bigSkillReadyTime"] = 1.0,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/lina1.mp3",
        ["bigMusic"] ="sound/skill/lina2.mp3",
        ["hpPosition"] = {x = 5,y =180}
    },
    
    ["10008"] = {
        ["name"] = "XiaoHei",
        ["scaleFactor"] = 0.7,
        ["attackRange"] = 5,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 5,
        ["bigSkillInterval"] = 2,
        ["firePosition"] = {x=20,y=40},
        ["hurtPosition"] = {x=0,y=45},
        ["dieDuration"] = 1,
        ["normalAttckDuration"] = 0.5,
        ["normalAttackReadyTime"] = 1.0,
        ["smallSkillDuration"] = 0.5,
        ["smallSkillReadyTime"] = 1.5,
        ["bigSkillDuration"] = 0.4,
        ["bigSkillReadyTime"] = 2.0,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/yuancheng.mp3",
        ["smallMusic"] ="sound/skill/dr1.mp3",
        ["bigMusic"] ="sound/skill/dr2.mp3",
        ["hpPosition"] = {x = 0,y =150}
    },
    
    ["10009"] = {
        ["name"] = "ZhuRiZhe",
        ["scaleFactor"] = 0.7,
        ["attackRange"] = 3,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 5,
        ["bigSkillInterval"] = 8,
        ["firePosition"] = {x=60,y=60},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 1,
        ["normalAttckDuration"] = 0.4,
        ["normalAttackReadyTime"] = 0.9,
        ["smallSkillDuration"] = 0.2,
        ["smallSkillReadyTime"] = 1.1,
        ["bigSkillDuration"] = 0.2,
        ["bigSkillReadyTime"] = 1.0,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/zhurizhe1.mp3",
        ["bigMusic"] ="sound/skill/zhurizhe2.mp3",
        ["hpPosition"] = {x = 0,y =200}
    },
    
    ["10010"] = {
        ["name"] = "Jaina",
        ["scaleFactor"] = 1.0,
        ["attackRange"] = 4,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 5,
        ["bigSkillInterval"] = 8,
        ["firePosition"] = {x=60,y=60},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 1,
        ["normalAttckDuration"] = 0.9,
        ["normalAttackReadyTime"] = 0.6,
        ["smallSkillDuration"] = 0.5,
        ["smallSkillReadyTime"] = 1.5,
        ["bigSkillDuration"] = 1,
        ["bigSkillReadyTime"] = 1.25,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/jaina1.mp3",
        ["bigMusic"] ="sound/skill/jaina2.mp3",
        ["hpPosition"] = {x = 0,y =120}
    },
    
    ["10011"] = {
        ["name"] = "Tyrande",
        ["scaleFactor"] = 1.0,
        ["attackRange"] = 4,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 5,
        ["bigSkillInterval"] = 8,
        ["firePosition"] = {x=75,y=60},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 1,
        ["normalAttckDuration"] = 0.6,
        ["normalAttackReadyTime"] = 0.625,
        ["smallSkillDuration"] = 0.2,
        ["smallSkillReadyTime"] = 1.3,
        ["bigSkillDuration"] = 4.0,
        ["bigSkillReadyTime"] = 1.0,
        ["persistentBigSkillFramesNum"] = 19,
        ["persistentBigSkillCount"] = 4,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/tyrande1.mp3",
        ["bigMusic"] ="sound/skill/tyrande2.mp3",
        ["hpPosition"] = {x = 0,y =130}
    },
    
    ["10012"] = {
        ["name"] = "DiXueLingZhu",
        ["scaleFactor"] = 1.0,
        ["attackRange"] = 1,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 4,
        ["bigSkillInterval"] = 5,
        ["firePosition"] = {x=75,y=60},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 1,
        ["normalAttckDuration"] = 0.625,
        ["normalAttackReadyTime"] = 1.0,
        ["smallSkillDuration"] = 0.8,
        ["smallSkillReadyTime"] = 0.6,
        ["bigSkillDuration"] = 0.7,
        ["bigSkillReadyTime"] = 0.65,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/dixuelingzhu1.mp3",
        ["bigMusic"] ="sound/skill/dixuelingzhu1.mp3",
        ["hpPosition"] = {x = 0,y =130}
    },
    
    ["10013"] = {
        ["name"] = "Kelthuzad",
        ["scaleFactor"] = 1.0,
        ["attackRange"] = 4,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 4,
        ["bigSkillInterval"] = 5,
        ["firePosition"] = {x=75,y=60},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 1.5,
        ["normalAttckDuration"] = 0.5,
        ["normalAttackReadyTime"] = 0.66,
        ["smallSkillDuration"] = 0.55,
        ["smallSkillReadyTime"] = 0.85,
        ["bigSkillDuration"] = 0.95,
        ["bigSkillReadyTime"] = 1.15,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/kelthuzad1.mp3",
        ["bigMusic"] ="sound/skill/kelthuzad2.mp3",
        ["hpPosition"] = {x = 0,y =130}
    },
    
    ["10014"] = {
        ["name"] = "YiLiDan",
        ["scaleFactor"] = 1.0,
        ["attackRange"] = 1,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 4,
        ["bigSkillInterval"] = 5,
        ["firePosition"] = {x=75,y=60},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 1.5,
        ["normalAttckDuration"] = 0.3,
        ["normalAttackReadyTime"] = 1.2,
        ["smallSkillDuration"] = 0.4,
        ["smallSkillReadyTime"] = 1.2,
        ["bigSkillDuration"] = 2.0,
        ["bigSkillReadyTime"] = 1.8,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/yilidan1.mp3",
        ["bigMusic"] ="sound/skill/yilidan2.mp3",
        ["hpPosition"] = {x = 0,y =130}
    },
    
    ["10015"] = {
        ["name"] = "ShahelasiZhuMu",
        ["scaleFactor"] = 1.0,
        ["attackRange"] = 1,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 4,
        ["bigSkillInterval"] = 5,
        ["firePosition"] = {x=75,y=60},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 1.5,
        ["normalAttckDuration"] = 0.3,
        ["normalAttackReadyTime"] = 0.9,
        ["smallSkillDuration"] = 0.8,
        ["smallSkillReadyTime"] = 0.5,
        ["bigSkillDuration"] = 1.0,
        ["bigSkillReadyTime"] = 2.2,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/zhumu1.mp3",
        ["bigMusic"] ="sound/skill/zhumu2.mp3",
        ["hpPosition"] = {x = 0,y =130}
    },
    
    ["10016"] = {
        ["name"] = "LaGeNaLuoSi",
        ["scaleFactor"] = 1.0,
        ["attackRange"] = 3,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 4,
        ["bigSkillInterval"] = 5,
        ["firePosition"] = {x=75,y=60},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 1.5,
        ["normalAttckDuration"] = 0.5,
        ["normalAttackReadyTime"] = 1.0,
        ["smallSkillDuration"] = 1.1,
        ["smallSkillReadyTime"] = 0.5,
        ["bigSkillDuration"] = 0.8,
        ["bigSkillReadyTime"] = 1.75,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 10,
        ["persistentSmallSkillCount"] = 3,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/lagenaluosi1.mp3",
        ["bigMusic"] ="sound/skill/lagenaluosi2.mp3",
        ["hpPosition"] = {x = 0,y =130}
    },
    
    ["10017"] = {
        ["name"] = "AoNiKeXiYa",
        ["scaleFactor"] = 1.0,
        ["attackRange"] = 3,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 4,
        ["bigSkillInterval"] = 5,
        ["firePosition"] = {x=75,y=60},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 1.5,
        ["normalAttckDuration"] = 0.5,
        ["normalAttackReadyTime"] = 0.7,
        ["smallSkillDuration"] = 0.5,
        ["smallSkillReadyTime"] = 0.6,
        ["bigSkillDuration"] = 1.8,
        ["bigSkillReadyTime"] = 0.7,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/lagenaluosi1.mp3",
        ["bigMusic"] ="sound/skill/lagenaluosi2.mp3",
        ["hpPosition"] = {x = 0,y =130}
    },
    
    ["10018"] = {
        ["name"] = "KeLuoMaGuSi",
        ["scaleFactor"] = 1.0,
        ["attackRange"] = 3,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 4,
        ["bigSkillInterval"] = 5,
        ["firePosition"] = {x=45,y=70},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 1.5,
        ["normalAttckDuration"] = 0.7,
        ["normalAttackReadyTime"] = 0.55,
        ["smallSkillDuration"] = 0.8,
        ["smallSkillReadyTime"] = 1.0,
        ["bigSkillDuration"] = 1.8,
        ["bigSkillReadyTime"] = 1.25,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/lagenaluosi1.mp3",
        ["bigMusic"] ="sound/skill/lagenaluosi2.mp3",
        ["hpPosition"] = {x = 0,y =130}
    },
    
    ["10019"] = {
        ["name"] = "MaNuoLuoSi",
        ["scaleFactor"] = 1.0,
        ["attackRange"] = 1,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 4,
        ["bigSkillInterval"] = 5,
        ["firePosition"] = {x=45,y=70},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 1.5,
        ["normalAttckDuration"] = 0.2,
        ["normalAttackReadyTime"] = 1.0,
        ["smallSkillDuration"] = 1.0,
        ["smallSkillReadyTime"] = 0.5,
        ["bigSkillDuration"] = 3.0,
        ["bigSkillReadyTime"] = 0.7,
        ["persistentBigSkillFramesNum"] = 30,
        ["persistentBigSkillCount"] = 2,
        ["persistentSmallSkillFramesNum"] = 0,
        ["persistentSmallSkillCount"] = 0,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/lagenaluosi1.mp3",
        ["bigMusic"] ="sound/skill/lagenaluosi2.mp3",
        ["hpPosition"] = {x = 0,y =130}
    },
    
    ["20000"] = {
        ["name"] = "tonglingta",
        ["scaleFactor"] = 1.5,
        ["attackRange"] = 6,
        ["walkSpeed"] = 0,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 999999,
        ["bigSkillInterval"] = 8,
        ["firePosition"] = {x=60,y=60},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 1,
        ["normalAttckDuration"] = 0.4,
        ["normalAttackReadyTime"] = 0.9,
        ["smallSkillDuration"] = 0.2,
        ["smallSkillReadyTime"] = 1.1,
        ["bigSkillDuration"] = 0.2,
        ["bigSkillReadyTime"] = 1.0,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/zhurizhe1.mp3",
        ["bigMusic"] ="sound/skill/zhurizhe2.mp3",
        ["hpPosition"] = {x = 0,y =200}
    },
    
    ["20001"] = {
        ["name"] = "Cannon",
        ["scaleFactor"] = 1,
        ["attackRange"] = 6,
        ["walkSpeed"] = 0,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 999999,
        ["bigSkillInterval"] = 999999,
        ["firePosition"] = {x=60,y=200},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 1,
        ["normalAttckDuration"] = 1.0,
        ["normalAttackReadyTime"] = 0.2,
        ["smallSkillDuration"] = 0.2,
        ["smallSkillReadyTime"] = 1.1,
        ["bigSkillDuration"] = 0.2,
        ["bigSkillReadyTime"] = 1.0,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/zhurizhe1.mp3",
        ["bigMusic"] ="sound/skill/zhurizhe2.mp3",
        ["hpPosition"] = {x = 0,y =200}
    },
    
    ["20002"] = {
        ["name"] = "mutong",
        ["scaleFactor"] = 1,
        ["attackRange"] = 6,
        ["walkSpeed"] = 0,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 999999,
        ["smallSkillInterval"] = 999999,
        ["bigSkillInterval"] = 999999,
        ["firePosition"] = {x=60,y=60},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 1,
        ["normalAttckDuration"] = 0.4,
        ["normalAttackReadyTime"] = 0.9,
        ["smallSkillDuration"] = 0.2,
        ["smallSkillReadyTime"] = 1.1,
        ["bigSkillDuration"] = 0.2,
        ["bigSkillReadyTime"] = 1.0,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["normalMusic"] ="sound/fashu.mp3",
        ["smallMusic"] ="sound/skill/zhurizhe1.mp3",
        ["bigMusic"] ="sound/skill/zhurizhe2.mp3",
        ["hpPosition"] = {x = 0,y =200}
    },
    
    ["20010"] = {
        ["name"] = "DuanGuZhe",
        ["scaleFactor"] = 0.7,
        ["attackRange"] = 1,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 5,
        ["bigSkillInterval"] = 8,
        ["firePosition"] = {x=0,y=0},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 0.708,
        ["normalAttckDuration"] = 0.15,
        ["normalAttackReadyTime"] = 0.56,
        ["smallSkillDuration"] = 0.2,
        ["smallSkillReadyTime"] = 0.85,
        ["bigSkillDuration"] = 0.6,
        ["bigSkillReadyTime"] = 2.0,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["normalMusic"] ="sound/chuizi.mp3",
        ["smallMusic"] ="sound/skill/shieduanguzhe1.mp3",
        ["bigMusic"] ="sound/skill/shieduanguzhe2.mp3",
        ["hpPosition"] = {x = 0,y =160}
    },
    
    ["20011"] = {
        ["name"] = "ShuiYuanSu",
        ["scaleFactor"] = 1.0,
        ["attackRange"] = 1,
        ["walkSpeed"] = 120,
        ["missileSpeed"] = 800,
        ["attackInterval"] = 3,
        ["smallSkillInterval"] = 5,
        ["bigSkillInterval"] = 8,
        ["firePosition"] = {x=40,y=60},
        ["hurtPosition"] = {x=0,y=50},
        ["dieDuration"] = 0.708,
        ["normalAttckDuration"] = 0.15,
        ["normalAttackReadyTime"] = 0.8,
        ["smallSkillDuration"] = 0.2,
        ["smallSkillReadyTime"] = 0.85,
        ["bigSkillDuration"] = 0.6,
        ["bigSkillReadyTime"] = 2.0,
        ["persistentBigSkillFramesNum"] = 0,
        ["persistentBigSkillCount"] = 0,
        ["normalMusic"] ="sound/chuizi.mp3",
        ["smallMusic"] ="sound/skill/shieduanguzhe1.mp3",
        ["bigMusic"] ="sound/skill/shieduanguzhe2.mp3",
        ["hpPosition"] = {x = 0,y =160}
    },
}

return define_npcs_info
