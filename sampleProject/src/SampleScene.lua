require "Cocos2d"


local homeScene = nil



local function createBgLayer()
	
-- 	-- test layer
	local bgLayer = cc.LayerColor:create(cc.c4b(255,255,195,255),300,300)
	return bgLayer
end





--run
if homeScene == nil	then
	homeScene = cc.Scene:create()
	homeScene:addChild(createBgLayer())
end

return homeScene
