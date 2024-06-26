#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/modify/InfoLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/modify/GJListLayer.hpp>
#include <Geode/modify/CCLayerColor.hpp>
#include <Geode/modify/MenuGameLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/AchievementCell.hpp>
#include <Geode/modify/LeaderboardsLayer.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/LevelLeaderboard.hpp>
#include <Geode/modify/FRequestProfilePage.hpp>
#include <Geode/modify/FriendsProfilePage.hpp>
#include <Geode/modify/MessagesProfilePage.hpp>
#include <Geode/modify/ShareLevelLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/GJLocalLevelScoreCell.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/GJLevelScoreCell.hpp>
#include <Geode/modify/LevelListCell.hpp>
#include <Geode/modify/GJScoreCell.hpp>
#include <Geode/modify/StatsCell.hpp>
#include <Geode/modify/StatsLayer.hpp>
#include <Geode/modify/URLCell.hpp>
using namespace geode::prelude;

template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}

//hola


class $modify(URLCell)
{
#ifdef GEODE_IS_ANDROID


	void updateBGColor(int a1)
	{
		URLCell::updateBGColor(a1);
		if (a1 % 2 == 1) // Si a1 es impar
		{
			this->m_backgroundLayer->setColor({ 80, 80, 80 });

		}
		else // Si a1 es par
		{
			this->m_backgroundLayer->setColor({ 48, 48, 48 });
		}
	}
#endif

};
class $modify(StatsLayer)
{
	void customSetup()
	{
		
		StatsLayer::customSetup();
		this->setTag(80);
		
		
		}



};
class $modify(StatsCell)
{



	void updateBGColor(int a1)
	{
		StatsCell::updateBGColor(a1);
		if (a1 % 2 == 1) // Si a1 es impar
		{
			this->m_backgroundLayer->setColor({ 80, 80, 80 });

		}

		else // Si a1 es par
		{
			this->m_backgroundLayer->setColor({ 48, 48, 48 });
		}

		
	}

};
class $modify(GJScoreCell)
{
#ifdef GEODE_IS_ANDROID


	void updateBGColor(int a1)
	{
		GJScoreCell::updateBGColor(a1);
		if (a1 % 2 == 1) // Si a1 es impar
		{
			this->m_backgroundLayer->setColor({ 80, 80, 80 });

		}
		else // Si a1 es par
		{
			this->m_backgroundLayer->setColor({ 48, 48, 48 });
		}
	}
#endif

};
class $modify(LevelListCell)
{
#ifdef GEODE_IS_ANDROID


	void updateBGColor(int a1)
	{
		LevelListCell::updateBGColor(a1);
		if (a1 % 2 == 1) // Si a1 es impar
		{
			this->m_backgroundLayer->setColor({ 80, 80, 80 });

		}
		else // Si a1 es par
		{
			this->m_backgroundLayer->setColor({ 48, 48, 48 });
		}
	}
#endif

};
class $modify(GJLevelScoreCell)
{
#ifdef GEODE_IS_ANDROID


	void updateBGColor(int a1)
	{
		GJLevelScoreCell::updateBGColor(a1);
		if (a1 % 2 == 1) // Si a1 es impar
		{
			this->m_backgroundLayer->setColor({ 80, 80, 80 });

		}
		else // Si a1 es par
		{
			this->m_backgroundLayer->setColor({ 48, 48, 48 });
		}
	}
#endif
	
};
class $modify(ProfilePage)
{

