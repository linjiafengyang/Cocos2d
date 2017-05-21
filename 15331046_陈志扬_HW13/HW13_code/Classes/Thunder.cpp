#include "Thunder.h"
#include <algorithm>

USING_NS_CC;

using namespace CocosDenshion;

Scene* Thunder::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Thunder::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Thunder::init() {
	if (!Layer::init()) {
		return false;
	}

	isMove = false;  // �Ƿ����ɴ�
	stoneType = 0;// �������µ�һ��ʱ��Ϊѡ���ǣ�1,2,3��
	visibleSize = Director::getInstance()->getVisibleSize();

	// ��������
	auto bgsprite = Sprite::create("bg.jpg");
	bgsprite->setPosition(visibleSize / 2);
	bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width,
						visibleSize.height / bgsprite->getContentSize().height);
	this->addChild(bgsprite, 0);

	// �����ɴ�
	player = Sprite::create("player.png");
	player->setAnchorPoint(Vec2(0.5, 0.5));
	player->setPosition(visibleSize.width / 2, player->getContentSize().height);
	player->setName("player");
	this->addChild(player, 1);

	// ��ʾ��ʯ���ӵ�����
	enemysNum = Label::createWithTTF("enemys: 0", "fonts/arial.TTF", 20);
	enemysNum->setColor(Color3B(255, 255, 255));
	enemysNum->setPosition(50, 60);
	this->addChild(enemysNum, 3);
	bulletsNum = Label::createWithTTF("bullets: 0", "fonts/arial.TTF", 20);
	bulletsNum->setColor(Color3B(255, 255, 255));
	bulletsNum->setPosition(50, 30);
	this->addChild(bulletsNum, 3);

	addEnemy(5);        // ��ʼ����ʯ
	preloadMusic();     // Ԥ��������
	playBgm();          // ���ű�������
	explosion();        // ������ը֡����

	// ��Ӽ�����
	addTouchListener();
	addKeyboardListener();
	addCustomListener();

	// ������
	schedule(schedule_selector(Thunder::update), 0.04f, kRepeatForever, 0);

	return true;
}

//Ԥ���������ļ�
void Thunder::preloadMusic() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("music/bgm.mp3");
	audio->preloadEffect("music/fire.wav");
	audio->preloadEffect("music/explore.wav");
}

//���ű�������
void Thunder::playBgm() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("music/bgm.mp3", true);
}

//��ʼ����ʯ
void Thunder::addEnemy(int n) {
	enemys.clear();
	for (unsigned i = 0; i < 3; ++i) {
		char enemyPath[20];
		sprintf(enemyPath, "stone%d.png", 3 - i);
		double width = visibleSize.width / (n + 1.0),
				height = visibleSize.height - (50 * (i + 1));
		for (int j = 0; j < n; ++j) {
			auto enemy = Sprite::create(enemyPath);
			enemy->setAnchorPoint(Vec2(0.5, 0.5));
			enemy->setScale(0.5, 0.5);
			enemy->setPosition(width * (j + 1), height);
			enemys.push_back(enemy);
			addChild(enemy, 1);
		}
	}
}

// ��ʯ�����ƶ��������µ�һ��(�ӷ���)
void Thunder::newEnemy() {
	// ����enemys�������գ���������һ��
	for (auto enemy : enemys) {
		if (enemy != nullptr)
			enemy->setPosition(enemy->getPosition() + Vec2(0, -50));
	}
	char enemyPath[20];
	sprintf(enemyPath, "stone%d.png", stoneType + 1);// ѡ��stone����
	stoneType = (stoneType + 1) % 3;// 1,2,3ѭ��
	double width = visibleSize.width / 6.0,
		height = visibleSize.height - 50;
	for (int j = 0; j < 5; ++j) {
		auto enemy = Sprite::create(enemyPath);
		enemy->setAnchorPoint(Vec2(0.5, 0.5));
		enemy->setScale(0.5, 0.5);
		enemy->setPosition(width * (j + 1) - 80, height);
		enemys.push_back(enemy);
		addChild(enemy, 1);
	}
}

// �ƶ��ɴ�
void Thunder::movePlane(char c) {
	switch (movekey) {
	case 'A':
		// ���÷ɴ��Ƴ���߽�
		if (player->getPositionX() - 30 > Director::getInstance()->getVisibleOrigin().x)
			player->runAction(MoveBy::create(0.08, Vec2(-10, 0)));
		break;
	case 'D':
		// ���÷ɴ��Ƴ��ұ߽�
		if (player->getPositionX() + 30 < Director::getInstance()->getVisibleOrigin().x + visibleSize.width)
			player->runAction(MoveBy::create(0.08, Vec2(10, 0)));
		break;
	}
}

//�����ӵ�
void Thunder::fire() {
	auto bullet = Sprite::create("bullet.png");
	bullet->setAnchorPoint(Vec2(0.5, 0.5));
	bullets.push_back(bullet);
	bullet->setPosition(player->getPosition());
	addChild(bullet, 1);
	SimpleAudioEngine::getInstance()->playEffect("music/fire.wav", false);

	// �Ƴ��ɳ���Ļ����ӵ�
	bullet->runAction(
		Sequence::create(
			MoveBy::create(1.0f, Vec2(0, visibleSize.height)),
			CallFuncN::create(CC_CALLBACK_1(Thunder::removeBullet, this)),
			nullptr
		)
	);
}

// �Ƴ��ӵ�
void Thunder::removeBullet(Node* bullet) {
	bullet->removeFromParentAndCleanup(true);
	bullets.remove((Sprite*)bullet);
}

