--UI.lua, created by php script. chris.li--
require "Cocos2d"
local soulConfig = require("config.define_soul")


local UI = class("UI", function()
    return cc.Layer:create()
end)

function UI:create(param)
    local layer = UI.new()
    layer:initM(param)
    return layer
end

function UI:ctor()
end

function UI:onEnter()
end

function UI:onExit()
    cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.ScriptFuncId)
    cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.ScriptFuncId2)
    if  self.ScriptFuncId3 ~= nil then
        cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.ScriptFuncId3)
    end
end


function UI:initM(param)
    local function onNodeEvent(event)
        if event == "enter" then self:onEnter() 
        elseif event == "exit" then self:onExit() end
    end
    self:registerScriptHandler(onNodeEvent)

    self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("battle/battleuilayer/BattleUILayer.csb")
    self:addChild(self.rootNode)

    local function touchEventHandler(sender, event)
        self:handleTouchEvent(sender, event)
    end
    
    --连击技序列
    self.globalSkillSeq = 0

    self:initData(param)

    --m_mapPanel: Layout  对应设置战斗地图
    self.m_mapPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_mapPanel")
    self.m_mapPanel:addTouchEventListener(touchEventHandler)

    --m_tipPanel: Layout 按暂停后出的Panel 初始设为隐藏不可按
    self.m_tipPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_tipPanel")
    self.m_tipPanel:addTouchEventListener(touchEventHandler)
    self.m_tipPanel:setVisible(false)
    self.m_tipPanel:setEnabled(false)

    --m_stage: Text 对应本关小关卡数格式X/X
    self.m_stage = ccui.Helper:seekWidgetByName(self.rootNode, "m_stage")
    self.m_stage:setVisible(false)

    --m_speed: ImageView 对应设置运行速度
    self.m_speed = ccui.Helper:seekWidgetByName(self.rootNode, "m_speed")
    self.m_speed:addTouchEventListener(touchEventHandler)
    self.m_speed:setVisible(false)

    --m_stopbtn: Button --暂停按钮
    self.m_stopbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_stopbtn")
    self.m_stopbtn:addTouchEventListener(touchEventHandler)

    --m_modebtn: Button --手动和自动模式的切换
    self.m_modebtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_modebtn")
    self.m_modebtn:addTouchEventListener(touchEventHandler)


    --m_backbtn: Button --回PVE或者布阵界面，自定
    self.m_backbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_backbtn")
    self.m_backbtn:addTouchEventListener(touchEventHandler)

    --m_musicbtn: Button
    self.m_musicbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_musicbtn")
    self.m_musicbtn:addTouchEventListener(touchEventHandler)


    --m_jixubtn: Button
    self.m_jixubtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_jixubtn")
    self.m_jixubtn:addTouchEventListener(touchEventHandler)

    --m_showjuexing: ImageView 当能放合体技时显示，否则隐藏
    self.m_showjuexing = ccui.Helper:seekWidgetByName(self.rootNode, "m_showjuexing")
    self.m_showjuexing:setVisible(false)

    --m_baoxiangnum: Text 宝箱数
    self.m_baoxiangnum = ccui.Helper:seekWidgetByName(self.rootNode, "m_baoxiangnum")

    self.m_baoxiangnum:setString(self.baoxiangnum)

    --m_timenum: Text 时间
    self.m_timenum = ccui.Helper:seekWidgetByName(self.rootNode, "m_timenum")
    local str = string.format("%02d:%02d",self.m_time/60,self.m_time%60)
    self.m_timenum:setString(str)

    --m_xuanzhuan: ImageView 旋转度数0：技能1 90:技能2 180：技能3  270：技能4
    self.m_xuanzhuan = ccui.Helper:seekWidgetByName(self.rootNode, "m_xuanzhuan")
    self.m_xuanzhuan:setRotation(0)


    --m_juexingnum: TextAtlas 默认觉醒0
    self.m_juexingnum = ccui.Helper:seekWidgetByName(self.rootNode, "m_juexingnum")

    self.m_juexingnum:setString(self.juexingnum)


    --m_juexingtiao: ImageView 初始为0
    self.m_juexingtiao = ccui.Helper:seekWidgetByName(self.rootNode, "m_juexingtiao")
    self.m_juexingtiao:setScaleX(0)
    
    --
    self.nuqitiao = ccui.Helper:seekWidgetByName(self.rootNode,"nuqiditiao")
    self.huiluoditiao = ccui.Helper:seekWidgetByName(self.rootNode,"huiluoditiao")
    self.nuqiBg = ccui.Helper:seekWidgetByName(self.rootNode,"Image_15")
    --
   
    --m_huiluotiao: ImageView 初始为0
    self.m_huiluotiao = ccui.Helper:seekWidgetByName(self.rootNode, "m_huiluotiao")

    self.m_huiluotiao:setScaleX(self.huiluonumPercent)

    for i = 1,5 do
        local m_hero = ccui.Helper:seekWidgetByName(self.rootNode, "m_hero_"..i)
        m_hero:setTag(i)
        m_hero:setVisible(false)
        if self.heroSum >=i then
            m_hero:setVisible(true)
            local dataitem=soulConfig[tostring(self.heroIDlist[i])]
            m_hero:loadTexture(dataitem["icon"]..".png")
            self.m_heroJXtimeMax[i] = tonumber(dataitem["wake_max"])/1000
            self.m_heroJXSpeed[i] = tonumber(dataitem["wake_inc_speed"])
            self.m_heroHLSpeed[i] = tonumber(dataitem["wake_dec_speed"])
        end
    end

    self:initUI()
