﻿#include "stdafx.h"
#include "system/system.h"
//#include "TrianglePolygon.h"
//#include "RectanglePolygon.h"
#include "PolygonSet.h"

// 関数宣言
void InitRootSignature(RootSignature& rs);

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

	// ルートシグネチャを作成
	RootSignature rootSignature;
	InitRootSignature(rootSignature);

	// 定数バッファを作成
	ConstantBuffer cb;

	cb.Init(sizeof(Matrix));

	// 頂点データ
	SimpleVertex vert[]
		= {
			{// 左下
				{-0.5f, -0.5f, 0.0f},
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f }
			},
			{	// 左上
				{ -0.5f, 0.5f, 0.0f },
				{ 0.0f, 1.0f, 0.0f },
				{ 0.0f, 0.0f }
			},
			{	// 右上
				{ 0.5f, 0.5f, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
				{1.0f, 0.0f}
			},
			{	// 右下
				{ 0.5f, -0.5f, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
				{1.0f, 1.0f}
			},
			{	// 中上
				{ 0.0f, 0.75f, 0.0f },
				{ 1.0f, 1.0f, 1.0f },
				{0.5f, 0.0f}
			}
	};
	//indexデータ
	unsigned short indices[] = {
		0,1,3, 1,2,3, 1,4,2
	};

	// 三角形ポリゴンを定義
	//TrianglePolygon triangle;
	//RectanglePolygon rectangle;
	PolygonSet polygonSet
		= PolygonSet(
			vert,
			_countof(vert),
			indices,
			_countof(indices));

	//triangle.Init(rootSignature);
	polygonSet.Init(rootSignature);

	// step-1 三角形ポリゴンにUV座標を設定
	//triangle.SetUVCoord(
	//    0,    // 頂点の番号  左下  
	//    0.0f, // U座標    
	//    1.0f  // V座標
	//);
	//triangle.SetUVCoord(
	//    1,    // 頂点の番号  中上  
	//    0.5f, // U座標    
	//    0.0f  // V座標
	//);
	//triangle.SetUVCoord(
	//    2,    // 頂点の番号  右下  
	//    1.0f, // U座標    
	//    1.0f  // V座標
	//);

	//四角形ポリゴンにUV座標を設定
	//rectangle.SetUVCoord(
	//	0,    // 頂点の番号  左下  
	//	0.0f, // U座標    
	//	1.0f  // V座標
	//);
	//rectangle.SetUVCoord(
	//	1,    // 頂点の番号  左上  
	//	0.0f, // U座標    
	//	0.0f  // V座標
	//);
	//rectangle.SetUVCoord(
	//	2,    // 頂点の番号  右上  
	//	1.0f, // U座標    
	//	0.0f  // V座標
	//);
	//rectangle.SetUVCoord(
	//	3,    // 頂点の番号  右下  
	//	1.0f, // U座標    
	//	1.0f  // V座標
	//);


	// step-2 テクスチャをロード
	Texture tex;
	tex.InitFromDDSFile(L"Assets/image/sample_00.dds");

	// ディスクリプタヒープを作成
	DescriptorHeap ds;
	ds.RegistConstantBuffer(0, cb); // ディスクリプタヒープに定数バッファを登録

	// step-3 テクスチャをディスクリプタヒープに登録
	ds.RegistShaderResource(
		0,  // レジスタ番号    
		tex // レジスタに設定するテクスチャ
	);

	ds.Commit();                    //ディスクリプタヒープへの登録を確定

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ
	while (DispatchWindowMessage())
	{
		// フレーム開始
		g_engine->BeginFrame();

		//////////////////////////////////////
		// ここから絵を描くコードを記述する
		//////////////////////////////////////

		// ルートシグネチャを設定
		renderContext.SetRootSignature(rootSignature);

		// ワールド行列を作成
		Matrix mWorld;

		// ワールド行列をグラフィックメモリにコピー
		cb.CopyToVRAM(mWorld);

		//ディスクリプタヒープを設定
		renderContext.SetDescriptorHeap(ds);

		//三角形をドロー
		//triangle.Draw(renderContext);
		polygonSet.Draw(renderContext);


		/// //////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		//フレーム終了
		g_engine->EndFrame();
	}
	return 0;
}

//ルートシグネチャの初期化
void InitRootSignature(RootSignature& rs)
{
	rs.Init(D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP);
}
