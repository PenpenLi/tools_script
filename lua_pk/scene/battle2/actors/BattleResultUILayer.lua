--BattleResultUILayer.lua, created by php script. chris.li--
require "Cocos2d"

local BattleResultUILayer = class("BattleResultUILayer", function()
	return cc.Layer:create()
end)

function BattleResultUILayer:create(data)
	local layer = BattleResultUILayer.new()
    layer:initM(data)
	return layer
end

function BattleResultUILayer:ctor()
end

function BattleResultUILayer:onEnter()
    --guard tag--
end

function BattleResultUILayer:onExit()
    if self.ScriptFuncId ~= nil then
        cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.ScriptFuncId)
    end
end


function BattleResultUILayer:processGuard()
    local function addGuardDialogLayer(cmd)
        cmd.type = "guard_dialog"
        cmd.cb = function() self:processGuard() end
        Helper.setTimeout(self, 1, function()
            self.guardLayer = require("common.Guard"):create(cmd)
            cc.Director:getInstance():getRunningScene():addChild(self.guardLayer)
        end)
    end
    local function addGuardClickLayer(cmd)
        assert(cmd.clickTarget~=nil,"")
        cmd.type = "guard_click"
        Helper.setTimeout(self, 1, function()
            self.guardLayer = require("common.Guard"):create(cmd)
            cc.Director:getInstance():getRunningScene():addChild(self.guardLayer)
        end)
    end
    local function removeGuardLayer()
        if self.guardLayer then
            cc.Director:getInstance():getRunningScene():removeChild(self.guardLayer)
            self.guardLayer = nil
        end
    end
    removeGuardLayer()

    if guardCenter:canEnterGuardMode("pve") == true then
        local cmd = guardCenter:getCmd("pve")
        if cmd == nil then return end
        if cmd.name == "pve_click_surebtn" then
            cmd.clickTarget = self.m_confirmBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("pve2") == true then
        local cmd = guardCenter:getCmd("pve2")
        if cmd == nil then return end
        if cmd.name == "pve2_click_surebtn" then
            cmd.clickTarget = self.m_confirmBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("pve3") == true then
        local cmd = guardCenter:getCmd("pve3")
        if cmd == nil then return end
        if cmd.name == "pve3_click_surebtn" then
            cmd.clickTarget = self.m_confirmBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("pve4") == true then
        local cmd = guardCenter:getCmd("pve4")
        if cmd == nil then return end
        if cmd.name == "pve4_click_surebtn" then
            cmd.clickTarget = self.m_confirmBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("pve5") == true then
        local cmd = guardCenter:getCmd("pve5")
        if cmd == nil then return end
        if cmd.name == "pve5_click_surebtn" then
            cmd.clickTarget = self.m_confirmBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("pve6") == true then
        local cmd = guardCenter:getCmd("pve6")
        if cmd == nil then return end
        if cmd.name == "pve6_click_surebtn" then
            cmd.clickTarget = self.m_confirmBtn
            addGuardClickLayer(cmd)
        end
    end
end

