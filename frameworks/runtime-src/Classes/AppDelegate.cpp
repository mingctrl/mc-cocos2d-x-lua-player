#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

using namespace CocosDenshion;

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if(!glview) {
        Size configSize = m_projectConfig.getFrameSize();
		glview = GLView::createWithRect("mc-cocos2d-x-lua-player", Rect(0,0,configSize.width,configSize.height));
		director->setOpenGLView(glview);
	}

    glview->setDesignResolutionSize(320, 480, ResolutionPolicy::NO_BORDER);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
//	auto engine = LuaEngine::getInstance();
//	ScriptEngineManager::getInstance()->setScriptEngine(engine);
//	engine->executeScriptFile("src/main.lua");
    
    auto *pEngine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(pEngine);
    
    LuaStack *pStack = pEngine->getLuaStack();
    // load script
    string path = FileUtils::getInstance()->fullPathForFilename(m_projectConfig.getScriptFileRealPath().c_str());
    size_t pos;
    while ((pos = path.find_first_of("\\")) != std::string::npos)
    {
        path.replace(pos, 1, "/");
    }
    size_t p = path.find_last_of("/\\");
    if (p != path.npos)
    {
        const string dir = path.substr(0, p);
        pStack->addSearchPath(dir.c_str());
        
        p = dir.find_last_of("/\\");
        if (p != dir.npos)
        {
            pStack->addSearchPath(dir.substr(0, p).c_str());
        }
    }
    
    //load custom framework
    if (1){
        //        const string precompiledFrameworkPath = SimulatorConfig::sharedDefaults()->getPrecompiledFrameworkPath();
        //        pStack->loadChunksFromZIP(precompiledFrameworkPath.c_str());
    }
    
//    string env = "__LUA_STARTUP_FILE__=\"";
//    env.append(path);
//    env.append("\"");
//    pEngine->executeString(env.c_str());
//    
//    CCLOG("------------------------------------------------");
//    CCLOG("LOAD LUA FILE: %s", path.c_str());
//    CCLOG("------------------------------------------------");
    pEngine->executeScriptFile(path.c_str());
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::setProjectConfig(const ProjectConfig& config)
{
    m_projectConfig = config;
}