	void setupCommentsBrowser(cocos2d::CCArray * a1)
	{
		ProfilePage::setupCommentsBrowser(a1);

		/*auto BG1 = (GJCommentListLayer*)this->getChildByID("icon-background");
		
		BG1->setColor({ 31,31,31 });*/
		CCObject* pObj = nullptr;
		CCARRAY_FOREACH(((CCLayer*)(this->m_mainLayer))->getChildren(), pObj) {
			GJCommentListLayer* currentSprite = (GJCommentListLayer*)pObj;


			if (currentSprite->getContentSize().height == 100)
			{
				currentSprite->setColor({ 44,44,44 });
				
				/*auto text = (CCScale9Sprite*)currentSprite->getChildren()->objectAtIndex(1);
				text->setColor({ 44, 44, 44 });*/
			}

			if (currentSprite->getContentSize().height == 45)
			{
				currentSprite->setColor({ 44,44,44 });

				/*auto text = (CCScale9Sprite*)currentSprite->getChildren()->objectAtIndex(1);
				text->setColor({ 44, 44, 44 });*/
			}
		}
		//auto hijos = Layer->getChildren();
		//for (int i = 0; i < hijos->count(); ++i) {
		//	auto hijo = hijos.at(i);

		//	// Verifica si el contentSize cumple con tus condiciones
		//	if (hijo->getContentSize().equals(Size(100.0f, 100.0f))) {
		//		// Llama a tu funci�n personalizada
		//		tuFuncionPersonalizada(hijo);
		//	}
		//}
		


	}
};
class $modify (CommentCell)
{
	void updateBGColor(int a1)
	{
		CommentCell::updateBGColor(a1);
		/*auto BG = (CCSprite*)this->getChildren()->objectAtIndex(0);*/
		/*BG->setVisible(false);*/
		/*std::cout << a1 << std::endl;*/
		

		
		if (a1 % 2 == 1) // Si a1 es impar
		{
			this->m_backgroundLayer->setColor({ 80, 80, 80 });
			
		}
		else // Si a1 es par
		{
			this->m_backgroundLayer->setColor({ 48, 48, 48 });
		}
		CCObject* pObj = nullptr;
		CCARRAY_FOREACH(((CCLayer*)(this->getChildren()->objectAtIndex(1)))->getChildren(), pObj) {

			if (instanceof<cocos2d::extension::CCScale9Sprite>(pObj)) {

				cocos2d::extension::CCScale9Sprite* scale9Sprite = dynamic_cast<cocos2d::extension::CCScale9Sprite*>(pObj);
				scale9Sprite->setColor(ccBLACK);
				scale9Sprite->setOpacity(32);
			}
		}
	}

	void LoadFromComment(GJComment* a1)
	{
		CommentCell::loadFromComment(a1);
		a1->m_color = (ccBLACK);
	
	}
};

class $modify (LevelCell)
{
	void updateBGColor(int a1)
	{
		LevelCell::updateBGColor(a1);
		/*auto BG = (CCLayerColor*)this->getChildren()->objectAtIndex(0);
		auto parent = this->getParent();*/
		this->m_backgroundLayer;
		
		if (a1 % 2 == 1) // Si a1 es impar
		{
			this->m_backgroundLayer->setColor({ 80, 80, 80 });

		}
		else // Si a1 es par
		{
			this->m_backgroundLayer->setColor({ 48, 48, 48 });
		}
	}
};

class $modify(InfoLayer)
{
	void setupCommentsBrowser(cocos2d::CCArray* a1)
	{
		InfoLayer::setupCommentsBrowser(a1);
		this->m_list->setColor({31,31,31});

	/*	this->m_mainLayer->setVisible(false);*/

		CCObject* pObj = nullptr;
		CCARRAY_FOREACH(((CCLayer*)(this->m_mainLayer))->getChildren(), pObj) {
			CCScale9Sprite* currentSprite = (CCScale9Sprite*)pObj;


			if (currentSprite->getContentSize().height == 55)
			{
				currentSprite->setColor({ 0, 0, 0 });
				currentSprite->setOpacity(40);
					/*auto text = (CCScale9Sprite*)currentSprite->getChildren()->objectAtIndex(1);
					text->setColor({ 44, 44, 44 });*/
			}
		}
		/*BG->setColor({ 0,0,0 });
		BG->setOpacity(40);*/
	}

