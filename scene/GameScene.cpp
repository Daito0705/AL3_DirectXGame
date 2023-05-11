#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "MathUtilityForText.h"
#include <time.h>

//コンストラクタ
GameScene::GameScene() {}

//デストラクタ
GameScene::~GameScene() { 

	//BG
	delete spriteBG_;
	//ステージ
	delete modelstage_;
	//プレイヤー
	delete modelPlayer_;
	//ビーム
	delete modelBeam_;
	//敵
	delete modelEnemy_;
	//タイトル
	delete spriteTitle_;
	//エンター
	delete spriteEnter_;
	//ゲームオーバー
	delete spriteGameOver_;
}

//初期化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//BG(2Dスプライト)
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//ステージ
	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelstage_ = Model::Create();
	worldTransformStage_.Initialize();

    //ビュープロジェクションの初期化
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	//ステージの位置を変更
	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {4.5f, 1, 40};

	//変換行列を更新
	worldTransformStage_.matWorld_ = MakeAffineMatrix(
	    worldTransformStage_.scale_, 
		worldTransformStage_.rotation_,
	    worldTransformStage_.translation_);

	//変換行列を定数バッファに転送
	worldTransformStage_.TransferMatrix();

	//プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	//ビーム
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	worldTransformBeam_.scale_ = {0.3f, 0.3f, 0.3f};
	worldTransformBeam_.Initialize();

	//敵
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy_.Initialize();

	srand((unsigned int)time(NULL));

	//デバッグテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();

	//タイトル(2Dスプライト)
	textureHandleTitle_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});

	//エンター
	textureHandleEnter_ = TextureManager::Load("enter.png");
	spriteEnter_ = Sprite::Create(textureHandleEnter_, {400, 400});

	//ゲームオーバー
	textureHandleGameOver_ = TextureManager::Load("gameover.png");
	spriteGameOver_ = Sprite::Create(textureHandleGameOver_, {0, 100});
}

//ゲーム開始関数
void GameScene::GamePlayStart() 
{
	//プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	//変換行列を更新
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);
	//変換行列を定数バッファに転送
	worldTransformPlayer_.TransferMatrix();

	//ビーム
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	worldTransformBeam_.scale_ = {0.3f, 0.3f, 0.3f};
	worldTransformBeam_.Initialize();

	//敵
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy_.Initialize();

	gameScore_ = 0; //ゲームスコア
	playerLife_ = 3; //プレイヤーライフ

}

//更新
void GameScene::Update() {
	//各シーンの更新処理を呼び出す
	switch (sceneMode_) {
	case 0:
		GameplayUpdate(); //ゲームプレイ更新
		break;
	case 1:
		TitleUpdate(); //タイトル更新
		break;
	}	
	gameTimer_++;
}

//ゲームプレイ更新
void GameScene::GameplayUpdate() {
	PlayerUpdate(); //プレイヤー更新
	EnemyUpdate();  //敵更新
	BeamUpdate();   //ビーム更新
	Collsion();     //衝突判定
	BeamBorn();  //ビーム発生
	BeamMove();  //ビーム移動
	EnemyBorn();  //発生
	EnemyMove();  //敵移動

	if (playerLife_ <= 0) {
		sceneMode_ = 2;
	}
}

void GameScene::PlayerUpdate() {
	//変換行列を更新
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);
	//変換行列を定数バッファに転送
	worldTransformPlayer_.TransferMatrix();

	//右へ移動
	if (input_->PushKey(DIK_RIGHT)) {	
		worldTransformPlayer_.translation_.x += 0.1f;
	}
	//左へ移動
	if (input_->PushKey(DIK_LEFT)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}
	//プレイヤーの移動制限
	if (worldTransformPlayer_.translation_.x >= 4) {
		worldTransformPlayer_.translation_.x = 4;
	}
	if (worldTransformPlayer_.translation_.x <= -4) {
		worldTransformPlayer_.translation_.x = -4;
	}
}

//ビーム更新
void GameScene::BeamUpdate() {

	//変換行列を更新
	worldTransformBeam_.matWorld_ = MakeAffineMatrix(
	    worldTransformBeam_.scale_, worldTransformBeam_.rotation_,
	    worldTransformBeam_.translation_);
	//変換行列を定数バッファに転送
	worldTransformBeam_.TransferMatrix();
}

//ビーム移動
void GameScene::BeamMove() { 

	worldTransformBeam_.translation_.z += 0.5f;
	
	if (worldTransformBeam_.translation_.z >= 40) {
		    beamFlag_ = 0;
	}
	worldTransformBeam_.rotation_.x += 0.1f;
	
}

