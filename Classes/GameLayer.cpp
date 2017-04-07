#include "GameLayer.h"
#include "vector"
#include <cmath>
#include "SimpleAudioEngine.h"
#include "Settings.h"
#include "time.h"

using namespace cocos2d;
using namespace std;

bool GameLayer::init() { //实现SpriteFrameLayer类中的init方法，初始化布景
	if (!Layer::init()) //调用父类的初始化
		return false;
	Size visibleSize = Director::getInstance()->getVisibleSize(); //获取可见区域尺寸
	Point origin = Director::getInstance()->getVisibleOrigin(); //获取可见区域原点坐标
	backgroundSprite = Sprite::create("pic/background.png"); //背景精灵的创建
	backgroundSprite->setAnchorPoint(Vec2(0, 0));
	backgroundSprite->setPosition(Vec2(origin.x, origin.y));
	this->addChild(backgroundSprite, -1);
	flushScore = Label::createWithTTF("0", "fonts/lzy.TTF", 110, Size::ZERO,
			TextHAlignment::LEFT, TextVAlignment::CENTER); //界面随时刷新的分数label
	flushScore->setPosition(
			Vec2(origin.x + visibleSize.width / 2 - 50 + 25 + 5 + 3,
					origin.y + visibleSize.height * 0.75 + 260));
	flushScore->setAnchorPoint(Vec2(0, 0));
	flushScore->setColor(Color3B::WHITE);
	this->addChild(flushScore, 10);
	_scorebg = Sprite::create("pic/scorebg.png"); //得分背景精灵
	_scorebg->setAnchorPoint(Vec2(0, 0));
	_scorebg->setScaleY(0.4f);
	_scorebg->setScaleX(0.5f);
	_scorebg->setPosition(
			Vec2(origin.x + visibleSize.width / 2 - 180,
					origin.y + visibleSize.height * 0.75 + 10 + 250));
	this->addChild(_scorebg, 9);
	decorate = Sprite::create("pic/decorate.png"); //晾衣架装饰品
	decorate->setAnchorPoint(Vec2(0, 0));
	decorate->setScaleX(1.87f);
	decorate->setScaleY(1.5f);
	decorate->setPosition(
			Vec2(origin.x + 64, origin.y + visibleSize.height - 400));
	this->addChild(decorate, 10);
	initAnmiRF(); //初始化帧动画
	foxSprite = Sprite::create(); //狐狸精灵的创建
	foxSprite->setAnchorPoint(Vec2(0, 0));
	foxSprite->setPosition(
			Vec2(origin.x + 64, origin.y + visibleSize.height - 250));
	foxSprite->setScale(2.0f);
	this->addChild(foxSprite, 10);
	foxSprite->runAction(RepeatForever::create(anmFox->clone()));
	foxSprite->setVisible(false);
	diedPopup = Sprite::create("pic/score.png"); //死亡弹出框
	diedPopup->setAnchorPoint(Vec2(0.5, 0.5));
	diedPopup->setVisible(false);
	diedPopup->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y + visibleSize.height
							+ diedPopup->getTexture()->getPixelsHigh()));
	this->addChild(diedPopup, 10);
	finalScore = Label::createWithTTF("1789", "fonts/lzy.TTF", 100, Size::ZERO,
			TextHAlignment::LEFT, TextVAlignment::CENTER); //死亡后弹出框中的显示得分的label
	finalScore->setPosition(
			Vec2(origin.x + visibleSize.width - 340,
					origin.y + visibleSize.height
							+ diedPopup->getTexture()->getPixelsHigh() + 500));
	finalScore->setAnchorPoint(Vec2(0, 0));
	finalScore->setScaleX(0.95);
	finalScore->setColor(Color3B::RED);
	this->addChild(finalScore, 20);
	maxScore = Label::createWithTTF("1789", "fonts/lzy.TTF", 160, Size::ZERO,
			TextHAlignment::LEFT, TextVAlignment::CENTER); //历史最高分的label
	maxScore->setPosition(
			Vec2(origin.x + visibleSize.width / 2 - 300,
					origin.y + visibleSize.height
							+ diedPopup->getTexture()->getPixelsHigh() + 500));
	maxScore->setAnchorPoint(Vec2(0, 0));
	maxScore->setColor(Color3B::WHITE);
	this->addChild(maxScore, 20);
	for (int i = 0; i < MaxSpritesCount; i++) //添加左侧障碍物屋檐精灵
			{
		if (leftEave[i] == 0)
			continue;
		leftEaveSprites[i] = Sprite::create("pic/eave.png"); //左侧屋檐的创建
		leftEaveSprites[i]->setAnchorPoint(Vec2(0, 0));
		leftEaveSprites[i]->setScale(0.35f);
		leftEaveSprites[i]->setPosition(
				Vec2(origin.x + 25, origin.y + L * i + 32));
		this->addChild(leftEaveSprites[i], 0);
	}
	for (int i = 0; i < MaxSpritesCount; i++) //添加右侧障碍物屋檐精灵
			{
		if (rightEave[i] == 0)
			continue;
		rightEaveSprites[i] = Sprite::create("pic/eave.png"); //右侧屋檐的创建
		rightEaveSprites[i]->setAnchorPoint(Vec2(1, 0));
		rightEaveSprites[i]->setFlippedX(true);
		rightEaveSprites[i]->setScale(0.35f);
		rightEaveSprites[i]->setPosition(
				Vec2(origin.x + visibleSize.width - 32, origin.y + L * i + 32));
		this->addChild(rightEaveSprites[i], 0);
	}
	_pause = MenuItemImage::create //创建暂停菜单项
			(
					"pic/zanting.png", //平时的图片
					"pic/zanting.png", //选中时的图片
					"pic/zanting.png",
					CC_CALLBACK_1(GameLayer::menuPauseCallback, this) //点击时执行的回调方法
					);
	_pause->setPosition //设置关闭菜单项的位置
	(Point(origin.x + visibleSize.width - _pause->getContentSize().width, //X坐标
	origin.y + _pause->getContentSize().height //Y坐标
			));
	_pause->setScale(2.0f);
	down_pop = Sprite::create("pic/pause_pop.png"); //暂停的弹出界面
	down_pop->setAnchorPoint(Vec2(0.5, 0.5));
	down_pop->setScaleX(1.8f);
	down_pop->setScaleY(2.5f);
	down_pop->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y - visibleSize.height / 2 - 200));
	this->addChild(down_pop, 20);
	resume_game = MenuItemImage::create(
			//恢复游戏菜单项
			"pic/resume_game.png", "pic/resume_game.png", "pic/resume_game.png",
			CC_CALLBACK_1(GameLayer::menuResume_GameCallback, this));
	resume_game->setAnchorPoint(Vec2(0.5, 0.5));
	resume_game->setScale(2.0f);
	resume_game->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y - visibleSize.height / 2));
	caidan = MenuItemImage::create(
			//回主界面的菜单选项
			"pic/caidan.png", "pic/caidan.png", "pic/caidan.png",
			CC_CALLBACK_1(GameLayer::menuCaidanCallback, this));
	caidan->setAnchorPoint(Vec2(0.5, 0.5));
	caidan->setScale(1.5f);
	caidan->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y - visibleSize.height / 2 - 500));
	restart = MenuItemImage::create(
			//重新开始的菜单项
			"pic/again.png", "pic/again.png",
			CC_CALLBACK_1(GameLayer::menuAgainCallback,this));
	restart->setAnchorPoint(Vec2(0.5, 0.5));
	restart->setScale(1.5f);
	restart->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y + visibleSize.height
							+ diedPopup->getTexture()->getPixelsHigh()));
	auto menu = Menu::create(_pause, resume_game, caidan, restart, NULL); //创建菜单对象
	menu->setPosition(Point::ZERO); //设置菜单位置
	this->addChild(menu, 20); //将菜单添加到布景中
	heroSprite = Sprite::create(); //创建英雄精灵对象
	heroSprite->setAnchorPoint(Point(0, 0)); //设置英雄精灵的锚点
	heroSprite->setPosition(Point(origin.x + 90, visibleSize.height / 2 + 50)); //设置英雄精灵的初始位置
	heroSprite->setScaleX(3.0f);
	heroSprite->setScaleY(3.0f);
	heroSprite->setFlippedY(true);
	this->addChild(heroSprite, 2); //将英雄精灵添加进布景
	heroSprite->runAction(RepeatForever::create(anmiAc[0]->clone())); //开始播放爬行动画
	for (int i = 0; i <= visibleSize.height / L; i++) //墙壁精灵的创建
			{
		Sprite *sprite = Sprite::create("pic/wall.png"); //左侧墙壁精灵的创建
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(Vec2(origin.x, origin.y + i * L));
		this->addChild(sprite, 0);
		leftWall[i] = sprite;
		sprite = Sprite::create("pic/wall.png"); //右侧墙壁精灵的创建
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setFlippedX(true);
		sprite->setPosition(
				Vec2(origin.x + visibleSize.width - 64, origin.y + i * L));
		this->addChild(sprite, 0);
		rightWall[i] = sprite;
	}
	for (int i = 0; i != MaxSpritesCount; ++i) //左壁金币精灵的创建
			{
		if (_jinbiLeft[i] == 0)
			continue;
		leftCoin = Sprite3D::create("goldCoin/MyGoldCoin.obj",
				"goldCoin/jb.png");
		leftCoin->setScale(1.3f);
		leftCoin->setPosition3D(Vec3(origin.x + 93, origin.y + L * i + 60, 20));
		leftCoin->setGlobalZOrder(1.0f);
		this->addChild(leftCoin, 1);
		leftCoinSprites[i] = leftCoin;
	}
	for (size_t i = 0; i != 500; ++i) //右壁金币精灵的创建
			{
		if (_jinbiRight[i] == 0)
			continue;
		rightCoin = Sprite3D::create("goldCoin/MyGoldCoin.obj",
				"goldCoin/jb.png");
		rightCoin->setScale(1.3f);
		rightCoin->setGlobalZOrder(1.0);
		rightCoin->setPosition3D(
				Vec3(origin.x + visibleSize.width - 95, origin.y + L * i + 60,
						20));
		this->addChild(rightCoin, 1);
		rightCoinSprites[i] = rightCoin;
	}
	birdSprite = Sprite::create(); //障碍物鸟精灵的创建
	birdSprite->setPosition(
			Vec2(origin.x + 60, origin.y - 200));
	birdSprite->setScale(1.6f);
	this->addChild(birdSprite);
	birdSprite->setVisible(false);
	birdSprite->runAction(RepeatForever::create(anmBird));
	startTime = clock(); //第一次进入此界面的时间戳
	foxStartTime = clock(); //狐狸第一次的时间戳
	auto listenerTouch = EventListenerTouchOneByOne::create(); //手势监听的创建
	listenerTouch->setSwallowTouches(true);
	listenerTouch->onTouchBegan = CC_CALLBACK_2(GameLayer::onMyTouchBegin,this);
	listenerTouch->onTouchMoved = CC_CALLBACK_2(GameLayer::onMyTouchMoved,this);
	listenerTouch->onTouchCancelled =
			CC_CALLBACK_2(GameLayer::onMyTouchCalcelled,this);
	listenerTouch->onTouchEnded = CC_CALLBACK_2(GameLayer::onMyTouchEnded,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouch,
			heroSprite);
	auto director = Director::getInstance(); //调用shedule执行回调方法update
	auto sched = director->getScheduler();
	sched->schedule(schedule_selector(GameLayer::update), this, 0.02, false);
	return true;
}

