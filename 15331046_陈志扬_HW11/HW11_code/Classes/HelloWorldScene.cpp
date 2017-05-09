#include "HelloWorldScene.h"
#pragma execution_character_set("utf-8")
USING_NS_CC;

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

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	// 设置背景并自适应屏幕
	auto background = Sprite::create("background.jpg");
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	Size size = Director::getInstance()->getWinSize();
	float sizeW = size.width;
	float sizeH = size.height;
	float spX = background->getTextureRect().getMaxX();
	float spY = background->getTextureRect().getMaxY();
	background->setScaleX(sizeW / spX);
	background->setScaleY(sizeH / spY);
	addChild(background, 0);

	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height/2));
	addChild(player, 3);

	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用hp条设置progressBar
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(origin.x+14*pT->getContentSize().width,origin.y + visibleSize.height - 2*pT->getContentSize().height));
	addChild(pT,1);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0,0);

	// 字体设置
	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 36;

	// 时间显示
	dtime = 180;
	char* strTime = new char[4];
	sprintf(strTime, "%d", dtime);
	time = Label::createWithTTF(ttfConfig, strTime);
	time->setColor(Color3B(255, 255, 255));
	time->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - time->getContentSize().height));
	addChild(time);
	// 定时器
	schedule(schedule_selector(HelloWorld::update), 1.0f, kRepeatForever, 0);

	// 静态动画
	idle.reserve(1);
	idle.pushBack(frame0);
	
	// 攻击动画
	attack.reserve(17);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113*i,0,113,113)));
		attack.pushBack(frame);
	}
	attack.pushBack(frame0);

	// 可以仿照攻击动画
	// 死亡动画(帧数：22帧，高：90，宽：79）
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
	dead.reserve(22);
	for (int i = 0; i < 22; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
		dead.pushBack(frame);
	}
	dead.pushBack(frame0);
	
	// 运动动画(帧数：8帧，高：101，宽：68）
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
	run.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
		run.pushBack(frame);
	}

	// add the "a" label with MenuItemFont Class
	auto a = Label::createWithTTF(ttfConfig, "A");
	auto a_item = MenuItemLabel::create(a, CC_CALLBACK_1(HelloWorld::wasd_move, this, 'A'));
	a_item->setPosition(Vec2(origin.x + a_item->getContentSize().width, origin.y + a_item->getContentSize().height));
	// add the a menu
	auto a_menu = Menu::create(a_item, NULL);
	a_menu->setPosition(Vec2::ZERO);
	this->addChild(a_menu, 1);
	/*
	auto a = MenuItemFont::create("A", CC_CALLBACK_1(HelloWorld::wasd_move, this));
	a->setFontNameObj("fonts/arial.ttf");
	a->setFontSizeObj(36);
	a->setColor(Color3B(255, 255, 255));
	a->setPosition(Vec2(origin.x + a->getContentSize().width, origin.y + a->getContentSize().height));
	// add the a menu
	auto menu0 = Menu::create(a, NULL);
	menu0->setPosition(Vec2::ZERO);
	this->addChild(menu0, 1);
	*/

	// add the "s" label with MenuItemFont Class
	auto s = Label::createWithTTF(ttfConfig, "S");
	auto s_item = MenuItemLabel::create(s, CC_CALLBACK_1(HelloWorld::wasd_move, this, 'S'));
	s_item->setPosition(Vec2(origin.x + s_item->getContentSize().width * 2.7, origin.y + s_item->getContentSize().height));
	// add the s menu
	auto s_menu = Menu::create(s_item, NULL);
	s_menu->setPosition(Vec2::ZERO);
	this->addChild(s_menu, 1);
	
	// add the "d" label with MenuItemFont Class
	auto d = Label::createWithTTF(ttfConfig, "D");
	auto d_item = MenuItemLabel::create(d, CC_CALLBACK_1(HelloWorld::wasd_move, this, 'D'));
	d_item->setPosition(Vec2(origin.x + d_item->getContentSize().width * 4, origin.y + d_item->getContentSize().height));
	// add the d menu
	auto d_menu = Menu::create(d_item, NULL);
	d_menu->setPosition(Vec2::ZERO);
	this->addChild(d_menu, 1);

	// add the "w" label with MenuItemFont Class
	auto w = Label::createWithTTF(ttfConfig, "W");
	auto w_item = MenuItemLabel::create(w, CC_CALLBACK_1(HelloWorld::wasd_move, this, 'W'));
	w_item->setPosition(Vec2(origin.x + w_item->getContentSize().width * 1.8, origin.y + w_item->getContentSize().height * 2));
	// add the w menu
	auto w_menu = Menu::create(w_item, NULL);
	w_menu->setPosition(Vec2::ZERO);
	this->addChild(w_menu, 1);

	// add the "X" label with MenuItemFont Class
	auto x = Label::createWithTTF(ttfConfig, "X");
	auto x_item = MenuItemLabel::create(x, CC_CALLBACK_1(HelloWorld::xy_action, this, 'X'));
	x_item->setPosition(Vec2(origin.x + visibleSize.width - x_item->getContentSize().width, w_item->getPositionY()));
	// add the x menu
	auto x_menu = Menu::create(x_item, NULL);
	x_menu->setPosition(Vec2::ZERO);
	this->addChild(x_menu, 1);

	// add the "Y" label with MenuItemFont Class
	auto y = Label::createWithTTF(ttfConfig, "Y");
	auto y_item = MenuItemLabel::create(y, CC_CALLBACK_1(HelloWorld::xy_action, this, 'Y'));
	y_item->setPosition(Vec2(x_item->getPositionX() - y_item->getContentSize().width, a_item->getPositionY()));
	// add the y menu
	auto y_menu = Menu::create(y_item, NULL);
	y_menu->setPosition(Vec2::ZERO);
	this->addChild(y_menu, 1);

	return true;
}

