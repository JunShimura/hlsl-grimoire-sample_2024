#include "stdafx.h"
#include "system/system.h"

const int NUM_DIRECTIONAL_LIGHT = 4; // ディレクションライトの数

/// <summary>
/// ディレクションライト
/// </summary>
struct DirectionalLight
{
	Vector3 direction;  // ライトの方向
	float pad0;         // パディング
	Vector4 color;      // ライトのカラー
};

/// <summary>
/// ライト構造体
/// </summary>
struct Light
{
	DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];   // ディレクションライト
	Vector3 eyePos;                 // カメラの位置
	float specPow;                  // スペキュラの絞り
	Vector3 ambinetLight;           // 環境光
};

// 関数宣言
void InitRootSignature(RootSignature& rs);
void CalcWeightsTableFromGaussian(float* weights, int numWeights, float sigma);

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する
	//////////////////////////////////////
	RootSignature rs;
	InitRootSignature(rs);

	// step-1 ゲームシーンを描画するレンダリングターゲットを作成
	RenderTarget mainRenderTarget;
	mainRenderTarget.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT
	);

	// 背景モデルを初期化
	ModelInitData bgModelInitData;
	bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";
	bgModelInitData.m_fxFilePath = "Assets/shader/sample3D.fx";

	// 背景モデルを初期化
	Model bgModel;
	bgModel.Init(bgModelInitData);

	// プレイヤーモデルを初期化
	// モデルの初期化情報を設定する
	ModelInitData plModelInitData;

	// tkmファイルを指定する
	plModelInitData.m_tkmFilePath = "Assets/modelData/sample.tkm";

	// シェーダーファイルを指定する
	plModelInitData.m_fxFilePath = "Assets/shader/sample3D.fx";

	// 設定した初期化情報をもとにモデルを初期化する
	Model plModel;
	plModel.Init(plModelInitData);

	// step-2 ガウスブラー用の重みテーブルを計算する
	const int NUM_WEIGHTS = 8;

	// テーブルのサイズは8
	float weights[NUM_WEIGHTS];

	// 重みテーブルを計算する
	CalcWeightsTableFromGaussian(
		weights,    // 重みの格納先
		NUM_WEIGHTS,    // 重みのテーブルサイズ
		8.0f        // ボケ具合。この数値が大きくなるとボケが強くなる
	);

	// step-3 横ブラー用のレンダリングターゲットを作成
	RenderTarget xBlurRenderTarget;
	xBlurRenderTarget.Create(
		FRAME_BUFFER_W/2,	// 横幅は解像度をmainRenderTargetの幅の半分にする
		FRAME_BUFFER_H,	// 高さはmainRenderTargetと同じ
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT
	);

	// step-4 横ブラー用のスプライトを初期化
	// 初期化情報を設定する
	SpriteInitData xBlurSpriteInitData;
	xBlurSpriteInitData.m_fxFilePath = "Assets/shader/samplePostEffect.fx";
	xBlurSpriteInitData.m_vsEntryPointFunc = "VSXBlur";
	xBlurSpriteInitData.m_psEntryPoinFunc = "VSXBlur";

	// スプライトの解像度はxBlirTargetと同じ
	xBlurSpriteInitData.m_width = FRAME_BUFFER_W / 2;
	xBlurSpriteInitData.m_height = FRAME_BUFFER_H;

	// 【注目】テクスチャはmainRenderTargetのカラーバッファー
	xBlurSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();

	// 【注目】ユーザー拡張の定数バッファーに重みテーブルを設定する
	xBlurSpriteInitData.m_expandConstantBuffer = &weights;
	xBlurSpriteInitData.m_expandConstantBufferSize = sizeof(weights);

	// 初期化情報を元に横ブラー用のスプライトを初期化する
	Sprite xBlurSprite;
	xBlurSprite.Init(xBlurSpriteInitData);

	// step-5 縦ブラー用のレンダリングターゲットを作成

	// step-6 縦ブラー用のスプライトを初期化

	// step-7 テクスチャを貼り付けるためのスプライトを初期化する

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//  ここからゲームループ
	while (DispatchWindowMessage())
	{
		// 1フレームの開始
		g_engine->BeginFrame();

		//////////////////////////////////////
		// ここから絵を描くコードを記述する
		//////////////////////////////////////

		// step-8 レンダリングターゲットをmainRenderTargetに変更する

		// step-9 mainRenderTargetに各種モデルを描画する

		// step-10 mainRenderTargetに描画された画像に横ブラーをかける

		// step-11 縦ブラーも行う

		// step-12 メインレンダリングターゲットの絵をフレームバッファにコピー

		//////////////////////////////////////
		// 絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		// 1フレーム終了
		g_engine->EndFrame();
	}
	return 0;
}

// ルートシグネチャの初期化
void InitRootSignature(RootSignature& rs)
{
	rs.Init(D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP);
}

/// <summary>
/// ガウシアン関数を利用して重みテーブルを計算する
/// </summary>
/// <param name="weightsTbl">重みテーブルの記録先</param>
/// <param name="sizeOfWeightsTbl">重みテーブルのサイズ</param>
/// <param name="sigma">分散具合。この数値が大きくなると分散具合が強くなる</param>
void CalcWeightsTableFromGaussian(float* weightsTbl, int sizeOfWeightsTbl, float sigma)
{
	// 重みの合計を記録する変数を定義する
	float total = 0;

	// ここからガウス関数を用いて重みを計算している
	// ループ変数のxが基準テクセルからの距離
	for (int x = 0; x < sizeOfWeightsTbl; x++)
	{
		weightsTbl[x] = expf(-0.5f * (float)(x * x) / sigma);
		total += 2.0f * weightsTbl[x];
	}

	// 重みの合計で除算することで、重みの合計を1にしている
	for (int i = 0; i < sizeOfWeightsTbl; i++)
	{
		weightsTbl[i] /= total;
	}
}
