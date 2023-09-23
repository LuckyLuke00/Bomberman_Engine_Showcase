//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Mirror;
	AddressV = Mirror;
};

Texture2D gTexture;

DepthStencilState EnableDepthWrite
{
	DepthWriteMask = ALL;
};

RasterizerState BackFaceCulling
{
	CullMode = BACK;
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
	float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};

//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.Position = float4(input.Position, 1.f);
	output.TexCoord = input.TexCoord;

	return output;
}

//PIXEL SHADER
//------------
float4 PS(PS_INPUT input) : SV_Target
{
	// Step 1: find the dimensions of the texture (the texture has a method for that)
	float2 texDim = float2(.0f, .0f);
	gTexture.GetDimensions(texDim.x, texDim.y);

	// Step 2: calculate dx and dy (UV space for 1 pixel)
	float dx = 1.f / texDim.x;
	float dy = 1.f / texDim.y;

	// Step 3: Create a double for loop (5 iterations each)
	float4 finalColor = float4(.0f, .0f, .0f, 1.f);

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			// Inside the loop, calculate the offset in each direction. Make sure not to take every pixel but move by 2 pixels each time
			// Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
			float2 offset = float2(i * dx * 2.f, j * dy * 2.f);
			finalColor += gTexture.Sample(samPoint, input.TexCoord + offset);
		}
	}

	// Step 4: Divide the final color by the number of passes (in this case 5*5)
	finalColor /= 25.f;
	// Step 5: return the final color
	return finalColor;
}

//TECHNIQUE
//---------
technique11 Blur
{
	pass P0
	{
		// Set states...
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));

		SetDepthStencilState(EnableDepthWrite, 0);
		SetRasterizerState(BackFaceCulling);
	}
}
