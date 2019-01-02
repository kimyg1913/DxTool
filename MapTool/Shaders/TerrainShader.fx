float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjectionMatrix : Projection;

float4 lightPosition;
float4 cameraPosition;

texture		DiffuseMap_Tex;
sampler2D	DiffuseSampler;

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
};


PS_INPUT VsMain(VS_INPUT input)
{
	PS_INPUT Output;

	input.mPosition.w = 1.0f;

	Output.mPosition = mul(input.mPosition, gWorldMatrix);

	float3 lightDir = normalize(Output.mPosition.xyz - lightPosition.xyz);

	float3 viewDir = normalize(Output.mPosition.xyz - cameraPosition.xyz);
	Output.mViewDir = viewDir;

	Output.mPosition = mul(Output.mPosition, gViewMatrix);
	Output.mPosition = mul(Output.mPosition, gProjectionMatrix);

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
	float3 directionlightColor = float3(0.1f, 0.f, 0.f);
	float4 	albedo = tex2D(DiffuseSampler, input.mTexCoord);

	float3 diffuse = lightColor * albedo.rgb * saturate(input.mDiffuse);

	float3 reflection = normalize(input.mReflection);
	float3 viewDir = normalize(input.mViewDir);

	float3 specular = 0;

	if (diffuse.x > 0)
	{
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 2.0f);

		specular = directionlightColor * specular;
	}

	float3 ambient = float3(0.3f, 0.3f, 0.3f) * albedo.rgb;
	
	return float4(ambient + diffuse + specular, 1);
}

technique Terrain
{
	pass Pass_0
	{
		VertexShader = compile vs_3_0 VsMain();
		PixelShader = compile ps_3_0 PsMain();
	}

}
