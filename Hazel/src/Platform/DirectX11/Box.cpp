#include "hzpch.h"
#include "Box.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"

namespace Hazel {

	Box::Box(DirectXGraphics& graphics, std::mt19937& rng,
		std::uniform_real_distribution<float> adist,
		std::uniform_real_distribution<float> ddist,
		std::uniform_real_distribution<float> odist,
		std::uniform_real_distribution<float> rdist)
		:
		r(rdist(rng)),
		droll(ddist(rng)),
		dpitch(ddist(rng)),
		dyaw(ddist(rng)),
		dphi(odist(rng)),
		dtheta(odist(rng)),
		dchi(odist(rng)),
		chi(adist(rng)),
		theta(adist(rng)),
		phi(adist(rng))
	{
		struct Vertex
		{
			struct
			{
				float x;
				float y;
				float z;
			} position;

		};

		const std::vector<Vertex> vertices =
		{
			{ -1.0f,-1.0f,-1.0f	 },
			{ 1.0f,-1.0f,-1.0f	 },
			{ -1.0f,1.0f,-1.0f	 },
			{ 1.0f,1.0f,-1.0f	 },
			{ -1.0f,-1.0f,1.0f	 },
			{ 1.0f,-1.0f,1.0f	 },
			{ -1.0f,1.0f,1.0f	 },
			{ 1.0f,1.0f,1.0f	 },
		};

		AddBind(std::make_unique<VertexBuffer>(graphics, vertices));

		auto pVertexShader = std::make_unique<VertexShader>(graphics, L"../Hazel/VertexShader.cso");
		auto pVertexShaderBytecode = pVertexShader->GetBytecode();

		AddBind(std::move(pVertexShader));

		AddBind(std::make_unique<PixelShader>(graphics, L"../Hazel/PixelShader.cso"));

		const std::vector<unsigned short> indices =
		{
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4
		};

		AddIndexBuffer(std::make_unique<IndexBuffer>(graphics, indices));

		struct ConstantBufferColour
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colours[6];
		};

		ConstantBufferColour cbColour =
		{
			{
				{1.0f,0.0f,1.0f},
				{1.0f,0.0f,0.0f},
				{0.0f,1.0f,0.0f},
				{0.0f,0.0f,1.0f},
				{1.0f,1.0f,0.0f},
				{0.0f,1.0f,1.0f},
			}
		};

		AddBind(std::make_unique<PixelConstantBuffer<ConstantBufferColour>>(graphics, cbColour));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc =
		{
			{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u }
		};

		AddBind(std::make_unique<InputLayout>(graphics, inputDesc, pVertexShaderBytecode));

		AddBind(std::make_unique<Topology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		AddBind(std::make_unique<TransformConstantBuffer>(graphics, *this));

	}

	void Box::Update(float dt) noexcept
	{
		roll += droll * dt;
		pitch += dpitch * dt;
		yaw += dyaw * dt;
		theta += dtheta * dt;
		phi += dphi * dt;
		chi += dchi * dt;
	}

	DirectX::XMMATRIX Box::GetTransformXM() const noexcept
	{
		return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
			DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
			DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
	}
}