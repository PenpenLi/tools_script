--guardCenter.itemtag = 0
local guardEnable = true

local guard_pickcard_cmdnames = {
   -- "pickcard_dialog_jitan0",
   -- "pickcard_dialog_jitan", 
    "pickcard_click_jitanbtn",
    "pickcard_click_pickcardbtn",
    --"pickcard_dialog_cardfunction",
    "pickcard_click_armaturebtn",
    "pickcard_click_surebtn",
    "pickcard_click_closebtn"
}

local guard_pickcard2_cmdnames = {
    -- "pickcard_dialog_jitan0",
    -- "pickcard_dialog_jitan", 
    "pickcard2_click_jitanbtn",
    "pickcard2_click_pickcardbtn",
    --"pickcard_dialog_cardfunction",
    --"pickcard2_click_armaturebtn",
    "pickcard2_click_surebtn",
    "pickcard2_click_closebtn"
}

local guard_pve_cmdnames = {
    --"pve_dialog_battle",
    "pve_click_battlebtn",
    "pve_click_level1btn",
    "pve_click_goonbtn",
    "pve_click_hero1btn",
    "pve_click_hero2btn",
    "pve_click_fightbtn",
    --"pve_dialog_skill",
    "pve_click_fireskillbtn",
   -- "pve_dialog_jxskill",
   -- "pve_dialog_jxskill2",
   -- "pve_dialog_jxskill3",
    "pve_click_fireJXbtn",
    "pve_click_surebtn",
    --"pve_dialog_log1",
   -- "pve_dialog_log2",
    "pve_click_closepvebtn"
}

local guard_pve2_cmdnames = {
   -- "pve2_dialog_battle",
    "pve2_click_battlebtn",
    "pve2_click_level2btn",
    "pve2_click_goonbtn",
    "pve2_click_hero3btn",
    "pve2_click_fightbtn",
    "pve2_click_fireskillbtn",
    "pve2_click_fireJXbtn",
    "pve2_click_surebtn",
    "pve2_click_closepvebtn"
}

local guard_pve3_cmdnames = {
    --[["pve3_click_battlebtn",
    "pve3_click_level3btn",
    "pve3_click_goonbtn",
    "pve3_click_fightbtn",
    "pve3_click_surebtn",--]]
    "pve3_click_closepvebtn"
}

local guard_pve4_cmdnames = {
    --[["pve4_click_battlebtn",
    "pve4_click_level4btn",
    "pve4_click_goonbtn",
    "pve4_click_fightbtn",
    "pve4_click_surebtn",--]]
    "pve4_click_closepvebtn"
}

local guard_pve5_cmdnames = {
    --[["pve5_click_battlebtn",
    "pve5_click_level5btn",
    "pve5_click_goonbtn",
    "pve5_click_fightbtn",
    "pve5_click_surebtn",--]]
    "pve5_click_closepvebtn"
}

local guard_pve6_cmdnames = {
    --[["pve6_click_battlebtn",
    "pve6_click_level6btn",
    "pve6_click_goonbtn",
    "pve6_click_fightbtn",
    "pve6_click_surebtn",--]]
    "pve6_click_closepvebtn"
}

local guard_qianghua_cmdnames = {
    "qianghua_click_closepvebtn",
    "qianghua_click_tiejiangpu",
    "qianghua_click_dazhaobtn",
    "qianghua_click_qianghuabtn",
    "qianghua_click_returnbtn",
    "qianghua_click_closebtn"
}

local guard_jinjie_cmdnames = {
    "jinjie_click_closepvebtn",
   -- "jinjie_dialog_jinjie",
    "jinjie_click_tiejiangpu",
    "jinjie_click_dazaobtn",
    "jinjie_click_jinjiebtn",
    "jinjie_click_jinjiebtn2"
    --[["jinjie_click_returnbtn",
    "jinjie_click_closebtn"--]]
}

local guard_duobao_cmdnames = {
    "duobao_click_closepvebtn",
   -- "duobao_dialog_huodong",
    "duobao_click_huodongbtn",
    "duobao_click_duobaobtn"
}

local guard_jingyanyaoshui_cmdnames = {
  --  "jingyanyaoshui_dialog_hero",
    "jingyanyaoshui_click_herobtn",
    "jingyanyaoshui_click_heroicon",
    "jingyanyaoshui_click_heroshengjibtn",
    "jingyanyaoshui_click_shengjibtn",
    "jingyanyaoshui_click_back1btn",
    "jingyanyaoshui_click_backbtn",
    "jingyanyaoshui_click_closebtn"
}

local guard_jinengyaoshui_cmdnames = {
    "jinengyaoshui_click_closepvebtn",
   -- "jinengyaoshui_dialog_hero",
    "jinengyaoshui_click_herobtn",
    "jinengyaoshui_click_heroicon",
    "jinengyaoshui_click_jinengbtn",
    "jinengyaoshui_click_shengjibtn",
    "jinengyaoshui_click_back2btn",
    "jinengyaoshui_click_backbtn",
    "jinengyaoshui_click_closebtn"
}

