#include "hzpch.h"
#include "Drawable.h"
#include "IndexBuffer.h"


namespace Hazel {
	
	void Drawable::Draw(DirectXGraphics& graphics) const
	{
		for (auto& b : binds)
		{
			b->Bind(graphics);
		}
		for (auto& b : GetStaticBinds())
		{
			b->Bind(graphics);
		}
		graphics.DrawIndexed(pIndexBuffer->GetCount());
	}

	void Drawable::AddBind(std::unique_ptr<Bindable> bind)
	{
		assert("*MUST* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
		binds.push_back(std::move(bind));
	}

	void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
	{
		pIndexBuffer = ibuf.get();
		binds.push_back(std::move(ibuf));
	}
}