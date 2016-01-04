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
end

function BattleResultUILayer:onExit()
    if self.ScriptFuncId ~= nil then
        cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.ScriptFuncId)
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
    
    self:initUI(data)
end

function BattleResultUILayer:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")
        
    elseif sender == self.ResultRootLayout and event == cc.EventCode.ENDED then
        cclog("[BattleResultUILayer]--ResultRootLayout touched--")
        if  self.newBattleReward ==nil then self:closeResultLayer() end
        if self.showBattleResult == false then
            if self.win == 0 then
                self:handleTouchEvent( self.m_BattleFailImage,event)   
            else
                self:handleTouchEvent( self.m_BattleWinImage,event)
            end

            self.showBattleResult = true
        end
    elseif sender == self.m_BattleWinImage and event == cc.EventCode.ENDED then
        cclog("[BattleResultUILayer]--m_BattleWinImage touched--")
        if  self.newBattleReward ==nil then self:closeResultLayer() end
        self.m_BattleWinImage:setVisible(false)
        self.BgImage:setVisible(true)
        self.m_winLayout:setVisible(true)
        self:showBattleExpReward()
        self:showBattleItemReward()
        
    elseif sender == self.m_BattleFailImage and event == cc.EventCode.ENDED then
        cclog("[BattleResultUILayer]--m_BattleFailImage touched--")
        if  self.newBattleReward ==nil then self:closeResultLayer() end
        self.m_BattleFailImage:setVisible(false)
        self.BgImage:setVisible(true)
        self.m_failLayout:setVisible(true)
    
    elseif sender == self.m_BattlleFinishImage and event == cc.EventCode.ENDED then
        cclog("[BattleResultUILayer]--m_BattlleFinishImage touched--")
        if  self.newBattleReward ==nil then self:closeResultLayer() end
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

	elseif sender == self.m_technologyBtn and event == cc.EventCode.ENDED then
		cclog("[BattleResultUILayer]--m_technologyBtn touched--")

	elseif sender == self.m_advancedBtn and event == cc.EventCode.ENDED then
		cclog("[BattleResultUILayer]--m_advancedBtn touched--")

	elseif sender == self.m_cancelBtn and event == cc.EventCode.ENDED then
		cclog("[BattleResultUILayer]--m_cancelBtn touched--")
        Helper.PlayBackGroundMuisc("sound/background.mp3")
        self:closeResultLayer()
        
	elseif sender == self.m_confirmBtn and event == cc.EventCode.ENDED then
		cclog("[BattleResultUILayer]--m_confirmBtn touched--")
        Helper.PlayBackGroundMuisc("sound/background.mp3")
        self:closeResultLayer()
        
	elseif sender == self.m_againBtn and event == cc.EventCode.ENDED then
		cclog("[BattleResultUILayer]--m_againBtn touched--")
        Helper.PlayBackGroundMuisc("sound/background.mp3")
        self:closeResultLayer()
        
	elseif sender == self.m_rewardListView and event == cc.EventCode.ENDED then
		cclog("[BattleResultUILayer]--m_rewardListView touched--")
    elseif sender == self.m_sportSureBtn and event == cc.EventCode.ENDED then
        self:closeResultLayer()
	end
	
