#pragma once
#include <Geode/Geode.hpp>

class ScreenshotPopup : public geode::Popup<> {
public:
    constexpr static float POPUP_WIDTH = 340.f;
    constexpr static float POPUP_HEIGHT = 240.f;

    static ScreenshotPopup* create();

protected:
    bool copyToClipboard = false;

    CCMenuItemToggler* copyClipboardToggle;
    CCMenuItemToggler* hidePlayerToggle;
    CCMenuItemToggler* hideUIToggle;

    bool setup() override;

    void onScreenshot(cocos2d::CCObject*);
    void onCopyClipboard(cocos2d::CCObject*);
};