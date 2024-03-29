#include "hzpch.h"
#include "Box.h"
#include "BindableBase.h"
#include "Cube.h"
#include "GraphicsThrowMacros.h"

namespace Hazel {

	Box::Box(DirectXGraphics& graphics, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist)
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
		if (!IsStaticInitialized())
		{

			struct Vertex
			{
				DirectX::XMFLOAT3 pos;
			};

			auto model = Cube::Make<Vertex>();

			AddStaticBind(std::make_unique<VertexBuffer>(graphics, model.vertices));

			auto pVertexShader = std::make_unique<VertexShader>(graphics, L"../Hazel/ColourIndexVS.cso");
			auto pVertexShaderBytecode = pVertexShader->GetBytecode();

			AddStaticBind(std::move(pVertexShader));

			AddStaticBind(std::make_unique<PixelShader>(graphics, L"../Hazel/ColourIndexPS.cso"));

		
			AddStaticIndexBuffer(std::make_unique<IndexBuffer>(graphics, model.indices));

			struct PixelShaderConstants
			{
				struct
				{
					float r;
					float g;
					float b;
					float a;
				} face_colours[8];
			};

			PixelShaderConstants cbColour =
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

			AddStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(graphics, cbColour));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc =
			{
				{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u }
			};

			AddStaticBind(std::make_unique<InputLayout>(graphics, inputDesc, pVertexShaderBytecode));

			AddStaticBind(std::make_unique<Topology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else 
		{
			SetIndexFromStatic();
		}

		AddBind(std::make_unique<TransformConstantBuffer>(graphics, *this));

		DirectX::XMStoreFloat3x3(&mt, DirectX::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
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
		return 
			DirectX::XMLoadFloat3x3(&mt) *
			DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
			DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
			DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
	}
}