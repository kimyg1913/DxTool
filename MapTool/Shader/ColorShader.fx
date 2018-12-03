float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjectionMatrix : Projection;

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float4 mColor : COLOR;
};

struct PS_INPUT
{
	float4 mPosition : POSITION;
	float4 mColor : COLOR;
};

PS_INPUT VsMain(VS_INPUT input)
{
	PS_INPUT Output;

	input.mPosition.w = 1.0f;

	Output.mPosition = mul(input.mPosition, gWorldMatrix);
	Output.mPosition = mul(Output.mPosition, gViewMatrix);
	Output.mPosition = mul(Output.mPosition, gProjectionMatrix);

	Output.mColor = input.mColor;

	return Output;
}

float4 PsMain(PS_INPUT input) : COLOR
{
	return input.mColor;
}

technique Color
{
	pass Pass_0
	{
		VertexShader = compile vs_3_0 VsMain();
		PixelShader = compile ps_3_0 PsMain();
	}

}
