#ifndef __FRIEND_SHIP_H__
#define __FRIEND_SHIP_H__

#include "cocos2d.h"
using namespace std;
USING_NS_CC;
class FriendShip : public Layer {
public:
  static PhysicsWorld* world;
  static cocos2d::Scene* createScene();
  void setPhysicsWorld(PhysicsWorld * world);
  virtual bool init();

  void addSprite();   // 添加背景和各种精灵
  void addPlayer();   // 添加玩家
  void addListener(); // 添加监听器

  void update(float dt);
  void updateShip(float dt);  // 更新船的平衡情况
  void boxFall(float dt);  // 掉落箱子

  void onKeyPressed(EventKeyboard::KeyCode code, Event * event);
  void onKeyReleased(EventKeyboard::KeyCode code, Event * event);
  bool onConcactBegin(PhysicsContact & contact);

  void preloadMusic();                   // 预加载音乐
  void GameOver();                      // 游戏结束
  void loadAnimation(string filepath); // 加载动画

  // 重玩或退出按钮响应函数
  void replayCallback(Ref * pSender);
  void exitCallback(Ref * pSender);

  // implement the "static create()" method manually
  CREATE_FUNC(FriendShip);

private:
  PhysicsWorld* m_world;
  Size visibleSize;
  Vec2 origin;

  Sprite* ship;
  list<PhysicsBody*> boxes;

  Node* edgeNode;
  Node* holdbox1;
  Node* holdbox2;

  Sprite* player1;
  Sprite* player2;

  SpriteFrame* frame1;
  SpriteFrame* frame2;
  SpriteFrame* IdleWithBox1;
  SpriteFrame* IdleWithBox2;

  // 固定距离关节, 用于举起箱子
  PhysicsJointDistance * joint1;

  char LastPlayer1Press;

  bool IsPlayer1Hold;
  bool IsPlayer1Jump;
  bool IsPlayer1Left;
  bool IsPlayer1Right;

  double deltaH;  // 每次更新的高度变化量
  double height;  // 左右高度差
};

#endif // __FRIEND_SHIP_H__