	void loadPage(int a1, bool a2)
	{
		InfoLayer::loadPage(a1, a2);
		/*auto Layer = (CCLayer*)this->getChildren()->objectAtIndex(0);
		auto BG =(CCScale9Sprite*)Layer->getChildByID("desc-background");
		BG->setColor({0,0,0});
		BG->setOpacity(40);*/
	}

};

class $modify(LevelBrowserLayer)
{
	void setupLevelBrowser(cocos2d::CCArray * a1)
	{
		LevelBrowserLayer::setupLevelBrowser(a1);
		this->m_list->setColor({ 31,31,31 });
	}
};

class $modify(CCLayerColor)
{

	
	
	void draw()
	{
		auto GeodeGenericCell = this->getParent();
		
		
		
		/*auto parent4 = parent3->getParent();*/

		/*CCObject* pObj = nullptr;*/
		auto currentScene = CCDirector::sharedDirector()->getRunningScene();
		auto Layer = (CCLayer*)currentScene->getChildren()->objectAtIndex(0);
		
		CCObject* pObj = nullptr;


		CCARRAY_FOREACH(((CCSprite*)(Layer))->getChildren(), pObj) {

			
			if (instanceof<GJListLayer>(pObj)) {

				GJListLayer* list = dynamic_cast<GJListLayer*>(pObj);
				list->setColor({ 31,31,31 });
			}


			if (instanceof<StatsLayer>(pObj)) {

				StatsLayer* stats = dynamic_cast<StatsLayer*>(pObj);
				auto layersos = (CCLayer*)stats->getChildren()->objectAtIndex(0);
				
				CCObject* pObj2 = nullptr;


				CCARRAY_FOREACH(((CCSprite*)(layersos))->getChildren(), pObj2) {

					/*CCSprite* currentSprite = (CCSprite*)pObj;*/

					if (instanceof<GJListLayer>(pObj2)) {

						GJListLayer* list = dynamic_cast<GJListLayer*>(pObj2);
						list->setColor({ 31,31,31 });
					}
					

				}
			}

		}

		CCObject* pObj3 = nullptr;
		CCARRAY_FOREACH(((CCSprite*)(currentScene))->getChildren(), pObj3) {

			CCNode* currentNode = (CCNode*)pObj3;

			

			if (currentNode->getZOrder() == 105 && currentNode->getContentSize().height == 320
				&& currentNode->getContentSize().width == 569)
			{
				CCLayer* roompopup = dynamic_cast<CCLayer*>(pObj3);
				auto layersos = (CCLayer*)roompopup->getChildren()->objectAtIndex(0);
				CCObject* pObj2 = nullptr;


				CCARRAY_FOREACH(((CCSprite*)(layersos))->getChildren(), pObj2) {

					/*CCSprite* currentSprite = (CCSprite*)pObj;*/

					if (instanceof<GJCommentListLayer>(pObj2)) {


						GJCommentListLayer* list = dynamic_cast<GJCommentListLayer*>(pObj2);
						if (list->getContentSize().width == 340 && list->getContentSize().height == 45 ||
							list->getContentSize().width == 340 && list->getContentSize().height == 100)
						{
							list->setColor({ 44,44,44 });
						}
						else
						{
							list->setColor({ 31,31,31 });
						}
						
					}


				}
			}
			
			if (instanceof<GJListLayer>(pObj3)) {

				GJListLayer* list = dynamic_cast<GJListLayer*>(pObj);
				list->setColor({ 31,31,31 });
			}


			

		}
		/*currentScene->setVisible(false);*/
		ccColor3B color1 = { 161, 88, 44 };

		ccColor3B color2 = { 194, 114, 62 };

	
		/*searchbar->setColor({ 80, 80, 80 });*/
		//CCARRAY_FOREACH(((GJListLayer*)(parent2->getChildren()->objectAtIndex(0)))->getChildren(), pObj) {
		//	CCLayerColor* currentSprite = (CCLayerColor*)pObj;


		//	if (currentSprite->getContentSize().height == 190)
		//	{
		//		currentSprite->setColor({ 80, 80, 80 });
		//			/*auto text = (CCScale9Sprite*)currentSprite->getChildren()->objectAtIndex(1);
		//			text->setColor({ 44, 44, 44 });*/
		//	}
		//}
		
		//Cell witout desc
		//if (GeodeGenericCell->getContentSize().width == 358 &&
		//	GeodeGenericCell->getContentSize().height == 40 
		//	&&  this->getColor() == color1 )
		//{
		//	
		//	/*auto GJListLayera = (GJListLayer*)ModListLayer->getChildren()->objectAtIndex(3);
		//	auto searchbar = (CCLayerColor*)GJListLayera->getChildren()->objectAtIndex(0);*/

		//	
		//	this->setColor({ 48, 48, 48 });
		//	/*std::cout << "Cell 1" << std::endl;*/
		//}
		
		//if (GeodeGenericCell->getContentSize().width == 358 &&
		//	GeodeGenericCell->getContentSize().height == 40 &&
		//	this->getColor() == color2)
		//{
		//	this->setColor({ 80, 80, 80 });
		//	/*std::cout << "Cell 1" << std::endl;*/
		//}

		//Cells with desc
		//if (GeodeGenericCell->getContentSize().width == 358 &&
		//	GeodeGenericCell->getContentSize().height == 60 &&
		//	this->getColor() == color1)
		//{
		//	this->setColor({ 48, 48, 48 });
		//	/*std::cout << "Cell 1" << std::endl;*/
		//}

		/*if (GeodeGenericCell->getContentSize().width == 358 &&
			GeodeGenericCell->getContentSize().height == 60 &&
			this->getColor() == color2)
		{
			
		}*/

		//search

		if (this->getContentSize().width == 358 &&
			this->getContentSize().height == 30 &&
			this->getColor() == color2)
		{
			this->setColor({ 80, 80, 80 });
			auto text = (CCScale9Sprite*)this->getChildren()->objectAtIndex(1);
			text->setColor({ 44, 44, 44 });
			/*std::cout << "Cell 1" << std::endl;*/
		}

		//for generic cells
		if (this->getColor() == color1)
		{
			this->setColor({ 48, 48, 48 });
		}

		if (this->getColor() == color2)
		{
			this->setColor({ 80, 80, 80 });
		}
		CCLayerColor::draw();
		/*c
		auto GJListLayera = (GJListLayer*)ModListLayer->getChildren()->objectAtIndex(3);
		GJListLayera->setVisible(false);*/
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
				ground->updateGround01Color({ 40,40,40 });
				ground->updateGround02Color({ 40,40,40 });
			}
		}
			


			/*CCARRAY_FOREACH(((CCLayer*)(this))->getChildren(), pObj2) {
				if (instanceof<GJGroundLayer>(pObj)) {

					GJGroundLayer* ground = dynamic_cast<GJGroundLayer*>(pObj2);
					ground->updateGround01Color({ 50,50,50 });
					ground->updateGround02Color({ 50,50,50 });
				}
			}*/
		/*this->m_backgroundSprite->setColor({ 50,50,50 });

		this->m_groundLayer->updateGround01Color({ 50,50,50 });
		this->m_groundLayer->updateGround02Color({ 50,50,50 });*/
		/*std::cout << "Omg funciona" << std::endl;*/
	}

	bool init()
	{
		auto fondolol = CCScale9Sprite::create("square02b_001.png");
		fondolol->setColor({ 50,50,50 });
		CCObject* pObj = nullptr;
		

		CCARRAY_FOREACH(((CCSprite*)(this))->getChildren(), pObj) {
			CCSprite* currentSprite = (CCSprite*)pObj;

			if (currentSprite->getContentSize().height == 512)
			{
				fondolol->setPosition(currentSprite->getRotationX(), currentSprite->getRotationY());
			}
		
				
		
		}
		/*auto BG = (CCSprite*)this->getChildren()->objectAtIndex(0);
		fondolol->setPosition(BG->getRotationX(), BG->getRotationY());*/
		this->addChild(fondolol, 1);

		return MenuGameLayer::init();
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
		
	/*	std::cout << "Omg funciona" << std::endl;*/
	}
};

