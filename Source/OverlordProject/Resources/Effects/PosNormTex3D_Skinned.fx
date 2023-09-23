float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);

float4x4 gBones[70];

Texture2D gDiffuseMap;
SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;// or Mirror or Clamp or Border
	AddressV = Wrap;// or Mirror or Clamp or Border
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};

struct VS_INPUT {
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 blendIndices : BLENDINDICES;
	float4 blendWeights : BLENDWEIGHTS;
};
struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

BlendState NoBlending
{
	BlendEnable[0] = FALSE;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 transformedPosition = float4(0, 0, 0, 0);
	float3 transformedNormal = float3(0, 0, 0);

	// For each bone influencing this vertex
	for (int i = 0; i < 4; ++i)
	{
		// Get the bone index
		float boneIndex = input.blendIndices[i];
		if (boneIndex < 0)
			continue;

		// Append this Position & Normal to the transformedPosition & transformedNormal based on the corresponding bone weight
		transformedPosition += mul(float4(input.pos, 1), gBones[int(boneIndex)]) * input.blendWeights[i];
		transformedNormal += mul(float4(input.normal, 0), gBones[int(boneIndex)]) * input.blendWeights[i];
	}
	transformedNormal = normalize(transformedNormal);

	// Ensure that the w-component of the transformedPosition is 1
	transformedPosition.w = 1;

	// Step 1: convert position into float4 and multiply with matWorldViewProj
	output.pos = mul(transformedPosition, gWorldViewProj);

	// Step 2: rotate the normal - No Translation
	//			this is achieved by clipping the 4x4 to a 3x3 matrix,
	//			thus removing the postion row of the matrix
	output.normal = normalize(mul(transformedNormal, (float3x3)gWorld));
	output.texCoord = input.texCoord;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET{
	float4 diffuseColor = gDiffuseMap.Sample(samLinear,input.texCoord);
	float3 color_rgb = diffuseColor.rgb;
	float color_a = diffuseColor.a;

	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;

	return float4(color_rgb , color_a);
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
	pass P0
	{
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}