//初始化各种精灵的动画
void GameLayer::initAnmiRF() {
	Size visibleSize = Director::getInstance()->getVisibleSize(); //获取可见区域尺寸
	Point origin = Director::getInstance()->getVisibleOrigin(); //获取可见区域原点坐标
	Vector<SpriteFrame*> sfFox;
	Vector<SpriteFrame*> sfBird;
	std::string picFox[9] = { "pic/foxs/fox_00.png", "pic/foxs/fox_01.png",
			"pic/foxs/fox_02.png", "pic/foxs/fox_03.png", "pic/foxs/fox_04.png",
			"pic/foxs/fox_05.png", "pic/foxs/fox_06.png", "pic/foxs/fox_07.png",
			"pic/foxs/fox_08.png", };
	for (int i = 0; i < 9; i++) {
		SpriteFrame *f = SpriteFrame::create(picFox[i], Rect(0, 0, 100, 50));
		sfFox.pushBack(f);
	}
	Animation *animFox = Animation::createWithSpriteFrames(sfFox, 0.05f);
	anmFox = Animate::create(animFox);
	anmFox->retain();
	std::string picBird[8] = { "pic/birds/bird_00.png", "pic/birds/bird_01.png",
			"pic/birds/bird_02.png", "pic/birds/bird_03.png",
			"pic/birds/bird_04.png", "pic/birds/bird_05.png",
			"pic/birds/bird_06.png", "pic/birds/bird_07.png", };
	for (int i = 0; i < 8; i++) {
		SpriteFrame *f = SpriteFrame::create(picBird[i], Rect(0, 0, 80, 126));
		sfBird.pushBack(f);
	}
	Animation *animBird = Animation::createWithSpriteFrames(sfBird, 0.05f);
	anmBird = Animate::create(animBird);
	anmBird->retain();
	std::string down[15] = { //下落动画
			"pic/down/ID_12_0.png", "pic/down/ID_12_1.png",
					"pic/down/ID_12_2.png", "pic/down/ID_12_3.png",
					"pic/down/ID_12_4.png", "pic/down/ID_12_5.png",
					"pic/down/ID_12_6.png", "pic/down/ID_12_7.png",
					"pic/down/ID_12_8.png", "pic/down/ID_12_9.png",
					"pic/down/ID_12_10.png", "pic/down/ID_12_11.png",
					"pic/down/ID_12_12.png", "pic/down/ID_12_13.png",
					"pic/down/ID_12_14.png" };
	std::string run[16] = { //奔跑动画
			"pic/renframes/ID_15_0.png", "pic/renframes/ID_15_1.png",
					"pic/renframes/ID_15_2.png", "pic/renframes/ID_15_3.png",
					"pic/renframes/ID_15_4.png", "pic/renframes/ID_15_5.png",
					"pic/renframes/ID_15_6.png", "pic/renframes/ID_15_7.png",
					"pic/renframes/ID_15_8.png", "pic/renframes/ID_15_9.png",
					"pic/renframes/ID_15_10.png", "pic/renframes/ID_15_11.png",
					"pic/renframes/ID_15_12.png", "pic/renframes/ID_15_13.png",
					"pic/renframes/ID_15_14.png", "pic/renframes/ID_15_15.png", };
	std::string rotation[10] = { "pic/rotation/ID_14_0.png",
			"pic/rotation/ID_14_1.png", "pic/rotation/ID_14_2.png",
			"pic/rotation/ID_14_3.png", "pic/rotation/ID_14_4.png",
			"pic/rotation/ID_14_5.png", "pic/rotation/ID_14_6.png",
			"pic/rotation/ID_14_7.png", "pic/rotation/ID_14_8.png",
			"pic/rotation/ID_14_9.png", };
	Vector<SpriteFrame*> animDown;
	Vector<SpriteFrame*> animRun;
	Vector<SpriteFrame*> animRightRotate;
	for (int i = 0; i < 15; i++) { //下落动画帧添加
		SpriteFrame *f = SpriteFrame::create(down[i], Rect(0, 0, 38, 50), false,
				Point(1, 1), Size(0, 0));
		animDown.pushBack(f);
	}
	for (int i = 0; i < 16; i++) { //奔跑动画帧添加
		SpriteFrame *f = SpriteFrame::create(run[i], Rect(0, 0, 22, 38), false,
				Point(1, 1), Size(0, 0)); //获取此动画中的指定两个帧
		animRun.pushBack(f); //将这一段动画中的两个帧存放到向量中
	}
	for (int i = 0; i < 10; i++) { //旋转动画帧添加
		SpriteFrame *f = SpriteFrame::create(rotation[i], Rect(0, 0, 75, 44),
				false, Point(1, 1), Size(0, 0));
		animRightRotate.pushBack(f);
	}
	Animation *_run = Animation::createWithSpriteFrames(animRun, 0.05f);
	anmiAc[0] = Animate::create(_run); //创建奔跑对象
	anmiAc[0]->retain(); //因为暂时不用，保持引用，防止被自动释放
	Animation *_down = Animation::createWithSpriteFrames(animDown, 0.05f); //创建坠落对象
	anmiAc[1] = Animate::create(_down);
	anmiAc[1]->retain();
	Animation *_rotation = Animation::createWithSpriteFrames(animRightRotate,
			0.05f); //创建旋转对象
	anmiAc[2] = Animate::create(_rotation);
	anmiAc[2]->retain();
}