class $modify(AchievementCell)
{
	void updateBGColor(int a1)
	{
		AchievementCell::updateBGColor(a1);
		if (a1 % 2 == 1) // Si a1 es impar
		{
			this->m_backgroundLayer->setColor({ 80, 80, 80 });

		}
		else // Si a1 es par
		{
			this->m_backgroundLayer->setColor({ 48, 48, 48 });
		}
	}
};


class $modify(LeaderboardsLayer)
{
	void setupLevelBrowser(cocos2d::CCArray * a1)
	{
		LeaderboardsLayer::setupLevelBrowser(a1);
		this->m_list->setColor({ 31,31,31 });
	}
};

class $modify(LevelSearchLayer)
{
	bool init(int a1)
	{
		

		if (!LevelSearchLayer::init(a1)) return false;

		auto BG1 = (CCScale9Sprite*)this->getChildByID("level-search-bg");
		auto BG2 = (CCScale9Sprite*)this->getChildByID("level-search-bar-bg");
		auto BG3 = (CCScale9Sprite*)this->getChildByID("quick-search-bg");
		auto BG4 = (CCScale9Sprite*)this->getChildByID("difficulty-filters-bg");
		auto BG5 = (CCScale9Sprite*)this->getChildByID("length-filters-bg");


		auto bar = (CCTextInputNode*)this->getChildByID("search-bar");
		auto label = (CCLabelBMFont*)bar->getChildren()->objectAtIndex(1);
		label->setColor({ 255,255,255 });
		BG1->setColor({20,20,20});
		
		BG2->setColor({ 10,10,10 });

		BG3->setColor({ 20,20,20 });

		BG4->setColor({ 20,20,20 });

		BG5->setColor({ 20,20,20 });
		/*this->setVisible(false);*/
		return true;
		
		
	}
};

