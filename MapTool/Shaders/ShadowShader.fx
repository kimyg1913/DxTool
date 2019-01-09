float4x4 gWorldMatrix : World;
float4x4 gLightViewMatrix : View;
float4x4 gLightProjectionMatrix : Projection;

float4x4 gLightWVPMatrix;
float4 gWorldLightPosition;

struct VS_INPUT
{
	float4 mPosition: POSITION;
};

struct PS_INPUT
{
	float4 mPosition: POSITION;
	float4 mClipPosition: TEXCOORD1;
};

PS_INPUT VsMain(VS_INPUT input)
{
	PS_INPUT Output;

	Output.mPosition = mul(input.mPosition, gWorldMatrix);
	Output.mPosition = mul(Output.mPosition, gLightViewMatrix);
	Output.mPosition = mul(Output.mPosition, gLightProjectionMatrix);
	
	Output.mClipPosition = Output.mPosition;

	return Output;
}

float4 PsMain(PS_INPUT input) : COLOR
{
	//w는 왜 곱하는 걸까?
	float depth = input.mClipPosition.z / input.mClipPosition.w;
	return float4(depth.xxx, 1);
}


technique Shadow
{
	pass Pass_0
	{
		VertexShader = compile vs_3_0 VsMain();
		PixelShader = compile ps_3_0 PsMain();
	}

}