void GameLayer::update(float dt) {
	Point origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto audioEngine=CocosDenshion::SimpleAudioEngine::getInstance();

	manHeight = manHeight + step;
	int middPic = manHeight / L;

	//赢游戏
	if (middPic > 480) {
		unscheduleAllCallbacks();
		isDied = true;
		ParticleSystemQuad* psq = ParticleExplosion::create(); //烟花
		psq->retain();
		this->addChild(psq, 30);
		psq->setScale(3.0f);
		psq->setSpeed(80);
		psq->setTexture(
				Director::getInstance()->getTextureCache()->addImage(
						"stars2.png"));
		psq->setPosition(
				Vec2(origin.x + visibleSize.width / 2,
						origin.y + visibleSize.height / 2));

		flushScore->setVisible(false);
		_scorebg->setVisible(false);
		_pause->setEnabled(false);
		heroSprite->runAction(MoveBy::create(4.0f, Vec2(0, 2000)));
        if(!IsDownMenu)
        {
             downMenu();
            IsDownMenu=!IsDownMenu;
        }
           
		destroyAllCoins();
		return;
	}

	showBirdAndFox();

	if (abs(birdSprite->getPositionY() - heroSprite->getPositionY()) < 60
			&& //鸟的碰撞检测
			abs(birdSprite->getPositionX() - heroSprite->getPositionX()) < 60
			&& abs(birdMethodOnlyOne - middPic) >= 2) {
		if (abs(heroSprite->getPositionX() - origin.x - 90) < 30
				|| abs(heroSprite->getPositionX() - origin.x - 90 - 225 * 4)
						< 30) {
			heroSprite->runAction(
					Spawn::create(anmiAc[1]->clone(),
							MoveBy::create(1.0f, Vec2(0, -1500)), NULL));
			unscheduleAllCallbacks();
            if(IsDownMenu)
            {
                 downMenu();
                IsDownMenu=!IsDownMenu;
            }
            
			if (UserDefault::getInstance()->getBoolForKey("soundFlag")) { //hero down sound
				audioEngine->playEffect("sound/down.wav", false, 1.0f, 0.0f,
						1.0f);
			}
		} else {
			birdSprite->runAction(MoveBy::create(1.0f, Vec2(0, -1500))); //鸟掉
			flushScore->setString(StringUtils::format("%d", iscore += 50));
			if (UserDefault::getInstance()->getBoolForKey("soundFlag")) {
				audioEngine->playEffect("sound/bird_down.wav", false, 1.0f,
						0.0f, 1.0f);
			}
			Label *showLeftScore = Label::createWithTTF("+50",
					"fonts/Dope Crisis.ttf", 110, Size::ZERO,
					TextHAlignment::LEFT, TextVAlignment::CENTER);
			showLeftScore->setPosition(
					Vec2(birdSprite->getPositionX(),
							birdSprite->getPositionY() + 20));
			showLeftScore->setColor(Color3B::BLUE);
			this->addChild(showLeftScore);
			showLeftScore->runAction(
					Sequence::create(
							Spawn::create(MoveBy::create(1.0f, Vec2(0, 250)),
									FadeOut::create(1.0f), NULL),
							CallFunc::create(
									CC_CALLBACK_0(GameLayer::removeLabel,this,showLeftScore)),
							NULL));
			birdMethodOnlyOne = middPic;
		}
	}
	if (abs(foxSprite->getPositionY() - heroSprite->getPositionY()) < 50
			&& //狐狸的碰撞检测
			abs(foxSprite->getPositionX() - heroSprite->getPositionX()) < 50
			&& abs(foxMethodOnlyOne - middPic) >= 2) {
		if (abs(heroSprite->getPositionX() - origin.x - 90) < 30
				|| abs(heroSprite->getPositionX() - origin.x - 90 - 225 * 4)
						< 30) { //英雄掉
			heroSprite->runAction(
					Spawn::create(anmiAc[1]->clone(),
							MoveBy::create(1.0f, Vec2(0, -1500)), NULL));
			unscheduleAllCallbacks();
            if(IsDownMenu)
            {
                downMenu();
                IsDownMenu=!IsDownMenu;
            }
			if (UserDefault::getInstance()->getBoolForKey("soundFlag")) { //hero down sound
				audioEngine->playEffect("sound/down.wav", false, 1.0f, 0.0f,
						1.0f);
			}
		} else {
			foxSprite->runAction(MoveBy::create(1.0f, Vec2(0, -1500))); //狐狸掉
			flushScore->setString(StringUtils::format("%d", iscore += 100));
			if (UserDefault::getInstance()->getBoolForKey("soundFlag")) {
				audioEngine->playEffect("sound/fox_down.wav", false, 1.0f,
						0.0f, 1.0f);
			}
			Label *showRightScore = Label::createWithTTF("+100",
					"fonts/Dope Crisis.ttf", 110, Size::ZERO,
					TextHAlignment::LEFT, TextVAlignment::CENTER);
			showRightScore->setPosition(
					Vec2(foxSprite->getPositionX(),
							foxSprite->getPositionY() + 20));
			showRightScore->setColor(Color3B::ORANGE);
			this->addChild(showRightScore);
			showRightScore->runAction(
					Sequence::create(
							Spawn::create(MoveBy::create(1.0f, Vec2(0, 250)),
									FadeOut::create(1.0f), NULL),
							CallFunc::create(
									CC_CALLBACK_0(GameLayer::removeLabel,this,showRightScore)),
							NULL));
			foxMethodOnlyOne = middPic;
		}
	}

	//修正得分label的位置
	if (iscore / 1000 > 0) {
		flushScore->setPosition(
				Vec2(origin.x + visibleSize.width / 2 - 50 - 40,
						origin.y + visibleSize.height * 0.75 + 260));
	} else if (iscore / 100 > 0) {
		flushScore->setPosition(
				Vec2(origin.x + visibleSize.width / 2 - 50 - 20 - 8 + 4 + 3,
						origin.y + visibleSize.height * 0.75 + 260));
	} else if (iscore / 10 > 0) {
		flushScore->setPosition(
				Vec2(origin.x + visibleSize.width / 2 - 50 - 10 + 5 + 5 + 5,
						origin.y + visibleSize.height * 0.75 + 260));
	}

	//降低分值更新的速度
	tempCount++;
	if (tempCount > 20) {
		flushScore->setString(StringUtils::format("%d", (++iscore)));
		tempCount = 0;
	}

	for (int i = 0; i < 500; i++) //左壁障碍物下滑
			{
		if (leftEaveSprites[i])
			leftEaveSprites[i]->setPosition(
					Vec2(leftEaveSprites[i]->getPositionX(),
							leftEaveSprites[i]->getPositionY() - step));
	}

	for (int i = 0; i < 500; i++) //右臂障碍物下滑
			{
		if (rightEaveSprites[i])
			rightEaveSprites[i]->setPosition(
					Vec2(rightEaveSprites[i]->getPositionX(),
							rightEaveSprites[i]->getPositionY() - step));
	}

	for (size_t i = 0; i != 500; ++i) //左壁金币下滑
			{
		if (leftCoinSprites[i])
			leftCoinSprites[i]->setPosition(
					Vec2(leftCoinSprites[i]->getPositionX(),
							leftCoinSprites[i]->getPositionY() - step));
	}

	for (size_t i = 0; i != 500; ++i) //右边金币下滑
			{
		if (rightCoinSprites[i])
			rightCoinSprites[i]->setPosition(
					Vec2(rightCoinSprites[i]->getPositionX(),
							rightCoinSprites[i]->getPositionY() - step));
	}

	if (_jinbiLeft[middPic] == 1
			&& abs(heroSprite->getPositionX() - origin.x - 90) < 100
			&& leftCoinMethodOnlyOne != middPic) { //吃掉左金币
		leftCoinSprites[middPic]->setVisible(false);
		flushScore->setString(StringUtils::format("%d", ++iscore)); //得分总数加5
		Label *tempLabel = Label::createWithTTF("+5", "fonts/Dope Crisis.ttf",
				110, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
		tempLabel->setPosition(
				Vec2(leftCoinSprites[middPic]->getPositionX() - 50,
						leftCoinSprites[middPic]->getPositionY() + 20));
		tempLabel->setPosition(
				Vec2(origin.x + 90, origin.y + visibleSize.height / 2 + 50));
		tempLabel->setColor(Color3B::YELLOW);
		this->addChild(tempLabel);
		tempLabel->runAction(
				Sequence::create(
						Spawn::create(MoveBy::create(1.0f, Vec2(0, 250)),
								FadeOut::create(1.0f), NULL),
						CallFunc::create(CC_CALLBACK_0(
								GameLayer::removeLabel,this,tempLabel)), NULL));
		if (UserDefault::getInstance()->getBoolForKey("soundFlag")) {
			audioEngine->playEffect("sound/getCoin.mp3", false, 1.0f, 0.0f,
					1.0f);
		}
		leftCoinMethodOnlyOne = middPic;
	}

	if (_jinbiRight[middPic] == 1
			&& abs(
					heroSprite->getPositionX() - origin.x - visibleSize.width
							+ 64) < 200 && rightCoinMethodOnlyOne != middPic) //吃掉右金币
					{
		rightCoinSprites[middPic]->setVisible(false);
		flushScore->setString(StringUtils::format("%d", ++iscore));

		Label *tempLabel = Label::createWithTTF("+5", "fonts/Dope Crisis.ttf",
				110, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
		tempLabel->setPosition(
				Vec2(rightCoinSprites[middPic]->getPositionX() - 50,
						rightCoinSprites[middPic]->getPositionY() + 20));
		tempLabel->setColor(Color3B::YELLOW);
		this->addChild(tempLabel);
		tempLabel->runAction(
				Sequence::create(
						Spawn::create(MoveBy::create(1.0f, Vec2(0, 250)),
								FadeOut::create(1.0f), NULL),
						CallFunc::create(
								CC_CALLBACK_0(GameLayer::removeLabel,this,tempLabel)),
						NULL));
		if (UserDefault::getInstance()->getBoolForKey("soundFlag")) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
					"sound/getCoin.mp3", false, 1.0f, 0.0f, 1.0f);
		}
		rightCoinMethodOnlyOne = middPic;
	}

	if (leftEave[middPic] == 1
			&& abs(heroSprite->getPositionX() - origin.x + 64) < 200) //碰到左边
					{
		unscheduleAllCallbacks();
		_pause->setEnabled(false);
		isDied = true;
		heroSprite->runAction(
				Spawn::create(anmiAc[1]->clone(),
						Sequence::create(
								MoveTo::create(0.3f,
										Vec2(origin.x + visibleSize.width / 2,
												origin.y
														+ visibleSize.height / 2
														- 80)),
								MoveTo::create(0.6f,
										Vec2(origin.x + visibleSize.width / 2,
												-50)), NULL), NULL));
                        if(!IsDownMenu)
                        {
                            downMenu();
                            IsDownMenu=!IsDownMenu;
                        }
		destroyAllCoins();

		if (UserDefault::getInstance()->getBoolForKey("soundFlag")) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
					"sound/down.wav", false, 1.0f, 0.0f, 1.0f);
		}

		return;
	}

	if (rightEave[middPic] == 1
			&& abs(
					heroSprite->getPositionX() - origin.x - visibleSize.width
							+ 64) < 200) //碰到右边
					{
		unscheduleAllCallbacks();
		isDied = true;
		_pause->setEnabled(false);
		heroSprite->runAction(
				Spawn::create(anmiAc[1]->clone(),
						Sequence::create(
								MoveTo::create(0.3f,
										Vec2(origin.x + visibleSize.width / 2,
												origin.y
														+ visibleSize.height / 2
														- 80)),
								MoveTo::create(0.6f,
										Vec2(origin.x + visibleSize.width / 2,
												-50)), NULL), NULL));
                        if(!IsDownMenu)
                        {
                            downMenu();
                            IsDownMenu=!IsDownMenu;
                        }
		destroyAllCoins();

		if (UserDefault::getInstance()->getBoolForKey("soundFlag"))
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
					"sound/down.wav", false, 1.0f, 0.0f, 1.0f);

		return;
	}

	int tempNumber = 0;

	for (int i = middPic; i <= middPic + visibleSize.height / L / 2 + 1; i++) //画上部分墙壁
			{
		if (leftWall[i] != NULL) {
			leftWall[i]->setPosition(
					Vec2(origin.x,
							origin.y + visibleSize.height / 2 - (manHeight % L)
									+ tempNumber * L));
			rightWall[i]->setPosition(
					Vec2(origin.x + visibleSize.width - 64,
							origin.y + visibleSize.height / 2 - (manHeight % L)
									+ tempNumber * L));

		} else {
			leftWall[i] = Sprite::create("pic/wall.png");
			leftWall[i]->setAnchorPoint(Vec2(0, 0));
			leftWall[i]->setPosition(
					Vec2(origin.x,
							origin.y + visibleSize.height / 2 - (manHeight % L)
									+ tempNumber * L));
			this->addChild(leftWall[i]);

			rightWall[i] = Sprite::create("pic/wall.png");
			rightWall[i]->setAnchorPoint(Vec2(0, 0));
			rightWall[i]->setFlippedX(true);
			rightWall[i]->setPosition(
					Vec2(origin.x + visibleSize.width - 64,
							origin.y + visibleSize.height / 2 - (manHeight % L)
									+ tempNumber * L));
			this->addChild(rightWall[i]);
		}

		tempNumber++;
	}

	tempNumber = 0;

	for (int i = middPic; i > middPic - visibleSize.height / L / 2; i--) //画下部分墙壁
			{
		if (leftWall[i] != NULL) {
			leftWall[i]->setPosition(
					Vec2(origin.x,
							origin.y + visibleSize.height / 2 - (manHeight % L)
									- tempNumber * L));
			rightWall[i]->setPosition(
					Vec2(origin.x + visibleSize.width - 64,
							origin.y + visibleSize.height / 2 - (manHeight % L)
									- tempNumber * L));
		} else {
			leftWall[i] = Sprite::create("pic/wall.png");
			leftWall[i]->setAnchorPoint(Vec2(0, 0));
			leftWall[i]->setPosition(
					Vec2(origin.x,
							origin.y + visibleSize.height / 2 - (manHeight % L)
									- tempNumber * L));
			this->addChild(leftWall[i]);

			rightWall[i] = Sprite::create("pic/wall.png");
			rightWall[i]->setAnchorPoint(Vec2(0, 0));
			rightWall[i]->setPosition(
					Vec2(origin.x + visibleSize.width - 64,
							origin.y + visibleSize.height / 2 - (manHeight % L)
									- tempNumber * L));
			this->addChild(rightWall[i]);
		}

		tempNumber++;
	}

	for (int i = 0; i < middPic; i++) {
		if (i < middPic - visibleSize.height / L / 2 - 1) {
			//销毁左右墙壁
			if (leftWall[i]) {
				this->removeChild(leftWall[i], true);
				this->removeChild(rightWall[i], true);
				log("=============delete wall");
				leftWall[i] = NULL;
				rightWall[i] = NULL;
			}
		}
		if (leftEaveSprites[i]) {
			//删除左壁障碍物
			if (leftEaveSprites[i]->getPositionY() < 0) {
				this->removeChild(leftEaveSprites[i], true);
				log("delete left zhang ai wu");
				leftEaveSprites[i] = NULL;
			}
		}
		if (rightEaveSprites[i]) {
			//删除右壁障碍物
			if (rightEaveSprites[i]->getPositionY() < 0) {
				this->removeChild(rightEaveSprites[i], true);
				log("delete right zaw");
				rightEaveSprites[i] = NULL;
			}

		}
		if (leftCoinSprites[i]) {
			//删除左边金币
			if (leftCoinSprites[i]->getPositionY() < 0) {
				this->removeChild(leftCoinSprites[i], true);
				log("delete left leftCoin");
				leftCoinSprites[i] = NULL;
			}
		}
		if (rightCoinSprites[i]) {
			//删除右边金币
			if (rightCoinSprites[i]->getPositionY() < 0) {
				this->removeChild(rightCoinSprites[i], true);
				log("delete right leftCoin");
				rightCoinSprites[i] = NULL;
			}
		}
	}
	rotateCoins();
}