class $modify(LevelLeaderboard)
{
	bool init(GJGameLevel* a1, LevelLeaderboardType a2, LevelLeaderboardMode a3)
	{


		if (!LevelLeaderboard::init(a1,a2,a3)) return false;

		
		/*auto Layer = (CCLayer*)this->getChildren()->objectAtIndex(0);*/
		CCObject* pObj = nullptr;
		CCARRAY_FOREACH(((CCLayer*)(this->m_mainLayer))->getChildren(), pObj) {

			if (instanceof<GJCommentListLayer>(pObj)) {

				GJCommentListLayer* list = dynamic_cast<GJCommentListLayer*>(pObj);
				list->setColor({ 31,31,31 });
			
			}
		}
		return true;


	}

	void loadLeaderboardFinished(cocos2d::CCArray* a1, char const* a2)
	{
		LevelLeaderboard::loadLeaderboardFinished(a1, a2);
		CCObject* pObj = nullptr;
		CCARRAY_FOREACH(((CCLayer*)(this->m_mainLayer))->getChildren(), pObj) {

			if (instanceof<GJCommentListLayer>(pObj)) {

				GJCommentListLayer* list = dynamic_cast<GJCommentListLayer*>(pObj);
				list->setColor({ 31,31,31 });
				
			}
		}
	}
	void loadScores()
	{
		LevelLeaderboard::loadScores();
		CCObject* pObj = nullptr;
		CCARRAY_FOREACH(((CCLayer*)(this->m_mainLayer))->getChildren(), pObj) {

			if (instanceof<GJCommentListLayer>(pObj)) {

				GJCommentListLayer* list = dynamic_cast<GJCommentListLayer*>(pObj);
				list->setColor({ 31,31,31 });
				
			}
		}
	}
};

