#include "StoreBScene.h"
#include "GameMenuItem.h"
#include "StoreAScene.h"
#include "cocos2dx_StoreInventory.h"
#include "cocos2dx_StoreController.h"
#include "Includes.h"

#include <string>
#include <sstream> 


USING_NS_CC;
using namespace std;

CCLabelTTF* StoreBScene::pLabelBalance;

CCScene* StoreBScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	StoreBScene *layer = StoreBScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool StoreBScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayerColor::initWithColor(ccc4(255,255,255,255)))
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCLabelTTF* pLabelTitleTest = CCLabelTTF::create("Soomla Test Store", "GoodDog.otf", 34);
	pLabelTitleTest->setColor(ccc3(255,0,0));
	pLabelTitleTest->setPosition(ccp(pLabelTitleTest->boundingBox().size.width/2 + origin.x + 20, visibleSize.height - 30 + origin.y));
	this->addChild(pLabelTitleTest, 1);

	CCLabelTTF* pLabelTitle = CCLabelTTF::create("Virtual Currency Packs", "GoodDog.otf", 54);
	pLabelTitle->setColor(ccc3(0,0,0));
	pLabelTitle->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height - 100 + origin.y));
	this->addChild(pLabelTitle, 1);


	CCSprite* pSpriteMuffin = CCSprite::create("muffin.png");
	pSpriteMuffin->setScale(0.5f);
	pSpriteMuffin->setPosition(ccp(origin.x + visibleSize.width - 90, visibleSize.height + origin.y - 50));
	this->addChild(pSpriteMuffin, 0);

	pLabelBalance = CCLabelTTF::create("24", "GoodDog.otf", 34);
	pLabelBalance->setColor(ccc3(0,255,255));
	pLabelBalance->setPosition(ccp(origin.x + visibleSize.width - 40, visibleSize.height + origin.y - 50));
	this->addChild(pLabelBalance, 1);
	setCurrencyBalanceLabel();


	GameMenuItem* backItem = GameMenuItem::itemWithLabel(
		CCSprite::create("back.png"),
		this,
		menu_selector(StoreBScene::menuBackCallback));

	backItem->setPosition(ccp(origin.x + visibleSize.width/2 , 110));

	// In Game Menu
	CCMenu* menu = CCMenu::create(backItem, NULL);

	createListViewItem(origin, menu, visibleSize, 0, "no_ads.png", "Remove Ads!", "Test purchase of MANAGED item.", 5.99);
	createListViewItem(origin, menu, visibleSize, 1, "muffins01.png", "10 Muffins", "Test refund of an item", 0.99);
	createListViewItem(origin, menu, visibleSize, 2, "muffins02.png", "50 Muffins", "Test cancellation of an item", 1.99);
	createListViewItem(origin, menu, visibleSize, 3, "muffins03.png", "400 Muffins", "Test purchase of an item", 4.99);
	createListViewItem(origin, menu, visibleSize, 4, "muffins04.png", "1000 Muffins", "Test item unavailable", 8.99);

	menu->setPosition(CCPointZero);
	this->addChild(menu);

	this->setKeypadEnabled(true);

	return true;
}

void StoreBScene::menuBackCallback(CCObject* pSender)
{
	CCScene *s = StoreAScene::scene();
	CCDirector::sharedDirector()->setDepthTest(true);
	CCTransitionScene *transition = CCTransitionMoveInL::create(0.8f, s);
	
	CCDirector::sharedDirector()->replaceScene(transition);
}


void StoreBScene::menuChooseCallback(CCObject* pSender)
{
	if (pSender)
	{
		GameMenuItem* item = (GameMenuItem*)pSender;

		int tag = item->getTag();
		char productId[512];
		snprintf(productId, sizeof(productId), productIdFromTag(tag));
		try{
			if (tag == 0) {
				
			} else {
				cocos2dx_StoreController::buyCurrencyPack(productId);
			}
		}
		catch (cocos2dx_VirtualItemNotFoundException& e) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			__android_log_write(ANDROID_LOG_ERROR, "SOOMLA JNI", "Cought cocos2dx_VirtualItemNotFoundException from NATIVE!");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            iOSHelper::LogMessage("Cought cocos2dx_VirtualItemNotFoundException!");
#endif
		}
	}
}