local guard_herojinjie_cmdnames = {
   -- "herojinjie_dialog_hero",
    "herojinjie_click_herobtn",
    "herojinjie_click_heroicon",
    "herojinjie_click_jinjiebtn"
}

local guard_herowear_cmdnames = {
    "herowear_click_closepvebtn",
   -- "herowear_dialog_hero",
    "herowear_click_herobtn",
    "herowear_click_heroicon",
    "herowear_click_weaponbtn",
    "herowear_click_wearbtn",
    "herowear_click_backbtn",
    "herowear_click_closebtn"
}

local guard_sport_cmdnames = {
    "sport_click_closepvebtn",
   -- "sport_dialog_sport",
    "sport_click_sportbtn",
    "sport_click_challenge"
   --[[ "sport_click_fomationicon1",
    "sport_click_fomationicon2",
    "sport_click_fomationfight"--]]
}

local guard_task_cmdnames = {
   -- "task_dialog_task",
    "task_click_taskbtn",
    "task_click_loadbtn",
    "task_click_award",
    "task_click_closetask"
}

local guard_task2_cmdnames = {
   -- "task2_dialog_task",
    "task2_click_taskbtn",
    "task2_click_levelupBtn",
    "task2_click_award",
    "task2_click_closetask"
}

local guard_task22_cmdnames = {

   --  "task22_dialog_hero",
     "task22_click_herobtn",
     "task22_click_heroicon",
     "task22_click_fixbtn",
     "task22_click_fixbtn2",
     "task22_click_fixbtn3",
      "task22_click_backbtn",
      "task22_click_closebtn"

}

local guard_herofix_cmdnames = {
   -- "herofix_dialog_hero",
   -- "herofix_dialog_hero2",
    "herofix_click_herobtn",
    "herofix_click_heroicon",
    "herofix_click_fixbtn",
   -- "herofix_dialog_hero3",
  --  "herofix_dialog_hero4",
    "herofix_click_fixbtn2",
    --"herofix_click_fixbtn3",
    "herofix_click_backbtn",
    "herofix_click_closebtn"
}

local guard_moni_cmdnames = {
   -- "moni_dialog_bt2",
   -- "moni_click_skillBtn",
   -- "moni_click_skillBtn2",
    --"moni_click_skillBtn3",
    --"moni_click_skillBtn4",
    --"moni_click_skillBtn5",
   -- "moni_click_skillBtn6",
    --"moni_dialog_bt3",
   -- "moni_dialog_bt1"
}

local guard_uplevel_cmdnames = {
}

local GuardCenterClass = class("GuardCenterClass",function()
    return {}
end)

function GuardCenterClass:create()
    local guardCenter = GuardCenterClass.new()
    guardCenter:initM()
    return guardCenter
end

function GuardCenterClass:initM()
    --require("data.Roler")
    self.guardCmds = require("config.define_guard_cmds")
    print("hereisthpath"..cc.FileUtils:getInstance():getWritablePath())
    self.rolerLv = Roler:get("level")
    
    local defaultServerNo = cc.UserDefault:getInstance():getIntegerForKey("DefaultServerNo")
    self.guardStoreKey = defaultServerNo.."_"..Roler:get("roleId").."_integerTag"
    self.guildKey = defaultServerNo.."_"..Roler:get("roleId").."_integerTag".."_new"
    self.guildLevel = defaultServerNo.."_"..Roler:get("roleId").."_integerTag".."_LV"
    self.pveStage = defaultServerNo.."_"..Roler:get("roleId").."_integerTag".."_pveStage"
    self.saveLv = defaultServerNo.."_"..Roler:get("roleId").."_integerTag".."_saveLevel"
    
    print("[GuardCenterClass]guardStoreKey "..self.guardStoreKey)
    local itemtag = cc.UserDefault:getInstance():getIntegerForKey(self.guardStoreKey)
    if itemtag == nil or itemtag == "" then
        cc.UserDefault:getInstance():setIntegerForKey(self.guardStoreKey,0)
        itemtag = cc.UserDefault:getInstance():getIntegerForKey(self.guardStoreKey)
    end
    assert("itemtag ~= nil", "")
    self.guardStep = itemtag

    local Gtag = cc.UserDefault:getInstance():getIntegerForKey(self.guildKey)
    if Gtag == nil or Gtag == "" then
        cc.UserDefault:getInstance():setIntegerForKey(self.guildKey,0)
        Gtag = cc.UserDefault:getInstance():getIntegerForKey(self.guildKey)
    end
    assert("Gtag ~= nil", "")
    self.guildtag = Gtag
    
    if self.guardStep < self.guildtag then
        self.guardStep = self.guildtag
    end
    
    --触发条件为PVE  本地保存
    local GuildLevelNum = cc.UserDefault:getInstance():getIntegerForKey(self.guildLevel)
    if GuildLevelNum == nil or GuildLevelNum == "" then
        cc.UserDefault:getInstance():setIntegerForKey(self.guildLevel,0)
        GuildLevelNum = cc.UserDefault:getInstance():getIntegerForKey(self.guildLevel)
    end
    assert("GuildLevelNum ~= nil", "")
    self.roleNum = GuildLevelNum
    
    local GuildPveNum = cc.UserDefault:getInstance():getIntegerForKey(self.pveStage)
    if GuildPveNum == nil or GuildPveNum == "" then
        cc.UserDefault:getInstance():setIntegerForKey(self.pveStage,0)
        GuildPveNum = cc.UserDefault:getInstance():getIntegerForKey(self.pveStage)
    end
    assert("GuildPveNum ~= nil", "")
    self.stageNum = GuildPveNum
    
    --触发条件为等级 本地保存
    local GuildLvNum = cc.UserDefault:getInstance():getIntegerForKey(self.saveLv)
    if GuildLvNum == nil or GuildLvNum == "" then
        cc.UserDefault:getInstance():setIntegerForKey(self.saveLv,0)
        GuildLvNum = cc.UserDefault:getInstance():getIntegerForKey(self.saveLv)
    end
    assert("GuildPveNum ~= nil", "")
    self.LvNum = GuildLvNum
