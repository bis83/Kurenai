//=============================================================
// Buffer
//=============================================================

cbuffer VS_Constant {
	// WorldMatrix
	float4x4 g_matWorld : WORLDVIEWPROJECTION;
	float4 g_uv : UV_COORD_SIZE;
};

//=============================================================
// Definision
//=============================================================

// VertexShaderOutput
struct VS_OUT {
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
};

//=============================================================
// Function
//=============================================================

// VertexShader
VS_OUT main(float3 pos : IA_POSITION, uint index : IA_INDEX) {
	VS_OUT Out;
	
	Out.pos = mul(g_matWorld, float4(pos, 1));
	
	if(index == 0) {
		Out.uv = float2(g_uv.x, g_uv.w);
	} else if(index == 1) {
		Out.uv = float2(g_uv.x, g_uv.y);
	} else if(index == 2) {
		Out.uv = float2(g_uv.z, g_uv.w);
	}else if(index == 3) {
		Out.uv = float2(g_uv.z, g_uv.y);
	}
	
	return Out;
}