//ビーム発生（発射）
void GameScene::BeamBorn() {

	if (beamFlag_==0) {

		worldTransformBeam_.translation_.x = worldTransformPlayer_.translation_.x;
		worldTransformBeam_.translation_.z = worldTransformPlayer_.translation_.z;

		if (input_->PushKey(DIK_SPACE)) {
			beamFlag_ = 1;
		}
	}
}

//敵更新
void GameScene::EnemyUpdate() { 

	//変換行列を更新
	worldTransformEnemy_.matWorld_ = MakeAffineMatrix(
	    worldTransformEnemy_.scale_, worldTransformEnemy_.rotation_,
	    worldTransformEnemy_.translation_);
	//変換行列を定数バッファに転送
	worldTransformEnemy_.TransferMatrix();
}

//敵移動
void GameScene::EnemyMove() {

	if (enemyFlag_==1) {
		worldTransformEnemy_.translation_.z -= 0.5;
	}
	if (worldTransformEnemy_.translation_.z <= -5) {
		enemyFlag_ = 0;
	}

	worldTransformEnemy_.rotation_.x -= 0.1f;
}

//敵発生
void GameScene::EnemyBorn() { 
    if (enemyFlag_ == 0) {
		enemyFlag_ = 1;
	    worldTransformEnemy_.translation_.z = 40;

		//乱数でX座標の指定
		int x = rand() % 80;
		float x2 = (float)x / 10 - 4;
		worldTransformEnemy_.translation_.x = x2;
	}
}

void GameScene::Collsion() {

	//衝突判定（プレイヤーと敵）
	CollisionPlayerEnemy();
	//(ビームと敵)
	CollisionBeamEnemy();
}

void GameScene::CollisionPlayerEnemy() {
	//敵が存在すれば
	if (enemyFlag_ == 1) {
		//差を求める
		float dx = abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_.translation_.z);
		//衝突したら
		if (dx < 1 && dz < 1) {
		//存在しない
			enemyFlag_ = 0;
		playerLife_ -= 1;
		}
	}
}

void GameScene::CollisionBeamEnemy() {
	//敵が存在すれば
	if (enemyFlag_ == 1 && beamFlag_ == 1) {
		//差を求める
		float dx = abs(worldTransformBeam_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformBeam_.translation_.z - worldTransformEnemy_.translation_.z);
		//衝突したら
		if (dx < 1 && dz < 1) {
			//存在しない
			enemyFlag_ = 0;
			gameScore_ += 100;
		}
	}
}

//タイトル更新
void GameScene::TitleUpdate() {
	if (input_->TriggerKey(DIK_RETURN)) {
		//ゲームプレイへ変更
		sceneMode_ = 0;
		GamePlayStart();
	}
}

//ゲームプレイ表示3D
void GameScene::GameplayDraw3D() {
	//ステージ
	modelstage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

	//プレイヤー
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);

	//ビーム
	if (beamFlag_ == 1) {
		modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);
	}

	//敵
	if (enemyFlag_ == 1) {
		modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
	}
}

//ゲームプレイ表示2D近景
void GameScene::GameplayDraw2DNear() {
	//ゲームスコア
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	//プレイヤーライフ
	char str2[100];
	sprintf_s(str2, "LIFE %d", playerLife_);
	debugText_->Print(str2, 900, 10, 2);
}

//ゲームプレイ表示2D背景
void GameScene::GameplayDraw2DBack() {
	//背景
	spriteBG_->Draw();
}

//タイトル表示
void GameScene::TitleDraw2DNear() { 
	spriteTitle_->Draw(); 
	//エンター表示
	if (gameTimer_%40>=20) 
	{
		spriteEnter_->Draw();
	}
}

void GameScene::GameOverDraw2DNear() 
{ 	
	spriteGameOver_->Draw();
}

//描画
void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	
	//各シーンの背景2D表示を呼び出す
	switch (sceneMode_) { 
	case 0:	
		GameplayDraw2DBack(); //ゲームプレイ表示2D背景
		break;
	case 2:
		GameplayDraw2DBack();
		break;
	}

	//背景
	debugText_->Print("", 10, 10, 2);
	debugText_->DrawAll();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	switch (sceneMode_) {
	case 0:
		GameplayDraw3D();
		break;
	case 2:
		GameplayDraw3D();
		break;
	}	
	
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	
	switch (sceneMode_) {
	case 0:
		GameplayDraw2DNear();
		break;
	case 1:
		TitleDraw2DNear();
		break;
	case 2:
		GameOverDraw2DNear();
		if (gameTimer_ % 40 >= 20) {
			spriteEnter_->Draw();
		}
		if (input_->TriggerKey(DIK_RETURN)) {
			//タイトルへ変更
			sceneMode_ = 1;
		}
		break;	
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
