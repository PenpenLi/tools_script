Roler = {}

--define data key
local _data = {
    roleId        =0,
    nickname      ="", --角色的昵称
    levelMax      =0, --等级上限
    level         =1, --当前等级
    sex           =0, --性别
    roleIndex         =0, --角色位置
    avatarId      =0, --虚拟形象ID
    exp           =0, --经验值
    vipExp        =0, --vip当前经验值
    expMax        = 0,
    achievement       =0, --成就
    achievementMax       =0, --最高成就
    title          ="", --称号
    guildId       =0, --所属社团id
    guildPosition     =0, --在社团职位
    hp            =0, --生命值
    energy        =0, --体力
    energyMax     =0, --体力上限
    physicalAtk       =0, --物理攻击
    physicalDef       =0, --物理防御
    magicAtk      =0, --魔法攻击
    magicDef      =0, --魔法防御
    dex           =0, --命中率
    cri           =0, --暴击率
    block         =0, --格挡率
    agl           =0, --闪避率
    fc            =0, --战斗力
    gold          =0, --金币
    coins         =0, --铜币
    weaponId      =0, --武器id
    weaponIcon        =0, --装备icon
    weaponQuality     =0,--武器品质
    headId        =0, --头盔id，,如果是-1，没有装备且没有可以装备的，是0，没有装备，且有可以装备的
    headIcon      =0, --头盔icon
    headQuality   =0,--头盔品质
    clothingId        =0, --服装id
    clothIcon     =0, --衣服icon
    clothQuality =0,--衣服品质
    bagNum        =0, --背包格子数 
    currentBagNum     =0, --当前使用格子数
    vipLevel      =0, --vip级别
    friendNum     =0, --已添加的好友数量
    friendMax     =0, --好友数量上限
    lastLoginTime     =0, --上次登陆时间
    lastOfflineTime   =0, --上次离线时间
    lineTime      =0, --今日在线分钟数
    friendship        =0, --友情点
    guideFlag     =0, --新手引导
    soul1Icon = 0,--魂魄1Icon
    soul2Icon = 0,--魂魄2Icon
    needLvExp=0,--升级所需经验
    potential=0,--潜力
    zhili=0,--智力
    playername="",--登录账号名
    settingStatus={},--设置信息
    hasBuyEnergyCnt=0,--体力已购买次数
    maxBuyEnergyCnt=0,--体力购买次数上限
    
    changeNameCost=0,--修改主角名的需花费N金币
    recoverEnergyInterval=0,--体力恢复间隔（分钟）
    recoverEnergyNum=0,--每次恢复N点体力
    unReadMails = 0,--未读邮件数
    firstRecharge = 0,--首充状态（0：未充值，1：已充未领，2：已领）
    firstRechargeReward = {},--首充奖励(形如 物品ID1:数量1,物品ID2:数量2）
    openAct = 1 -- 开服活动状态(0:已结束，1：进行中)
}

function Roler:get(key)
    if key == nil then return nil end
    assert(_data[key] ~= nil, "[Roler]key not defined in _data")
    return _data[key]
end

function Roler:set(key, val)
    assert(key ~= nil, "[Roler]key invalid")
    if _data[key] == nil then cclog("[Roler]%s not defined in _data", key) end
    _data[key] = val
end

function Roler:setData(data)
    assert(type(data) == "table", "[Roler] data invalid")
    for k, v in pairs(data) do
        self:set(k, v)
    end
end

return Roler