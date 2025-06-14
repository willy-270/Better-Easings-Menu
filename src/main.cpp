#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include "main.hpp"
#include "EasingsPopup.hpp"

$override
void mySetupTriggerPopup::show() {
	SetupTriggerPopup::show();

	bool foundEasingLabel = false;
	for (auto node : CCArrayExt<CCNode*>(this->m_mainLayer->getChildren())) {
		auto label = typeinfo_cast<CCLabelBMFont*>(node);
		if (!label) continue;
		std::string labelText = label->getString();
		if (labelText == "Easing" && label->isVisible()) {
			foundEasingLabel = true;
			break;
		}
	}
	if (!foundEasingLabel) return;


	// these have easing label @ (189.5, 84)
	static const std::unordered_set<int> triggersThatDontUseEasingLabel = {
		3006, 3007, 3008, 3017, 3018, 3019, 3020, 3021,  
	};

	// these have easing label @ (172.5, 81)
	static const std::unordered_set<int> triggersThatDontUseEasingLabelVERSIONTWO = {
		3011, 3012, 3013, 3014, 3015
	};

	// 2nd label @ (379.5, 84)
	static const std::unordered_set<int> triggersWithTwoEasings = {
		3006, 3007, 3008
	};

	int objID;
	if (m_gameObjects) {
		// i know that if there are multiple triggers selected they must have the same id for easing to be an option
		objID = typeinfo_cast<EffectGameObject*>(m_gameObjects->objectAtIndex(0))->m_objectID;
	} else {
		objID = m_gameObject->m_objectID;
	}

	CCLabelBMFont* easingLabel = m_easingLabel;
	// literally only object that uses that
	if (objID == 1916) {
		easingLabel = typeinfo_cast<SetupCameraOffsetTrigger*>(this)->m_cameraEasingLabel;
	}

	if (objID == 2999) {
		easingLabel = typeinfo_cast<SetupMGTrigger*>(this)->m_mgEasingLabel;
	}

	float easingID;
	if (SetupAreaMoveTriggerPopup* areaMovePopup = typeinfo_cast<SetupAreaMoveTriggerPopup*>(this)) {
		easingID = areaMovePopup->getValue(242);
	} else if (SetupEnterEffectPopup* enterEffectTrigger = typeinfo_cast<SetupEnterEffectPopup*>(this)) {
		easingID = enterEffectTrigger->getValue(242);
	} else {
		easingID = getValue(30);
	}

	if (objID == 1346) {
		m_easingRateButton->setVisible(easingID >= 1.0f && easingID <= 6.0f);
	}


	if (!easingLabel) {
		if (!triggersThatDontUseEasingLabel.count(objID) && !triggersThatDontUseEasingLabelVERSIONTWO.count(objID)) {
			return;
		}
		hideEasingLabel();
	} else {
		easingLabel->setVisible(false);
	}

	hideEasingArrowButtons();
	updateEasingRateButtonVisibility();

	std::string easingStr = "Select Easing";
	if (easingID > 0) {
		easingStr = easingTypes[easingID];
	}
	auto spr = ButtonSprite::create(easingStr.c_str());
	spr->setScale(0.7f);

	m_fields->m_easingButton = CCMenuItemSpriteExtra::create(spr, this, menu_selector(mySetupTriggerPopup::onSelect));
	m_fields->m_easingButton->setTag(0);
	CCPoint labelPos;
	if (easingLabel) labelPos = easingLabel->getPosition();
	if (triggersThatDontUseEasingLabel.count(objID) == 1) labelPos = CCPoint(189.5f, 84.f);
	if (triggersThatDontUseEasingLabelVERSIONTWO.count(objID) == 1) labelPos = CCPoint(172.5f, 81.f);
	auto worldPos = convertToWorldSpace(labelPos);
	auto localPos = m_buttonMenu->convertToNodeSpace(worldPos);
	m_fields->m_easingButton->setPosition(localPos);

	if (triggersWithTwoEasings.count(objID)) {\
		easingID = getValue(248); 
		std::string easingStr = "Select Easing";
		if (easingID > 0) {
			easingStr = easingTypes[easingID];
		}
		auto spr = ButtonSprite::create(easingStr.c_str());
		spr->setScale(0.7f);

		m_fields->m_easingButtonTwo = CCMenuItemSpriteExtra::create(spr, this, menu_selector(mySetupTriggerPopup::onSelect));
		m_fields->m_easingButtonTwo->setTag(1);
		auto worldPos = convertToWorldSpace({379.5f, 84.f});
		auto localPos = m_buttonMenu->convertToNodeSpace(worldPos);
		m_fields->m_easingButtonTwo->setPosition(localPos);
		m_fields->m_easingButtonTwo->setVisible(getValue(261));

		m_buttonMenu->addChild(m_fields->m_easingButtonTwo);
	}

	m_buttonMenu->addChild(m_fields->m_easingButton);
}