// �иը����֡
void Thunder::explosion() {
	auto texture = Director::getInstance()->getTextureCache()->addImage("explosion.png");
	explore.reserve(8);
	for (int i = 0; i < 5; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(188.8 * i, 0, 188.8, 160)));
		explore.pushBack(frame);
	}
	for (int i = 5; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(188.8 * (i - 5), 160, 188.8, 160)));
		explore.pushBack(frame);
	}
}

void Thunder::update(float f) {
	// ʵʱ����ҳ������ʯ���ӵ�����(����ɾ��)
	// Ҫ��������ʾ��ȷ(�ӷ���)
	char str[15];
	sprintf(str, "enemys: %d", enemys.size());
	enemysNum->setString(str);
	sprintf(str, "bullets: %d", bullets.size());
	bulletsNum->setString(str);

	// �ɴ��ƶ�
	if (isMove)
		this->movePlane(movekey);

	static int ct = 0;
	static int dir = 4;
	++ct;
	if (ct == 120)
		ct = 40, dir = -dir;
	else if (ct == 80) {
		dir = -dir;
		newEnemy();  // ��ʯ�����ƶ��������µ�һ��(�ӷ���)
	}
	else if (ct == 20)
		ct = 40, dir = -dir;

	//��ʯ�����ƶ�
	for (Sprite* s : enemys) {
		if (s != NULL) {
			s->setPosition(s->getPosition() + Vec2(dir, 0));
		}
	}

	// �ַ��Զ����¼�
	EventCustom e("meet");
	_eventDispatcher->dispatchEvent(&e);
}

// �Զ�����ײ�¼�
void Thunder::meet(EventCustom * event) {
	// �ж��ӵ��Ƿ������ʯ��ִ�ж�Ӧ����
	for (list<Sprite*>::iterator b = bullets.begin(); b != bullets.end(); ) {
		bool isHit = false;// ��Ǵ���
		for (auto enemy : enemys) {
			if ((*b)->getPosition().getDistance(enemy->getPosition()) < 25) {
				Sprite* temp = enemy;
				enemy->runAction(
					Sequence::create(
						Animate::create(Animation::createWithSpriteFrames(explore, 0.05f, 1)),
						CallFunc::create([temp] {
							temp->removeFromParentAndCleanup(true);
						}),
						nullptr
					)
				);
				isHit = true;// ��ΪTrue
				SimpleAudioEngine::getInstance()->playEffect("music/explore.wav", false);
				enemys.remove(enemy);
				break;
			}
		}
		// true�����У��Ƴ��ӵ�
		if (isHit == true) {
			(*b)->removeFromParentAndCleanup(true);
			b = bullets.erase(b);
		}
		else {
			b++;
		}
	}

	// �ж���Ϸ�Ƿ������ִ�ж�Ӧ����
	for (auto enemy : enemys) {
		if (enemy->getPositionY() < 100) {
			auto gameOver = Sprite::create("gameOver.png");
			gameOver->setAnchorPoint(Vec2(0.5, 0.5));
			gameOver->setPosition(visibleSize.width / 2, visibleSize.height / 2);
			this->addChild(gameOver, 2);

			Sprite* temp = player;
			player->runAction(
				Sequence::create(
					Animate::create(Animation::createWithSpriteFrames(explore, 0.05f, 1)),
					CallFunc::create([temp] {
						temp->removeFromParentAndCleanup(true);
					}),
					nullptr
				)
			);
			unschedule(schedule_selector(Thunder::update));// ֹͣ��ʱ��
			this->getEventDispatcher()->removeAllEventListeners();// �Ƴ�������
		}
	}
	
}

// ����Զ��������
void Thunder::addCustomListener() {
	auto customListener = EventListenerCustom::create("meet", CC_CALLBACK_1(Thunder::meet, this));
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(customListener, this);
}

// ��Ӽ����¼�������
void Thunder::addKeyboardListener() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(Thunder::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(Thunder::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void Thunder::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_A:
		movekey = 'A';
		isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_D:
		movekey = 'D';
		isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		fire();
		break;
	}
}

void Thunder::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
  switch (code) {
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
  case EventKeyboard::KeyCode::KEY_CAPITAL_A:
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
  case EventKeyboard::KeyCode::KEY_CAPITAL_D:
    isMove = false;
    break;
  }
}

// ��Ӵ����¼�������
void Thunder::addTouchListener() {
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Thunder::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Thunder::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Thunder::onTouchMoved, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

// ����������ڵ�
bool Thunder::onTouchBegan(Touch *touch, Event *event) {
	isClick = true;
	fire();
	return true;
}

void Thunder::onTouchEnded(Touch *touch, Event *event) {
	isClick = false;
}

// ����갴ס�ɴ���ɿ��Ʒɴ��ƶ� (�ӷ���)
void Thunder::onTouchMoved(Touch *touch, Event *event) {
	Vec2 delta = touch->getDelta();
	double playerPositionX = player->getPositionX();
	double playerPositionY = player->getPositionY();
	double newPositionX = playerPositionX + delta.x;
	Vec2 newPosition = Vec2(newPositionX, playerPositionY);// ע��playerPositionY�������ɴ�ֻ����ˮƽ�����ƶ�
	// ���Ʒɴ��ƶ���Χ
	if (newPositionX - 30 > Director::getInstance()->getVisibleOrigin().x
		&& newPositionX + 30 < Director::getInstance()->getVisibleOrigin().x + visibleSize.width) {
		player->setPosition(newPosition);
	}
}
