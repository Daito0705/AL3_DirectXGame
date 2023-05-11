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
	WorldTransform worldTransformStage_;

	//プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	void PlayerUpdate();    //プレイヤー更新
	
	//ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_;

	void BeamUpdate();   //ビーム更新

	void BeamMove();   //ビーム移動

	int beamFlag_ = 0;   //ビーム生存フラグ

	void BeamBorn();  //ビーム発生

	//敵
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_;

	void EnemyUpdate();  //敵更新
	void EnemyMove();   //敵移動

	int enemyFlag_ = 0;  //敵存在フラグ

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

	int sceneMode_ = 0; //シーンモード(0:ゲームプレイ　1:タイトル)

};
