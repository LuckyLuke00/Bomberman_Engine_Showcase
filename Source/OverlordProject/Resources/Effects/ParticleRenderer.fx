float4x4 gWorldViewProj : WorldViewProjection;
float4x4 gViewInverse : ViewInverse;
Texture2D gParticleTexture;

SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;
};

//STATES
//******
BlendState AlphaBlending
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0f;
};

DepthStencilState DisableDepthWriting
{
	//Enable Depth Rendering
	DepthEnable = TRUE;
	//Disable Depth Writing
	DepthWriteMask = ZERO;
};

RasterizerState BackCulling
{
	CullMode = BACK;
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
	float3 Position : POSITION;
	float4 Color: COLOR;
	float Size : TEXCOORD0;
	float Rotation : TEXCOORD1;
};

struct GS_DATA
{
	float4 Position : SV_POSITION;
	float2 TexCoord: TEXCOORD0;
	float4 Color : COLOR;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
	return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float2 texCoord, float4 col, float2x2 uvRotation)
{
	//Step 1. Create a GS_DATA object
	GS_DATA output = (GS_DATA)0;

	//Step 2. Transform the position using the WVP Matrix and assign it to (GS_DATA object).Position (Keep in mind: float3 -> float4, Homogeneous Coordinates)
	output.Position = mul(float4(pos, 1.f), gWorldViewProj);

	//Step 3. Assign texCoord to (GS_DATA object).TexCoord
	output.TexCoord = texCoord;

	//This is a little formula to do texture rotation by transforming the texture coordinates (Can cause artifacts)
	texCoord -= float2(0.5f, 0.5f);
	texCoord = mul(texCoord, uvRotation);
	texCoord += float2(0.5f, 0.5f);

	//Step 4. Assign color to (GS_DATA object).Color
	output.Color = col;

	//Step 5. Append (GS_DATA object) to the TriangleStream parameter (TriangleStream::Append(...))
	triStream.Append(output);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	float size = vertex[0].Size;
    float3 origin = vertex[0].Position;

    // Calculate the camera view vector in world space
    float3 viewVector = normalize(gViewInverse[2].xyz);

    // Calculate the right and up vectors for the billboard
    float3 rightVector = normalize(cross(float3(0, 1, 0), viewVector));
    float3 upVector = normalize(cross(viewVector, rightVector));

    // Calculate the billboard corners
    float3 topLeft = origin - rightVector * size + upVector * size;
    float3 topRight = origin + rightVector * size + upVector * size;
    float3 bottomLeft = origin - rightVector * size - upVector * size;
    float3 bottomRight = origin + rightVector * size - upVector * size;

    // This is the 2x2 rotation matrix we need to transform our TextureCoordinates (Texture Rotation)
    float2x2 uvRotation = { cos(vertex[0].Rotation), -sin(vertex[0].Rotation),
                            sin(vertex[0].Rotation), cos(vertex[0].Rotation) };

    // Create Geometry (Triangle strip)
    CreateVertex(triStream, topLeft, float2(0, 0), vertex[0].Color, uvRotation);
    CreateVertex(triStream, topRight, float2(1, 0), vertex[0].Color, uvRotation);
    CreateVertex(triStream, bottomLeft, float2(0, 1), vertex[0].Color, uvRotation);
    CreateVertex(triStream, bottomRight, float2(1, 1), vertex[0].Color, uvRotation);
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET{
	//Simple Texture Sampling
	float4 result = gParticleTexture.Sample(samPoint,input.TexCoord);
	return input.Color * result;
}

// Default Technique
technique10 Default {
	pass p0 {
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));

		SetRasterizerState(BackCulling);
		SetDepthStencilState(DisableDepthWriting, 0);
		SetBlendState(AlphaBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}
