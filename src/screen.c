#include "screen.h"
#include "os.h"

#if defined(IS_MACOSX)
	#include <ApplicationServices/ApplicationServices.h>
#elif defined(USE_X11)
	#include <X11/Xlib.h>
	#include "xdisplay.h"
#endif

MMSize getMainDisplaySize(void)
{
#if defined(IS_MACOSX)
	CGDirectDisplayID displayID = CGMainDisplayID();
	return MMSizeMake(CGDisplayPixelsWide(displayID),
	                  CGDisplayPixelsHigh(displayID));
#elif defined(USE_X11)
	Display *display = XGetMainDisplay();
	const int screen = DefaultScreen(display);

	return MMSizeMake((size_t)DisplayWidth(display, screen),
	                  (size_t)DisplayHeight(display, screen));
#elif defined(IS_WINDOWS)
	return MMSizeMake((size_t)GetSystemMetrics(SM_CXSCREEN),
										(size_t)GetSystemMetrics(SM_CYSCREEN));
#endif
}

MMSize getLocalDisplaySize(void)
{
#if defined(IS_MACOSX)
	CGEventRef event = CGEventCreate(NULL);
	CGPoint point = CGEventGetLocation(event);
	CFRelease(event);

	// Modified!
	// Idk why this would ever be greater than 1?
	int MAX_DISPLAYS = 1;

	uint32_t *actualCount = (uint32_t *)malloc(sizeof(uint32_t));
	CGDirectDisplayID *displays = (CGDirectDisplayID *)malloc(MAX_DISPLAYS * sizeof(CGDirectDisplayID));
	CGGetDisplaysWithPoint(point, MAX_DISPLAYS, displays, actualCount);

	free(actualCount);

	MMSize size = MMSizeMake(CGDisplayPixelsWide(displays[0]),
													 CGDisplayPixelsHigh(displays[0]));

	free(displays);
	return size;
#elif defined(USE_X11)
	Display *display = XGetMainDisplay();
	const int screen = DefaultScreen(display);

	return MMSizeMake((size_t)DisplayWidth(display, screen),
										(size_t)DisplayHeight(display, screen));
#elif defined(IS_WINDOWS)
	return MMSizeMake((size_t)GetSystemMetrics(SM_CXSCREEN),
										(size_t)GetSystemMetrics(SM_CYSCREEN));
#endif
}

bool pointVisibleOnMainDisplay(MMPoint point)
{
	MMSize displaySize = getMainDisplaySize();
	return point.x < displaySize.width && point.y < displaySize.height;
}