$override
void mySetupTriggerPopup::onPage(CCObject* sender) {
	SetupTriggerPopup::onPage(sender);

	if (!m_fields->m_easingButton) return;
	hideEasingArrowButtons();
	m_fields->m_easingButton->setVisible(!m_fields->m_easingButton->isVisible());

	if (m_easingLabel) m_easingLabel->setVisible(false);

	static const std::unordered_set<int> triggersWithTwoEasings = {
		3006, 3007, 3008
	};

	int objID;
	if (m_gameObjects) {
		objID = typeinfo_cast<EffectGameObject*>(m_gameObjects->objectAtIndex(0))->m_objectID;
	} else {
		objID = m_gameObject->m_objectID;
	}

	if(triggersWithTwoEasings.count(objID) && getValue(261)) {
		hideEasingLabel();
		m_fields->m_easingButtonTwo->setVisible(!m_fields->m_easingButtonTwo->isVisible());
	}

	updateEasingRateButtonVisibility();
}


$override
void mySetupTriggerPopup::valueChanged(int property, float value) {	
	SetupTriggerPopup::valueChanged(property, value);

	if (property == 261) {
		hideEasingArrowButtons();
		hideEasingLabel();
		m_fields->m_easingButtonTwo->setVisible(value);
	}

	if (property == 30 || property == 248 || property == 242) {
		updateEasingRateButtonVisibility();
		hideEasingLabel();
	}
}

void mySetupTriggerPopup::onSelect(CCObject* sender) {
	auto popup = EasingsPopup::create(this, sender->getTag());
	popup->show();
}

void mySetupTriggerPopup::hideEasingArrowButtons() {
	static const std::unordered_set<int> triggersWith0_7ScaleEasingArrows = {
		1916, 2910, 2912, 2919, 2920, 2922, 2924, 2999
	};

	int objID;
	if (m_gameObjects) {
		// i know that if there are multiple triggers selected they must have the same id for easing to be an option
		objID = typeinfo_cast<EffectGameObject*>(m_gameObjects->objectAtIndex(0))->m_objectID;
	} else {
		objID = m_gameObject->m_objectID;
	}

	for (auto node : CCArrayExt<CCNode*>(m_buttonMenu->getChildren())) {
		auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(node);
		if (!btn) continue;

		auto sprite = typeinfo_cast<CCSprite*>(btn->getNormalImage());
		if (!sprite) continue;
		// i dont want to hide the page change arrows which all use 0.7 scale, and i know that all triggers with 0.7 scale easing arrows dont have multiple pages
		if (sprite->getScale() == 0.7f && !triggersWith0_7ScaleEasingArrows.count(objID)) continue; 

		auto texture = sprite->getTexture();
		if (!texture) continue;

		const auto rect = sprite->getTextureRect();
		auto* cachedFrames = CCSpriteFrameCache::sharedSpriteFrameCache()->m_pSpriteFrames;

		for (auto [key, frame] : CCDictionaryExt<std::string, CCSpriteFrame*>(cachedFrames)) {
			if (frame->getTexture() == texture && frame->getRect() == rect) {
				if (key == "GJ_arrow_01_001.png") {
					btn->setVisible(false);
					break;
				}
			}
		}
	}
}

