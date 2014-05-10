require "Cocos2d"
require "Cocos2dConstants"
require "src.debug"

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
    collectgarbage("collect")
    -- avoid memory leak
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)



    local visibleSize = cc.Director:getInstance():getVisibleSize()
    local origin = cc.Director:getInstance():getVisibleOrigin()



    local function createBgLayer()
    	
    	local s = cc.Director:getInstance():getWinSize()

    	local bg = cc.LayerColor:create(cc.c4b(255, 255, 255, 255))

    	-- tip
    	local tipLabel = cc.LabelTTF:create("Command+O", "Arial", 24)
    	bg:addChild(tipLabel)
    	tipLabel:setPosition(cc.p(s.width/2, s.height/2))


    	return bg;
    end



    -- run
    local sceneGame = cc.Scene:create()
    sceneGame:addChild(createBgLayer())
	
	if cc.Director:getInstance():getRunningScene() then
		cc.Director:getInstance():replaceScene(sceneGame)
	else
		cc.Director:getInstance():runWithScene(sceneGame)
	end

end


xpcall(main, __G__TRACKBACK__)

