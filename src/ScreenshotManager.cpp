#include "ScreenshotManager.hpp"

#ifdef GEODE_IS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#endif

using namespace geode::prelude;

Screenshot Screenshot::create(bool hidePlayer, bool hideUI) {
    Screenshot ss;

    auto size = CCDirector::sharedDirector()->getWinSize();
    auto renderer = CCRenderTexture::create(size.width, size.height, cocos2d::kCCTexture2DPixelFormat_RGB888);
    log::info("okay created the renderer");

    renderer->begin();
    /*bool uivisible = nodeIsVisible(PlayLayer::get()->getChildByID("UILayer"));
    bool progvisible = nodeIsVisible(PlayLayer::get()->getChildByID("progress-bar"));
    bool percvisible = nodeIsVisible(PlayLayer::get()->getChildByID("percentage-label"));
    bool p1visible = nodeIsVisible(PlayLayer::get()->m_player1);
    bool p2visible = nodeIsVisible(PlayLayer::get()->m_player2);*/
    bool uivisible = true;
    bool progvisible = true;
    bool percvisible = true;
    bool p1visible = true;
    bool p2visible = true;
    if (PlayLayer::get() && hideUI) {
        PlayLayer::get()->getChildByID("UILayer")->setVisible(false);
        PlayLayer::get()->getChildByID("progress-bar")->setVisible(false);
        PlayLayer::get()->getChildByID("percentage-label")->setVisible(false);
    }
    if (PlayLayer::get() && hidePlayer) {
        PlayLayer::get()->m_player1->setVisible(false);
        PlayLayer::get()->m_player2->setVisible(false);
    }
    if (static_cast<CCNode*>(CCDirector::get()->getRunningScene())->getChildrenCount()) {
        log::info("started visiting");
        CCArrayExt<CCNode*> children = CCDirector::get()->getRunningScene()->getChildren();
        for (auto* child : children) {
            if (child->getID() == "PauseLayer" || child->getID() == "ScreenshotPopup") {
                // nothing
            } else {
                child->visit();
                /*if (child->getID() == "LevelEditorLayer" && child->getChildByID("EditorPauseLayer") && child->getChildByID("EditorUI") && static_cast<CCNode*>(child->getChildByID("main-node")->getChildByID("batch-layer")->getChildren()->objectAtIndex(0))) {
                    child->getChildByID("EditorUI")->setVisible(false);
                    child->getChildByID("EditorPauseLayer")->setVisible(false);
                    static_cast<CCNode*>(child->getChildByID("main-node")->getChildByID("batch-layer")->getChildren()->objectAtIndex(0))->setVisible(false);
                    child->visit();
                    child->getChildByID("EditorPauseLayer")->setVisible(true);
                    child->getChildByID("EditorUI")->setVisible(true);
                    static_cast<CCNode*>(child->getChildByID("main-node")->getChildByID("batch-layer")->getChildren()->objectAtIndex(0))->setVisible(true);
                }*/
            }
        }
        log::info("finished visiting");
    }
    renderer->end();
    if (PlayLayer::get() && hideUI) {
        PlayLayer::get()->getChildByID("UILayer")->setVisible(uivisible);
        PlayLayer::get()->getChildByID("progress-bar")->setVisible(progvisible);
        PlayLayer::get()->getChildByID("percentage-label")->setVisible(percvisible);
    }
    if (PlayLayer::get() && hidePlayer) {
        PlayLayer::get()->m_player1->setVisible(p1visible);
        PlayLayer::get()->m_player2->setVisible(p2visible);
    }

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
        free(buffer);
        log::info("freed the stuff");
    }).detach();
    log::info("left the windows thread");
}
#endif