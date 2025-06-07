#pragma once

#include <Geode/modify/SetupTriggerPopup.hpp>

using namespace geode::prelude;

inline const std::vector<std::string> easingTypes = {
	"None",
	"Ease In Out",
	"Ease In",
	"Ease Out",
	"Elastic In Out",
	"Elastic In",
	"Elastic Out",
	"Bounce In Out",
	"Bounce In",
	"Bounce Out",
	"Exponential In Out",
	"Exponential In",
	"Exponential Out",
	"Sine In Out",
	"Sine In",
	"Sine Out",
	"Back In Out",
	"Back In",
	"Back Out"
};

inline const std::vector<std::string> parentEasingTypes = {
	"None",
	"Ease",
	"Elastic",
	"Bounce",
	"Exponential",
	"Sine",
	"Back"
};

inline const std::vector<std::string> easingSubTypes = {
	"InOut",
	"In",
	"Out"
};

class $modify(mySetupTriggerPopup, SetupTriggerPopup) {
	struct Fields {
		CCMenuItemSpriteExtra* m_easingButton;
		CCMenuItemSpriteExtra* m_easingButtonTwo;
	};

	$override
	void show();
	
	$override
	void onPage(CCObject* sender);

	$override
	void valueChanged(int property, float value);

	void onSelect(CCObject* sender);
	void hideEasingArrowButtons();
	void updateEasingRateButtonVisibility();
	void hideEasingLabel();
};