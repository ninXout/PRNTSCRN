#include "ScreenshotPopup.hpp"
#include "ScreenshotManager.hpp"

using namespace geode::prelude;

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

    auto btn2 = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Close"),
        this,
        menu_selector(ScreenshotPopup::keyBackClicked)
    );
    btn2->setPosition(ccp(79.f, 25.f));
    menu->addChild(btn2);

    CCMenu* toggle1 = CCMenu::create();
    toggle1->setLayout(RowLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(1.f));
    toggle1->setPosition(ccp(285.f, 190.f));
    toggle1->setContentWidth(235.f);
    menu->addChild(toggle1);
    copyClipboardToggle = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 0.75f);
    toggle1->addChild(copyClipboardToggle);
    CCLabelBMFont* copyClipboardLabel = CCLabelBMFont::create("Copy to Clipboard", "bigFont.fnt");
    copyClipboardLabel->setScale(0.4f);
    toggle1->addChild(copyClipboardLabel);
    toggle1->updateLayout();

    CCMenu* toggle2 = CCMenu::create();
    toggle2->setLayout(RowLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(1.f));
    toggle2->setPosition(ccp(285.f, 160.f));
    toggle2->setContentWidth(235.f);
    menu->addChild(toggle2);
    hidePlayerToggle = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 0.75f);
    toggle2->addChild(hidePlayerToggle);
    CCLabelBMFont* hidePlayerLabel = CCLabelBMFont::create("Hide Player Icon", "bigFont.fnt");
    hidePlayerLabel->setScale(0.4f);
    toggle2->addChild(hidePlayerLabel);
    toggle2->updateLayout();

    CCMenu* toggle3 = CCMenu::create();
    toggle3->setLayout(RowLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(1.f));
    toggle3->setPosition(ccp(285.f, 130.f));
    toggle3->setContentWidth(235.f);
    menu->addChild(toggle3);
    hideUIToggle = CCMenuItemToggler::createWithStandardSprites(this, nullptr, 0.75f);
    toggle3->addChild(hideUIToggle);
    CCLabelBMFont* hideUILabel = CCLabelBMFont::create("Hide UI Layer", "bigFont.fnt");
    hideUILabel->setScale(0.4f);
    toggle3->addChild(hideUILabel);
    toggle3->updateLayout();

    return true;
}

void ScreenshotPopup::onScreenshot(CCObject*) {
    Screenshot::create(hidePlayerToggle->isToggled(), hideUIToggle->isToggled()).saveImage(copyClipboardToggle->isToggled());
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