void StoreBScene::setCurrencyBalanceLabel() {
	int balance = 0;
	try{
		balance = cocos2dx_StoreInventory::getCurrencyBalance("currency_muffin");
	} catch (cocos2dx_VirtualItemNotFoundException& e) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            __android_log_write(ANDROID_LOG_ERROR, "SOOMLA JNI", "getCurrencyBalance Cought cocos2dx_VirtualItemNotFoundException from NATIVE!"); 
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            iOSHelper::LogMessage("getCurrencyBalance Cought cocos2dx_VirtualItemNotFoundException!");
#endif
	}

	ostringstream convert;
	convert << balance;
	
	pLabelBalance->setString(convert.str().c_str());
}

void StoreBScene::createListViewItem(CCPoint& origin, CCMenu* menu, CCSize& visibleSize, int tag, const char* img, const char* name, const char* info, float price) {
	GameMenuItem *pChooseItem = GameMenuItem::itemWithLabel(
		CCSprite::create("button.png"),
					this,
					menu_selector(StoreBScene::menuChooseCallback));

	float yOffset = - 200;

	pChooseItem->setPosition(ccp(origin.x + visibleSize.width/2, yOffset + origin.y + visibleSize.height - 100 - (tag * pChooseItem->boundingBox().size.height)));
	pChooseItem->setTag(tag);


	CCSprite* pSpritePic = CCSprite::create(img);
	pSpritePic->setPosition(ccp(pSpritePic->boundingBox().size.width/2 + 20, pChooseItem->boundingBox().size.height/2));
	pChooseItem->addChild(pSpritePic, 0);


	CCLabelTTF* pLabelName = CCLabelTTF::create(name, "GoodDog.otf", 44);
	pLabelName->setColor(ccc3(0,0,0));
	pLabelName->setPosition(ccp(pSpritePic->getPositionX() + (pSpritePic->boundingBox().size.width / 2) + (pLabelName->boundingBox().size.width / 2) + 20 , pChooseItem->boundingBox().size.height/2));
	pChooseItem->addChild(pLabelName);


	CCLabelTTF* pLabelInfo = CCLabelTTF::create(info, "GoodDog.otf", 20);
	pLabelInfo->setColor(ccc3(50,50,50));
	pLabelInfo->setPosition(ccp(pSpritePic->getPositionX() + (pSpritePic->boundingBox().size.width / 2) + (pLabelInfo->boundingBox().size.width / 2) + 20 , -50 + (pChooseItem->boundingBox().size.height/2)));
	pChooseItem->addChild(pLabelInfo);


	CCLabelTTF* pLabelClickToBuy = CCLabelTTF::create("Click to Buy", "GoodDog.otf", 24);
	pLabelClickToBuy->setColor(ccc3(0,255,255));
	pLabelClickToBuy->setPosition(ccp(pChooseItem->boundingBox().size.width - (pLabelClickToBuy->boundingBox().size.width / 2) - 20 , 60 + pChooseItem->boundingBox().size.height/2));
	pChooseItem->addChild(pLabelClickToBuy);

	char buffer[512];

	snprintf(buffer, sizeof(buffer), "price: $%.2f",  price);

	CCLabelTTF* pLabelPriceBalance = CCLabelTTF::create(buffer, "GoodDog.otf", 24);
	pLabelPriceBalance->setColor(ccc3(0,255,255));
	pLabelPriceBalance->setPosition(ccp(pChooseItem->boundingBox().size.width - (pLabelPriceBalance->boundingBox().size.width / 2) - 20 , 60));
	pChooseItem->addChild(pLabelPriceBalance);

	menu->addChild(pChooseItem, 1);
}

const char* StoreBScene::productIdFromTag(int tag) {
	switch (tag)
	{
	case 0: return "no_ads";
	    break;
	case 1: return "android.test.refunded";
	    break;
	case 2: return "android.test.canceled";
	    break;
	case 3: return "android.test.purchased";
	    break;
	case 4: return "android.test.item_unavailable";
		break;
	default: return "ERROR";
		break;
	}
	
	return "ERROR";
}


