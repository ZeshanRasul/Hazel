#include "hzpch.h"
#include "Pyramid.h"
#include "BindableBase.h"
#include "Cone.h"

namespace Hazel {

	Pyramid::Pyramid(DirectXGraphics& graphics, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist)
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
				struct
				{
					unsigned char r;
					unsigned char g;
					unsigned char b;
					unsigned char a;
				} colour;
			};

			auto model = Cone::MakeTesselated<Vertex>(4);

			// Set Vertex Colours
			model.vertices[0].colour = { 255, 255, 0 };
			model.vertices[1].colour = { 255, 255, 0 };
			model.vertices[2].colour = { 255, 255, 0 };
			model.vertices[3].colour = { 255, 255, 0 };
			model.vertices[4].colour = { 255, 255, 80 };
			model.vertices[5].colour = { 255, 10, 0 };

			// Deform mesh linearly
			model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 0.7f));

			// Create Vertex Buffer using model vertices and bind
			AddStaticBind(std::make_unique<VertexBuffer>(graphics, model.vertices));

			auto pVertexShader = std::make_unique<VertexShader>(graphics, L"../Hazel/ColourBlendVS.cso");

			auto pVertexShaderBytecode = pVertexShader->GetBytecode();

			AddStaticBind(std::move(pVertexShader));

			AddStaticBind(std::make_unique<PixelShader>(graphics, L"../Hazel/ColourBlendPS.cso"));

			AddStaticIndexBuffer(std::make_unique<IndexBuffer>(graphics, model.indices));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc =
			{
				{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
				{"Colour", 0u, DXGI_FORMAT_R8G8B8A8_UNORM, 0u, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0u}
			};

			AddStaticBind(std::make_unique<InputLayout>(graphics, inputDesc, pVertexShaderBytecode));

			AddStaticBind(std::make_unique<Topology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else
		{
			SetIndexFromStatic();
		}

		AddBind(std::make_unique<TransformConstantBuffer>(graphics, *this));
	}

	void Pyramid::Update(float dt) noexcept
	{
		roll += droll * dt;
		pitch += dpitch * dt;
		yaw += dyaw * dt;
		theta += dtheta * dt;
		phi += dphi * dt;
		chi += dchi * dt;
	}

	DirectX::XMMATRIX Pyramid::GetTransformXM() const noexcept
	{
		return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
			DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
			DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
	}
}