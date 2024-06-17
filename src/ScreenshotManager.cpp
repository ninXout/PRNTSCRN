#include "ScreenshotManager.hpp"

#ifdef GEODE_IS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#endif

using namespace geode::prelude;

Screenshot Screenshot::create(bool hidePlayer, bool hideUI) {
    Screenshot ss;

    auto size = CCDirector::sharedDirector()->getWinSize();
    auto renderer = CCRenderTexture::create(size.width, size.height, cocos2d::kCCTexture2DPixelFormat_RGBA8888);
    log::info("okay created the renderer");

    renderer->begin();
    bool uivisible = nodeIsVisible(PlayLayer::get()->getChildByID("UILayer"));
    bool progvisible = nodeIsVisible(PlayLayer::get()->getChildByID("progress-bar"));
    bool percvisible = nodeIsVisible(PlayLayer::get()->getChildByID("percentage-label"));
    bool p1visible = nodeIsVisible(PlayLayer::get()->m_player1);
    bool p2visible = nodeIsVisible(PlayLayer::get()->m_player2);
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

    // TODO: do the CustomRenderTexture thing for mac as well cuz its faster
    #ifdef GEODE_IS_WINDOWS
    ss.imageData = static_cast<CustomRenderTexture*>(renderer)->getData();
    #endif
    #ifdef GEODE_IS_MACOS
    CCImage* image = renderer->newCCImage(true);
    ss.imageData = image->getData();
    #endif
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
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int a = 0;
    unsigned int b = 0; // TODO: image cropping
    const auto src_width = static_cast<unsigned int>(contentSize.width);
    const auto src_height = static_cast<unsigned int>(contentSize.height);
    a = a ? a : src_width;
    b = b ? b : src_height;
    log::info("about to start windows thread");
    std::thread([this]() {
        log::info("inside windows thread");
        auto len = static_cast<size_t>((a - x) * (b - y));
        auto buffer = static_cast<uint32_t*>(malloc(len * sizeof(uint32_t)));
        log::info("buffer made");
        auto cx = x, cy = y;
        for (size_t i = 0; i < len; ++i) {
            const size_t j = ((src_height - cy) * src_width + cx) << 2;
            if (++cx == a) ++cy, cx = x;
            buffer[i] = imageData[j] << 16 | imageData[j + 1] << 8 | imageData[j + 2];
        }
        log::info("buffer finished");
        auto bitmap = CreateBitmap((a - x), (b - y), 1, 32, buffer);
        log::info("bitmap created");

        if (OpenClipboard(NULL))
            if (EmptyClipboard()) {
                log::info("clipboard opened and emptied");
                SetClipboardData(CF_BITMAP, bitmap);
                log::info("clipboard data set");
                CloseClipboard();
            }
        free(buffer);
        free(imageData);
        log::info("freed the stuff");
    }).detach();
    log::info("left the windows thread");
}
#endif