void GameLayer::setMoveFlag() {
	moveFlag = false;
}
bool GameLayer::onMyTouchBegin(Touch *touch, Event *event) {
	if (isDied)
		return false;
	if ((++count) % 2 == 0) { //左蹦
		leftJump();
	} else { //右蹦
		rightJump();
	}
	return true;
}
//向左跳的方法
void GameLayer::leftJump() {
	if (moveFlag) {
		count++;
		return;
	}
	auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (UserDefault::getInstance()->getBoolForKey("soundFlag")) { //播放跳跃音乐
		audioEngine->playEffect("sound/jump1.wav", false, 1.0f, 0.0f, 1.0f);
		audioEngine->playEffect("sound/jump2.wav", false, 1.0f, 0.0f, 1.0f);
	}
	heroSprite->setFlippedX(false);
	moveFlag = true;
	leftJumpAction = Spawn::create(
			Sequence::create(
					MoveTo::create(0.1f,
							Vec2(origin.x + 90 + 225 * 3,
									visibleSize.height / 2 + 150)),
					MoveTo::create(0.1f,
							Vec2(origin.x + 90 + 225 * 2,
									visibleSize.height / 2 + 200)),
					MoveTo::create(0.1f,
							Vec2(origin.x + 90 + 225 * 1,
									visibleSize.height / 2 + 150)),
					MoveTo::create(0.1f,
							Vec2(origin.x + 90, visibleSize.height / 2 + 50)),
					CallFuncN::create(
							CC_CALLBACK_0(GameLayer::setMoveFlag,this)), NULL),
			anmiAc[2], NULL);
	heroSprite->runAction(leftJumpAction);
}
void GameLayer::rightJump() { //向右跳的方法
	if (moveFlag) {
		count++;
		return;
	}
	auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (UserDefault::getInstance()->getBoolForKey("soundFlag")) { //播放跳跃音乐
		audioEngine->playEffect("sound/jump2.wav", false, 1.0f, 0.0f, 1.0f);
		audioEngine->playEffect("sound/jump1.wav", false, 1.0f, 0.0f, 1.0f);
	}
	heroSprite->setFlippedX(true);
	moveFlag = true;
	rightJumpAction = Spawn::create(
			Sequence::create(
					MoveTo::create(0.1f,
							Vec2(origin.x + 90 + 225,
									visibleSize.height / 2 + 150)),
					MoveTo::create(0.1f,
							Vec2(origin.x + 90 + 225 * 2,
									visibleSize.height / 2 + 200)),
					MoveTo::create(0.1f,
							Vec2(origin.x + 90 + 225 * 3,
									visibleSize.height / 2 + 150)),
					MoveTo::create(0.1f,
							Vec2(origin.x + 90 + 225 * 4,
									visibleSize.height / 2 + 50)),
					CallFuncN::create(
							CC_CALLBACK_0(GameLayer::setMoveFlag,this)), NULL),
			anmiAc[2], NULL);
	heroSprite->runAction(rightJumpAction);
}

