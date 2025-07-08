#include <map>
#include <tuple>
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/GJListLayer.hpp>
#include <Geode/modify/CCScale9Sprite.hpp>
#include <Geode/modify/CCSprite.hpp>
#include <Geode/modify/CCLayerColor.hpp>
#include <Geode/modify/MenuGameLayer.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include "TexturePackSelector/TexturePackSelector.hpp"
#include "TexturePackSelector/TexturePackSelector.cpp"

using namespace geode::prelude;

template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}



struct ColorComparator {
	bool operator()(const ccColor3B& lhs, const ccColor3B& rhs) const {
		return std::tie(lhs.r, lhs.g, lhs.b) < std::tie(rhs.r, rhs.g, rhs.b);
	}
};

void updateGroundColorM(CCSpriteBatchNode* batch, const cocos2d::ccColor3B& color)
{
	
	if (!batch) return;
	auto children = batch->getChildren();
	if (!children) return;

	for (int i = 0; i < children->count(); ++i) {
		auto sprite = dynamic_cast<CCSprite*>(children->objectAtIndex(i));
		if (!sprite) continue;

		sprite->setColor(color);

		auto spriteChildren = sprite->getChildren();
		if (!spriteChildren) continue;

		for (int o = 0; o < spriteChildren->count(); ++o) {
			auto spriteChild = dynamic_cast<CCSprite*>(spriteChildren->objectAtIndex(o));
			if (!spriteChild) continue;

			spriteChild->setColor(color);
		}
	}
}

class $modify(GJListLayer) {

	static GJListLayer* create(BoomListView * p0, char const* p1, cocos2d::ccColor4B p2, float p3, float p4, int p5) {
		auto ret = GJListLayer::create(p0, p1, p2, p3, p4, p5);

		if (ret->getColor() == ccColor3B{191, 114, 62}) {
			ret->setColor({ 31,31,31 });
			
		}

		return ret;
	}


};

