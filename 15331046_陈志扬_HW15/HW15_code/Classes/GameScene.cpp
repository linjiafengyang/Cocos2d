#include "GameScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>

#define database UserDefault::getInstance()

using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

USING_NS_CC;

cocos2d::Scene* GameScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameScene::init() {
    if (!Layer::init())
    {
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;

    score_field = TextField::create("Score", "Arial", 30);
    score_field->setPosition(Size(visibleWidth / 4, visibleHeight / 4 * 3));
    this->addChild(score_field, 2);

    submit_button = Button::create();
    submit_button->setTitleText("Submit");
    submit_button->setTitleFontSize(30);
    submit_button->setPosition(Size(visibleWidth / 4, visibleHeight / 4));
    this->addChild(submit_button, 2);
	submit_button->addClickEventListener(CC_CALLBACK_1(GameScene::submit_button_click, this));

    rank_field = TextField::create("", "Arial", 30);
    rank_field->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4 * 3));
    this->addChild(rank_field, 2);

    rank_button = Button::create();
    rank_button->setTitleText("Rank");
    rank_button->setTitleFontSize(30);
    rank_button->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4));
    this->addChild(rank_button, 2);
	rank_button->addClickEventListener(CC_CALLBACK_1(GameScene::rank_button_click, this));

    return true;
}

void GameScene::submit_button_click(Ref *pSender) {
	bool temp = true;
	if (score_field->getString().length() > 0) {
		for (int i = 0; i < score_field->getString().length(); i++) {
			// 判断score是否是数字
			if (!isdigit(score_field->getString()[i])) {
				temp = false;
			}
		}
		if (temp) {
			HttpRequest *request = new HttpRequest();
			request->setRequestType(HttpRequest::Type::POST);
			request->setUrl("http://localhost:8080/submit");
			request->setResponseCallback(CC_CALLBACK_2(GameScene::onHttpRequestCompleted_submit, this));

			string score = "score=" + score_field->getString();
			const char* postData = score.c_str();
			request->setRequestData(postData, strlen(postData));

			vector<string> headers;
			headers.push_back("Cookie:GAMESESSIONID=" + Global::gameSessionId);
			request->setHeaders(headers);

			//cocos2d::network::HttpClient::getInstance()->enableCookies(NULL);
			cocos2d::network::HttpClient::getInstance()->send(request);
			request->release();
		}
	}
}

void GameScene::onHttpRequestCompleted_submit(HttpClient *sender, HttpResponse *response) {
	if (!response) return;
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	string responseData = Global::toString(response->getResponseData());
	string responseHeader = Global::toString(response->getResponseHeader());

	rapidjson::Document d;
	d.Parse<0>(responseData.c_str());
	// 解析错误
	if (d.HasParseError()) {
		CCLOG("GetParseError %s\n", d.GetParseError());
	}
	// submit成功
	if (d.IsObject() && d.HasMember("result") && d["result"].GetBool()) {
		// 获取最高分数
		if (d.HasMember("info")) {
			// 获取最高分数填入输入框中
			score_field->setText(d["info"].GetString());
			// 保存header和body
			database->setStringForKey("header", responseHeader);
			database->setStringForKey("body", responseData);
		}
	}
}

void GameScene::rank_button_click(Ref *pSender) {
	HttpRequest *request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::GET);
	request->setUrl("http://localhost:8080/rank?top=10");
	request->setResponseCallback(CC_CALLBACK_2(GameScene::onHttpRequestCompleted_rank, this));

	vector<string> headers;
	headers.push_back("Cookie:GAMESESSIONID=" + Global::gameSessionId);
	request->setHeaders(headers);

	//cocos2d::network::HttpClient::getInstance()->enableCookies(NULL);
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void GameScene::onHttpRequestCompleted_rank(HttpClient * sender, HttpResponse * response)
{
	if (!response) return;
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	string responseData = Global::toString(response->getResponseData());
	string responseHeader = Global::toString(response->getResponseHeader());

	rapidjson::Document d;
	d.Parse<0>(responseData.c_str());
	// 解析错误
	if (d.HasParseError()) {
		CCLOG("GetParseError %s\n", d.GetParseError());
	}
	// rank成功
	if (d.IsObject() && d.HasMember("result") && d["result"].GetBool()) {
		if (d.HasMember("info")) {
			string rank = d["info"].GetString();
			for (int i = 0; i < rank.length(); i++) {
				// "|"转换为"\n"
				if (rank[i] == '|') rank[i] = '\n';
			}
			// rank排行榜
			rank_field->setText(rank + '\n');
			// 保存header和body
			database->setStringForKey("header", responseHeader);
			database->setStringForKey("body", responseData);
		}
	}
}