void HelloWorld::wasd_move(Ref * pSender, char option)
{
	if (isAction) return;
	auto begin = CallFuncN::create([&](Ref* sender) {
		isAction = true;
	});
	auto end = CallFuncN::create([&](Ref* sender) {
		isAction = false;
	});

	// 创建一个Animation，参数：SpriteFrame*的Vector容器，每一帧之间的间隔
	auto runAnimation = Animation::createWithSpriteFrames(run, 0.1f);
	//runAnimation->setRestoreOriginalFrame(false);
	// 使用animation创建一个animate，animate继承了ActionInterval，可以当做动作来使用
	auto runAnimate = Animate::create(runAnimation);

	auto position_x = player->getPositionX();
	auto position_y = player->getPositionY();

	switch (option)
	{
		case 'A':
		{
			if (position_x <= origin.x + 45) break;
			auto moveBy = MoveBy::create(0.3, Point(-30, 0));
			auto seq = Sequence::create(begin, runAnimate, end, NULL);
			auto spawn = Spawn::createWithTwoActions(seq, moveBy);
			player->runAction(spawn);
			break;
		}
		case 'S':
		{
			if (position_y <= origin.y + 48) break;
			auto moveBy = MoveBy::create(0.3, Point(0, -30));
			auto seq = Sequence::create(begin, runAnimate, end, NULL);
			auto spawn = Spawn::createWithTwoActions(seq, moveBy);
			player->runAction(spawn);
			break;
		}
		case 'D':
		{
			if (position_x >= visibleSize.width + origin.x - 45) break;
			auto moveBy = MoveBy::create(0.3, Point(30, 0));
			auto seq = Sequence::create(begin, runAnimate, end, NULL);
			auto spawn = Spawn::createWithTwoActions(seq, moveBy);
			player->runAction(spawn);
			break;
		}
		case 'W':
		{
			if (position_y >= visibleSize.height + origin.y - 48) break;
			auto moveBy = MoveBy::create(0.3, Point(0, 30));
			auto seq = Sequence::create(begin, runAnimate, end, NULL);
			auto spawn = Spawn::createWithTwoActions(seq, moveBy);
			player->runAction(spawn);
			break;
		}
	}
}

void HelloWorld::xy_action(Ref * pSender, char option)
{
	if (isAction) return;
	auto begin = CallFuncN::create([&](Ref* sender) {
		isAction = true;
	});
	auto end = CallFuncN::create([&](Ref* sender) {
		isAction = false;
	});
	switch (option)
	{
		case 'X':
		{
			// 创建一个Animation，参数：SpriteFrame*的Vector容器，每一帧之间的间隔
			auto deadAnimation = Animation::createWithSpriteFrames(dead, 0.1f);
			//deadAnimation->setRestoreOriginalFrame(true);
			// 使用animation创建一个animate，animate继承了ActionInterval，可以当做动作来使用
			auto deadAnimate = Animate::create(deadAnimation);
			auto seq = Sequence::create(begin, deadAnimate, end, NULL);
			player->runAction(seq);
			// 以1/5的速度减血条
			if (pT->getPercentage() >= 20)
				pT->runAction(CCProgressFromTo::create(1.5f, pT->getPercentage(), pT->getPercentage() - 20));
			else
				pT->runAction(CCProgressFromTo::create(1.5f, pT->getPercentage(), 0));
			break;
		}
		case 'Y': 
		{
			// 创建一个Animation，参数：SpriteFrame*的Vector容器，每一帧之间的间隔
			auto attackAnimation = Animation::createWithSpriteFrames(attack, 0.1f);
			//attackAnimation->setRestoreOriginalFrame(true);
			// 使用animation创建一个animate，animate继承了ActionInterval，可以当做动作来使用
			auto attackAnimate = Animate::create(attackAnimation);
			auto seq = Sequence::create(begin, attackAnimate, end, NULL);
			player->runAction(seq);
			// 以1/5的速度加血条
			if (pT->getPercentage() <= 80)
				pT->runAction(CCProgressFromTo::create(1.5f, pT->getPercentage(), pT->getPercentage() + 20));
			else
				pT->runAction(CCProgressFromTo::create(1.5f, pT->getPercentage(), 100));
			break;
		}
	}
}

void HelloWorld::update(float dt)
{
	if (dtime == 0) {
		unschedule(schedule_selector(HelloWorld::update));
	}
	else {
		dtime = dtime - dt;
		char* strTime = new char[4];
		sprintf(strTime, "%d", dtime);
		time->setString(strTime);
	}
}
