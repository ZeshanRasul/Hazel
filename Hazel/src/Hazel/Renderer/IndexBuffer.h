#pragma once

#include "hzpch.h"

namespace Hazel {

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};

}