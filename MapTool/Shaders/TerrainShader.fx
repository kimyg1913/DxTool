float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjectionMatrix : Projection;

texture		DiffuseMap_Tex;
sampler2D	DiffuseSampler;

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

struct PS_INPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
};


PS_INPUT VsMain(VS_INPUT input)
{
	PS_INPUT Output;

	input.mPosition.w = 1.0f;

	Output.mPosition = mul(input.mPosition, gWorldMatrix);
	Output.mPosition = mul(Output.mPosition, gViewMatrix);
	Output.mPosition = mul(Output.mPosition, gProjectionMatrix);

	Output.mTexCoord = input.mTexCoord;

	return Output;
}

float4 PsMain(PS_INPUT input) : COLOR
{
	float4 albedo = tex2D(DiffuseSampler, input.mTexCoord);
	return albedo.rgba;
}

technique Terrain
{
	pass Pass_0
	{
		VertexShader = compile vs_3_0 VsMain();
		PixelShader = compile ps_3_0 PsMain();
	}

}
