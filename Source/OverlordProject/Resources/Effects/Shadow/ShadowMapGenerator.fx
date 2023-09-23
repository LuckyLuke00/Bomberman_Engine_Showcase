float4x4 gWorld;
float4x4 gLightViewProj;
float4x4 gBones[70];

DepthStencilState depthStencilState
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState rasterizerState
{
	FillMode = SOLID;
	CullMode = NONE;
};

//--------------------------------------------------------------------------------------
// Vertex Shader [STATIC]
//--------------------------------------------------------------------------------------
float4 ShadowMapVS(float3 position:POSITION) :SV_POSITION
{
	//Return the position of the vertex in correct space (hint: seen from the view of the light)
	return mul(mul(float4(position, 1.0f), gWorld), gLightViewProj);
}

//--------------------------------------------------------------------------------------
// Vertex Shader [SKINNED]
//--------------------------------------------------------------------------------------
float4 ShadowMapVS_Skinned(float3 position:POSITION, float4 BoneIndices : BLENDINDICES, float4 BoneWeights : BLENDWEIGHTS) : SV_POSITION
{
	//Return the position of the ANIMATED vertex in correct space (hint: seen from the view of the light)
	float4 pos = float4(position, 1.0f);
	float4 skinnedPos = 0;

	[unroll]
	for (int i = 0; i < 4; ++i)
	{
		float4x4 boneTransform = gBones[int(BoneIndices[i])];
		skinnedPos += BoneWeights[i] * mul(pos, boneTransform);
	}

	skinnedPos = mul(skinnedPos, gWorld);
	skinnedPos = mul(skinnedPos, gLightViewProj);
	return skinnedPos;
}

//--------------------------------------------------------------------------------------
// Pixel Shaders
//--------------------------------------------------------------------------------------
void ShadowMapPS_VOID(float4 position:SV_POSITION) {}

technique11 GenerateShadows
{
	pass P0
	{
		SetRasterizerState(rasterizerState);
		SetDepthStencilState(depthStencilState, 0);
		SetVertexShader(CompileShader(vs_4_0, ShadowMapVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, ShadowMapPS_VOID()));
	}
}

technique11 GenerateShadows_Skinned
{
	pass P0
	{
		SetRasterizerState(rasterizerState);
		SetDepthStencilState(depthStencilState, 0);
		SetVertexShader(CompileShader(vs_4_0, ShadowMapVS_Skinned()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, ShadowMapPS_VOID()));
	}
}
