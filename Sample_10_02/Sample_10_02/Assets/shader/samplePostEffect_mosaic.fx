cbuffer cb : register(b0)
{
    float4x4 mvp;       // MVP行列
    float4 mulColor;    // 乗算カラー
};

struct VSInput
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

Texture2D<float4> sceneTexture : register(t0);  // シーンテクスチャ
sampler Sampler : register(s0);

PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

float4 PSMain(PSInput In) : SV_Target0
{
    // float4 color = sceneTexture.Sample(Sampler, In.uv);

    // 1ブロックのサイズ（モザイクのサイズ）
	const int blockSize = 32;

    // テクスチャのサイズを取得（仮定としてシェーダー側で渡す場合）
	float2 textureSize = float2(1720.0f, 1080.0f); // テクスチャの解像度（例：1024x1024）

    // テクスチャ座標をブロック単位でスナップ
	float2 blockCoord = floor(In.uv * textureSize / blockSize) * blockSize / textureSize;

    // ブロックの中心からサンプルを取得
	float4 color = sceneTexture.Sample(Sampler, blockCoord);

    // 結果を返す
    
    return color;
}