end

function GuardCenterClass:setCmdIdx(idx)
    self.currIdx = idx
end

function GuardCenterClass:getCmd(guardType)
    local cmdnames = {}
    if guardType == "pickcard" then --1
        cmdnames = guard_pickcard_cmdnames
    elseif guardType == "pickcard2" then --1
        cmdnames = guard_pickcard2_cmdnames
    elseif guardType == "pve" then --1
        cmdnames = guard_pve_cmdnames
    elseif guardType == "pve2" then --1
        cmdnames = guard_pve2_cmdnames
    elseif guardType == "pve3" then --1
        cmdnames = guard_pve3_cmdnames
    elseif guardType == "pve4" then --1
        cmdnames = guard_pve4_cmdnames
    elseif guardType == "pve5" then --1
        cmdnames = guard_pve5_cmdnames
    elseif guardType == "pve6" then --1
        cmdnames = guard_pve6_cmdnames
    elseif guardType == "qianghua" then --1
        cmdnames = guard_qianghua_cmdnames
    elseif guardType == "jinjie" then --1
        cmdnames = guard_jinjie_cmdnames
    elseif guardType == "duobao" then --1
        cmdnames = guard_duobao_cmdnames 
    elseif guardType == "jingyanyaoshui" then --1
        cmdnames = guard_jingyanyaoshui_cmdnames 
    elseif guardType == "sport" then --1
        cmdnames = guard_sport_cmdnames 
    elseif guardType == "jinengyaoshui" then --1
        cmdnames = guard_jinengyaoshui_cmdnames
    elseif guardType == "herojinjie" then --1
        cmdnames = guard_herojinjie_cmdnames 
    elseif guardType == "herowear" then --1
        cmdnames = guard_herowear_cmdnames
    elseif guardType == "task" then --1
        cmdnames = guard_task_cmdnames 
    elseif guardType == "task2" then --1
        cmdnames = guard_task2_cmdnames
    elseif guardType == "task22" then --1
        cmdnames = guard_task22_cmdnames 
    elseif guardType == "herofix" then --1
        cmdnames = guard_herofix_cmdnames 
    elseif guardType == "moni" then 
        cmdnames = guard_moni_cmdnames     
    end
    
    printJSON(cmdnames)
    if cmdnames == {} or cmdnames == nil then return nil end
    local cmdName = cmdnames[1]
    --printJSON(self.guardCmds)
    for _, v in pairs(self.guardCmds) do
        if v["name"] == cmdName then
            --printJSON(v)
            table.remove(cmdnames,1)
            return v
        end
    end
    --print("[GuardCenterClass]no cmd for "..cmdName)
    return nil
end

