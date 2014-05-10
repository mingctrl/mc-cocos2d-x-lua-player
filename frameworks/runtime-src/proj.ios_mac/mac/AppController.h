#import "cocos2d.h"
#include "SimulatorConfig.h"
#include "AppDelegate.h"

class AppControllerBridge;

using namespace cocos2d;

@interface AppController : NSObject <NSApplicationDelegate, NSWindowDelegate>
{
    GLView *glView;
    
    AppDelegate *app;
    ProjectConfig projectConfig;
    
    //console pipe
    NSPipe *pipe;
    NSFileHandle *pipeReadHandle;
    
}

@property (nonatomic, assign) IBOutlet NSMenu* menu;
@property (assign) IBOutlet NSTextView *textView;
@property (assign) IBOutlet NSWindow *window;

-(IBAction)relauchAction:(id)sender;
-(IBAction)openAction:(id)sender;

-(IBAction)lauchIphoneAction:(id)sender;
-(IBAction)lauchIphone5Action:(id)sender;
-(IBAction)lauchIpadAction:(id)sender;

-(IBAction)portaitAction:(id)sender;
-(IBAction)landscapeAction:(id)sender;

-(IBAction)zoomOut100Action:(id)sender;
-(IBAction)zoomOut50Action:(id)sender;


@end