void onSceneSwitch(CCScene * scene) {
	if (scene)
	{
		if (scene->getChildrenCount() > 0)
		{
			if (auto layer = as<CCLayer*>(scene->getChildren()->objectAtIndex(0)); layer->getChildrenCount() > 0)
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

template <typename T>
class GlobedListCell : public cocos2d::CCLayer, public T {}; //List cell recreate for globed cells


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
		if (it != colorMap.end()) {
			this->setColor(it->second);
		}
	
		// Option for transparent lists
		if (Mod::get()->getSettingValue<bool>("transparent-lists") && this->getOpacity() != 10) {
			auto parent = this->getParent();
			auto grandparent = parent ? parent->getParent() : nullptr;
			auto loadingCircle = parent ? parent->getChildByType<LoadingCircle*>(0) : nullptr;

			auto currentColor = this->getColor();

			bool isValidCell =
				typeinfo_cast<LevelCell*>(parent) ||
				typeinfo_cast<GJScoreCell*>(parent) ||
				typeinfo_cast<LevelListCell*>(parent) ||
				typeinfo_cast<MapPackCell*>(parent) ||
				dynamic_cast<geode::GenericListCell*>(parent) != nullptr;

			// For yellow color in list
			if (typeinfo_cast<GlobedListCell<cocos2d::CCNode>*>(parent) != nullptr &&
				currentColor == ccColor3B{ 90, 90, 90 }) {
				isValidCell = true;
			}

			bool isListLayer = typeinfo_cast<GJListLayer*>(this);
			bool hasListView = this->getChildByID("list-view") != nullptr;

			bool hasVisibleLoadingCircle = loadingCircle && loadingCircle->isVisible();
			bool isGeodeListView = dynamic_cast<geode::ListView*>(this) != nullptr;

			bool isBackgroundLayer = isListLayer && this->getID() == "background";

			if (!isBackgroundLayer && (isValidCell || (isListLayer && (hasListView || hasVisibleLoadingCircle)) || isGeodeListView)) {

				if (typeinfo_cast<DailyLevelNode*>(grandparent)) {
					auto size = this->getContentSize();
					this->setVisible(size.height == 90 && size.width == 160);
				}

				
				if (currentColor == ccColor3B{ 230, 150, 10 }) {
					this->setOpacity(10);
				}
				else if (currentColor == ccColor3B{ 48, 48, 48 }) {
					this->setColor(ccColor3B{ 0, 0, 0 });
					this->setOpacity(10);
				}
				else {
					this->setColor(ccColor3B{ 255, 255, 255 });
					this->setOpacity(10);
				}
			}
		}


		CCLayerColor::draw();
	}
};


class $modify(CCScale9Sprite)
{
		
	void visit() //init doesnt work with this too
	{
		//new code of something stuff
		static const std::map<ccColor3B, ccColor3B, ColorComparator> colorMap = {
			{{0, 56, 141}, {20, 20, 20}},
			{{130, 64, 33}, {20, 20, 20}},
			{{0, 46, 117}, {20, 20, 20}},
			{{0, 36, 91}, {20, 20, 20}},
			{{0, 31, 79}, {20, 20, 20}},
			{{123, 60, 31}, {15, 15, 15}},
			{{108, 60, 36}, {15, 15, 15}},
			{{0, 39, 98}, {10, 10, 10}},
			{{76,42,25},{25,25,25}}
		};

		auto it = colorMap.find(this->getColor());
		if (it != colorMap.end()) {
			this->setColor(it->second);
		}
		CCScale9Sprite::visit();
	}
};

class $modify(MenuGameLayer)
{
	void update(float a1)
	{
		MenuGameLayer::update(a1);

		CCObject* pObj = nullptr;


		CCARRAY_FOREACH(((CCLayer*)(this))->getChildren(), pObj) {
			CCSprite* currentSprite = (CCSprite*)pObj;

			if (currentSprite->getContentSize().height == 512)
			{
				currentSprite->setColor({ 40,40,40 });
			}

			if (instanceof<GJGroundLayer>(pObj)) {

				GJGroundLayer* ground = dynamic_cast<GJGroundLayer*>(pObj);

				auto m_pGround01Sprite = static_cast<CCSpriteBatchNode*>(ground->getChildByID("ground-sprites"));
				auto m_pGround02Sprite = static_cast<CCSpriteBatchNode*>(ground->getChildByID("ground-sprites-2"));


				CCArray* children = nullptr;  

				for (int i = 0; i < m_pGround01Sprite->getChildren()->count(); ++i) {
					if (m_pGround01Sprite != nullptr) {
						updateGroundColorM(m_pGround01Sprite, { 40,40,40 });
					}
					if (m_pGround02Sprite != nullptr) {
						updateGroundColorM(m_pGround02Sprite, { 40,40,40 });
					}
				}
				
			}
		}
	}
};
class $modify(LevelInfoLayer)
{
	void onPlay(cocos2d::CCObject * sender)
	{
		LevelInfoLayer::onPlay(sender);

		int counter = 0;
		CCObject* pObj = nullptr;
		auto PlayButton = (CCSprite*)this->m_playBtnMenu->getChildren()->objectAtIndex(0);
		
		CCARRAY_FOREACH(((CCSprite*)(PlayButton->getChildren()->objectAtIndex(0)))->getChildren(), pObj) {
			CCSprite* currentSprite = (CCSprite*)pObj;

			
			if (counter > 2)
			{
				currentSprite->setVisible(true);
			}
			else
			{
				currentSprite->setColor({ 0,0,0 });
			}
			counter++;
		}
	}
};

class $modify(LevelSelectLayer)
{
	void scrollLayerMoved(cocos2d::CCPoint a1)
	{
		LevelSelectLayer::scrollLayerMoved(a1);

		CCObject* pObj = nullptr;

		CCARRAY_FOREACH(this->getChildren(), pObj) {
			CCNode* currentNode = (CCNode*)pObj;
			
			if (instanceof<GJGroundLayer>(pObj)) {
				GJGroundLayer* groundLayer = (GJGroundLayer*)currentNode;
				auto m_pGround01Sprite = static_cast<CCSpriteBatchNode*>(groundLayer->getChildByID("ground-sprites"));
				auto m_pGround02Sprite = static_cast<CCSpriteBatchNode*>(groundLayer->getChildByID("ground-sprites-2"));


				CCArray* children = nullptr;

				for (int i = 0; i < m_pGround01Sprite->getChildren()->count(); ++i) {
					if (m_pGround01Sprite != nullptr) {
						updateGroundColorM(m_pGround01Sprite, { 40,40,40 });
					}
					if (m_pGround02Sprite != nullptr) {
						updateGroundColorM(m_pGround02Sprite, { 40,40,40 });
					}
				}
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

class $modify(DarkModeMenuLayer,MenuLayer)
{

	bool init()
	{
		if (!MenuLayer::init()) return false;

		auto darkmodeIcon = CCSprite::createWithSpriteFrameName("DMv4_Main_Btn.png"_spr);
		
		darkmodeIcon->setScale(1.1);
		

		auto myButton = CCMenuItemSpriteExtra::create(
		darkmodeIcon,
		this,
		menu_selector(DarkModeMenuLayer::onDarkomodeButton)
		);

		
		auto menu = this->getChildByID("bottom-menu");

		if (Mod::get()->getSettingValue<bool>("Disable-selector-button") == false)
		{
			menu->addChild(myButton);

			myButton->setID("TexturePackSelector"_spr);

			menu->updateLayout();
		}
	
	
	
		return true;
	}


	void onDarkomodeButton(CCObject*) {
		TexturePackSelector::create()->show();
	}
};