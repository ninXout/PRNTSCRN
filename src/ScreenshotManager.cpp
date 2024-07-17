#include "ScreenshotManager.hpp"

#ifdef GEODE_IS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#endif

using namespace geode::prelude;

Screenshot Screenshot::create(bool hidePlayer, bool hideUI) {
    Screenshot ss;

    auto size = CCDirector::sharedDirector()->getWinSize();
    CCRenderTexture* renderer = CCRenderTexture::create(size.width, size.height, cocos2d::kCCTexture2DPixelFormat_RGB888);
    log::info("okay created the renderer: {}", renderer ? "true" : "false");

    renderer->begin();
    if (static_cast<CCNode*>(CCDirector::get()->getRunningScene())->getChildrenCount()) {
        log::info("started visiting");
        CCArrayExt<CCNode*> children = CCDirector::get()->getRunningScene()->getChildren();
        for (auto* child : children) {
            if (child->getID() != "PauseLayer" && child->getID() != "ScreenshotPopup") {
                if (child->getID() == "PlayLayer") {
                    CCArrayExt<CCNode*> newchildren = child->getChildren();
                    for (auto* newchild : newchildren) {
                        if (newchild->getID() == "UILayer" || newchild->getID() == "progress-bar" || newchild->getID() == "percentage-label" || newchild->getID() == "debug-text" || newchild->getID() == "dankmeme.globed2/game-overlay") {
                            if (!hideUI) newchild->visit();
                        } else {
                            newchild->visit();
                        }
                    }
                } else {
                    child->visit();
                }
            }
        }
        log::info("finished visiting");
    }
    renderer->end();

    ss.imageData = static_cast<CustomRenderTexture*>(renderer)->getData();
    ss.contentSize = static_cast<CustomRenderTexture*>(renderer)->getSizeInPixels();

    return ss;
}

std::string Screenshot::getFileName() {
    std::string name = geode::Mod::get()->getConfigDir().append("").string();
    if (PlayLayer::get()) {
        int iter = 0;
        while (std::filesystem::exists(name + std::to_string(PlayLayer::get()->m_level->m_levelID) + "-" + std::to_string(iter) + ".png")) {
            iter++;
        }
        name += std::to_string(PlayLayer::get()->m_level->m_levelID) + "-" + std::to_string(iter) + ".png";
    }
    if (LevelEditorLayer::get()) {
        int iter = 0;
        while (std::filesystem::exists(name + LevelEditorLayer::get()->m_level->m_levelName + "-" + std::to_string(iter) + ".png")) {
            iter++;
        }
        name += LevelEditorLayer::get()->m_level->m_levelName + "-" + std::to_string(iter) + ".png";
    }
    return name;
}

#ifdef GEODE_IS_WINDOWS
void Screenshot::saveImage(bool toClipboard) {
    // TODO: image cropping
    const auto src_width = static_cast<unsigned int>(contentSize.width);
    const auto src_height = static_cast<unsigned int>(contentSize.height);
    log::info("about to start windows thread");
    std::thread([=]() {
        log::info("inside windows thread");
        auto bitmap = CreateBitmap(src_width, src_height, 1, 32, imageData);
        log::info("bitmap created");

        if (OpenClipboard(NULL))
            if (EmptyClipboard()) {
                log::info("clipboard opened and emptied");
                SetClipboardData(CF_BITMAP, bitmap);
                log::info("clipboard data set");
                CloseClipboard();
            }
        free(imageData);
        log::info("freed the stuff");
    }).detach();
    log::info("left the windows thread");
}
#endif