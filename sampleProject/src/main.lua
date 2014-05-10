
require "Cocos2d"


-- cclog
cclog = function(...)
    print(string.format(...))
end


-- for CCLuaEngine traceback
function __G__TRACKBACK__(msg)
    cclog("----------------------------------------")
    cclog("LUA ERROR: " .. tostring(msg) .. "\n")
    cclog(debug.traceback())
    cclog("----------------------------------------")
end




local function main()
	-- avoid memory leak
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)

    -- run home scene
	local scene = require("src.SampleScene")
	cc.Director:getInstance():runWithScene(scene)

end



--run
xpcall(main,__G__TRACKBACK__)
