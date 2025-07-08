#pragma once

#include <Geode/DefaultInclude.hpp>
#include <cocos2d.h>
#include <Geode/ui/ScrollLayer.hpp>


class PackNode;

using namespace geode::prelude;

class TexturePackSelector : public Popup<> {
protected:

	ScrollLayer* m_mainList = nullptr;
	CCLabelBMFont* m_infoLabel = nullptr;
	EventListener<web::WebTask> m_downloadTP;
	EventListener<web::WebTask> m_listListener;
	EventListener<web::WebTask> m_downloadListener;
	std::string jsonString;
	std::vector<std::string> versions;
	std::vector<std::string> downloadsCounter;
	std::string empty = "empty";
	CCScale9Sprite* m_statusBG;
	Slider* m_progressBar;
	CCMenuItemSpriteExtra* reloadBtn = nullptr;

	std::vector<std::string> imageNames = {
		"DMv4_Metal_Btn.png"_spr,
		"DMv4_Blood_Btn.png"_spr,
		"DMv4_Earth_Btn.png"_spr,
		"DMv4_Fire_Btn.png"_spr,
		"DMv4_Ice_Btn.png"_spr,
		"DMv4_Lava_Btn.png"_spr,
		"DMv4_Light_Btn.png"_spr,
		"DMv4_Moon_Btn.png"_spr,
		"DMv4_Poison_Btn.png"_spr,
		"DMv4_Shadow_Btn.png"_spr,
		"DMv4_Soul_Btn.png"_spr,
		"DMv4_Star_Btn.png"_spr,
		"DMv4_Bonus_Btn.png"_spr
	};
	std::vector<std::string> modes = {
		"METAL", "BLOOD", "EARTH", "FIRE", "ICE",
		"LAVA", "LIGHT", "MOON", "POISON", "SHADOW",
		"SOUL", "STAR","BONUS"
	};
	bool setup() override { return true; }
	bool init() override;

public:
    static TexturePackSelector* create();
	void reloadData();
	void reloadDownloadCounts();
	void loadEnter();
	void reloadDataCallBack(CCObject* sender);
	void onSelectTP(CCObject* sender);
	void onDownloadTP(std::string id);
	void onBack(CCObject* sender);
	void showSuccessNotification();
};