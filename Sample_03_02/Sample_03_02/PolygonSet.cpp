﻿#include "stdafx.h"
#include "PolygonSet.h"

PolygonSet::PolygonSet(
	SimpleVertex vertices[],
	unsigned int num_v,
	unsigned short indices[],
	unsigned int num_i) {

}
void PolygonSet::Init(RootSignature& rs)
{
	//シェーダーをロード。
	LoadShaders();
	//パイプラインステートを作成。
	InitPipelineState(rs);
	//頂点バッファを作成。
	InitVertexBuffer();
	//インデックスバッファを作成。
	InitIndexBuffer();
}
void PolygonSet::Draw(RenderContext& rc)
{
	//パイプラインステートを設定。
	rc.SetPipelineState(m_pipelineState);
	//プリミティブのトポロジーを設定。
	rc.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//頂点バッファを設定。
	rc.SetVertexBuffer(m_vertexBuffer);
	//インデックスバッファを設定。
	rc.SetIndexBuffer(m_indexBuffer);
	//ドローコール
	rc.DrawIndexed(numI);
}
void PolygonSet::LoadShaders()
{
	m_vertexShader.LoadVS("Assets/shader/sample.fx", "VSMain");
	m_pixelShader.LoadPS("Assets/shader/sample.fx", "PSMain");
}
void PolygonSet::InitPipelineState(RootSignature& rs)
{
	// 頂点レイアウトを定義する。
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//パイプラインステートを作成。
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = rs.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vertexShader.GetCompiledBlob());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_pixelShader.GetCompiledBlob());
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_pipelineState.Init(psoDesc);
}
void PolygonSet::InitVertexBuffer()
{
	m_vertexBuffer.Init(sizeof(SimpleVertex) * numV, sizeof(SimpleVertex));
	m_vertexBuffer.Copy(vertices);
}

void PolygonSet::SetUVCoord(int vertNo, float U, float V)
{

	(vertices + vertNo)->uv[0] = U;
	(vertices + vertNo)->uv[1] = V;
	m_vertexBuffer.Copy(vertices);
}
void PolygonSet::InitIndexBuffer()
{
	m_indexBuffer.Init(sizeof(*indices), 2);
	m_indexBuffer.Copy(static_cast<uint16_t*>(indices));

}