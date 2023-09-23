//************
// VARIABLES *
//************
float4x4 m_MatrixWorldViewProj : WORLDVIEWPROJECTION;
float4x4 m_MatrixWorld : WORLD;
float3 m_LightDirection : DIRECTION
<
	string Object = "TargetLight";
> = float3(0.577f, -0.577f, 0.577f);

float3 gColorDiffuse : COLOR = float3(1, 1, 1);

float gSpikeLength
<
	string UIWidget = "Slider";
	float UIMin = 0.0f;
	float UIMax = 0.5f;
	float UIStep = 0.0001f;
> = 0.2f;

RasterizerState FrontCulling
{
	CullMode = FRONT;
};

//**********
// STRUCTS *
//**********
struct VS_DATA
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
};

struct GS_DATA
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
};

//****************
// VERTEX SHADER *
//****************
VS_DATA MainVS(VS_DATA vsData)
{
	return vsData;
}

//******************
// GEOMETRY SHADER *
//******************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float3 normal, float2 texCoord)
{
	//Step 1. Create a GS_DATA object
	GS_DATA data = (GS_DATA)0;

	//Step 2. Transform the position using the WVP Matrix and assign it to (GS_DATA object).Position (Keep in mind: float3 -> float4)
	data.Position = mul(float4(pos, 1), m_MatrixWorldViewProj);

	//Step 3. Transform the normal using the World Matrix and assign it to (GS_DATA object).Normal (Only Rotation, No translation!)
	data.Normal = mul(float4(normal, 0), m_MatrixWorld).xyz;

	//Step 4. Append (GS_DATA object) to the TriangleStream parameter (TriangleStream::Append(...))
	triStream.Append(data);
}

[maxvertexcount(9)]
void SpikeGenerator(triangle VS_DATA vertices[3], inout TriangleStream<GS_DATA> triStream)
{
	//Use these variable names
	float3 basePoint, spikeNormal;
	float3 top = vertices[0].Position;
	float3 left = vertices[1].Position;
	float3 right = vertices[2].Position;

	//Step 1. Calculate CENTER_POINT
	basePoint = (top + left + right) / 3.0f;

	//Step 2. Calculate Face Normal (Original Triangle)
	spikeNormal = normalize(cross(top - left, right - left));

	//Step 3. Offset CENTER_POINT (use gSpikeLength)
	basePoint += spikeNormal * gSpikeLength;

	//Step 4 + 5. Calculate Individual Face Normals (Cross Product of Face Edges) & Create Vertices for every face

		//FACE 1
	spikeNormal = normalize(cross(top - left, basePoint - left));
	CreateVertex(triStream, basePoint, spikeNormal, float2(0, 0));
	CreateVertex(triStream, top, spikeNormal, float2(0, 0));
	CreateVertex(triStream, left, spikeNormal, float2(0, 0));

	triStream.RestartStrip();

	//FACE 2
	spikeNormal = normalize(cross(left - right, basePoint - right));
	CreateVertex(triStream, basePoint, spikeNormal, float2(0, 0));
	CreateVertex(triStream, left, spikeNormal, float2(0, 0));
	CreateVertex(triStream, right, spikeNormal, float2(0, 0));

	triStream.RestartStrip();

	//Face 3
	spikeNormal = normalize(cross(right - top, basePoint - top));
	CreateVertex(triStream, basePoint, spikeNormal, float2(0, 0));
	CreateVertex(triStream, right, spikeNormal, float2(0, 0));
	CreateVertex(triStream, top, spikeNormal, float2(0, 0));

	triStream.RestartStrip();

	//Step 6. Complete code in CreateVertex(...)
	//Step 7. Bind this Geometry Shader function to the effect pass (See Technique Struct)
}

//***************
// PIXEL SHADER *
//***************
float4 MainPS(GS_DATA input) : SV_TARGET
{
	input.Normal = normalize(input.Normal);
	float diffuseStrength = max(dot(normalize(m_LightDirection),-input.Normal),0.2f);
	return float4(gColorDiffuse * diffuseStrength,1.0f);
}

//*************
// TECHNIQUES *
//*************
technique11 Default //FXComposer >> Rename to "technique10 Default"
{
	pass p0
	{
		SetRasterizerState(FrontCulling);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, SpikeGenerator()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