function GuardCenterClass:canEnterGuardMode(guardType)
    if guardEnable == false then return false end
    self.rolerLv = Roler:get("level")
    if guardType == "moni" and self.guardStep == 0 and self.rolerLv == 1 then
        return true
    elseif guardType == "pickcard" and self.guardStep == 1 and self.rolerLv == 1 then
        return true
    elseif guardType == "pve"  and self.guardStep == 3 and self.rolerLv <=2 then
        return true
    elseif guardType == "qianghua" and (self.stageNum == 3 and self.roleNum == 2) then--111 14
        return true
    elseif guardType == "jinjie" and (self.stageNum == 4 and self.roleNum == 3) then--111 16
        return true
    elseif guardType == "duobao" and (self.rolerLv == 12 and self.LvNum == 0) then
        return true
    elseif guardType == "jingyanyaoshui" and self.guardStep == 7 then
        return true
    elseif guardType == "pve3" and self.guardStep == 8 then --222
        return false
    elseif guardType == "pve4" and self.guardStep == 11 then--222
        return false
    elseif guardType == "pve5" and self.guardStep == 13 then--222
        return false
    elseif guardType == "pve6" and self.guardStep == 15 then--222
        return false
    elseif guardType == "sport" and (self.rolerLv == 18 and self.LvNum == 1) then
        return true
   -- elseif guardType == "sport" then
        --return true
    elseif guardType == "jinengyaoshui" and (self.stageNum == 2 and self.roleNum == 1) then--111 12
        return true
    elseif guardType == "herojinjie" then
        return false
    elseif guardType == "herowear" and (self.stageNum == 1 and self.roleNum == 0) then--111 10
        return true
    elseif guardType == "task" and self.guardStep == 2 and self.rolerLv == 1 then
        return true
    elseif guardType == "task2" and self.guardStep == 4 and self.rolerLv == 2 then
        return true
    elseif guardType == "task22" and self.guardStep == 5 and self.rolerLv == 2 then
        return true
    --elseif guardType == "herofix" and self.guardStep == 3 and self.rolerLv == 1 then
        --return true
    elseif guardType == "pve2" and self.guardStep == 6 then
        return true
    end
    return false
end

function GuardCenterClass:itemTag()
end

function GuardCenterClass:jumpNextGuardStage()
    self.guardStep = self.guardStep + 1
    self:save2Flash()
    print("[GuardCenterClass] jump guardStep "..self.guardStep)
end

function GuardCenterClass:isJump2Stage()
    self.guildtag = self.guildtag + 1
    self:save2Flash()
    print("[GuardCenterClass] jump guardStep "..self.guildtag)
end

function GuardCenterClass:guildlvStage()
    self.roleNum = self.roleNum + 1
    self:save2Flash()
end

function GuardCenterClass:guildPveStage()
    self.stageNum = self.stageNum + 1
    self:save2Flash()
end

function GuardCenterClass:guildsavelv()
    self.LvNum = self.LvNum + 1
    self:save2Flash()
end

function GuardCenterClass:save2Flash()
    cc.UserDefault:getInstance():setIntegerForKey(self.guardStoreKey, self.guardStep)
    cc.UserDefault:getInstance():setIntegerForKey(self.guildKey, self.guildtag)
    cc.UserDefault:getInstance():setIntegerForKey(self.guildLevel,self.roleNum)
    cc.UserDefault:getInstance():setIntegerForKey(self.pveStage,self.stageNum)
    cc.UserDefault:getInstance():setIntegerForKey(self.saveLv,self.LvNum)
end

function GuardCenterClass:isGuilding()--是否正在引导
    return guardCenter:canEnterGuardMode("pve2") 
        or guardCenter:canEnterGuardMode("pve")
        or guardCenter:canEnterGuardMode("moni")
        or guardCenter:canEnterGuardMode("pickcard")
        or guardCenter:canEnterGuardMode("jingyanyaoshui")
        or guardCenter:canEnterGuardMode("jinengyaoshui")
        or guardCenter:canEnterGuardMode("task")
        or guardCenter:canEnterGuardMode("task2")
        or guardCenter:canEnterGuardMode("task22")
        or guardCenter:canEnterGuardMode("herofix")
        or guardCenter:canEnterGuardMode("duobao")
        or guardCenter:canEnterGuardMode("sport")
        --or guardCenter:canEnterGuardMode("task22")
end

function GuardCenterClass:isSwallow()--额外遮罩层
    local colorLayer = cc.LayerColor:create(cc.c4b(0, 0, 0,1), WIN_SIZE.width, WIN_SIZE.height)
    local function onTouchBegan(touch, event) return true end
    local function onTouchMoved(touch, event) end
    local function onTouchEnded(touch, event) print("Mask Swallow Touches") end
    local listener = cc.EventListenerTouchOneByOne:create()
    listener:setSwallowTouches(true)
    listener:registerScriptHandler(onTouchBegan,cc.Handler.EVENT_TOUCH_BEGAN )
    listener:registerScriptHandler(onTouchMoved,cc.Handler.EVENT_TOUCH_MOVED )
    listener:registerScriptHandler(onTouchEnded,cc.Handler.EVENT_TOUCH_ENDED )
    local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
    eventDispatcher:addEventListenerWithSceneGraphPriority(listener, colorLayer)
    
    return colorLayer
end

guardCenter = GuardCenterClass:create()

--return GuardCenterClass