end

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
    self.isResultClosed = false
    if  data == nil then self:closeResultLayer() return end
    self.battleType = data["bt"] --战斗类型
    self.win = data["win"]
    self.starNum = data["star"]
    self.newBattleReward = data["reward"]
    self.sportReward = data["sportHisReward"]
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
    
    local winnner = nil
    if  self.battleType == 5 then
        self.m_BattlleFinishImage:setVisible(true)
        local m_levelLabel = ccui.Helper:seekWidgetByName(self.m_BattlleFinishImage,"m_LvLabel")
        local m_expLabel = ccui.Helper:seekWidgetByName(self.m_BattlleFinishImage,"m_expLabel")
        local m_scoreLabel = ccui.Helper:seekWidgetByName(self.m_BattlleFinishImage,"m_scoreLabel")
        local m_damageLabel = ccui.Helper:seekWidgetByName(self.m_BattlleFinishImage,"m_damageLabel")
        local m_CoinsLabel = ccui.Helper:seekWidgetByName(self.m_BattlleFinishImage,"m_CoinsLabel")
        if  zhujiaoExpReward ~= nil then
            m_levelLabel:setString("+"..zhujiaoExpReward["level"])
            m_expLabel:setString("+"..zhujiaoExpReward["addExp"])
        end
        m_scoreLabel:setString(self.newBattleReward["score"])
        m_CoinsLabel:setString(self.newBattleReward["coins"])
        --m_damageLabel:setString()

        local function finishFunc()
            if  self.newBattleReward ~=nil then 
                self.m_BattlleFinishImage:setVisible(false)
                self.BgImage:setVisible(true)
                self.m_winLayout:setVisible(true)
                self:showBattleExpReward()
                self:showBattleItemReward()
            end
        end

        self:runAction(cc.Sequence:create(cc.DelayTime:create(1.0),cc.CallFunc:create()))

    else
        if  data["win"] == 0 then 
            self.m_BattleFailImage:setVisible(true)
            local m_levelLabel = ccui.Helper:seekWidgetByName(self.m_BattleFailImage,"m_LvLabel")
            local m_expLabel = ccui.Helper:seekWidgetByName(self.m_BattleFailImage,"m_expLabel")
            if  zhujiaoExpReward ~= nil then
                m_levelLabel:setString("+"..zhujiaoExpReward["level"])
                m_expLabel:setString("+"..zhujiaoExpReward["addExp"])
            end

            local function falseFunc()
                if  self.newBattleReward ==nil then self:closeResultLayer() end
                self.m_BattleFailImage:setVisible(false)
                self.BgImage:setVisible(true)
                self.m_failLayout:setVisible(true)
            end

            self:runAction(cc.Sequence:create(cc.DelayTime:create(1.0),cc.CallFunc:create(falseFunc)))


        else
            self.m_BattleWinImage:setVisible(true)
            local m_levelLabel = ccui.Helper:seekWidgetByName(self.m_BattleWinImage,"m_LvLabel")
            local m_expLabel = ccui.Helper:seekWidgetByName(self.m_BattleWinImage,"m_expLabel")
            local m_CoinsLabel = ccui.Helper:seekWidgetByName(self.m_BattleWinImage,"m_CoinsLabel")
            if  zhujiaoExpReward ~= nil then
                m_levelLabel:setString("+"..zhujiaoExpReward["level"])
                m_expLabel:setString("+"..zhujiaoExpReward["addExp"])
                m_CoinsLabel:setString("+"..data["reward"]["coins"])
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

            end
            self:runAction(cc.Sequence:create(cc.DelayTime:create(1.2),cc.CallFunc:create(winFunc)))

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
    self.showBattleResult = false
end

--refresh all ui here
function BattleResultUILayer:refreshUI()
end

function BattleResultUILayer:showBattleExpReward()
    --self.m_roleRewardListView
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
            
            local seq = cc.Sequence:create(cc.DelayTime:create(0.1) ,cc.Hide:create() , cc.DelayTime:create(0.1) ,cc.Show:create())
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

        local m_itemRootLayout = ccui.Helper:seekWidgetByName(rootNode, "m_itemRootLayout")
        local m_picImage = ccui.Helper:seekWidgetByName(rootNode, "m_picImage")
        local m_pinzhiImage = ccui.Helper:seekWidgetByName(rootNode, "m_pinzhiImage")
        local m_numLabel = ccui.Helper:seekWidgetByName(rootNode, "m_numLabel")

        m_picImage:loadTexture("goods/"..dataItem["id"]..".png")
        --m_pinzhiImage:loadTexture("normal_aochao_0"..dataItem["quality"]..".png")
        m_numLabel:setString(dataItem["num"])
        
        self.m_itemRewardListView:pushBackCustomItem(rootNode)
    end

end

function BattleResultUILayer:closeResultLayer()
    if self.isResultClosed == true then return end
    self.isResultClosed = true
    cc.Director:getInstance():popScene()
    if  self.battleType == nil then return end
    if  self.battleType == 0 then
        local scene = require("scene.pve.PVEScene"):create()
        cc.Director:getInstance():replaceScene(scene)
    elseif  self.battleType == 1 then
        local scene = require("scene.sport.SportScene"):create()
        cc.Director:getInstance():replaceScene(scene)
    end
end

return BattleResultUILayer