void GameLayer::onMyTouchMoved(Touch *touch, Event *event) {
}
void GameLayer::onMyTouchCalcelled(Touch *touch, Event *event) {
}
void GameLayer::onMyTouchEnded(Touch *touch, Event *event) {
}

//下一个菜单回调方法
void GameLayer::menuPauseCallback(Ref* pSender) {
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	pause(); //暂停回调方法和动作特效
	heroSprite->pause();
	_pause->setEnabled(false);

	down_pop->setVisible(true);
	resume_game->setVisible(true);
	caidan->setVisible(true);

	down_pop->runAction(
			MoveTo::create(0.8f,
					Vec2(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height / 2 - 200)));
	resume_game->runAction(
			MoveTo::create(0.8f,
					Vec2(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height / 2)));
	caidan->runAction(
			MoveTo::create(0.8f,
					Vec2(origin.x + visibleSize.width / 2, origin.y + 400)));
}

void GameLayer::menuCaidanCallback(Ref* ref) {
	sceneManager->gotoMainScene();
}
void GameLayer::menuResume_GameCallback(Ref* ref) {
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	_pause->setEnabled(true);
	resume();
	heroSprite->resume();
	down_pop->setVisible(false);
	down_pop->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y - visibleSize.height / 2 - 200));
	resume_game->setVisible(false);
	resume_game->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y - visibleSize.height / 2));
	caidan->setVisible(false);
	caidan->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
					origin.y - visibleSize.height / 2 - 500));

}
void GameLayer::menuAgainCallback(Ref* ref) {
	_pause->setEnabled(true);
	sceneManager->gotoGameScene();
}

