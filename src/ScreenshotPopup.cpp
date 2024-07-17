#include "ScreenshotPopup.hpp"
#include "ScreenshotManager.hpp"
#include <UIBuilder.hpp>

using namespace geode::prelude;

CCMenu* ScreenshotPopup::createSetting(const std::string& title, const std::string& key) {
    CCMenu* thing = Build<CCMenu>(CCMenu::create())
        .layout(RowLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(1.f))
        .width(235.f)
        .collect();

    CCMenuItemToggler* toggler = Build<CCMenuItemToggler>::createToggle(CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"), CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), 
    [&](CCMenuItemToggler* toggler){ 
        toggler->toggle(!Mod::get()->getSettingValue<bool>(key));
        Mod::get()->setSettingValue<bool>(key, !Mod::get()->getSettingValue<bool>(key));
    })
        .scale(0.75f)
        .parent(thing)
        .collect();

    toggler->toggle(Mod::get()->getSettingValue<bool>(key));

    Build<CCLabelBMFont>::create(title.c_str(), "bigFont.fnt")
        .scale(0.4f)
        .parent(thing);
    
    thing->updateLayout();
    return thing;
}

bool ScreenshotPopup::setup() {
    this->setTitle("Screenshot");
    this->setID("ScreenshotPopup");

    CCMenu* menu = CCMenu::create();

    float centerX = CCDirector::get()->getWinSize().width / 2;
    float centerY = CCDirector::get()->getWinSize().height / 2;

    menu->setPosition(ccp(0.f, 0.f));
    m_mainLayer->addChild(menu);

    auto btn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Screenshot!"),
        this,
        menu_selector(ScreenshotPopup::onScreenshot)
    );
    btn->setPosition(ccp(221.f, 25.f));
    menu->addChild(btn);

    CCMenu* settingsMenu = CCMenu::create();
    settingsMenu->setPosition(ccp(300.f, 130.f));
    settingsMenu->setContentWidth(240.f);
    settingsMenu->setLayout(ColumnLayout::create()->setAutoScale(false)->setAxisReverse(true));

    settingsMenu->addChild(createSetting("Copy To Clipboard", "copy-clipboard"));
    settingsMenu->addChild(createSetting("Hide Player Icon", "hide-player"));
    settingsMenu->addChild(createSetting("Hide UI Layer", "hide-ui"));
    settingsMenu->addChild(createSetting("Auto Screenshot", "auto-screenshot"));

    CCMenu* autoPercent = Build<CCMenu>::create()
        .layout(RowLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(4.f))
        .width(230.f)
        .parent(settingsMenu)
        .collect();

    auto* input = Build<InputNode>::create(25.f, "%", "bigFont.fnt", "1234567890", 2)
        .scale(0.75f)
        .parent(autoPercent)
        .collect();

    input->setString(std::to_string(Mod::get()->getSettingValue<int64_t>("auto-percent")));
    input->getInput()->setDelegate(this);

    Build<CCLabelBMFont>::create("Auto Percent", "bigFont.fnt")
        .scale(0.4f)
        .parent(autoPercent);

    autoPercent->updateLayout();

    m_mainLayer->addChild(settingsMenu);
    settingsMenu->updateLayout();

    return true;
}

void ScreenshotPopup::onScreenshot(CCObject*) {
    Screenshot::create(Mod::get()->getSettingValue<bool>("hide-player"), Mod::get()->getSettingValue<bool>("hide-ui")).saveImage(Mod::get()->getSettingValue<bool>("copy-clipboard"));
}

void ScreenshotPopup::textChanged(CCTextInputNode* p0) {
    Mod::get()->setSettingValue<int64_t>("auto-percent", std::stoi(p0->getString()));
}

ScreenshotPopup* ScreenshotPopup::create() {
    auto ret = new ScreenshotPopup;
    if (ret->initAnchored(POPUP_WIDTH, POPUP_HEIGHT)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}