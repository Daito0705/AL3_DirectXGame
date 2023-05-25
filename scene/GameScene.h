#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugText.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//BG(スプライト)
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;

	//ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	//ステージ
	uint32_t textureHandleStage_ = 0;
	Model* modelstage_ = nullptr;
	WorldTransform worldTransformStage_[20];

	//プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	void PlayerUpdate();    //プレイヤー更新
	
	//ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = {};
	WorldTransform worldTransformBeam_[10];

	void BeamUpdate();   //ビーム更新

	void BeamMove();   //ビーム移動

	int beamFlag_[10] = {}; //ビーム生存フラグ

	void BeamBorn();  //ビーム発生

	//敵
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = {};
	WorldTransform worldTransformEnemy_[10];

	void EnemyUpdate();  //敵更新
	void EnemyMove();   //敵移動

	int enemyFlag_[10] = {}; //敵存在フラグ

	void EnemyBorn();  //敵発生

	void Collsion();  //衝突判定

	void CollisionPlayerEnemy();  //衝突判定（プレイヤーと敵）

	void CollisionBeamEnemy();  //衝突判定(ビームと敵)

	DebugText* debugText_ = nullptr;

	int gameScore_ = 0; //ゲームスコア

	int playerLife_ = 3; //プレイヤーライフ

	void GameplayUpdate();  //ゲームプレイ更新
	void GameplayDraw3D();  //ゲームプレイ3D表示
	void GameplayDraw2DBack();  //ゲームプレイ背景2D表示
	void GameplayDraw2DNear();  //ゲームプレイ近景2D表示
	void GameOverDraw2DNear(); //ゲームオーバー近景2D表示
	void GameOverUpdate();

	int sceneMode_ = 1; //シーンモード(0:ゲームプレイ　1:タイトル 2:ゲームオーバー)

	void TitleUpdate(); //タイトル更新
	void TitleDraw2DNear(); //タイトル2D

	//タイトル(スプライト)
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;

	int gameTimer_ = 0;

	//エンタースプライト
	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;

	//ゲームオーバースプライト
	uint32_t textureHandleGameOver_ = 0;
	Sprite* spriteGameOver_ = nullptr;

	void GamePlayStart(); //ゲームプレイ開始関数

	int beamTimer_ = 0; //ビーム発射タイマー

	float enemySpeed_[10] = {}; //敵のスピード

	uint32_t soundDataHandleTitleBGM_ = 0; //タイトルBGM
	uint32_t soundDataHandleGamePlayBGM_ = 0; //ゲームプレイBGM
	uint32_t soundDataHandleGameOverBGM_ = 0; //ゲームオーバーBGM
	uint32_t soundDataHandleEnemyHitSE_ = 0; //敵ヒットSE
	uint32_t soundDataHandlePlayerHitSE_ = 0; //プレイヤーヒットSE
	uint32_t voiceHandleBGM_ = 0; //音声再生ハンドル

	void StageUpdate(); //ステージ更新関数

	float enemyJumpSpeed_[10] = {}; //敵ジャンプの移動速度

	void EnemyJump(); //敵ジャンプ

	//スコア数値（スプライト）
	uint32_t textureHandleNumber_ = 0;
	Sprite* spriteNumber_[5] = {};

	void DrawScore();

	uint32_t textureHandleScore_ = 0;
	Sprite* spriteScore_ = nullptr;

	//プレイヤーライフ(スプライト)
	uint32_t textureHandleLife_ = 0;
	Sprite* spriteLife_[3] = {};

	void DrawLife();

	int playerTimer_ = 0;
};
