
#include "Geode/binding/CCMenuItemSpriteExtra.hpp"
#include <Geode/utils/web.hpp>
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "TexturePackCell.hpp"

using namespace geode::prelude;

// small wrapper with precalculated sizes to make ui easier
struct PopupLayout {
	cocos2d::CCSize winSize, popupSize;
	float left, right, bottom, top;

	cocos2d::CCSize center, centerLeft, centerTop, centerRight, centerBottom;
	cocos2d::CCSize bottomLeft, topLeft, bottomRight, topRight;

	cocos2d::CCPoint fromTop(float y);
	cocos2d::CCPoint fromTop(cocos2d::CCSize off);

	cocos2d::CCPoint fromBottom(float y);
	cocos2d::CCPoint fromBottom(cocos2d::CCSize off);

	cocos2d::CCPoint fromCenter(cocos2d::CCSize off);
	cocos2d::CCPoint fromCenter(float x, float y);

	cocos2d::CCPoint fromBottomRight(cocos2d::CCSize off);
	cocos2d::CCPoint fromBottomRight(float x, float y);

	cocos2d::CCPoint fromTopRight(cocos2d::CCSize off);
	cocos2d::CCPoint fromTopRight(float x, float y);

	cocos2d::CCPoint fromBottomLeft(cocos2d::CCSize off);
	cocos2d::CCPoint fromBottomLeft(float x, float y);

	cocos2d::CCPoint fromTopLeft(cocos2d::CCSize off);
	cocos2d::CCPoint fromTopLeft(float x, float y);
};

static PopupLayout popupLayoutWith(const CCSize& popupSize, bool useWinSize) {
	PopupLayout layout;

	layout.winSize = CCDirector::get()->getWinSize();
	layout.popupSize = popupSize;

	if (useWinSize) {
		layout.center = CCSize{ layout.winSize.width / 2, layout.winSize.height / 2 };
	}
	else {
		layout.center = CCSize{ layout.popupSize.width / 2, layout.popupSize.height / 2 };
	}

	layout.left = layout.center.width - popupSize.width / 2;
	layout.bottom = layout.center.height - popupSize.height / 2;
	layout.right = layout.center.width + popupSize.width / 2;
	layout.top = layout.center.height + popupSize.height / 2;

	layout.centerLeft = CCSize{ layout.left, layout.center.height };
	layout.centerRight = CCSize{ layout.right, layout.center.height };
	layout.centerTop = CCSize{ layout.center.width, layout.top };
	layout.centerBottom = CCSize{ layout.center.width, layout.bottom };

	layout.bottomLeft = CCSize{ layout.left, layout.bottom };
	layout.topLeft = CCSize{ layout.left, layout.top };
	layout.bottomRight = CCSize{ layout.right, layout.bottom };
	layout.topRight = CCSize{ layout.right, layout.top };

	return layout;
}

class TexturePackSelector : public geode::Popup<std::string const&> {
protected:

