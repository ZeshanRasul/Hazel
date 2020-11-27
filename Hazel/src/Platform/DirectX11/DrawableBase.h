#pragma once
#include "Drawable.h"
#include "IndexBuffer.h"

namespace Hazel {

	template<class T>
	class DrawableBase : public Drawable
	{
	protected:
		static bool IsStaticInitialized() noexcept
		{
			return !staticBinds.empty();
		}

		static void AddStaticBind(std::unique_ptr<Bindable> bind)
		{
			assert("*MUST* use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
			staticBinds.push_back(std::move(bind));
		}

		void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf)
		{
			assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
			pIndexBuffer = ibuf.get();
			staticBinds.push_back(std::move(ibuf));
		}

		void SetIndexFromStatic() 
		{
			for (const auto& b : staticBinds)
			{
				if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
				{
					pIndexBuffer = p;
					return;
				}
			}

			assert("Failed to find index buffer in static binds" && pIndexBuffer != nullptr);
		}

	private:
		const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
		{
			return staticBinds;
		}

	private:
		static std::vector<std::unique_ptr<Bindable>> staticBinds;
	};

	template<class T>
	std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;
}