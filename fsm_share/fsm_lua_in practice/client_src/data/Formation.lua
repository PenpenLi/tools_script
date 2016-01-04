Formation = {}

--define data key
local _data = {
    sport_formation = {}, --竞技场防守
    sport_battle = {}, --竞技场挑战
    convoy = {}, --押镖
    pve = {},   
    book = {}, --夺书
    battledata = {}, --战斗数据
    fightpower_sport = "",
    pveAgainTab={},
    releaseBattleRes = true
}

function Formation:get(key)
    if key == nil then return _data end
    assert(_data[key] ~= nil, "[Player]key not defined in _data")
    return _data[key]
end

function Formation:set(key, val)
    assert(key ~= nil, "[Player]key invalid")
    if _data[key] == nil then cclog("[Formation]%s not defined in _data", key) end
    _data[key] = val
end

function Formation:setData(data)
    assert(type(data) == "table", "[Formation] data invalid")
    for k, v in pairs(data) do
        self:set(k, v)
    end
end

return Formation