class $modify(FRequestProfilePage)
{
	void setupCommentsBrowser(cocos2d::CCArray* a1)
	{


		FRequestProfilePage::setupCommentsBrowser(a1);

		CCObject* pObj = nullptr;
		CCARRAY_FOREACH(((CCLayer*)(this->m_mainLayer))->getChildren(), pObj) {

			if (instanceof<GJCommentListLayer>(pObj)) {

				GJCommentListLayer* list = dynamic_cast<GJCommentListLayer*>(pObj);
				list->setColor({ 31,31,31 });

			}
		}


	}
};

class $modify(FriendsProfilePage)
{
	void setupUsersBrowser(cocos2d::CCArray * a1, UserListType a2)
	{


		FriendsProfilePage::setupUsersBrowser(a1, a2);

		CCObject* pObj = nullptr;
		CCARRAY_FOREACH(((CCLayer*)(this->m_mainLayer))->getChildren(), pObj) {

			if (instanceof<GJCommentListLayer>(pObj)) {

				GJCommentListLayer* list = dynamic_cast<GJCommentListLayer*>(pObj);
				list->setColor({ 31,31,31 });

			}
		}


	}
};

class $modify(MessagesProfilePage)
{
	void setupCommentsBrowser(cocos2d::CCArray * a1)
	{


		MessagesProfilePage::setupCommentsBrowser(a1);

		CCObject* pObj = nullptr;
		CCARRAY_FOREACH(((CCLayer*)(this->m_mainLayer))->getChildren(), pObj) {

			if (instanceof<GJCommentListLayer>(pObj)) {

				GJCommentListLayer* list = dynamic_cast<GJCommentListLayer*>(pObj);
				list->setColor({ 31,31,31 });

			}
		}


	}
};

class $modify(ShareLevelLayer)
{
	bool init(GJGameLevel* a1)
	{


		if (!ShareLevelLayer::init(a1)) return false;


		/*auto Layer = (CCLayer*)this->getChildren()->objectAtIndex(0);*/
		CCObject* pObj = nullptr;
		CCARRAY_FOREACH(((CCLayer*)(this->m_mainLayer))->getChildren(), pObj) {

			CCScale9Sprite* currentSprite = (CCScale9Sprite*)pObj;


			if (currentSprite->getContentSize().height == 50)
			{
				currentSprite->setColor({ 20, 20, 20 });
				
				/*auto text = (CCScale9Sprite*)currentSprite->getChildren()->objectAtIndex(1);
				text->setColor({ 44, 44, 44 });*/
			}
		}

		
		return true;


	}
};

class $modify(EditLevelLayer)
{
	bool init(GJGameLevel * a1)
	{


		if (!EditLevelLayer::init(a1)) return false;

		auto BG1 = (CCScale9Sprite*)this->getChildByID("level-name-background");
		auto BG2 = (CCScale9Sprite*)this->getChildByID("description-background");
		

		BG1->setColor({ 20,20,20 });
		BG2->setColor({ 20,20,20 });
		return true;


	}
};

class $modify(GJLocalLevelScoreCell)
{
	void updateBGColor(int a1)
	{
		GJLocalLevelScoreCell::updateBGColor(a1);
		/*auto BG = (CCLayerColor*)this->getChildren()->objectAtIndex(0);
		auto parent = this->getParent();*/
		this->m_backgroundLayer;

		if (a1 % 2 == 1) // Si a1 es impar
		{
			this->m_backgroundLayer->setColor({ 80, 80, 80 });

		}
		else // Si a1 es par
		{
			this->m_backgroundLayer->setColor({ 48, 48, 48 });
		}
	}
};
