cbuffer cb : register(b0)
{
	float4x4 mvp; // MVP行列
	float4 mulColor; // 乗算カラー
};

struct VSInput
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

Texture2D<float4> sceneTexture : register(t0); // シーンテクスチャ
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
	float4 color = sceneTexture.Sample(Sampler, In.uv);

    // step-7 ピクセルカラーをモノクロ化する
	color.r = (color.r - 0.5) * (color.r - 0.5) * 4;
	color.g = (color.g - 0.5) * (color.g - 0.5) * 4;
	color.b = (color.b - 0.5) * (color.b - 0.5) * 4;
    
	return color;
}