	EventListener<web::WebTask> m_downloadTP;

public:
	bool setup(std::string const& value) {

		this->setTitle("DarkMode TexturePack selector");

		auto director = CCDirector::sharedDirector();
		auto winSize = director->getWinSize();
		CCArray* texturepacks = CCArray::create();

	
		// Lista de nombres de imágenes
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

		// Lista de nombres de modos
		std::vector<std::string> modes = {
			"METAL", "BLOOD", "EARTH", "FIRE", "ICE",
			"LAVA", "LIGHT", "MOON", "POISON", "SHADOW",
			"SOUL", "STAR"
		};


		const CCSize cellSize = { 415.0f, 72.0f };


#define TPS_URL "https://raw.githubusercontent.com/iArtie/DarkModeV4-Index/refs/heads/main/texturepacks.json"


		std::string jsonString;



		static std::optional<web::WebTask> task = std::nullopt;
		task = web::WebRequest().get(TPS_URL).map([this, &jsonString](web::WebResponse* res) {
			if (!res->ok()) {
				queueInMainThread([] { Notification::create("DarkMode Texture Pack index Load Failed", NotificationIcon::Error)->show(); });
				task = std::nullopt;
				return *res;
			}

			if (!res->json().isOk()) {
				log::error("Failed to parse developer badges: {}", res->json().unwrapErr());
				return *res;
			}




			jsonString = res->json().unwrapOr("").dump();

			// Vector de modos
			std::vector<std::string> modes = {
				"METAL", "BLOOD", "EARTH", "FIRE", "ICE",
				"LAVA", "LIGHT", "MOON", "POISON", "SHADOW",
				"SOUL", "STAR"
			};

			std::string empty = "empty";

			std::string resultString;

			for (const auto& mode : modes) {

				std::string modeStr = "\"" + mode + "\"";
				size_t modePos = jsonString.find(modeStr);

				if (modePos != std::string::npos) {

					size_t modeStartPos = jsonString.find("{", modePos);
					size_t modeEndPos = jsonString.find("}", modeStartPos);

					if (modeStartPos != std::string::npos && modeEndPos != std::string::npos) {
						std::string modeContent = jsonString.substr(modeStartPos + 1, modeEndPos - modeStartPos - 1);


						size_t latestPos = modeContent.find("\"Lastest\"");

						if (latestPos != std::string::npos) {

							size_t versionPos = modeContent.find("\"version\":", latestPos);
							size_t downloadPos = modeContent.find("\"download\":", latestPos);

							if (versionPos != std::string::npos && downloadPos != std::string::npos) {
								size_t versionStart = modeContent.find("\"", versionPos + 11);
								size_t versionEnd = modeContent.find("\"", versionStart + 1);
								size_t downloadStart = modeContent.find("\"", downloadPos + 11);
								size_t downloadEnd = modeContent.find("\"", downloadStart + 1);

								if (versionStart != std::string::npos && versionEnd != std::string::npos &&
									downloadStart != std::string::npos && downloadEnd != std::string::npos) {
									std::string version = modeContent.substr(versionStart + 1, versionEnd - versionStart - 1);
									std::string downloadLink = modeContent.substr(downloadStart + 1, downloadEnd - downloadStart - 1);

									Mod::get()->setSavedValue(mode + "_version", version);
									Mod::get()->setSavedValue(mode + "_link", downloadLink);

								}
							}
						}
						else {

							Mod::get()->setSavedValue(mode + "_version", empty);
							Mod::get()->setSavedValue(mode + "_link", empty);

						}
					}
					else {

					}
				}
				else {
				}
			}

			task = std::nullopt;
			return *res;
			});



		std::vector<std::string> versions;


		for (const auto& mode : modes) {

			std::string versionKey = mode + "_version";
			const char* versionKeyCStr = versionKey.c_str();


			std::string version = Mod::get()->getSavedValue<std::string>(versionKeyCStr);
		


			if (version == "empty" || version.empty()) {
				versions.push_back("not available");
			}
			else {
				versions.push_back("v" + version);
			}
		}

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


		auto* listView = ListView::create(texturepacks, 72.0F, 410.0f, 200.0f);

		auto listLayer = GJListLayer::create(listView, "", { 191, 114, 62, 255 }, 410.f, 200.f, 1);
	
		listLayer->setPosition(winSize / 2 - listLayer->getScaledContentSize() / 2 - ccp(0, 5)); //Center position sucks on GJListLayer

	
		listLayer->setScale(0.1f);

		
		
		auto scaleTo = CCScaleTo::create(0.3f, 1.0f); 
		auto easeBackOut = CCEaseBackOut::create(scaleTo);

	
		listLayer->runAction(easeBackOut);

		addChild(listLayer);

		auto scrollbar = Scrollbar::create(listView->m_tableView);

		listView->m_tableView->setContentSize({ 415,200 });
		scrollbar->setPosition({ (winSize.width / 2) + (listLayer->getScaledContentSize().width / 2) + 215, winSize.height / 2 });
		

		scrollbar->setScale(0.1f);
		auto scaleTo2 = CCScaleTo::create(0.3f, 1.0f);
		auto easeBackOut2 = CCEaseBackOut::create(scaleTo2);


		scrollbar->runAction(easeBackOut2);
		addChild(scrollbar);


		return true;
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