end

function UI:handleTouchEvent(sender, event)
    if sender == nil then
        cclog("[ERR]unknown sender")

    elseif sender == self.m_mapPanel and event == cc.EventCode.ENDED then
        cclog("[UI]--m_mapPanel touched--")

    elseif sender == self.m_stopbtn and event == cc.EventCode.ENDED then
        cclog("[UI]--m_stopbtn touched--")
        self.m_tipPanel:setVisible(true)
        self.m_tipPanel:setEnabled(true)
        cc.Director:getInstance():pause()
        --暂停游戏--

    elseif sender == self.m_modebtn and event == cc.EventCode.ENDED then
        cclog("[UI]--m_modebtn touched--")
        if self.mode == 1 then
            self.mode = 2
            self.m_modebtn:loadTextures("button_shoudong01.png","button_shoudong02.png","button_shoudong02.png")
            
            ---temp solution
            self:stopJX()
        else
            self.mode = 1
            self.m_modebtn:loadTextures("button_zidong01.png","button_zidong02.png","button_zidong02.png")
            
            ---temp solution
            self:startJX()
        end

    elseif sender == self.m_speed and event == cc.EventCode.ENDED then
        cclog("[UI]--m_speed touched--")
        if self.m_speednum == 1 then
            self.m_speednum =2 
            self.m_speed:loadTexture("2bei.png")

        else
            self.m_speednum =1 
            self.m_speed:loadTexture("1bei.png")
        end

    elseif sender == self.m_backbtn and event == cc.EventCode.ENDED then
        cclog("[UI]--m_backbtn touched--")
        cc.Director:getInstance():popScene()
        cc.Director:getInstance():resume()

    elseif sender == self.m_musicbtn and event == cc.EventCode.ENDED then
        cclog("[UI]--m_musicbtn touched--")
        if self.musicstatus == 0 then
            self.musicstatus = 1
            local status = Roler:get("settingStatus")
            status[4] = 1
            Roler:set("settingStatus",status)
            self.m_musicbtn:loadTextures("yinxiao_03.png","yinxiao_04.png","yinxiao_04.png")
            --开启音乐--
            Helper.PlayBackGroundMuisc("sound/background.mp3")
        else
            self.musicstatus = 0
            local status = Roler:get("settingStatus")
            status[4] = 0
            Roler:set("settingStatus",status)
            self.m_musicbtn:loadTextures("yinxiao_01.png","yinxiao_02.png","yinxiao_02.png")
            --关闭音乐--
            cc.SimpleAudioEngine:getInstance():stopMusic()
        end

    elseif sender == self.m_jixubtn and event == cc.EventCode.ENDED then
        cclog("[UI]--m_jixubtn touched--")
        self.m_tipPanel:setVisible(false)
        self.m_tipPanel:setEnabled(false)
        cc.Director:getInstance():resume()
        --继续游戏--
    end
end


function UI:initData(param)
    self.isJX = false
    self.istimeOn = true

    self.heroIDlist =param.memberList   --英雄对应种类ID
    self.zhiliList =param.zhiliList   --英雄对应智力
    self.m_time = param.roundTime -- 战斗时间
    self.heroSum = table.getn(self.heroIDlist)  --总英雄数
    self.m_speednum = 1 --设置初始速度
    self.mode = 1 --1为手动，2为自动

    self.musicstatus = 0 --0为关，1为开

    self.baoxiangnum = 0    --宝箱数
    self.m_timemax = 90     --每小关总时间，单位秒
    self.skillid = 0        --当前合体技能ID
    self.juexingnum = 0     --觉醒级别

    self.huiluonumPercent = 0   --回落条的百分比
    self.huiluoSpeed = 0        --回落条速度
    self.huiluoTime = 0         --回落还是回落的总时间

    self.m_heroJXSpeed = {0,0,0,0,0}    --觉醒5人的觉醒条增加速度
    self.m_heroJXPercent = {0,0,0,0,0}  --觉醒5人的觉醒当前百分比
    self.m_heroJXtime = {0,0,0,0,0}     --觉醒5人的觉醒从0到现在所用了时间
    self.m_heroJXstatus = {0,0,0,0,0}   --觉醒5人的觉醒条状态，0为未满，1为满
    self.m_heroJXtimeMax = {0,0,0,0,0}  --觉醒5人的觉醒条增加到满值所需要的总时间
    self.m_heroHLSpeed = {0,0,0,0,0}    --觉醒5人的对应回落的速度
