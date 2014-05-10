

#ifndef __mcPlayer__SimulatorConfig__
#define __mcPlayer__SimulatorConfig__

#include <string>
#include <vector>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;


#if defined(_WINDOWS)
#define DIRECTORY_SEPARATOR "\\"
#define DIRECTORY_SEPARATOR_CHAR '\\'
#else
#define DIRECTORY_SEPARATOR "/"
#define DIRECTORY_SEPARATOR_CHAR '/'
#endif


#define kProjectConfigProjectDir                1
#define kProjectConfigScriptFile                2
#define kProjectConfigFrameSize                 4
#define kProjectConfigFrameScale                8


#define kProjectConfigAll (kProjectConfigProjectDir | kProjectConfigScriptFile | kProjectConfigFrameSize | kProjectConfigFrameScale)



class ProjectConfig
{
public:
    ProjectConfig(void)
    : m_scriptFile("./src/main.lua")
    , m_frameSize(640, 960)
    , m_frameScale(1.0f)

    {
    }
    
    const string getProjectDir(void);
    void setProjectDir(const string projectDir);
    
    const string getScriptFile(void);
    const string getScriptFileRealPath(void);
    void setScriptFile(const string scriptFile);
    
    const cocos2d::Size getFrameSize(void);
    void setFrameSize(cocos2d::Size frameSize);
    bool isLandscapeFrame(void);
    void changeFrameOrientation(void);
    void changeFrameOrientationToPortait(void);
    void changeFrameOrientationToLandscape(void);
    
    const float getFrameScale(void);
    void setFrameScale(float frameScale);

    //
    void parseCommandLine(vector<string>& args);
    const string makeCommandLine(unsigned int mask = kProjectConfigAll);

    
private:
    string	m_projectDir;
    string	m_scriptFile;
    cocos2d::Size	m_frameSize;
    float	m_frameScale;

    
    //
    const string replaceProjectDirToFullPath(const string& path);
    bool isAbsolutePath(const string& path);
};


#endif
