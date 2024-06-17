#define CommentType CommentTypeDummy
#include <CoreGraphics/CoreGraphics.h>
#include <ImageIO/ImageIO.h>
#include <CoreServices/CoreServices.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#undef CommentType

#include <Geode/Geode.hpp>
#include "ScreenshotManager.hpp"

bool CGImageWriteToFile(CGImageRef image, std::string name) {
    CFStringRef file = CFStringCreateWithCString(kCFAllocatorDefault,
    name.c_str(),
    kCFStringEncodingMacRoman);
    CFStringRef type = CFSTR("public.png");
    CFURLRef urlRef = CFURLCreateWithFileSystemPath( kCFAllocatorDefault, file, kCFURLPOSIXPathStyle, false );
    CGImageDestinationRef destination = CGImageDestinationCreateWithURL( urlRef, kUTTypePNG, 1, NULL );
    CGImageDestinationAddImage( destination, image, NULL );
    CGImageDestinationFinalize( destination );
    if (!destination) {
        return false;
    }

    CGImageDestinationAddImage(destination, image, nil);

    if (!CGImageDestinationFinalize(destination)) {
        CFRelease(destination);
        return false;
    }

    CFRelease(destination);
    return true;
}

#ifdef GEODE_IS_MACOS

void Screenshot::saveImage(bool toClipboard) {
    float width = contentSize.width;
    float height = contentSize.height;
    int dataLen = width * height * 4;

    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, imageData, dataLen, NULL);

    CGImageRef cgImg = CGImageCreate(
        width, height, 
        8, 8 * 4, width * 4, 
        CGColorSpaceCreateDeviceRGB(), 
        kCGBitmapByteOrderDefault | kCGImageAlphaPremultipliedLast, 
        provider,   // data provider
        NULL,       // decode
        true,        // should interpolate
        kCGRenderingIntentDefault
    );

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        if (toClipboard) {
            NSImage * image = [[NSImage alloc] initWithCGImage:cgImg size:NSMakeSize(width, height)];

            if (image != nil) {
                NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
                [pasteboard clearContents];
                NSArray *copiedObjects = [NSArray arrayWithObject:image];
                [pasteboard writeObjects:copiedObjects];
            }
        } else {
            CGImageWriteToFile(cgImg, getFileName());
        }
    });
}

#endif