end

function UI:FireSkill()
    --fire skill event notify others
    if self.isJX == false then return end

    if self.skillid >= 1 then
        local event = cc.EventCustom:new("EVENT_GLOBAL_SKILL")
        event.data = {skillid=self.skillid,hp=self:calculateGlobalSkillAttackValue(self.skillid),dt=3,seq =self.globalSkillSeq}
        --self:getEventDispatcher():dispatchEvent(event)
        directorAgent:handleGlobalSkill(event.data)
        self:addGlobalSq()
        --cc  
    end

    --重置数据--
    for i = 1,5 do
        if self.m_heroJXstatus[i] == 1 then
            self.m_heroJXstatus[i] = 0
            self.m_heroJXtime[i] = 0
            self.m_heroJXPercent[i] = 0
            local m_hero = ccui.Helper:seekWidgetByName(self.rootNode, "m_hero_"..i)
            m_hero:setPosition(self.m_juexingtiao:getPositionX(),m_hero:getPositionY())
        end
    end
    self.skillid = 0
    self.juexingnum = 0
    self.huiluoSpeed = 0

    self:refreshHeroSkillUI()
    self:refreshAwakenUI()
end

function UI:calculateGlobalSkillAttackValue(skillId)
    if  self.zhiliList == nil then return 0 end
    local totalZhiLi = 0
    for i=1,#self.zhiliList do
        if self.m_heroJXstatus[i] == 1 then 
            totalZhiLi = totalZhiLi + self.zhiliList[i]
        end
    end
    print("value:"..totalZhiLi*skillId)
    return totalZhiLi*0.3
end

function UI:addGlobalSq()
    self.globalSkillSeq = self.globalSkillSeq +1
end

function UI:initSkillUI()
    for i=1, 4 do
        local m_jineng = ccui.Helper:seekWidgetByName(self.rootNode, "m_jineng"..i)
        m_jineng:setTag(i)

        local function skilLBtnCallBack(sender, event)
            if  event == cc.EventCode.ENDED then
                cclog("[skilLBtnCallBack]--m_jineng touched--")
                local skillid = sender:getTag()
                --对应释放合体技能，注意加入释放条件
                if self.skillid<1 then
                    return
                end
                self:FireSkill()
            end
        end

        m_jineng:addTouchEventListener(skilLBtnCallBack)
    end
end

function UI:refreshHeroHuiluoUI(id)   
    if self.m_heroJXstatus[id] == 0 then
        self.huiluonumPercent = 100
        self.m_huiluotiao:setScaleX(1)
        self.huiluoSpeed = self.m_heroHLSpeed[id]
    end
end

--刷新进度条
function UI:refreshHeroJinDuUI()
    for i=1, 5 do
        local m_hero = ccui.Helper:seekWidgetByName(self.rootNode, "m_hero_"..i)
        local x =  self.m_juexingtiao:getPositionX()+ self.m_juexingtiao:getContentSize().width*self.m_heroJXPercent[i]/100
        local mt = cc.MoveTo:create(1,cc.p(x,m_hero:getPositionY()))

        m_hero:runAction(mt)

        if self.m_heroJXPercent[i] == 100 then
            self:refreshHeroHuiluoUI(i)
            self.m_heroJXstatus[i] = 1

            self.juexingnum = self.m_heroJXstatus[1]+self.m_heroJXstatus[2]+self.m_heroJXstatus[3]+self.m_heroJXstatus[4]+self.m_heroJXstatus[5]
            self.skillid = self.juexingnum-1
        end
    end
end

--刷新技能
function UI:refreshHeroSkillUI()
    local skillid = self.skillid

    if skillid <= 1 then
        self.m_xuanzhuan:runAction(cc.RotateTo:create(0.3,0))
    elseif skillid == 2 then
        self.m_xuanzhuan:runAction(cc.RotateTo:create(0.3,90))
    elseif skillid == 3 then
        self.m_xuanzhuan:runAction(cc.RotateTo:create(0.3,180))
    elseif skillid == 4 then
        self.m_xuanzhuan:runAction(cc.RotateTo:create(0.3,270))
    end

    for i = 1,4 do
        local m_jineng = ccui.Helper:seekWidgetByName(self.rootNode, "m_jineng"..i)
        m_jineng:setScale(1)
        if i ~= skillid then
            m_jineng:setScale(0.9)
        end
        if skillid <= 1 and i == 1 then
            m_jineng:setScale(1)
        end
    end
