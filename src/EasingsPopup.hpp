#pragma once

#include "main.hpp"

class EasingsPopup : public geode::Popup<mySetupTriggerPopup*, bool> {
protected:
	mySetupTriggerPopup* triggerPopup;
    bool isEasingTwo;

    const float popupWidth = 480.f;
    const float popupHeight = 280.f;

    const float nodeWidth = 135;
    const float nodeHeight = 60;

    const float padding = 8.f;
    const float innerYPadding = 10.f;

    const std::vector<ccColor3B> typeColors = {
        {180, 180, 180}, // Gray
        {255, 204, 0},   // Gold
        {0, 200, 255},   // Cyan
        {255, 100, 100}, // Red
        {200, 100, 255}, // Purple
        {100, 255, 150}, // Light Green
        {255, 150, 0},   // Orange
    };

    bool setup(mySetupTriggerPopup* triggerPopup, bool isEasingTwo = false) override {
		this->triggerPopup = triggerPopup;
        this->isEasingTwo = isEasingTwo;

        this->setTitle("Select Easing");

        float startX = (popupWidth + padding - 3 * (nodeWidth + padding)) / 2;
        float startY = (popupHeight + padding - 3 * (nodeHeight + padding)) / 2 - 10;

		for (size_t i = 0; i < parentEasingTypes.size(); ++i) {
			auto node = CCScale9Sprite::create("square02_001.png");
            node->setContentSize({ nodeWidth, nodeHeight });
            node->setAnchorPoint({ 0.f, 0.f });
			node->setColor({ 0, 0, 0 });
			node->setOpacity(50);

            int row = (i - 1) / 3 + 1; 
            int col = (i - 1) % 3;

            if (i == 0) {
                node->setPosition({ startX + nodeWidth + padding, startY });
            } else {
                node->setPosition({ startX + (nodeWidth + padding) * col, startY + (nodeHeight + padding) * row });
            }

			auto menu = CCMenu::create();
            menu->setPosition(ccp(0, 0));
            menu->setContentSize({0, 0});
            menu->ignoreAnchorPointForPosition(false);
            menu->setAnchorPoint(ccp(0, 0));
            
            if (i == 0) {
                auto spr = CCLabelBMFont::create(parentEasingTypes[i].c_str(), "bigFont.fnt");
                spr->setScale(0.5f);
                spr->setColor(typeColors[i]);

                CCMenuItemSpriteExtra* btn = nullptr;
                if (isEasingTwo) {
                    btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(EasingsPopup::onSelectEasingTwo));
                } else {
                    btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(EasingsPopup::onSelectEasing));
                }

                btn->setAnchorPoint({ 0.5f, 0.5f });
                btn->setPosition({ nodeWidth / 2, nodeHeight / 2 });
                btn->setTag(i);
                menu->addChild(btn);
                node->addChild(menu);
                m_mainLayer->addChild(node);
                continue;
            }

            auto easingLabel = CCLabelBMFont::create(parentEasingTypes[i].c_str(), "bigFont.fnt");
            easingLabel->setScale(0.5f);
            easingLabel->setAnchorPoint({0.5f, 1.f});
            easingLabel->setColor(typeColors[i]);

            easingLabel->setPosition({nodeWidth / 2, nodeHeight - innerYPadding});
            node->addChild(easingLabel);

            std::vector<CCMenuItemSpriteExtra*> buttons;
            float totalWidth = 0.f;
            for (size_t j = 1; j <= easingSubTypes.size(); ++j) {
                auto spr = CCLabelBMFont::create(easingSubTypes[j - 1].c_str(), "bigFont.fnt");
                spr->setScale(0.5f);

                CCMenuItemSpriteExtra* btn = nullptr;
                if (isEasingTwo) {
                    btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(EasingsPopup::onSelectEasingTwo)
                    );
                } else {
                    btn = CCMenuItemSpriteExtra::create(spr, this,
                        menu_selector(EasingsPopup::onSelectEasing)
                    );
                }

                btn->setAnchorPoint({0, 0});
                btn->setTag(((i - 1) * 3) + j);
                buttons.push_back(btn);
                totalWidth += btn->getScaledContentWidth();
            }

            float spacing = nodeWidth - totalWidth;
            float gap = spacing / (buttons.size() + 1);
            float currentX = gap;
            for (auto* button : buttons) {
                button->setPosition({currentX, innerYPadding});
                currentX += button->getScaledContentWidth() + gap;
                menu->addChild(button);
            }

			node->addChild(menu);
			m_mainLayer->addChild(node);
		}

		return true;
	}

	void onSelectEasing(CCObject* sender) {
        if (SetupAreaMoveTriggerPopup* areaMovePopup = typeinfo_cast<SetupAreaMoveTriggerPopup*>(this->triggerPopup)) {
            areaMovePopup->updateValue(242, sender->getTag());
        } else if (SetupEnterEffectPopup* enterEffectTrigger = typeinfo_cast<SetupEnterEffectPopup*>(this->triggerPopup)) {
            enterEffectTrigger->updateValue(242, sender->getTag());
        } else {
            triggerPopup->updateValue(30, sender->getTag());
        }

        auto spr = static_cast<ButtonSprite*>(triggerPopup->m_fields->m_easingButton->getNormalImage());
        spr->setString(sender->getTag() == 0 ? "Select Easing" : easingTypes[sender->getTag()].c_str());

		onClose(nullptr);
	}

    void onSelectEasingTwo(CCObject* sender) {
        triggerPopup->updateValue(248, sender->getTag());

        auto spr = static_cast<ButtonSprite*>(triggerPopup->m_fields->m_easingButtonTwo->getNormalImage());
        spr->setString(sender->getTag() == 0 ? "Select Easing" : easingTypes[sender->getTag()].c_str());

		onClose(nullptr);
    }

public:
    static EasingsPopup* create(mySetupTriggerPopup* triggerPopup, bool isEasingTwo) {
        auto ret = new EasingsPopup();
        if (ret->initAnchored(ret->popupWidth, ret->popupHeight, triggerPopup, isEasingTwo)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};

