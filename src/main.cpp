#include <map>
#include <tuple>
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CCScale9Sprite.hpp>
#include <Geode/modify/MenuGameLayer.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>
#include <Geode/modify/CCLayerColor.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/CCNode.hpp>
#include <Geode/modify/AchievementNotifier.hpp>
#include "TexturePackSelector/TexturePackSelector.hpp"
#include "TexturePackSelector/TexturePackSelector.cpp"
#include "extras/utils.hpp"


using namespace geode::prelude;


void updateGroundColorM(CCSpriteBatchNode* batch, const cocos2d::ccColor3B& color) {
	if (!batch) return;

	for (auto child : batch->getChildrenExt<CCNode>()) {
		if (auto sprite = typeinfo_cast<CCSprite*>(child)) {
			sprite->setColor(color);

			for (auto spriteChild : sprite->getChildrenExt<CCNode>()) {
				if (auto childSprite = typeinfo_cast<CCSprite*>(spriteChild)) {
					childSprite->setColor(color);
				}
			}
		}
	}
}

void onSceneSwitch(CCScene * scene) {
	if (scene)
	{
		if (scene->getChildrenCount() > 0)
		{
			if (auto layer = static_cast<CCLayer*>(scene->getChildren()->objectAtIndex(0)); layer->getChildrenCount() > 0)
			{
				if (scene->getChildByType<LevelEditorLayer>(0) || scene->getChildByType<LoadingLayer>(0))
					return;

				layer->sortAllChildren();

				if (auto sprite = typeinfo_cast<CCSprite*>(layer->getChildren()->objectAtIndex(0)))
				{
					ccColor3B spriteColor = sprite->getColor();


					if ((spriteColor.r == 164 && spriteColor.g == 0 && spriteColor.b == 255) || //for betterinfo
						(spriteColor.r == 37 && spriteColor.g == 50 && spriteColor.b == 167) || //for globed
						(spriteColor.r == 0 && spriteColor.g == 101 && spriteColor.b == 253) || //for gdutils
						spriteColor == ccc3(255, 255, 255))
					{
						sprite->setColor({ 64, 64, 64 });
					}
				}
			}
		}
	}
}

class $modify(AppDelegate)

{
	#ifndef GEODE_IS_IOS
	virtual void willSwitchToScene(CCScene * scene)
	{
		AppDelegate::willSwitchToScene(scene);
		onSceneSwitch(scene);
		}
	#endif
};

class $modify(DMAchievementNotifier, AchievementNotifier) {
	#ifdef GEODE_IS_IOS
	void willSwitchToScene(CCScene * scene) {
		AchievementNotifier::willSwitchToScene(scene);
		onSceneSwitch(scene);
	}
	#endif
};


class GlobedListCell : public cocos2d::CCLayer, public CCNode {}; //List cell recreate for globed cells


class $modify(CCLayerColor)
{


