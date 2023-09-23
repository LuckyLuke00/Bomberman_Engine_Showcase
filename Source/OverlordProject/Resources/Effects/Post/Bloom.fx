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

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
/// Create Rasterizer State (Backface culling) 

DepthStencilState EnableDepthWriting
{
DepthEnable = true;
DepthWriteMask = ALL;
DepthFunc = LESS_EQUAL;
};

RasterizerState BackfaceCulling
{
CullMode = BACK;
FillMode = SOLID;
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
	// Set the Position
	output.Position = float4(input.Position, 1.f);

	// Set the TexCoord
	output.TexCoord = input.TexCoord;

	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
    // Step 1: Find the dimensions of the texture
    float width, height, levels;
    gTexture.GetDimensions(0, width, height, levels);

    // Step 2: Calculate dx and dy (UV space for 1 pixel)
    float dx = 1.f / width;
    float dy = 1.f / height;

    // Step 3: Create a double for loop (5 iterations each)
    float4 finalColor = float4(.0f, .0f, .0f, .0f);
    int blurDiameter = 10;
    for (int i = 0; i < blurDiameter; i++)
    {
        for (int j = 0; j < blurDiameter; j++)
        {
            float2 offset = float2(dx * 2 * (i - blurDiameter / 2), dy * 2 * (j - blurDiameter / 2));
            float2 uv = input.TexCoord + offset;

            // Do a texture lookup using the calculated UV coordinates + the offset
            float4 color = gTexture.Sample(samPoint, uv);

            // Apply a brightness threshold to only consider bright pixels
            float brightnessThreshold = 1.25f; // Adjust this value to control the bloom threshold
            if (color.r + color.g + color.b > brightnessThreshold)
            {
                // Increase the brightness of the bright pixels
                float bloomStrength = .5f; // Adjust this value to control the bloom strength
                color.rgb *= bloomStrength;

                // Add the bright color to the final color
                finalColor += color;
            }
        }
    }

    // Step 4: Divide the final color by the number of passes (in this case 5*5)
    finalColor /= (blurDiameter * blurDiameter);

    // Step 5: Add the original color to the final color to preserve the original image details
    finalColor += gTexture.Sample(samPoint, input.TexCoord);

    // Step 6: Return the final color
    return finalColor;
}



//TECHNIQUE
//---------
technique11 Blur
{
    pass P0
    {
		SetDepthStencilState(EnableDepthWriting, 0);
        SetRasterizerState(BackfaceCulling);
        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetPixelShader(CompileShader(ps_4_0, PS()));
    }
}