--BattleUILayer.lua, created by php script. chris.li--
require "Cocos2d"
local colorLayer =nil
local soulConfig = require("config.define_soul")

local heroIDlist = {}

local BattleUILayer = class("BattleUILayer", function()
	return cc.Layer:create()
end)

function BattleUILayer:create(param)
	local layer = BattleUILayer.new()
    layer:initM(param)
	return layer
end

function BattleUILayer:ctor()
end

function BattleUILayer:onEnter()
   
end

function BattleUILayer:onExit()
    cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.ScriptFuncId)
    cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.ScriptFuncId2)
    --colorLayer = nil
end

function BattleUILayer:setHeroIdList(list)
    table.merge(heroIDlist,list)
end


function BattleUILayer:initM(param)
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("BattleUILayer.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end
	
    --遮罩
    colorLayer = cc.LayerColor:create(cc.c4b(0,0,0,100))
    self:addChild(colorLayer,-1)
    colorLayer:setVisible(false)
	
    local visibleSize = cc.Director:getInstance():getVisibleSize()
    local armatureFile = "battle/ui/leiyun/leiyun.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
    local armature = ccs.Armature:create("leiyun")
    armature:setPosition(visibleSize.width/2,visibleSize.height/2)
    armature:setScaleX(visibleSize.width*1.1/960)
    armature:setScaleY(visibleSize.height*1.1/640)
    --armature:getAnimation():play("Animation1")
    armature:setTag(9999)
    colorLayer:addChild(armature,1)
	
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
	
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

	--m_speed: ImageView 对应设置运行速度
	self.m_speed = ccui.Helper:seekWidgetByName(self.rootNode, "m_speed")
    self.m_speed:addTouchEventListener(touchEventHandler)
    

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
    
    self.m_timenum:setString(self.m_time)

	--m_xuanzhuan: ImageView 旋转度数0：技能1 90:技能2 180：技能3  270：技能4
	self.m_xuanzhuan = ccui.Helper:seekWidgetByName(self.rootNode, "m_xuanzhuan")
	self.m_xuanzhuan:setRotation(0)
	

	--m_juexingnum: TextAtlas 默认觉醒0
	self.m_juexingnum = ccui.Helper:seekWidgetByName(self.rootNode, "m_juexingnum")
    
    self.m_juexingnum:setString(self.juexingnum)
	

	--m_juexingtiao: ImageView 初始为0
	self.m_juexingtiao = ccui.Helper:seekWidgetByName(self.rootNode, "m_juexingtiao")
    self.m_juexingtiao:setScaleX(0)

	
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

function BattleUILayer:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_mapPanel and event == cc.EventCode.ENDED then
        cclog("[BattleUILayer]--m_mapPanel touched--")

    elseif sender == self.m_stopbtn and event == cc.EventCode.ENDED then
        cclog("[BattleUILayer]--m_stopbtn touched--")
        self.m_tipPanel:setVisible(true)
        self.m_tipPanel:setEnabled(true)
        --暂停游戏--

    elseif sender == self.m_modebtn and event == cc.EventCode.ENDED then
		cclog("[BattleUILayer]--m_modebtn touched--")
        if self.mode == 1 then
            self.mode = 2
            self.m_modebtn:loadTextures("button_shoudong01.png","button_shoudong02.png","button_shoudong02.png")
		else
            self.mode = 1
            self.m_modebtn:loadTextures("button_zidong01.png","button_zidong02.png","button_zidong02.png")
		end

    elseif sender == self.m_speed and event == cc.EventCode.ENDED then
        cclog("[BattleUILayer]--m_speed touched--")
        if self.m_speednum == 1 then
        	self.m_speednum =2 
        	self.m_speed:loadTexture("2bei.png")
        
        else
            self.m_speednum =1 
            self.m_speed:loadTexture("1bei.png")
        end

    elseif sender == self.m_backbtn and event == cc.EventCode.ENDED then
        cclog("[BattleUILayer]--m_backbtn touched--")

    elseif sender == self.m_musicbtn and event == cc.EventCode.ENDED then
        cclog("[BattleUILayer]--m_musicbtn touched--")
        if self.musicstatus == 0 then
        	self.musicstatus = 1
            self.m_musicbtn:loadTextures("yinxiao_03.png","yinxiao_04.png","yinxiao_04.png")
            --开启音乐--

        else
            self.musicstatus = 0
            self.m_musicbtn:loadTextures("yinxiao_01.png","yinxiao_02.png","yinxiao_02.png")
            --关闭音乐--
        end

    elseif sender == self.m_jixubtn and event == cc.EventCode.ENDED then
        cclog("[BattleUILayer]--m_jixubtn touched--")
        self.m_tipPanel:setVisible(false)
        self.m_tipPanel:setEnabled(false)
        --继续游戏--
	end
end


function BattleUILayer:initData(param)
    self.isJX = false
    
    self.heroIDlist =heroIDlist    --英雄对应种类ID
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

function BattleUILayer:FireSkill()
    --fire skill event notify others
    if self.isJX == false then return end
    
    if self.skillid >= 1 then
        local event = cc.EventCustom:new("EVENT_GLOBAL_SKILL")
        event.data = {skillid=self.skillid}
        self:getEventDispatcher():dispatchEvent(event)
        
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

function BattleUILayer:initSkillUI()
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

function BattleUILayer:refreshHeroHuiluoUI(id)   
    if self.m_heroJXstatus[id] == 0 then
		self.huiluonumPercent = 100
		self.m_huiluotiao:setScaleX(1)
		self.huiluoSpeed = self.m_heroHLSpeed[id]
	end
end

--刷新进度条
function BattleUILayer:refreshHeroJinDuUI()
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
function BattleUILayer:refreshHeroSkillUI()
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
function BattleUILayer:refreshAwakenUI()
    self.m_juexingnum:setString(self.juexingnum)
    
    if self.juexingnum == 0 then
    	self.m_showjuexing:setVisible(false)
    else
        self.m_showjuexing:setVisible(true)
    end

end

function BattleUILayer:refreshLabelUI()
    --更新宝箱数，时间，还有小关卡这些label
end

--2个定时器，控制觉醒条，回落条--
function BattleUILayer:CombatRefresh()
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
    self.ScriptFuncId = cc.Director:sharedDirector():getScheduler():scheduleScriptFunc(UpdateSome,1.001,false)
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

--init your ui here
function BattleUILayer:initUI()
    self:initSkillUI()
    self:refreshUI()
    self:CombatRefresh()
end

--refresh all ui here
function BattleUILayer:refreshUI()
    self:refreshHeroJinDuUI()
    self:refreshHeroSkillUI()
    self:refreshLabelUI()
    self:refreshAwakenUI()
end

function BattleUILayer:getCoverLayer()
    return colorLayer
end

function BattleUILayer:startJX()
    self.isJX = true
end

function BattleUILayer:stopJX()
    self.isJX = false
end

function BattleUILayer:updateUI(data)
    self.m_stage:setString(data.round.."/".."3")
end

return BattleUILayer
