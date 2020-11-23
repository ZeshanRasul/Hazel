#pragma once

namespace Hazel {

	class Graphics
	{
	public:
		// TODO can refactor to take window instead when implementing other rendering APIs 
		virtual void Init(HWND hWnd) = 0;
		virtual void ClearBuffer(float red, float green, float blue) = 0;
		virtual void EndFrame() = 0;
		virtual void DrawTestTriangle() = 0;
	};

}