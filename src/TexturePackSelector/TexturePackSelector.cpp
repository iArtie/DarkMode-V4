
#include "Geode/binding/CCMenuItemSpriteExtra.hpp"
#include <Geode/utils/web.hpp>
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "TexturePackCell.hpp"

using namespace geode::prelude;

class TexturePackSelector : public geode::Popup<std::string const&> {
protected:

	EventListener<web::WebTask> m_downloadTP;
	EventListener<web::WebTask> m_listListener;
	std::string jsonString;
	std::vector<std::string> versions;
	std::string empty = "empty";
	ListView* listView;
	GJListLayer* listLayer;

	Scrollbar* scrollbar;
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
		"DMv4_Star_Btn.png"_spr
	};
	std::vector<std::string> modes = {
		"METAL", "BLOOD", "EARTH", "FIRE", "ICE",
		"LAVA", "LIGHT", "MOON", "POISON", "SHADOW",
		"SOUL", "STAR"
	};
	bool m_finishedLoading = false;

public:

	bool init() {
		if (!CCLayer::init()) {
			return false;
		}

		// Llamar a reloadData() cuando se inicializa la pantalla
		reloadData();

		return true;
	}

	bool setup(std::string const& value) {

		this->setTitle("DarkMode TexturePack selector");

		
		this->runAction(CCSequence::create(
			CCDelayTime::create(1.5),
			CCCallFunc::create(this, callfunc_selector(TexturePackSelector::reloadData)), // Utilizamos CCCallFunc para llamar a la función runParticle
			nullptr
		));
		auto director = CCDirector::sharedDirector();
		auto winSize = director->getWinSize();
		CCArray* texturepacks = CCArray::create();

		
		reloadData();
	
		auto loadingCircle = LoadingCircle::create();
		
		loadingCircle->setParentLayer(this);
		loadingCircle->show();

		loadingCircle->setID("selector-loading-circle");
		this->runAction(CCSequence::create(
			CCDelayTime::create(1.0f), 
			CCCallFunc::create(this, callfunc_selector(TexturePackSelector::loadEnter)),
			nullptr
		));

		

		const CCSize cellSize = { 415.0f, 72.0f };
	
			for (int i = 0; i < 12; ++i) {

				auto texture = TexturePackCell::create({
						cellSize,
						(imageNames[i]).c_str(),
						modes[i].c_str(),
						versions[i].c_str(),
						this,
						menu_selector(TexturePackSelector::onSelectTP)
					});

				texture->button->setID(modes[i].c_str());
				texturepacks->addObject(texture);
			}
		
			//reload menu
			auto reloadMenu = CCMenu::create();
			reloadMenu->setPosition({ 0, 0 });
			auto reloadBtnSprite = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
			auto reloadBtn = CCMenuItemSpriteExtra::create(reloadBtnSprite, this, menu_selector(TexturePackSelector::reloadDataCallBack));
			reloadBtn->setPosition({ 30.f, 30.f });
			reloadMenu->addChild(reloadBtn);
			reloadMenu->setID("reload-menu");
			m_mainLayer->addChild(reloadMenu);

		listView = ListView::create(texturepacks, 72.0F, 410.0f, 200.0f);

		listLayer = GJListLayer::create(listView, "", { 191, 114, 62, 255 }, 410.f, 200.f, 1);
		listLayer->setScale(0.9f);
		listLayer->setPosition(winSize / 2 - listLayer->getScaledContentSize() / 2 - ccp(25, 5)); //Center position sucks on GJListLayer

	
		listLayer->setScale(0.1f);

		
		
		auto scaleTo = CCScaleTo::create(0.3f, 0.9f); 
		auto easeBackOut = CCEaseBackOut::create(scaleTo);

	
		listLayer->runAction(easeBackOut);

		addChild(listLayer,2);

		scrollbar = Scrollbar::create(listView->m_tableView);

		listView->m_tableView->setContentSize({ 415,190 });
		scrollbar->setPosition({ (winSize.width / 2) + (listLayer->getScaledContentSize().width / 2) + 194, (winSize.height / 2)+5 });
		

		scrollbar->setScale(0.1f);

	
		auto scaleTo2 = CCScaleTo::create(0.3f, 1.0f);
		auto easeBackOut2 = CCEaseBackOut::create(scaleTo2);

		reloadData();
		scrollbar->runAction(easeBackOut2);
		addChild(scrollbar,1);
		this->removeChild(scrollbar);

		listLayer->removeChildByID("list-view"); 

		return true;
	}

	void reloadData()
	{
		std::string dataURL;

		dataURL = "https://raw.githubusercontent.com/iArtie/DarkModeV4-Index/refs/heads/main/texturepacks.json";




		// List listener (Thanks minemaker0430)
		m_listListener.bind([&](web::WebTask::Event* e) {
			if (auto res = e->getValue()) {
				if (res->ok()) {

					// Unwrap the JSON into a matjson::Value object
					matjson::Value jsonData = res->json().unwrapOrDefault();
					Mod::get()->setSavedValue("cached-json", jsonData);

					m_finishedLoading = true;

				}
				else if (e->isCancelled()) {
					log::info("Cancelled request.");

					m_finishedLoading = true;
				}
			}
			});

		// Make the list request
		auto listReq = web::WebRequest();
		m_listListener.setFilter(listReq.get(dataURL));

		versions.clear();
		auto jsonData = Mod::get()->getSavedValue<matjson::Value>("cached-json");
		// Iterate through the modes
		for (const auto& mode : modes) {

			// Check if the mode exists in the JSON data
			if (jsonData.contains(mode)) {
				auto modeData = jsonData[mode];

				// Check if the "Lastest" key exists and extract data
				if (modeData["Lastest"].contains("version")) {
					auto latestData = modeData["Lastest"];

					// Extract version and download link
					std::string version = latestData["version"].as<std::string>().unwrapOrDefault();


					std::string downloadLink = latestData["download"].as<std::string>().unwrapOrDefault();


					// Save extracted data to persistent storage
					Mod::get()->setSavedValue(mode + "_version", version);
					Mod::get()->setSavedValue(mode + "_link", downloadLink);

					// Add version to the versions list
					if (version == "empty" || version.empty()) {
						versions.push_back("not available");
					}
					else {
						versions.push_back("v" + version);
					}
				}
				else {
					// If "Lastest" key is not found, set values to "empty"
					Mod::get()->setSavedValue(mode + "_version", empty);
					Mod::get()->setSavedValue(mode + "_link", empty);

					// Add "not available" to versions list
					versions.push_back("not available");
				}
			}
			else {
				// If the mode doesn't exist in the JSON, set values to "empty"
				Mod::get()->setSavedValue(mode + "_version", empty);
				Mod::get()->setSavedValue(mode + "_link", empty);

				// Add "not available" to versions list
				versions.push_back("not available");
			}
		}

	}

	void loadEnter()
	{
		reloadData();

		auto director = CCDirector::sharedDirector();
		auto winSize = director->getWinSize();
		CCArray* texturepacks = CCArray::create();
		const CCSize cellSize = { 415.0f, 72.0f };

		for (int i = 0; i < 12; ++i) {

			auto texture = TexturePackCell::create({
					cellSize,
					(imageNames[i]).c_str(),
					modes[i].c_str(),
					versions[i].c_str(),
					this,
					menu_selector(TexturePackSelector::onSelectTP)
				});

			texture->button->setID(modes[i].c_str());
			texturepacks->addObject(texture);
		}
		listView = ListView::create(texturepacks, 72.0F, 410.0f, 200.0f);
		listView->setID("list-view");
		/*this->removeChild(scrollbar);

		listLayer->removeChildByID("list-view");*/


		
		listLayer->addChild(listView);


		listView->m_tableView->setContentSize({ 415,190 });
		scrollbar = Scrollbar::create(listView->m_tableView);


		this->removeChildByID("selector-loading-circle");

		this->addChild(scrollbar, 1);
		scrollbar->setPosition({ (winSize.width / 2) + (listLayer->getScaledContentSize().width / 2) + 30, (winSize.height / 2) + 5 });
		Notification::create("Index Updated!", CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"))->show();

	}
	void reloadDataCallBack(cocos2d::CCObject* sender)
	{
		reloadData();

		auto director = CCDirector::sharedDirector();
		auto winSize = director->getWinSize();
		CCArray* texturepacks = CCArray::create();
		const CCSize cellSize = { 415.0f, 72.0f };

		for (int i = 0; i < 12; ++i) {

			auto texture = TexturePackCell::create({
					cellSize,
					(imageNames[i]).c_str(),
					modes[i].c_str(),
					versions[i].c_str(),
					this,
					menu_selector(TexturePackSelector::onSelectTP)
				});

			texture->button->setID(modes[i].c_str());
			texturepacks->addObject(texture);
		}
		listView = ListView::create(texturepacks, 72.0F, 410.0f, 200.0f);
		listView->setID("list-view");
		this->removeChild(scrollbar);

		listLayer->removeChildByID("list-view");
		listLayer->addChild(listView);


		listView->m_tableView->setContentSize({ 415,190 });
		scrollbar = Scrollbar::create(listView->m_tableView);
	
		this->addChild(scrollbar,1);
		scrollbar->setPosition({ (winSize.width / 2) + (listLayer->getScaledContentSize().width / 2) + 30, (winSize.height / 2) + 5 });
		Notification::create("Index Updated!", CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"))->show();

	}
	static TexturePackSelector* create(std::string const& text) {
		auto ret = new TexturePackSelector();



		if (ret->initAnchored(520.f, 285.f, text)) {
			ret->autorelease();
			return ret;
		}

		delete ret;
		return nullptr;
	}
	void getPopupLayoutAnchored(const CCSize& popupSize) {
		return ;
	}

	void onSelectTP(CCObject* sender)
	{
		auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(sender);

		geode::createQuickPopup(
			"Download Texture Pack",
			"Are you sure you want to download the " + button->getID() + " version? ",
			"No", "Yes",      // buttons
			[this, button](auto, bool btn2) {
				if (btn2) {
					onDownloadTP(button->getID());
				}
			}

		);


	}

	//static TexturePackSelector* getInstance() {
	//	static auto instance = TexturePackSelector::create("some_text");
	//	return instance;
	//}

	void onDownloadTP(std::string id)
	{


		std::string fileName = fmt::format("{}/packs/{}.zip", Loader::get()->getLoadedMod("geode.texture-loader")->getConfigDir(), id);


		std::string downloadLink = Mod::get()->getSavedValue<std::string>(id + "_link");

		if (downloadLink == "empty")
		{
			FLAlertLayer::create(
				"Oh no",
				"This texture pack is not available! Select another texture pack or wait for it to become available",
				"OK"
			)->show();
		}
		else
		{

			Notification::create("Downloading...", CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png"))->show();
			
			if (std::filesystem::remove_all(fmt::format("{}/packs/{}", Loader::get()->getInstalledMod("geode.texture-loader")->getConfigDir(), id)))
			{
				std::filesystem::remove_all(fmt::format("{}/packs/{}", Loader::get()->getInstalledMod("geode.texture-loader")->getConfigDir(), id));
			}; //delete old folder

			m_downloadTP.bind([this, downloadLink, id](web::WebTask::Event* e) {
				if (web::WebResponse* res = e->getValue()) {

					if (res->into(fmt::format("{}/packs/{}.zip", Loader::get()->getInstalledMod("geode.texture-loader")->getConfigDir(), id))) {



						Notification::create("Download Successful", CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"))->show();

					}
					else {
						// Si falla la descarga
						Notification::create("Download Failed", CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"))->show();
						std::filesystem::remove(fmt::format("{}/packs/{}.zip", Loader::get()->getInstalledMod("geode.texture-loader")->getConfigDir(), id));
					
					}
				}

				});
			auto req = web::WebRequest();
			m_downloadTP.setFilter(req.get(downloadLink));

		}


	}

};