// for triggers that dont have a field to access it
void mySetupTriggerPopup::updateEasingRateButtonVisibility() {
	if (m_page != 0) return;	
	
	int objID;
	if (m_gameObjects) {
		objID = typeinfo_cast<EffectGameObject*>(m_gameObjects->objectAtIndex(0))->m_objectID;
	} else {
		objID = m_gameObject->m_objectID;
	}

	CCMenuItemSpriteExtra* easingRateButton = nullptr;
	if (objID == 1916) {
		easingRateButton = typeinfo_cast<SetupCameraOffsetTrigger*>(this)->m_cameraEasingRateButton;
	} else if (objID == 2999) {
		easingRateButton = typeinfo_cast<SetupMGTrigger*>(this)->m_mgEasingRateButton;
	} else {
		easingRateButton = m_easingRateButton;
	}

	auto currentEasingID = getValue(30);
	if (easingRateButton) {
		easingRateButton->setVisible(currentEasingID >= 1.0f && currentEasingID <= 6.0f);
		return;
	}

	for (auto node : CCArrayExt<CCNode*>(m_buttonMenu->getChildren())) {
		auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(node);
		if (!btn) continue;

		auto sprite = typeinfo_cast<CCSprite*>(btn->getNormalImage());
		if (!sprite) continue;

		auto texture = sprite->getTexture();
		if (!texture) continue;

		const auto rect = sprite->getTextureRect();
		auto* cachedFrames = CCSpriteFrameCache::sharedSpriteFrameCache()->m_pSpriteFrames;

		bool isEasingTwo = false;
		auto currentEasingTwoID = getValue(248);

		float easing1;
		if (SetupAreaMoveTriggerPopup* areaMovePopup = typeinfo_cast<SetupAreaMoveTriggerPopup*>(this)) {
			easing1 = areaMovePopup->getValue(242);
		} else if (SetupEnterEffectPopup* enterEffectTrigger = typeinfo_cast<SetupEnterEffectPopup*>(this)) {
			easing1 = enterEffectTrigger->getValue(242);
		} else {
			easing1 = getValue(30);
		}
		float easing2 = getValue(248);
		bool easingTwoEnabled = getValue(261);

		auto* targetFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("GJ_colorBtn_001.png");
		if (!targetFrame) return;

		bool foundFirst = false;

		for (auto node : CCArrayExt<CCNode*>(m_buttonMenu->getChildren())) {
			auto* btn = typeinfo_cast<CCMenuItemSpriteExtra*>(node);
			if (!btn) continue;

			auto* sprite = typeinfo_cast<CCSprite*>(btn->getNormalImage());
			if (!sprite) continue;

			if (sprite->getTexture() == targetFrame->getTexture() &&
				sprite->getTextureRect() == targetFrame->getRect()) {

				if (!foundFirst) {
					btn->setVisible(easing1 >= 1.0f && easing1 <= 6.0f);
					foundFirst = true;

					if (!easingTwoEnabled) break;
				} else {
					btn->setVisible(easing2 >= 1.0f && easing2 <= 6.0f);
					break;
				}
			}
		}
	}
}

// for objects that dont have a field to access it
void mySetupTriggerPopup::hideEasingLabel() {
	for (auto node : CCArrayExt<CCNode*>(this->m_mainLayer->getChildren())) {
		auto label = typeinfo_cast<CCLabelBMFont*>(node);
		if (!label) continue;
		std::string labelText = label->getString();
		std::transform(labelText.begin(), labelText.end(), labelText.begin(), ::tolower);
		for (const auto& easing : easingTypes) {
			std::string easingLower = easing;
			std::transform(easingLower.begin(), easingLower.end(), easingLower.begin(), ::tolower);
			if (labelText == easingLower) {
				label->setVisible(false);
				break;
			}
		}
	}
}