end

--刷新觉醒部分的一些UI
function UI:refreshAwakenUI()
    self.m_juexingnum:setString(self.juexingnum)

    if self.juexingnum == 0 then
        self.m_showjuexing:setVisible(false)
    else
        self.m_showjuexing:setVisible(true)
    end

end

function UI:refreshLabelUI()
--更新宝箱数，时间，还有小关卡这些label
end

--2个定时器，控制觉醒条，回落条--
function UI:CombatRefresh()
    local function UpdateSome(dt)
        if self.isJX == false then return end

        for i =1,5 do
            self.m_heroJXtime[i] = self.m_heroJXtime[i]+1
            self.m_heroJXPercent[i] = self.m_heroJXSpeed[i]*self.m_heroJXtime[i]

            if self.m_heroJXPercent[i] >= 100 then
                self.m_heroJXPercent[i] = 100
            end
        end

        self.m_juexingPercent = math.max(self.m_heroJXPercent[1],self.m_heroJXPercent[2],
            self.m_heroJXPercent[3],self.m_heroJXPercent[4],self.m_heroJXPercent[5])

        self.m_juexingtiao:runAction(cc.ScaleTo:create(1,self.m_juexingPercent/100,1))

        self:refreshUI()
    end
    self.ScriptFuncId = cc.Director:getInstance():getScheduler():scheduleScriptFunc(UpdateSome,1.001,false)
    --self:scheduleUpdateWithPriorityLua(UpdateSome, 1)

    local function UpdateSome2(dt)
        if self.isJX == false then return end

        if self.huiluoSpeed == 0 then
            self.huiluoTime = 0
            self.huiluonumPercent = 0
            self.m_huiluotiao:setScaleX(0)
            return
        end
        self.huiluoTime = self.huiluoTime+1
        self.huiluonumPercent = 100-self.huiluoSpeed*self.huiluoTime
        if self.huiluonumPercent <= 0 then
            self.huiluonumPercent = 0
        end

        local function isFireCallBack()
            if self.m_huiluotiao:getScaleX() == 0 then
                self:FireSkill()
            end
        end

        local st = cc.ScaleTo:create(1,self.huiluonumPercent/100,1)
        local seq = cc.Sequence:create(st,cc.CallFunc:create(isFireCallBack))
        self.m_huiluotiao:runAction(seq)
    end
    self.ScriptFuncId2 = cc.Director:getInstance():getScheduler():scheduleScriptFunc(UpdateSome2,1.001,false)
end

function UI:refreshTimeLabel()
	 local function updateTimeLabel()
	    if self.istimeOn == false then return end
        if  self.m_time > 0 then
            self.m_time = self.m_time -1
            local time = self.m_time
            local str = string.format("%02d:%02d",time/60,time%60)
            self.m_timenum:setString(str)
        else
            if  self.ScriptFuncId3 ~= nil then
                cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.ScriptFuncId3)
            end
        end
    end
    self.ScriptFuncId3 = cc.Director:getInstance():getScheduler():scheduleScriptFunc(updateTimeLabel,1.0,false)
end

--init your ui here
function UI:initUI()
    self:startJX()
    self:initSkillUI()
    self:refreshUI()
    self:CombatRefresh()
    self:refreshTimeLabel()
end

--refresh all ui here
function UI:refreshUI()
    self:refreshHeroJinDuUI()
    self:refreshHeroSkillUI()
    self:refreshLabelUI()
    self:refreshAwakenUI()
end

function UI:startJX()
    self.isJX = true
end

function UI:stopJX()
    self.isJX = false
end

function UI:skillBegin()
    self.nuqitiao:setVisible(false)
    self.huiluoditiao:setVisible(false)
    self.nuqiBg:setVisible(false)
    self.m_showjuexing:setVisible(false)
    self.m_xuanzhuan:setVisible(false)
    self.istimeOn = false
end

function UI:skillEnd()
    self.nuqitiao:setVisible(true)
    self.huiluoditiao:setVisible(true)
    self.nuqiBg:setVisible(true)
    self.m_showjuexing:setVisible(true)
    self.m_xuanzhuan:setVisible(true)
    self.istimeOn = true
end

function UI:updateUI(data)
    self.m_stage:setString(data.round.."/".."3")
end

function UI:getTreasurePos()
    local pos = self.m_baoxiangnum:getWorldPosition()
    return pos
end

function UI:getTreasureNum()
    local num = tonumber(self.m_baoxiangnum:getString())
    return num
end

function UI:addTreasureNum()
    local num = tonumber(self.m_baoxiangnum:getString())
    self.m_baoxiangnum:setString(num+1)
end

return UI
