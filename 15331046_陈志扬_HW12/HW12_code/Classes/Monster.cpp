#include"Monster.h"
USING_NS_CC;

Factory* Factory::factory = NULL;
Factory::Factory() {
	initSpriteFrame();
}
Factory* Factory::getInstance() {
	if (factory == NULL) {
		factory = new Factory();
	}
	return factory;
}
void Factory::initSpriteFrame(){
	auto texture = Director::getInstance()->getTextureCache()->addImage("Monster.png");
	monsterDead.reserve(4);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(258-48*i,0,42,42)));
		monsterDead.pushBack(frame);
	}
}

Sprite* Factory::createMonster() {
	Sprite* mons = Sprite::create("Monster.png", CC_RECT_PIXELS_TO_POINTS(Rect(364,0,42,42)));
	monster.pushBack(mons);
	return mons;
}

void Factory::removeMonster(Sprite* sp) {
	Animation* animation = Animation::createWithSpriteFrames(monsterDead, 0.1f);
	Animate* animate = Animate::create(animation);
	Sequence* seq = Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp)), NULL);
	sp->runAction(seq);
	monster.eraseObject(sp);
}
void Factory::moveMonster(Vec2 playerPos,float time){
	for (int i = 0; i < monster.size(); i++) {
		Vec2 position = monster.at(i)->getPosition();
		Vec2 direction = playerPos - position;
		direction.normalize();
		monster.at(i)->runAction(MoveBy::create(time, direction * 30));
	}
}

Sprite* Factory::collider(Rect rect) {
	for (int i = 0; i < monster.size(); i++) {
		Vec2 position = monster.at(i)->getPosition();
		if (rect.containsPoint(position)) {
			return monster.at(i);
		}
	}
	return NULL;
}