	void draw() //Idk why but init doesnt work
	{
		//new code of something stuff
		static const std::map<ccColor3B, ccColor3B, ColorComparator> colorMap = {

			{{161, 88, 44}, {48, 48, 48}}, //For light brown cells
			{{40, 53, 119}, {48, 48, 48}},
			{{168, 85, 44}, {48, 48, 48}},
			{{50, 50, 100}, {48, 48, 48}},
			{{156, 85, 42}, {48, 48, 48}},
			{{0, 0, 83}, {48, 48, 48}},


			{{194, 114, 62}, {30, 30, 30}}, //For dark brown cells
			{{191, 114, 62}, {30, 30, 30}},
			{{51, 68, 153}, {30, 30, 30}},
			{{114, 63, 31}, {30, 30, 30}},
			{{63, 63, 127}, {30, 30, 30}},
			{{144, 79, 39}, {30, 30, 30}},
			{{0, 13, 98}, {30, 30, 30}}
		};


		auto it = colorMap.find(this->getColor());
		auto parent = this->getParent();

		if (it != colorMap.end())
			this->setColor(it->second);

		if (Mod::get()->getSettingValue<bool>("transparent-lists") && this->getOpacity() != 10)
		{
			bool isValidCell =
				typeinfo_cast<LevelCell*>(parent) ||
				typeinfo_cast<GJScoreCell*>(parent) ||
				typeinfo_cast<LevelListCell*>(parent) ||
				typeinfo_cast<MapPackCell*>(parent) ||
				dynamic_cast<geode::GenericListCell*>(parent) != nullptr;

			bool isListLayer = typeinfo_cast<GJListLayer*>(this);
			bool hasListView = this->getChildByID("list-view") != nullptr;

			auto grandparent = parent ? parent->getParent() : nullptr;
			auto loadingCircle = parent ? parent->getChildByType<LoadingCircle*>(0) : nullptr;
			bool hasVisibleLoadingCircle = loadingCircle && loadingCircle->isVisible();
			bool isGeodeListView = dynamic_cast<geode::ListView*>(this) != nullptr;
			bool isBackgroundLayer = isListLayer && this->getID() == "background";

			if (!isBackgroundLayer && (isValidCell || (isListLayer && (hasListView || hasVisibleLoadingCircle)) || isGeodeListView)) {
				if (typeinfo_cast<DailyLevelNode*>(grandparent)) {
					auto size = this->getContentSize();
					this->setVisible(size.height == 90 && size.width == 160);
				}
				this->setOpacity(50);
			}
		}


		CCLayerColor::draw();
	}
};

class $modify(CCScale9Sprite)
{
	void visit()
	{
		// new code of something stuff
		static const std::map<ccColor3B, ccColor3B, ColorComparator> colorMap = {
			{{0, 56, 141}, {20, 20, 20}},
	        {{130, 64, 33}, {20, 20, 20}},
	        {{0, 46, 117}, {20, 20, 20}},
	        {{0, 36, 91}, {20, 20, 20}},
	        {{0, 31, 79}, {20, 20, 20}},
	        {{123, 60, 31}, {15, 15, 15}},
			{{93, 52, 31}, {20, 20, 20}},
	        {{108, 60, 36}, {15, 15, 15}},
	        {{0, 39, 98}, {10, 10, 10}},
	        {{76, 42, 25}, {25, 25, 25}},
	        {{130, 64, 32}, {20, 20, 20}},
	        {{32, 49, 130}, {15, 15, 15}},
	        {{191, 114, 62}, {20, 20, 20}},
	        {{176, 92, 52}, {20, 20, 20}}
		};

		auto it = colorMap.find(this->getColor());
		if (it != colorMap.end()) {
			this->setColor(it->second);
		}


		CCScale9Sprite::visit();
	}
};

class $modify(MenuGameLayer) {
    void update(float dt) {
        MenuGameLayer::update(dt);

        for (auto child : this->getChildrenExt<CCNode>()) {

            if (auto sprite = typeinfo_cast<CCSprite*>(child)) {
                if (sprite->getContentSize().height == 512.f) {
                    sprite->setColor({40, 40, 40});
                }
            }

            if (auto ground = typeinfo_cast<GJGroundLayer*>(child)) {
                if (auto ground1 = typeinfo_cast<CCSpriteBatchNode*>(ground->getChildByID("ground-sprites")))
                    updateGroundColorM(ground1, {40, 40, 40});

                if (auto ground2 = typeinfo_cast<CCSpriteBatchNode*>(ground->getChildByID("ground-sprites-2")))
                    updateGroundColorM(ground2, {40, 40, 40});
            }
        }
    }
};

class $modify(LevelInfoLayer)
{
	void onPlay(cocos2d::CCObject * sender) {
		LevelInfoLayer::onPlay(sender);

		if (!m_playSprite) return;

		for (int i = 0; i < 3; i++)
			if (auto s = typeinfo_cast<CCSprite*>(m_playSprite->getChildByIndex(i)))
				s->setColor(i == 2 ? ccColor3B{ 20, 20, 20 } : ccColor3B{ 0, 0, 0 });

	}
};

