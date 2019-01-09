float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjectionMatrix : Projection;

float4x4 gLightViewMatrix;
float4x4 gLightProjectionMatrix;

float4 lightPosition;
float4 cameraPosition;

texture		DiffuseMap_Tex;
sampler2D	DiffuseSampler;

texture		ShadowMap_Tex;
sampler2D	ShadowSampler;

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float3 mNormal : NORMAL;
	float2 mTexCoord : TEXCOORD0;
};

struct PS_INPUT
{
	float4 mPosition : POSITION;
	float3 mNormal : NORMAL;
	float2 mTexCoord : TEXCOORD0;
	float3 mDiffuse : TEXCOORD1;
	float3 mViewDir : TEXCOORD2;
	float3 mReflection : TEXCOORD3;
	float4 mClipPosition: TEXCOORD4;
};


PS_INPUT VsMain(VS_INPUT input)
{
	PS_INPUT Output;

	float4x4 lightViewMatrix = gLightViewMatrix;

	input.mPosition.w = 1.0f;
	float4 worldPosition = mul(input.mPosition, gWorldMatrix);
	
	float3 lightDir = normalize(worldPosition.xyz - lightPosition.xyz);

	float3 viewDir = normalize(worldPosition.xyz - cameraPosition.xyz);
	Output.mViewDir = viewDir;

	Output.mPosition = mul(worldPosition, gViewMatrix);
	Output.mPosition = mul(Output.mPosition, gProjectionMatrix);
	
	Output.mClipPosition = mul(worldPosition, lightViewMatrix);
	Output.mClipPosition = mul(Output.mClipPosition, gLightProjectionMatrix);

	float3 worldNormal = mul(input.mNormal, (float3x3)gWorldMatrix);
	worldNormal = normalize(worldNormal);

	Output.mDiffuse = dot(-lightDir, worldNormal);
	Output.mReflection = reflect(lightDir, worldNormal);

	Output.mNormal = mul(input.mNormal, (float3x3)gWorldMatrix);
	Output.mNormal = normalize(Output.mNormal);

	Output.mTexCoord = input.mTexCoord;

	return Output;
}

float4 PsMain(PS_INPUT input) : COLOR
{
	float3 lightColor = float3(1.0f, 1.0f, 1.0f);
	float3 directionlightColor = float3(0.1f, 0.1f, 0.1f);
	float4 	albedo = tex2D(DiffuseSampler, input.mTexCoord);

	float3 diffuse = lightColor * albedo.rgb * saturate(input.mDiffuse);

	float3 reflection = normalize(input.mReflection);
	float3 viewDir = normalize(input.mViewDir);

	float3 specular = 0;

	if (diffuse.x > 0)
	{
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 24.0f);

		specular = directionlightColor * specular;
	}

	float currentDepth = input.mClipPosition.z / input.mClipPosition.w;
	float2 uv = input.mClipPosition.xy / input.mClipPosition.w;
	uv.y = -uv.y;
	uv = uv * 0.5 + 0.5;

	float shadowDepth = tex2D(ShadowSampler, uv).r;

	if (currentDepth > shadowDepth + 0.0000125f)
	{
		diffuse *= 0.5f;
	}

	float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo.rgb;

	return float4(ambient + diffuse + specular, 1);
}

technique ApplyShadow
{
	pass Pass_0
	{
		VertexShader = compile vs_3_0 VsMain();
		PixelShader = compile ps_3_0 PsMain();
	}

}
