
#include "SimulatorConfig.h"

const string ProjectConfig::getProjectDir(void)
{
    return m_projectDir;
}

void ProjectConfig::setProjectDir(const string projectDir)
{
    m_projectDir = projectDir;
    string scriptPath = projectDir;
    m_scriptFile = scriptPath.append("/main.lua");
}

const string ProjectConfig::getScriptFile(void)
{
    return m_scriptFile;
}

const string ProjectConfig::getScriptFileRealPath(void)
{
    return replaceProjectDirToFullPath(m_scriptFile);
}

void ProjectConfig::setScriptFile(const string scriptFile)
{
    m_scriptFile = scriptFile;
}

const Size ProjectConfig::getFrameSize(void)
{
    return m_frameSize;
}

void ProjectConfig::setFrameSize(Size frameSize)
{
    if (frameSize.width <= 0 || frameSize.width > 2048 || frameSize.height <= 0 || frameSize.height > 2048) return;
    m_frameSize = frameSize;
}

bool ProjectConfig::isLandscapeFrame(void)
{
    return m_frameSize.width > m_frameSize.height;
}

void ProjectConfig::changeFrameOrientation(void)
{
    float w = m_frameSize.width;
    m_frameSize.width = m_frameSize.height;
    m_frameSize.height = w;
}

void ProjectConfig::changeFrameOrientationToPortait(void)
{
    if (isLandscapeFrame()) changeFrameOrientation();
}

void ProjectConfig::changeFrameOrientationToLandscape(void)
{
    if (!isLandscapeFrame()) changeFrameOrientation();
}

const float ProjectConfig::getFrameScale(void)
{
    return m_frameScale;
}

void ProjectConfig::setFrameScale(float frameScale)
{
    CCAssert(frameScale > 0, "Invalid frameScale");
    m_frameScale = frameScale;
}


#pragma mark -
#pragma mark commandLine

void ProjectConfig::parseCommandLine(vector<string>& args)
{
    for (vector<string>::iterator it = args.begin(); it != args.end(); ++it)
    {
        const string& arg = *it;
        
        if (arg.compare("-workdir") == 0)
        {
            ++it;
            setProjectDir(*it);
        }
        else if (arg.compare("-file") == 0)
        {
            ++it;
            setScriptFile(*it);
        }
        
        else if (arg.compare("-size") == 0)
        {
            ++it;
            const string& sizeStr(*it);
            size_t pos = sizeStr.find('x');
            int width = 0;
            int height = 0;
            if (pos != sizeStr.npos && pos > 0)
            {
                string widthStr, heightStr;
                widthStr.assign(sizeStr, 0, pos);
                heightStr.assign(sizeStr, pos + 1, sizeStr.length() - pos);
                width = atoi(widthStr.c_str());
                height = atoi(heightStr.c_str());
                setFrameSize(Size(width, height));
            }
        }
        else if (arg.compare("-scale") == 0)
        {
            ++it;
            float scale = atof((*it).c_str());
            setFrameScale(scale);
        }
    }
}

const string ProjectConfig::makeCommandLine(unsigned int mask /* = kProjectConfigAll */)
{
    stringstream buff;
    
    if (mask & kProjectConfigProjectDir)
    {
		string path = getProjectDir();
		if (path.length())
		{
			buff << "-workdir ";
			buff << path;
		}
    }
    
    if (mask & kProjectConfigScriptFile)
    {
		string path = getScriptFileRealPath();
		if (path.length())
		{
			buff << " -file ";
			buff << path;
		}
    }
    
    if (mask & kProjectConfigFrameSize)
    {
        buff << " -size ";
        buff << (int)getFrameSize().width;
        buff << "x";
        buff << (int)getFrameSize().height;
    }
    
    if (mask & kProjectConfigFrameScale)
    {
        if (getFrameScale() < 1.0f)
        {
            buff << " -scale ";
            buff.precision(2);
            buff << getFrameScale();
        }
    }
    
    return buff.str();
}



//
const string ProjectConfig::replaceProjectDirToFullPath(const string& path)
{
    if (isAbsolutePath(path)) return path;
    
	if (path.length() == 0) return m_projectDir;
    
    string result = path;
    if (path.compare(0, 8, "$PROJDIR") == 0)
    {
        result = m_projectDir;
        string suffix = path.substr(8);
        if (suffix[0] == DIRECTORY_SEPARATOR_CHAR)
        {
            suffix = suffix.substr(1);
        }
        result.append(suffix);
    }
    return result;
}

bool ProjectConfig::isAbsolutePath(const string& path)
{
    if (DIRECTORY_SEPARATOR_CHAR == '/')
    {
        return path.length() > 0 && path[0] == '/';
    }
    return path.length() > 2 && path[1] == ':';
}

