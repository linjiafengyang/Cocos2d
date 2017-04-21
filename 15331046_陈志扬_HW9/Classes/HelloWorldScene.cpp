#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

    // add a "music" icon to play the music
    auto musicItem = MenuItemImage::create(
                                           "music0.jpg",
                                           "music1.jpg",
                                           CC_CALLBACK_1(HelloWorld::click, this));
    
    musicItem->setPosition(Vec2(origin.x + visibleSize.width - musicItem->getContentSize().width / 2,
                                origin.y + musicItem->getContentSize().height / 2));

    // create menu, it's an autorelease object
    menu = Menu::create(musicItem, NULL);
    menu->setPosition(Vec2(0, 280));
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

	// add a click label shows "Click me!"
	auto label = Label::createWithTTF("Click me!", "fonts/LHANDW.TTF", 20);
	// the position of the label
	label->setPosition(Vec2(origin.x + visibleSize.width - musicItem->getContentSize().width * 1.5,
		origin.y + visibleSize.height - label->getContentSize().height * 2));
	// the color of the label
	label->setColor(ccc3(255, 0, 0));
	this->addChild(label, 1);

    
	// create a dictionary with a plist file
	auto *chnStrings = Dictionary::createWithContentsOfFile("myName.xml");

	// add a name label shows "³ÂÖ¾Ñï"
	const char *name = ((String*)chnStrings->objectForKey("myName"))->getCString();
	// this is also available
	// auto nameLabel = Label::create(name, "Arial", 26);
	auto nameLabel = Label::createWithTTF(name, "fonts/simkai.ttf", 26);

	// the position of the name label
	nameLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - nameLabel->getContentSize().height));
	// the color of the label
	nameLabel->setColor(ccc3(135, 206, 250));
	// add the label as a child to this layer
	this->addChild(nameLabel, 1);

	// add a id label shows "15331046"
	const char *id = ((String*)chnStrings->objectForKey("studentID"))->getCString();
    auto idLabel = Label::createWithTTF(id, "fonts/simkai.ttf", 24);
    
    // position the label on the center of the screen
    idLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - idLabel->getContentSize().height * 2));

    // add the label as a child to this layer
    this->addChild(idLabel, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("sysu.jpg");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}

void HelloWorld::click(cocos2d::Ref * pSender)
{
	if (!isClicked) {
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		// add a sprite
		auto sprite = Sprite::create("Alan-Walker.jpg");
		// position the sprite on the center of the screen
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(sprite, 0);

		// play the music
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/Fade.mp3", true);
		isClicked = true;
	}
}
