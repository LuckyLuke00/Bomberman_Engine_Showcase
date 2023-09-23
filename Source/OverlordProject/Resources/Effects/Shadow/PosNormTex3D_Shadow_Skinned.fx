float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION;
float4x4 gWorldViewProj_Light;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float gShadowMapBias = 0.01f;
float4x4 gBones[70];

Texture2D gDiffuseMap;
Texture2D gShadowMap;

SamplerComparisonState cmpSampler
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;// or Mirror or Clamp or Border
	AddressV = Wrap;// or Mirror or Clamp or Border
};

SamplerComparisonState samComparison
{
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
	ComparisonFunc = LESS_EQUAL;
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 BoneIndices : BLENDINDICES;
	float4 BoneWeights : BLENDWEIGHTS;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 lPos : TEXCOORD1;
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

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 transformedPosition = float4(0, 0, 0, 0);
	float3 transformedNormal = float3(0, 0, 0);

	// For each bone influencing this vertex
	for (int i = 0; i < 4; ++i)
	{
		// Get the bone index
		float boneIndex = input.BoneIndices[i];
		if (boneIndex < 0)
			continue;

		// Append this Position & Normal to the transformedPosition & transformedNormal based on the corresponding bone weight
		transformedPosition += mul(float4(input.pos, 1), gBones[int(boneIndex)]) * input.BoneWeights[i];
		transformedNormal += mul(float4(input.normal, 0), gBones[int(boneIndex)]) * input.BoneWeights[i];
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
	output.lPos = mul(transformedPosition, gWorldViewProj_Light);
	return output;
}

float2 texOffset(int u, int v)
{
	// Compute the texel size for the shadow map
	float2 texelSize = 1.0f / float2(1280, 720);

	// Compute the offset for the given texel
	return texelSize * float2(u, v);
}

float EvaluateShadowMap(float4 lpos)
{
	//re-homogenize position after interpolation
	lpos.xyz /= lpos.w;

	//if position is not visible to the light - dont illuminate it
	//results in hard light frustum
	if (lpos.x < -1.0f || lpos.x > 1.0f ||
		lpos.y < -1.0f || lpos.y > 1.0f ||
		lpos.z < 0.0f || lpos.z > 1.0f)
		return 1.f;

	//transform clip space coords to texture space coords (-1:1 to 0:1)
	lpos.x = lpos.x * 0.5f + 0.5f;
	lpos.y = lpos.y * -0.5f + 0.5f;
	lpos.z -= gShadowMapBias;

	float sum = 0.0f;
	for (float u = -1.5f; u <= 1.5f; u += 1.0f)
	{
		for (float v = -1.5f; v <= 1.5f; v += 1.0f)
		{
			sum += gShadowMap.SampleCmpLevelZero(samComparison, lpos.xy + texOffset(u, v), lpos.z);
		}
	}

	return sum / 16.0f * 0.5f + 0.5f;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float shadowValue = EvaluateShadowMap(input.lPos);

	float4 diffuseColor = gDiffuseMap.Sample(samLinear,input.texCoord);
	float3 color_rgb = diffuseColor.rgb;
	float color_a = diffuseColor.a;

	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;

	return float4(color_rgb * shadowValue, color_a);
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

		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}
