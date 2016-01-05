Player = {}

--define data key
local _data = {
    user = "", --用户账号
    region = 0, --服务器区域ID
    enable = 0, --该ID是否被禁用 1是0否
    role1 = 0,
    role2 = 0,
    role3 = 0,
    role4 = 0,
    lastLoginTime = 0, --最后登录时间
    lastOfflineTime = 0, --最后离线时间
    loginTime = 0, --本次登录时间
    playId = 0, --主键id
}

function Player:get(key)
    if key == nil then return _data end
    assert(_data[key] ~= nil, "[Player]key not defined in _data")
    return _data[key]
end

function Player:set(key, val)
    assert(key ~= nil, "[Player]key invalid")
    if _data[key] == nil then cclog("[Player]%s not defined in _data", key) end
    _data[key] = val
end

function Player:setData(data)
    assert(type(data) == "table", "[Player] data invalid")
    for k, v in pairs(data) do
        self:set(k, v)
    end
end

return Player