void GameLayer::downMenu() {
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	diedPopup->setVisible(true);
	diedPopup->runAction(
			Sequence::create(
					MoveTo::create(1.0f,
							Vec2(origin.x + visibleSize.width / 2,
									origin.y + visibleSize.height / 2)),
					MoveBy::create(0.3f, Vec2(0, 180)),
					MoveBy::create(0.7f, Vec2(0, -180)), NULL));

	finalScore->setString(StringUtils::format("%d", iscore));
	float labelX = 0;
	if (iscore / 1000 > 0) {
		labelX = origin.x + visibleSize.width - 375;
	} else if (iscore / 100 > 0) {
		labelX = origin.x + visibleSize.width - 370 + 17;
	} else if (iscore / 10 > 0) {
		labelX = origin.x + visibleSize.width - 360 + 32;
	} else {
		labelX = origin.x + visibleSize.width - 360 + 10;
	}

	finalScore->runAction(
			Sequence::create(
					MoveTo::create(1.0f,
							Vec2(labelX,
									origin.y + visibleSize.height / 2 + 200)),
					MoveBy::create(0.3f, Vec2(0, 180)),
					MoveBy::create(0.7f, Vec2(0, -180)), NULL));

	restart->runAction(
			Sequence::create(
					MoveTo::create(1.0f,
							Vec2(origin.x + visibleSize.width / 2,
									origin.y + visibleSize.height / 2)),
					MoveBy::create(0.3f, Vec2(0, 180)),
					MoveBy::create(0.7f, Vec2(0, -180)), NULL));

	int _maxScore = UserDefault::getInstance()->getIntegerForKey("maxScore");
	if (_maxScore < iscore) {
		UserDefault::getInstance()->setIntegerForKey("maxScore", iscore);
		maxScore->setString(StringUtils::format("%d", iscore)); //最后得分版的历史最高分数
	} else {
		maxScore->setString(StringUtils::format("%d", _maxScore));

	}

	maxScore->runAction(
			Sequence::create(
					MoveTo::create(1.0f,
							Vec2(origin.x + visibleSize.width / 2 - 300,
									origin.y + visibleSize.height / 2 + 195)),
					MoveBy::create(0.3f, Vec2(0, 180)),
					MoveBy::create(0.7f, Vec2(0, -180)), NULL));
}