class $modify(LevelSelectLayer) {
	void scrollLayerMoved(CCPoint offset) {
		LevelSelectLayer::scrollLayerMoved(offset);

		
		for (auto child : this->getChildrenExt<CCNode>()) {
		
			if (auto groundLayer = typeinfo_cast<GJGroundLayer*>(child)) {

				
				auto ground1 = groundLayer->getChildByID("ground-sprites");
				auto ground2 = groundLayer->getChildByID("ground-sprites-2");

			
				if (auto batch1 = typeinfo_cast<CCSpriteBatchNode*>(ground1))
					updateGroundColorM(batch1, { 40, 40, 40 });

				if (auto batch2 = typeinfo_cast<CCSpriteBatchNode*>(ground2))
					updateGroundColorM(batch2, { 40, 40, 40 });
			}
		}
	}
};

class $modify(LevelSearchLayer)
{
	bool init(int a1)
	{
		if (!LevelSearchLayer::init(a1)) return false;

		this->m_searchInput->setLabelNormalColor({ 255,255,255 });
		return true;
	}
};

class $modify(DarkModeMenuLayer, MenuLayer) {
	struct Fields {
		CCSprite* m_darkmodeButton = nullptr;
		bool m_hasCheckedUpdates = false;
	};

	bool init() {
		if (!MenuLayer::init()) return false;

		if (Mod::get()->getSettingValue<bool>("Disable-Happy-Textures") != true) {
			if (auto HT = Loader::get()->getLoadedMod("alphalaneous.happy_textures"))
				HT->setSettingValue<bool>("ccscale9sprite-fix", false);
		}

		if (!Mod::get()->getSettingValue<bool>("Disable-selector-button")) {
			auto menu = getChildByID("bottom-menu");
			auto darkmodeIcon = CCSprite::createWithSpriteFrameName("DMv4_Main_Btn.png"_spr);
			darkmodeIcon->setScale(1.1f);

			auto myButton = CCMenuItemSpriteExtra::create(darkmodeIcon, this, menu_selector(DarkModeMenuLayer::onDarkomodeButton));
			myButton->setID("TexturePackSelector"_spr);
			menu->addChild(myButton);
			m_fields->m_darkmodeButton = darkmodeIcon;
			menu->updateLayout();

			if(!Mod::get()->getSettingValue<bool>("Disable-updates-indicator"))
			{
				scheduleOnce(schedule_selector(DarkModeMenuLayer::checkUpdates), 0.1f);
			}
			
		}

		return true;
	}

	void checkUpdates(float) {
		if (m_fields->m_hasCheckedUpdates) return;
		m_fields->m_hasCheckedUpdates = true;

		auto self = this;
		this->retain();  

		TexturePackSelector::getPendingUpdates([self](int count) {
			if (count > 0)
				self->updateMarker(count);

			self->release();  
			});
	}

	void updateMarker(int count) {
		if (!m_fields->m_darkmodeButton) return;

		m_fields->m_darkmodeButton->removeChildByID("update-marker");

		if (count > 0) {
			auto marker = CCSprite::createWithSpriteFrameName("geode.loader/updates-available.png");
			marker->setScale(0.65f);

			auto label = CCLabelBMFont::create(fmt::format("{}", count).c_str(), "bigFont.fnt");
			label->setScale(0.5f);
			marker->addChildAtPosition(label, Anchor::Center);

			auto size = m_fields->m_darkmodeButton->getContentSize();
			marker->setPosition({ size.width - 5, size.height - 5 });
			marker->setID("update-marker");
			m_fields->m_darkmodeButton->addChild(marker);
		}
	}

	void onDarkomodeButton(CCObject*) {
		auto selector = TexturePackSelector::create();
		if (!Mod::get()->getSettingValue<bool>("Disable-updates-indicator"))
		{
			selector->setUpdateCallback([self = Ref(this)](int remaining) {
				if (self)
					self->updateMarker(remaining);
				});
		}

		selector->show();
	}
};
