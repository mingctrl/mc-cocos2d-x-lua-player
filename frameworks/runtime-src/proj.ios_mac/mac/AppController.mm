#import "AppController.h"


@implementation AppController

@synthesize menu;

-(void) dealloc{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    
    self.window = nil;
    self.menu = nil;
    self.textView = nil;
    
    [super dealloc];
}



#pragma mark -
#pragma delegates

- (void) applicationDidFinishLaunching:(NSNotification *)aNotification
{
    //update config
    [self updateProjectConfigFromCommandLineArgs:&projectConfig];
    
    [self setupConsole];
    
    //load search paths
    vector<string> args;
    args.push_back(projectConfig.getProjectDir());
    FileUtils::getInstance()->setSearchPaths(args);
    
    //app
    app = new AppDelegate();
    app->setProjectConfig(projectConfig);
    app->run();
    
    //setting space
    [self.window orderFrontRegardless];
    [[NSApplication sharedApplication] activateIgnoringOtherApps:YES];
    [self.window becomeFirstResponder];
    [self.window makeKeyAndOrderFront:self];
    [self.window setAcceptsMouseMovedEvents:NO];
    [self.window setFrameOrigin:CGPointMake(1000, 800)];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication
{
    return YES;
}

- (BOOL) applicationShouldHandleReopen:(NSApplication *)sender hasVisibleWindows:(BOOL)flag
{
    return NO;
}

- (void) windowWillClose:(NSNotification *)notification
{
    [[NSApplication sharedApplication] terminate:self];
    
    //debug:glview window will no call exit complete
    exit(1);
}

-(void)relauch{
    
    if (1) {
        NSArray *args = [self makeCommandLineArgsFromProjectConfig];
        NSURL *url = [NSURL fileURLWithPath:[[NSBundle mainBundle] bundlePath]];
        NSMutableDictionary *configuration = [NSMutableDictionary dictionaryWithObject:args forKey:NSWorkspaceLaunchConfigurationArguments];
        NSError *error = [[[NSError alloc] init] autorelease];
        [[NSWorkspace sharedWorkspace] launchApplicationAtURL:url
                                                      options:NSWorkspaceLaunchNewInstance
                                                configuration:configuration error:&error];
        
    }
    
    //close all
    Director::getInstance()->end();
    [self.window close];
}

- (NSString*) browseFolder:(NSString*)title
{
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    [openDlg setTitle:title];
    [openDlg setCanChooseDirectories:YES];
    [openDlg setCanChooseFiles:NO];
    [openDlg setCanHide:YES];
    [openDlg setCanCreateDirectories:NO];
    [openDlg setCanSelectHiddenExtension:NO];
    [openDlg setAllowsMultipleSelection:NO];
    
    if ([openDlg runModal] == NSFileHandlingPanelOKButton)
    {
        NSURL *url = [openDlg.URLs objectAtIndex:0];
        return [url path];
    }
    return nil;
}



#pragma mark -
#pragma mark commandLine

- (NSMutableArray*) makeCommandLineArgsFromProjectConfig{
    return [self makeCommandLineArgsFromProjectConfig:kProjectConfigAll];
}

- (NSMutableArray*) makeCommandLineArgsFromProjectConfig:(unsigned int)mask{
    
    NSString *commandLine = [NSString stringWithCString:projectConfig.makeCommandLine(mask).c_str() encoding:NSUTF8StringEncoding];
    return [NSMutableArray arrayWithArray:[commandLine componentsSeparatedByString:@" "]];
}

- (void) updateProjectConfigFromCommandLineArgs:(ProjectConfig *)config
{
    NSArray *nsargs = [[NSProcessInfo processInfo] arguments];
    vector<string> args;
    for (int i = 0; i < [nsargs count]; ++i)
    {
        args.push_back([[nsargs objectAtIndex:i] cStringUsingEncoding:NSUTF8StringEncoding]);
    }
    config->parseCommandLine(args);
    
    if (config->getProjectDir().length() == 0)
    {
    }
}


#pragma mark -
#pragma mark action

-(IBAction)relauchAction:(id)sender{
    
    //relauch
    [self relauch];
}

-(IBAction)openAction:(id)sender{
    //browseFolder
    NSString *path = [self browseFolder:@"Choose Project Directory"];
    if (path)
    {
        projectConfig.setProjectDir([path cStringUsingEncoding:NSUTF8StringEncoding]);
        [self relauch];
    }
}

-(IBAction)lauchIphoneAction:(id)sender{
    cocos2d::Size aSize = cocos2d::Size(640, 960);
    projectConfig.setFrameSize(aSize);
    [self relauch];
}

-(IBAction)lauchIphone5Action:(id)sender{
    cocos2d::Size aSize = cocos2d::Size(640, 1136);
    projectConfig.setFrameSize(aSize);
    [self relauch];
}

-(IBAction)lauchIpadAction:(id)sender{
    cocos2d::Size aSize = cocos2d::Size(768, 1024);
    projectConfig.setFrameSize(aSize);
    [self relauch];
}

-(IBAction)portaitAction:(id)sender{
    projectConfig.changeFrameOrientationToPortait();
    [self relauch];
}
-(IBAction)landscapeAction:(id)sender{
    projectConfig.changeFrameOrientationToLandscape();
    [self relauch];
}

-(IBAction)zoomOut100Action:(id)sender{
    Director::getInstance()->getOpenGLView()->setFrameZoomFactor(1.0f);
    projectConfig.setFrameScale(1.0f);
}

-(IBAction)zoomOut50Action:(id)sender{
    Director::getInstance()->getOpenGLView()->setFrameZoomFactor(0.5f);
    projectConfig.setFrameScale(0.5f);
}



#pragma mark -
#pragma mark Console

-(void)setupConsole{
    //console
    if (1) {
        pipe = [NSPipe pipe] ;
        pipeReadHandle = [pipe fileHandleForReading] ;
        int outfd = [[pipe fileHandleForWriting] fileDescriptor];
        
        if (dup2(outfd, fileno(stderr)) != fileno(stderr) || dup2(outfd, fileno(stdout)) != fileno(stdout))
        {
            perror("Unable to redirect output");
        }
        else
        {
            [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(handleNotification:) name: NSFileHandleReadCompletionNotification object: pipeReadHandle] ;
            [pipeReadHandle readInBackgroundAndNotify];
        }
    }
}

- (void)handleNotification:(NSNotification *)note
{
    [pipeReadHandle readInBackgroundAndNotify] ;
    
    NSData *data = [[note userInfo] objectForKey:NSFileHandleNotificationDataItem];
    NSString *str = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
    
    [self consloseAppend:str];
}

-(void)consloseAppend:(NSString*)aString{
    NSFont *font = [NSFont fontWithName:@"Monaco" size:12.0];
    NSDictionary *attrsDictionary = [NSDictionary dictionaryWithObject:font forKey:NSFontAttributeName];
    NSAttributedString *string = [[NSAttributedString alloc] initWithString:aString attributes:attrsDictionary];
    
    NSTextStorage *storage = [self.textView textStorage];
	[storage beginEditing];
	[storage appendAttributedString:string];
    [storage endEditing];
    
    [self.textView scrollRangeToVisible: NSMakeRange(self.textView.string.length, 0)];
}


@end



