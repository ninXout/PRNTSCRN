#include <Geode/Geode.hpp>
#include "ScreenshotManager.hpp"
#include "ScreenshotPopup.hpp"

using namespace geode::prelude;

#include <Geode/modify/PauseLayer.hpp>
class $modify(NewPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();

		auto menu = CCMenu::create();

		auto btn = CCMenuItemSpriteExtra::create(
			CCSprite::create("PS_screenshot.png"_spr),
			this,
			menu_selector(NewPauseLayer::onScreenshotPopup)
		);
		btn->setPosition(100.f, 100.f);
		menu->addChild(btn);

		this->addChild(menu);
	}

	void onScreenshotPopup(CCObject*) {
		ScreenshotPopup::create()->show();
	}
};