void GameLayer::rotateCoins() {
	for (int i = 0; i < 500; ++i) { //旋转金币
		if (leftCoinSprites[i])
			leftCoinSprites[i]->setRotation3D(
					leftCoinSprites[i]->getRotation3D() + Vec3(0, 3, 0));
	}
	for (int i = 0; i < 500; ++i) {
		if (rightCoinSprites[i])
			rightCoinSprites[i]->setRotation3D(
					rightCoinSprites[i]->getRotation3D() + Vec3(0, 3, 0));
	}
}

void GameLayer::removeLabel(Label *label) {
	log("remove label node");
	this->removeChild(label, true);
}

void GameLayer::destroyAllCoins() {
	for (size_t i = 0; i < 500; ++i) {
		if (leftCoinSprites[i]) {
			this->removeChild(leftCoinSprites[i], true);
			leftCoinSprites[i] = NULL;
		}

		if (rightCoinSprites[i]) {
			this->removeChild(rightCoinSprites[i], true);
			rightCoinSprites[i] = NULL;
		}
	}
}
void GameLayer::showBirdAndFox() {
	Size visibleSize = Director::getInstance()->getVisibleSize(); //获取可见区域尺寸
	Point origin = Director::getInstance()->getVisibleOrigin(); //获取可见区域原点坐标
	//结束时间戳
	clock_t endTime = clock();
	clock_t foxEndTime = clock();
	double temp = (endTime - startTime) / (double) CLOCKS_PER_SEC;
	double foxTemp = (foxEndTime - foxStartTime) / (double) CLOCKS_PER_SEC;

	if (temp > 8) {
		if (birdIsLeft) {
			birdSprite->setFlippedX(false);
			birdSprite->setPosition(
					Vec2(origin.x + 100, origin.y + visibleSize.height - 200));
			birdSprite->setVisible(true);
		} else {
			birdSprite->setPosition(
					Vec2(origin.x + 255 * 4 - 50,
							origin.y + visibleSize.height - 200));
			birdSprite->setFlippedX(true);
			birdSprite->setVisible(true);
		}
		if (temp > 10) {
			if (birdIsLeft) {
				birdIsLeft = !birdIsLeft;
				birdSprite->runAction(
						MoveTo::create(2.0f,
								Vec2(origin.x + visibleSize.width + 50,
										origin.y + visibleSize.height / 2
												- 120)));
                
                birdSprite->runAction(Place::create(Vec2(-300,-300)));
			} else {
				birdIsLeft = !birdIsLeft;
				birdSprite->runAction(
						MoveTo::create(2.0f,
								Vec2(origin.x - 50,
										origin.y + visibleSize.height / 2
												- 120)));
                birdSprite->runAction(Place::create(Vec2(-300,-300)));
			}
			startTime = endTime;
		}
	}
	if (foxTemp > 13) {
		if (foxIsLeft) {
			foxSprite->setPosition(
					Vec2(origin.x + 40, origin.y + visibleSize.height - 250));
			foxSprite->setFlippedX(false);
			foxSprite->setVisible(true);
		} else {
			foxSprite->setPosition(
					Vec2(origin.x + 255 * 4 - 120,
							origin.y + visibleSize.height - 250));
			foxSprite->setFlippedX(true);
			foxSprite->setVisible(true);
		}

		if (foxTemp > 15) {
			if (foxIsLeft) {
				foxIsLeft = !foxIsLeft;
				foxSprite->runAction(
						Sequence::create(MoveBy::create(0.6f, Vec2(250, 200)),
								MoveTo::create(1.3f,
										Vec2(origin.x + visibleSize.width,
												origin.y
														+ visibleSize.height / 2
														- 200)), NULL));
                foxSprite->runAction(Place::create(Vec2(-300,-300)));
			} else {
				foxIsLeft = !foxIsLeft;
				foxSprite->runAction(
						Sequence::create(MoveBy::create(0.6f, Vec2(-100, 200)),
								MoveTo::create(1.3f,
										Vec2(origin.x - 200,
												origin.y
														+ visibleSize.height / 2
														- 300)), NULL));
                foxSprite->runAction(Place::create(Vec2(-300,-300)));
			}

			foxStartTime = foxEndTime;
		}
	}
}