function BattleResultUILayer:initM(data)
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("BattleResultUILayer.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end
    
    --ResultRootLayout: Layout
    self.ResultRootLayout = ccui.Helper:seekWidgetByName(self.rootNode, "ResultRootLayout")
    self.ResultRootLayout:addTouchEventListener(touchEventHandler)

    
   --bgImage: ImageView
    self.BgImage = ccui.Helper:seekWidgetByName(self.rootNode, "BgImage")
    
	--m_BattleFailImage: ImageView
	self.m_BattleFailImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_BattleFailImage")
    self.m_BattleFailImage:addTouchEventListener(touchEventHandler)
    
	--m_BattleWinImage: ImageView
	self.m_BattleWinImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_BattleWinImage")
    self.m_BattleWinImage:addTouchEventListener(touchEventHandler)
    
    --m_BattlleFinishImage: ImageView
    self.m_BattlleFinishImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_BattlleFinishImage")
    self.m_BattlleFinishImage:addTouchEventListener(touchEventHandler)
    
    --m_sportHisImage: ImageView
    self.m_sportHisImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_sportHisImage")
    
	--m_roleBgImage: ImageView
	self.m_roleBgImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_roleBgImage")

	--m_failLayout: Layout
	self.m_failLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_failLayout")
	self.m_failLayout:addTouchEventListener(touchEventHandler)

	--m_winLayout: Layout
	self.m_winLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_winLayout")
	self.m_winLayout:addTouchEventListener(touchEventHandler)

	--m_winStarImage1: ImageView
	self.m_winStarImage1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_winStarImage1")

	--m_winStarImage2: ImageView
	self.m_winStarImage2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_winStarImage2")

	--m_winStarImage3: ImageView
	self.m_winStarImage3 = ccui.Helper:seekWidgetByName(self.rootNode, "m_winStarImage3")

	--m_picImage1: ImageView
	self.m_picImage1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_picImage1")

	--m_picImage2: ImageView
	self.m_picImage2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_picImage2")

	--m_picImage3: ImageView
	self.m_picImage3 = ccui.Helper:seekWidgetByName(self.rootNode, "m_picImage3")

	--m_picImage4: ImageView
	self.m_picImage4 = ccui.Helper:seekWidgetByName(self.rootNode, "m_picImage4")

	--m_picImage5: ImageView
	self.m_picImage5 = ccui.Helper:seekWidgetByName(self.rootNode, "m_picImage5")

	--m_strengthenBtn: Button
	self.m_strengthenBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_strengthenBtn")
	self.m_strengthenBtn:addTouchEventListener(touchEventHandler)

	--m_technologyBtn: Button
	self.m_technologyBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_technologyBtn")
	self.m_technologyBtn:addTouchEventListener(touchEventHandler)
	self.m_technologyBtn:setVisible(false)

	--m_advancedBtn: Button
	self.m_advancedBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_advancedBtn")
	self.m_advancedBtn:addTouchEventListener(touchEventHandler)

	--m_cancelBtn: Button
	self.m_cancelBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_cancelBtn")
	self.m_cancelBtn:addTouchEventListener(touchEventHandler)

	--m_confirmBtn: Button
	self.m_confirmBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_confirmBtn")
	self.m_confirmBtn:addTouchEventListener(touchEventHandler)

	--m_againBtn: Button
	self.m_againBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_againBtn")
	self.m_againBtn:addTouchEventListener(touchEventHandler)

	--m_itemRewardListView: ListView
    self.m_itemRewardListView = ccui.Helper:seekWidgetByName(self.rootNode, "m_itemRewardListView")
    self.m_itemRewardListView:addTouchEventListener(touchEventHandler)
        
    --m_roleRewardListView: ListView
    self.m_roleRewardListView = ccui.Helper:seekWidgetByName(self.rootNode, "m_roleRewardListView")
    self.m_roleRewardListView:addTouchEventListener(touchEventHandler)
    
	--m_pinzhiImage: ImageView
	self.m_pinzhiImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_pinzhiImage")

	--m_levelLabel: Text
	self.m_levelLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_levelLabel")

	--m_expProgressBar: LoadingBar
	self.m_expProgressBar = ccui.Helper:seekWidgetByName(self.rootNode, "m_expProgressBar")

	--m_pinzhiImage: ImageView
	self.m_pinzhiImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_pinzhiImage")

	--m_levelLabel: Text
	self.m_levelLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_levelLabel")

	--m_expProgressBar: LoadingBar
	self.m_expProgressBar = ccui.Helper:seekWidgetByName(self.rootNode, "m_expProgressBar")

	--m_pinzhiImage: ImageView
	self.m_pinzhiImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_pinzhiImage")

	--m_levelLabel: Text
	self.m_levelLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_levelLabel")

	--m_expProgressBar: LoadingBar
	self.m_expProgressBar = ccui.Helper:seekWidgetByName(self.rootNode, "m_expProgressBar")

	--m_pinzhiImage: ImageView
	self.m_pinzhiImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_pinzhiImage")

	--m_levelLabel: Text
	self.m_levelLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_levelLabel")

	--m_expProgressBar: LoadingBar
	self.m_expProgressBar = ccui.Helper:seekWidgetByName(self.rootNode, "m_expProgressBar")

	--m_pinzhiImage: ImageView
	self.m_pinzhiImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_pinzhiImage")

	--m_levelLabel: Text
	self.m_levelLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_levelLabel")

	--m_expProgressBar: LoadingBar
	self.m_expProgressBar = ccui.Helper:seekWidgetByName(self.rootNode, "m_expProgressBar")

	--m_expLabel: Text
	self.m_expLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_expLabel")

    --m_sportChangeLabel: Text
    self.m_sportChangeLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_sportChangeLabel")

    --m_sportSureBtn: Button
    self.m_sportSureBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_sportSureBtn")
    self.m_sportSureBtn:addTouchEventListener(touchEventHandler)

    --m_sportGuangImage: ImageView
    self.m_sportGuangImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_sportGuangImage")

    --m_sportHisLabel: Text
    self.m_sportHisLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_sportHisLabel")

    --m_sportNowLabel: Text
    self.m_sportNowLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_sportNowLabel")
    
    --m_rewardNum: Text --sport
    self.m_rewardNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_rewardNum")
    
    --m_TeamLevelUpPanel: panel --levelup
    self.m_TeamLevelUpPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_TeamLevelUpPanel")
    self.m_TeamLevelUpPanel:addTouchEventListener(touchEventHandler)
    
    
    --m_TeamLevelUpCoverPanel: panel --levelup
    self.m_TeamLevelUpCoverPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_TeamLevelUpCoverPanel")
    self.m_TeamLevelUpCoverPanel:addTouchEventListener(touchEventHandler)
    
    --m_preTeamLevel: text --levelup
    self.m_preTeamLevel = ccui.Helper:seekWidgetByName(self.rootNode, "m_preTeamLevel")
    
    --m_nowTeamLevel: text --levelup
    self.m_nowTeamLevel = ccui.Helper:seekWidgetByName(self.rootNode, "m_nowTeamLevel")
    
    --m_preMaxTeamEnergy: text --levelup
    self.m_preMaxTeamEnergy = ccui.Helper:seekWidgetByName(self.rootNode, "m_preMaxTeamEnergy")
    
    --m_nowMaxTeamEnergy: text --levelup
    self.m_nowMaxTeamEnergy = ccui.Helper:seekWidgetByName(self.rootNode, "m_nowMaxTeamEnergy")
    
    --m_preEnergy: text --levelup
    self.m_preEnergy = ccui.Helper:seekWidgetByName(self.rootNode, "m_preEnergy")

    --m_nowEnergy: text --levelup
    self.m_nowEnergy = ccui.Helper:seekWidgetByName(self.rootNode, "m_nowEnergy")
    
    --m_preMaxTeamLevel: text --levelup
    self.m_preMaxTeamLevel = ccui.Helper:seekWidgetByName(self.rootNode, "m_preMaxTeamLevel")

    --m_nowMaxTeamLevel: text --levelup
    self.m_nowMaxTeamLevel = ccui.Helper:seekWidgetByName(self.rootNode, "m_nowMaxTeamLevel")
    
    self:initUI(data)
end

function BattleResultUILayer:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")
        
    elseif sender == self.ResultRootLayout and event == cc.EventCode.ENDED then
        cclog("[BattleResultUILayer]--ResultRootLayout touched--")
        if  self.newBattleReward ==nil then self:closeResultLayer() end
        if self.showBattleResult == false then
            if  self.battleType == 5 then
                self:handleTouchEvent( self.m_BattlleFinishImage,event) 
            else
                if self.win == 0 then
                    self:handleTouchEvent( self.m_BattleFailImage,event)   
                else
                    self:handleTouchEvent( self.m_BattleWinImage,event)
                end

            end
            self.showBattleResult = true
        end
    elseif sender == self.m_BattleWinImage and event == cc.EventCode.ENDED then
        cclog("[BattleResultUILayer]--m_BattleWinImage touched--")
        if  self.newBattleReward ==nil then self:closeResultLayer() end
        self.showBattleResult = true
        self.m_BattleWinImage:setVisible(false)
        self.BgImage:setVisible(true)
        self.m_winLayout:setVisible(true)
        self:showBattleExpReward()
        self:showBattleItemReward()
        
    elseif sender == self.m_BattleFailImage and event == cc.EventCode.ENDED then
        cclog("[BattleResultUILayer]--m_BattleFailImage touched--")
        if  self.newBattleReward ==nil then self:closeResultLayer() end
        self.showBattleResult = true
        self.m_BattleFailImage:setVisible(false)
        self.BgImage:setVisible(true)
        self.m_failLayout:setVisible(true)
    
    elseif sender == self.m_BattlleFinishImage and event == cc.EventCode.ENDED then
        cclog("[BattleResultUILayer]--m_BattlleFinishImage touched--")
        if  self.newBattleReward ==nil then self:closeResultLayer() end
        self.showBattleResult = true
        self.m_BattlleFinishImage:setVisible(false)
        self.BgImage:setVisible(true)
        self.m_winLayout:setVisible(true)
        self:showBattleExpReward()
        self:showBattleItemReward()
    
	elseif sender == self.m_failLayout and event == cc.EventCode.ENDED then
		cclog("[BattleResultUILayer]--m_failLayout touched--")

	elseif sender == self.m_winLayout and event == cc.EventCode.ENDED then
		cclog("[BattleResultUILayer]--m_winLayout touched--")

	elseif sender == self.m_strengthenBtn and event == cc.EventCode.ENDED then
		cclog("[BattleResultUILayer]--m_strengthenBtn touched--")
        Http:req("AllEquipmentList",nil,function(res)
            if res.err then cclog("err!!!") return end
            cc.Director:getInstance():popScene()
            local scene = require("scene.tiejiangpu.TiejiangpuScene"):create(res)
            cc.Director:getInstance():replaceScene(scene)
        end)

	elseif sender == self.m_technologyBtn and event == cc.EventCode.ENDED then
		cclog("[BattleResultUILayer]--m_technologyBtn touched--")

	elseif sender == self.m_advancedBtn and event == cc.EventCode.ENDED then
		cclog("[BattleResultUILayer]--m_advancedBtn touched--")
        Http:req("GetAllSoul",nil,function(res)
            if res.err then cclog("err!!!") return end
            cc.Director:getInstance():popScene()
            local scene = require("scene.newhero.HeroScene"):create(res)
            cc.Director:getInstance():replaceScene(scene)
        end)
        
	elseif sender == self.m_cancelBtn and event == cc.EventCode.ENDED then
		cclog("[BattleResultUILayer]--m_cancelBtn touched--")
        Helper.PlayBackGroundMuisc("sound/background.mp3")
        self:closeResultLayer()
        
	elseif sender == self.m_confirmBtn and event == cc.EventCode.ENDED then
		cclog("[BattleResultUILayer]--m_confirmBtn touched--")
		
        local PveStageGuild = require "scene.pve.PVEUILayer"
        self.stageTag = PveStageGuild:getChallengeStageId()
        self.pStage = Roler:get("roleId").."_integerTag".."_pStage"
        local pvestage = cc.UserDefault:getInstance():getIntegerForKey(self.pStage)
        print("pvestage:"..pvestage)
        
        if pvestage == nil or pvestage == 0 then
            cc.UserDefault:getInstance():setIntegerForKey(self.pStage,1)
            pvestage = cc.UserDefault:getInstance():getIntegerForKey(self.pStage)
        end
        
        self.pveNum = pvestage
        if self.pveNum == self.stageTag then
            self.pveNum = self.pveNum + 1
            cc.UserDefault:getInstance():setIntegerForKey(self.pStage,self.pveNum)
        elseif (self.pveNum - self.stageTag) == 1 then
            self.pveNum = self.pveNum 
            cc.UserDefault:getInstance():setIntegerForKey(self.pStage,self.pveNum)  
        end  
        
         Http: req("GetAllStage",
            {pveType = 0,mapId = 1},
            function(res)
            self.guildPveAttack3 = res.data.stageList[3].attackSum
            self.guildPveAttack4 = res.data.stageList[4].attackSum
            self.guildPveAttack5 = res.data.stageList[5].attackSum
            self.guildPveAttack6 = res.data.stageList[6].attackSum
             
            print("self.guildPveAttack3_"..self.guildPveAttack3)
                if self.win == 1 and self.stageTag == 3 then
                    if self.pveNum == 4 then
                        if self.guildPveAttack3 ~= nil then 
                            if self.guildPveAttack3 <= 1 then
                                guardCenter:guildPveStage()
                                self.pveNum = 5
                                cc.UserDefault:getInstance():setIntegerForKey(self.pStage,self.pveNum)
                            end
                        end    
                    end
                elseif  self.win == 1 and self.stageTag == 4 then 
                    if self.pveNum == 5 then
                        if self.guildPveAttack4 ~= nil then 
                            if self.guildPveAttack4 <= 1 then
                                guardCenter:guildPveStage()
                                self.pveNum = 6
                                cc.UserDefault:getInstance():setIntegerForKey(self.pStage,self.pveNum)
                            end
                        end
                    end 
                elseif  self.win == 1 and self.stageTag == 5 then 
                    if self.pveNum == 6 then
                        if self.guildPveAttack5 ~= nil then 
                            if self.guildPveAttack5 <= 1 then
                                guardCenter:guildPveStage()
                                self.pveNum = 7
                                cc.UserDefault:getInstance():setIntegerForKey(self.pStage,self.pveNum)
                            end
                        end
                    end 
                elseif  self.win == 1 and self.stageTag == 6 then 
                    if self.pveNum == 7 then
                        if self.guildPveAttack6 ~= nil then 
                            if self.guildPveAttack6 <= 1 then
                                guardCenter:guildPveStage()
                                self.pveNum = 8
                                cc.UserDefault:getInstance():setIntegerForKey(self.pStage,self.pveNum)
                            end
                        end
                    end
                end
             
         end)    
        
        --关卡 判断是否首次通过--引导
        --print("guildPveNum_"..GuildPveStage)
        
        Helper.PlayBackGroundMuisc("sound/background.mp3")
        self:closeResultLayer()
        
	elseif sender == self.m_againBtn and event == cc.EventCode.ENDED then
		cclog("[BattleResultUILayer]--m_againBtn touched--")
        Helper.PlayBackGroundMuisc("sound/background.mp3")
        self:closeResultLayer("again")
        
	elseif sender == self.m_rewardListView and event == cc.EventCode.ENDED then
		cclog("[BattleResultUILayer]--m_rewardListView touched--")
    elseif sender == self.m_sportSureBtn and event == cc.EventCode.ENDED then
        self:closeResultLayer()
    elseif sender == self.m_TeamLevelUpPanel and event == cc.EventCode.ENDED then
        
        cclog("[BattleResultUILayer]--m_TeamLevelUpPanel touched--")
        local seq = cc.Sequence:create(cc.ScaleTo:create(0.5,0,0),cc.Hide:create())
        self.m_TeamLevelUpPanel:runAction(seq)
    elseif sender == self.m_TeamLevelUpCoverPanel and event == cc.EventCode.ENDED then

        cclog("[BattleResultUILayer]--m_TeamLevelUpCoverPanel touched--")
        local seq = cc.Sequence:create(cc.ScaleTo:create(0.25,0,0),cc.Hide:create())
        self.m_TeamLevelUpPanel:runAction(seq)
	end
	
end

--[[function BattleResultUILayer:savepve()
    --if self.pveNum == self.stageTag then
        self.pveNum = self.pveNum + 1
        cc.UserDefault:getInstance():setIntegerForKey(self.pStage,self.pveNum)
   -- end 
end--]]

--init your ui here
function BattleResultUILayer:initUI(data)
    --[[
    THE_PVE=0;//PVE推图
    SPORT=1;//竞技场
    ROB_BOOK=2;//夺书
    COINS_ECTYPE=3;//铜钱本
    ELITE_PVE=4;//精英本
    GUILD_PVE=5;//公会副本
    ROB_CONVOY=6;//抢劫护送
    ]]
    --self.m_againBtn:setVisible(false)
    
    self.isResultClosed = false
    self.showBattleResult = false
    self.isshowBattleExpReward = false
    if  data == nil then self:closeResultLayer() return end
    self.battleType = data["bt"] --战斗类型
    self.win = data["win"]
    self.starNum = data["star"]
    self.newBattleReward = data["reward"]
    self.sportReward = data["sportHisReward"]
    self.convoyRobReward = data["convoyRobReward"]
    
    if self.battleType ==0 or self.battleType == 4 then
        self.m_againBtn:setVisible(true)
    else
        self.m_againBtn:setVisible(false)
    end
    
    local soulExpList = {}
    local zhujiaoExpReward = nil
    if self.newBattleReward ~= nil then
        local expReward = self.newBattleReward["expReward"]      
        for i=1,table.getn(expReward) do
        	if expReward[i]["type"] == 0 then
        		zhujiaoExpReward = expReward[i]
            else
                table.insert(soulExpList,expReward[i])
        	end
        end
        self.soulExpList = soulExpList
    end
    self.sportHisReward = data["sportHisReward"]
    local isZhuJiaoLevelUp = zhujiaoExpReward["level"] - zhujiaoExpReward["oldLevel"] > 0 
    --local function zhujiaoLevelUpFunc()   
    local function zhujiaoLevleUpFunc()
        if  isZhuJiaoLevelUp then            
            local scaleTo = cc.ScaleTo:create(0,0,0)
            local seq = cc.Sequence:create(cc.Hide:create(),cc.DelayTime:create(0.2),cc.Show:create(),scaleTo,cc.ScaleTo:create(0.25,1.0,1.0))
            self.m_TeamLevelUpPanel:runAction(seq)
            local roleLv = require("config.define_role_lv")
            self.m_TeamLevelUpPanel:setVisible(true)
            self.m_preTeamLevel:setString(zhujiaoExpReward["oldLevel"])
            self.m_nowTeamLevel:setString(zhujiaoExpReward["level"])
            self.m_preMaxTeamEnergy:setString(roleLv[tostring(zhujiaoExpReward["oldLevel"])]["energy_max"])
            self.m_nowMaxTeamEnergy:setString(roleLv[tostring(zhujiaoExpReward["level"])]["energy_max"])
            self.m_preMaxTeamLevel:setString(zhujiaoExpReward["oldLevel"])
            self.m_nowMaxTeamLevel:setString(zhujiaoExpReward["level"])
        end
    end
    
    local winnner = nil
    if  self.battleType == 5 then
        self.m_BattlleFinishImage:setVisible(true)
        --local m_levelLabel = ccui.Helper:seekWidgetByName(self.m_BattlleFinishImage,"m_LvLabel")
        --local m_expLabel = ccui.Helper:seekWidgetByName(self.m_BattlleFinishImage,"m_expLabel")
        local m_scoreLabel = ccui.Helper:seekWidgetByName(self.m_BattlleFinishImage,"m_scoreLabel")
        local m_damageLabel = ccui.Helper:seekWidgetByName(self.m_BattlleFinishImage,"m_damageLabel")
        local m_CoinsLabel = ccui.Helper:seekWidgetByName(self.m_BattlleFinishImage,"m_CoinsLabel")
        --[[
        if  zhujiaoExpReward ~= nil then
            m_levelLabel:setString("+"..zhujiaoExpReward["level"])
            m_expLabel:setString("+"..zhujiaoExpReward["addExp"])
        end
        ]]
        m_scoreLabel:setString(self.newBattleReward["score"])
        m_CoinsLabel:setString(self.newBattleReward["coins"])
        --m_damageLabel:setString() --guild boss damage

        local function finishFunc()
            if  self.newBattleReward ~=nil and self.showBattleResult == false then 
                self.m_BattlleFinishImage:setVisible(false)
                self.BgImage:setVisible(true)
                self.m_winLayout:setVisible(true)
                self:showBattleExpReward()
                self:showBattleItemReward()
                self.showBattleResult = true
            end
        end

        self:runAction(cc.Sequence:create(cc.DelayTime:create(1.0),cc.CallFunc:create(finishFunc),cc.CallFunc:create(zhujiaoLevleUpFunc)))

    else
        if  data["win"] == 0 then 
            self.m_BattleFailImage:setVisible(true)
            
            local m_coinsImage = ccui.Helper:seekWidgetByName(self.m_BattleFailImage,"CoinsImage")
            local m_expImage = ccui.Helper:seekWidgetByName(self.m_BattleFailImage,"ExpImage")
            local m_zuanshiImage = ccui.Helper:seekWidgetByName(self.m_BattleFailImage,"ZuanShiImage")
            local m_gongxianImage = ccui.Helper:seekWidgetByName(self.m_BattleFailImage,"GongXianImage")
            
            local m_coinsLabel = ccui.Helper:seekWidgetByName(self.m_BattleFailImage,"m_CoinsLabel")
            local m_expLabel = ccui.Helper:seekWidgetByName(self.m_BattleFailImage,"m_expLabel")
            local m_zuanshiLabel = ccui.Helper:seekWidgetByName(self.m_BattleFailImage,"m_zuanshiLabel")
            local m_gongxianLabel = ccui.Helper:seekWidgetByName(self.m_BattleFailImage,"m_gongxianLabel")
            
            if  self.battleType == 6 then
                m_coinsImage:setVisible(false)
                m_expImage:setVisible(false)
                m_zuanshiImage:setVisible(true)
                m_gongxianImage:setVisible(true)
            end
            
            local faileLabel = self.m_failLayout:getChildByName("failLabel")
            if  self.battleType == 0 or self.battleType == 4  then                
                faileLabel:setVisible(true)
            else
                faileLabel:setVisible(false)
            end
            
            if  self.convoyRobReward ~= nil then

                m_zuanshiLabel:setString("+"..self.convoyRobReward["gold"])
                m_gongxianLabel:setString("+"..self.convoyRobReward["devote"])
            else
                if  zhujiaoExpReward ~= nil then

                    m_expLabel:setString("+"..zhujiaoExpReward["addExp"])
                    
                end
                if data["reward"] ~= nil then
                    m_coinsLabel:setString("+"..data["reward"]["coins"])
                end
               

            end

            
            local function falseFunc()
                if  self.newBattleReward ==nil then 
                    --self:closeResultLayer() 
                end
                self.m_BattleFailImage:setVisible(false)
                self:showBattleExpReward()
                self.BgImage:setVisible(true)
                self.m_failLayout:setVisible(true)
                self.showBattleResult = true
            end

            self:runAction(cc.Sequence:create(cc.DelayTime:create(1.0),cc.CallFunc:create(falseFunc),cc.CallFunc:create(zhujiaoLevleUpFunc)))


        else
            if  self.battleType == 2 then
                self.m_roleBgImage:setVisible(false)
                self.m_againBtn:setVisible(false)
            end
            self.m_BattleWinImage:setVisible(true)
            --local m_levelLabel = ccui.Helper:seekWidgetByName(self.m_BattleWinImage,"m_LvLabel")           
            local m_coinsImage = ccui.Helper:seekWidgetByName(self.m_BattleWinImage,"CoinsImage")
            local m_expImage = ccui.Helper:seekWidgetByName(self.m_BattleWinImage,"ExpImage")
            local m_zuanshiImage = ccui.Helper:seekWidgetByName(self.m_BattleWinImage,"ZuanShiImage")
            local m_gongxianImage = ccui.Helper:seekWidgetByName(self.m_BattleWinImage,"GongXianImage")

            local m_coinsLabel = ccui.Helper:seekWidgetByName(self.m_BattleWinImage,"m_CoinsLabel")
            local m_expLabel = ccui.Helper:seekWidgetByName(self.m_BattleWinImage,"m_expLabel")
            local m_zuanshiLabel = ccui.Helper:seekWidgetByName(self.m_BattleWinImage,"m_zuanshiLabel")
            local m_gongxianLabel = ccui.Helper:seekWidgetByName(self.m_BattleWinImage,"m_gongxianLabel")
            
            if  self.battleType == 6 then
                m_coinsImage:setVisible(false)
                m_expImage:setVisible(false)
                m_zuanshiImage:setVisible(true)
                m_gongxianImage:setVisible(true)
            end
            
            if  self.convoyRobReward ~= nil then
                m_zuanshiLabel:setString("+"..self.convoyRobReward["gold"])
                m_gongxianLabel:setString("+"..self.convoyRobReward["devote"])
            else

                if  zhujiaoExpReward ~= nil then
                    --m_levelLabel:setString("+"..zhujiaoExpReward["level"])
                    m_expLabel:setString("+"..zhujiaoExpReward["addExp"])
                    m_coinsLabel:setString("+"..data["reward"]["coins"])
                end
            end


            for i=1,self.starNum do
                local str = string.format("m_winStarImage%d",i)
                local m_winStarImage = ccui.Helper:seekWidgetByName(self.rootNode, str)
                m_winStarImage:loadTexture("xing_battle.png") 
                m_winStarImage:setVisible(false)
                local scaleBy = cc.ScaleBy:create(0.15,0.3,0.3)
                local seq = cc.Sequence:create(cc.DelayTime:create(0.2*i),cc.Show:create(),scaleBy,cc.ScaleTo:create(0.15,1.0,1.0))
                m_winStarImage:runAction(seq)

            end
            local function winFunc()
                if  self.sportReward ~= nil then
                    self.m_sportGuangImage:runAction(cc.RepeatForever:create(cc.RotateBy:create(2,360)))
                    self.m_BattleWinImage:setVisible(false)
                    local data = self.sportReward
                    print("high")
                    self.m_sportHisImage:setVisible(true)
                    self.m_sportHisLabel:setString(data["highest"])
                    self.m_sportNowLabel:setString(data["rank"])
                    self.m_sportChangeLabel:setString(data["rank"]-data["highest"]..")")
                    self.m_rewardNum:setString(data["gold"])
                               
                end
                if  self.newBattleReward ~=nil then 
                    self.m_BattleWinImage:setVisible(false)
                    self.BgImage:setVisible(true)
                    self.m_winLayout:setVisible(true)
                    self:showBattleExpReward()
                    self:showBattleItemReward()
                end
                self.showBattleResult = true
                
                -- guard tag --
                self:processGuard()
            end
            self:runAction(cc.Sequence:create(cc.DelayTime:create(1.2),cc.CallFunc:create(winFunc),cc.CallFunc:create(zhujiaoLevleUpFunc)))

        end
    end
    
    
  
    --[[
    if  data == -1 then
        self.win = 0 
        self.m_BattleFailImage:setVisible(true)
        local effectMusicPath = cc.FileUtils:getInstance():fullPathForFilename("sound/battle_failed.mp3") 
        cc.SimpleAudioEngine:getInstance():preloadEffect(effectMusicPath)
        cc.SimpleAudioEngine:getInstance():stopMusic()
        cc.SimpleAudioEngine:getInstance():playEffect(effectMusicPath)
    else
        self.win = 1
        self.m_BattleWinImage:setVisible(true)
        local effectMusicPath = cc.FileUtils:getInstance():fullPathForFilename("sound/battle_win.mp3") 
        cc.SimpleAudioEngine:getInstance():preloadEffect(effectMusicPath)
        cc.SimpleAudioEngine:getInstance():stopMusic()
        cc.SimpleAudioEngine:getInstance():playMusic(effectMusicPath)
    end
    ]]

end

--refresh all ui here
function BattleResultUILayer:refreshUI()
end

function BattleResultUILayer:showBattleExpReward()
    --self.m_roleRewardListView
    if  self.isshowBattleExpReward == true then return end
    self.isshowBattleExpReward = true
    local expData = self.soulExpList
    if expData == nil then return end
    local curExpList = {}
    local preExpList = {}
    local curLevelUpList = {}
    local RESULT_EXP_TAG = 8000
    for i=1,table.getn(expData) do  
        local rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("battleRoleRewardItem.csb")
        
        
        local dataItem = expData[i]
        
        local m_rewardRootLayout = ccui.Helper:seekWidgetByName(rootNode, "m_rewardRootLayout")
        local m_picImage = ccui.Helper:seekWidgetByName(rootNode, "m_picImage")
        local m_pinzhiImage = ccui.Helper:seekWidgetByName(rootNode, "m_pinzhiImage")
        local m_levelLabel = ccui.Helper:seekWidgetByName(rootNode, "m_levelLabel")
        local m_expProgressBar = ccui.Helper:seekWidgetByName(rootNode, "m_expProgressBar")
        local m_expLabel = ccui.Helper:seekWidgetByName(rootNode, "m_expLabel")
        local m_levelUpImage = ccui.Helper:seekWidgetByName(rootNode, "m_levelUpImage")
        
        m_picImage:loadTexture("soul/"..dataItem["icon"]..".png")
        m_pinzhiImage:loadTexture("normal_aochao_0"..dataItem["quality"]..".png")
        m_expLabel:setString(dataItem["addExp"])
        m_levelLabel:setString(dataItem["level"])

    --[[
    arr_cruExp[_id]=per;
    arr_preExp[_id]=preExp;
    arr_levelUp[_id]=itemData.level()-itemData.oldlevel();
    ]]
        local pre = dataItem["oldExp"]/dataItem["oldExpMax"]*100
        local cur = dataItem["exp"]/dataItem["expMax"]*100
        local preExp = math.floor(pre)
        local curExp = math.floor(cur)
        local levelUp = dataItem["level"] - dataItem["oldLevel"]
        table.insert(curExpList,curExp)
        table.insert(preExpList,preExp)
        table.insert(curLevelUpList,levelUp)
        m_expProgressBar:setPercent(preExp)
        rootNode:setTag(RESULT_EXP_TAG+i)
        
        if  levelUp >0 then
            local function show()
                m_levelUpImage:setVisible(true)
            end
            
            local function hide()
                m_levelUpImage:setVisible(false)
            end
            
            local seq = cc.Sequence:create(cc.DelayTime:create(0.2) ,cc.Hide:create() , cc.DelayTime:create(0.2) ,cc.Show:create())
            local rep = cc.Repeat:create(seq , 6)
            m_levelUpImage:runAction(cc.Sequence:create(cc.CallFunc:create(show),rep,cc.CallFunc:create(hide)))
        end
        
        self.m_roleRewardListView:pushBackCustomItem(rootNode)
    end
    
    local function UpdateExp(dt)
        for i=1,table.getn(expData) do
            if  curLevelUpList[i] == 0 then
                if preExpList[i] < curExpList[i] then
                	preExpList[i] = preExpList[i]+1
                end
            elseif curLevelUpList[i] > 0 then
                if preExpList[i] < 100 then
                    preExpList[i] = preExpList[i]+1
                else
                    curLevelUpList[i] = curLevelUpList[i]-1
                    preExpList[i] = 0
                end     
            end
        end
        for i=1,table.getn(expData) do
            if  self.m_roleRewardListView ~= nil then
                local roleExpItem = self.m_roleRewardListView:getChildByTag(RESULT_EXP_TAG+i)
                local m_expProgressBar = ccui.Helper:seekWidgetByName(roleExpItem, "m_expProgressBar")

                m_expProgressBar:setPercent(preExpList[i])
            end
        end
    end
    
    self.ScriptFuncId = cc.Director:getInstance():getScheduler():scheduleScriptFunc(UpdateExp,0.02,false)
end

function BattleResultUILayer:showBattleItemReward()
    if  self.m_itemRewardListView:getChildrenCount() ~= 0 then 
        self.m_itemRewardListView:removeAllItems()
    end
    if self.newBattleReward == nil then return end
    local itemData = self.newBattleReward["itemReward"]

    for i=1,table.getn(itemData) do       
        local rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("battleRewardItem.csb")
        

        local dataItem = itemData[i]
        local shopConfig = require("config.define_shop")
        local shopItme = shopConfig[tostring(itemData[i]["id"])]
        local m_itemRootLayout = ccui.Helper:seekWidgetByName(rootNode, "m_itemRootLayout")
        local m_picImage = ccui.Helper:seekWidgetByName(rootNode, "m_picImage")
        local m_pinzhiImage = ccui.Helper:seekWidgetByName(rootNode, "m_pinzhiImage")
        local m_numLabel = ccui.Helper:seekWidgetByName(rootNode, "m_numLabel")

        m_picImage:loadTexture(shopItme["icon"]..".png")
        m_pinzhiImage:loadTexture("normal_aochao_0"..shopItme["quality"]..".png")
        m_numLabel:setString(dataItem["num"])
        
        self.m_itemRewardListView:pushBackCustomItem(rootNode)
    end

end

function BattleResultUILayer:closeResultLayer(param)
    if self.isResultClosed == true then return end
    self.isResultClosed = true
    
    if  self.battleType == nil then return end
    if  self.battleType == 0 or self.battleType == 4 then
        if  param == "again" then
            local pveStageId = require("scene.pve.PVEUILayer")
            local Formation = require "data.Formation"
            local BattleScene = require("scene.battle2.BattleScene")
            local challenge_tab = Formation:get("pveAgainTab")
            Formation:set("releaseBattleRes",false)
            Http: req("NewStartCheck",
                {stageId =  pveStageId:getChallengeStageId() ,matrix = challenge_tab},
                function(res) 
                res.data["bt"] = self.battleType
                res.data["onExitTag"] = false
                cc.Director:getInstance():replaceScene(BattleScene:create(res.data))
            end)
            return             
        end
        local Formation = require "data.Formation"
        Formation:set("releaseBattleRes",true)
        if  self.win == 0 then
            --local layer = require("scene.pve.PVEUILayer")
            --local challengeCntValue = layer:getChallengeCntValue()
            --layer:setChallengeCntValue(math.min(challengeCntValue.attackSum+1,challengeCntValue.totalSum),challengeCntValue.totalSum)
            cc.Director:getInstance():popScene()
        else
            local pveType = 0
            if  self.battleType == 4 then pveType = 1 end
            Http: req("GetAllStage",
                {pveType = pveType,mapId = 0},
                function(res) 
                    if res.err then cclog("err!!!") return end
                    cc.Director:getInstance():popScene()
                    res.pveType = pveType
                    local scene = require("scene.pve.PVEScene"):create(res)
                    cc.Director:getInstance():replaceScene(scene)
                end)
        end
    else
        local Formation = require "data.Formation"
        Formation:set("releaseBattleRes",true)
        cc.Director:getInstance():popScene()
    end
end

return BattleResultUILayer
