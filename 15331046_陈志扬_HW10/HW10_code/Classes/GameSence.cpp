#include "GameSence.h"

USING_NS_CC;

Scene* GameSence::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameSence::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameSence::init()
{

	if (!Layer::init())
	{
		return false;
	}

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add the background of the game
	auto level = Sprite::create("level-background-0.jpg");
	level->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(level, 0);

	// add the stone layer
	stoneLayer = Layer::create();
	stoneLayer->setAnchorPoint(Vec2(0, 0));
	stoneLayer->setPosition(0, 0);
	// add the stone sprite
	stone = Sprite::create("stone.png");
	stone->setPosition(Vec2(560, 480));
	stoneLayer->addChild(stone);
	this->addChild(stoneLayer, 1);

	// add the mouse layer
	mouseLayer = Layer::create();
	mouseLayer->setAnchorPoint(Vec2(0, 0));
	mouseLayer->setPosition(Vec2(0, visibleSize.height / 2));
	// add the mouse sprite
	// includes its animation
	mouse = Sprite::createWithSpriteFrameName("gem-mouse-0.png");
	Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	mouse->runAction(RepeatForever::create(mouseAnimate));
	mouse->setPosition(Vec2(visibleSize.width / 2, 0));
	mouseLayer->addChild(mouse);
	this->addChild(mouseLayer, 1);

	// add the "Shoot" label with MenuItemFont Class
	auto shoot = MenuItemFont::create("Shoot", CC_CALLBACK_1(GameSence::shootMenuCallback, this));
	shoot->setFontSizeObj(70);
	shoot->setColor(Color3B(255, 255, 255));
	shoot->setPosition(Vec2(visibleSize.width / 2 + origin.x + 330, visibleSize.height / 2 + origin.y + 160));
	// add the shoot menu
	auto menu = Menu::create(shoot, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {

	auto location = touch->getLocation();
	// add the cheese sprite
	auto cheese = Sprite::create("cheese.png");
	cheese->setPosition(location);
	this->addChild(cheese, 0);

	// cheese fade out after 8 seconds
	auto cheeseFadeOut = FadeOut::create(8);
	cheese->runAction(cheeseFadeOut);

	// convert the touched location to the node space coordinates
	// move the mouse to the node location
	auto location_node = mouseLayer->convertToNodeSpace(location);
	auto mouseMoveTo = MoveTo::create(2, location_node);
	// 放大和缩小
	auto mouseScaleTo = ScaleTo::create(0.5, 2);
	auto mouseScaleBack = ScaleTo::create(0.5, 1);
	// 序列动作
	auto seq = Sequence::create(mouseMoveTo, mouseScaleTo, mouseScaleBack, nullptr);
	mouse->runAction(seq);

	return true;
}

void GameSence::shootMenuCallback(Ref * pSender)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// when shot, the mouse moves to another location randomly
	int random_width = CCRANDOM_0_1() * visibleSize.width;
	int random_height = CCRANDOM_0_1() * visibleSize.height;
	auto location = mouseLayer->convertToNodeSpace(Vec2(random_width, random_height));
	auto mouseMoveTo = MoveTo::create(1, location);
	// rotate itself
	auto mouseRotateTo = RotateBy::create(1, 360);
	// let the two actions run at the same time
	auto spa = Spawn::createWithTwoActions(mouseMoveTo, mouseRotateTo);
	mouse->runAction(spa);

	// convert the position of mouse to world space coordinates
	auto position = mouse->getPosition();
	auto position_world = mouseLayer->convertToWorldSpace(position);

	// add the diamond sprite to the world position of the previous mouse
	auto diamond = Sprite::create("diamond.png");
	diamond->setPosition(position_world);
	this->addChild(diamond, 1);

	// add the stone and move it to the world position of the previous mouse
	auto stone = Sprite::create("stone.png");
	stone->setPosition(Vec2(560, 480));
	this->addChild(stone, 1);
	auto stoneMoveTo = MoveTo::create(1, position_world);
	stone->runAction(stoneMoveTo);
	// stone fades out after 3 seconds
	auto stoneFadeOut = FadeOut::create(3);
	stone->runAction(stoneFadeOut);
}
