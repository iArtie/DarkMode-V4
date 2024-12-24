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
#include "TexturePackSelector/TexturePackCell.hpp"
#include "TexturePackSelector/TexturePackCell.cpp"
#include <Geode/utils/web.hpp>
#include "TexturePackSelector/TexturePackSelector.cpp"

using namespace geode::prelude;

template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}

#include <map>
#include <tuple>


// Comparador externo para ccColor3B
struct ColorComparator {
	bool operator()(const ccColor3B& lhs, const ccColor3B& rhs) const {
		return std::tie(lhs.r, lhs.g, lhs.b) < std::tie(rhs.r, rhs.g, rhs.b);
	}
};

void updateGroundColorM(CCSpriteBatchNode* batch, const cocos2d::ccColor3B& color)
{
	
		for (int i = 0; i < batch->getChildren()->count(); ++i) {
			auto sprite = (CCSprite*)batch->getChildren()->objectAtIndex(i);
			sprite->setColor(color);
			for (int o = 0; o < sprite->getChildren()->count(); ++o) {
				auto spriteChild = (CCSprite*)sprite->getChildren()->objectAtIndex(o);
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

class $modify(CCSprite)
{
	void draw()
	{
		auto currentScene = CCDirector::sharedDirector()->getRunningScene();
		

		// NEW IDEA FOR CHANGE THE BACKGROUNDS COLOR
		static const std::vector<std::pair<std::string, std::string>> layerBackgroundPairs = {
			{"dankmeme.globed2/GlobedMenuLayer", "background"},
			{"cvolton.betterinfo/DailyViewLayer", "cvolton.betterinfo/background"},
			{"cvolton.betterinfo/CustomCreatorLayer", "cvolton.betterinfo/background"},
			{"cvolton.betterinfo/LevelSearchViewLayer", "cvolton.betterinfo/background"},
			{"cvolton.betterinfo/RewardGroupLayer", "cvolton.betterinfo/background"}
		};

	
		for (const auto& [layerID, backgroundID] : layerBackgroundPairs) {
			if (currentScene->getChildByID(layerID)) {
				auto layer = static_cast<CCLayer*>(currentScene->getChildByID(layerID));
				auto bg = static_cast<CCSprite*>(layer->getChildByID(backgroundID));
				if (bg) {
					bg->setColor({ 64, 64, 64 });
				}
			}
		}
		CCSprite::draw();
	}
};


class $modify(CCLayerColor)
{
	void draw()
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
		
		CCLayerColor::draw();
	}
};


class $modify(CCScale9Sprite)
{

	void visit()
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
			{{0, 39, 98}, {10, 10, 10}}
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


				CCArray* children = nullptr;  // Inicializamos children a nullptr

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

		auto bar = (CCTextInputNode*)this->getChildByID("search-bar");
		bar->setLabelNormalColor({ 255,255,255 });

		return true;
	}
};

class $modify(DarkModeMenuLayer,MenuLayer)
{

	void onMyButton(CCObject * target) {
	
		auto awa = TexturePackSelector::create("");
		awa->show();
	}

	bool init()
	{
		if (!MenuLayer::init()) return false;

		auto darkmodeIcon = CCSprite::createWithSpriteFrameName("DMv4_Main_Btn.png"_spr);
		
		darkmodeIcon->setScale(1.1);
		

		auto myButton = CCMenuItemSpriteExtra::create(
		darkmodeIcon,
		this,
		menu_selector(DarkModeMenuLayer::onMyButton)
		);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);
			
		myButton->setID("TexturePackSelector"_spr);
			
		menu->updateLayout();
	
	
		return true;
	}
};