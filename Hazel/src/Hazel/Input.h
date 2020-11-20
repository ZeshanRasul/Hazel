#pragma once

#include "hzpch.h"
#include "Hazel/Core.h"

namespace Hazel {

	class HAZEL_API Input
	{
#ifdef HZ_PLATFORM_WINDOWS
			friend class WindowsWindow;
	public:
		class InputKeyboardEvent
		{
		public:
			enum class Type
			{
				Press,
				Release,
				Invalid
			};
		private:
			Type type;
			unsigned char code;
		public:
			InputKeyboardEvent()
				:
				type(Type::Invalid),
				code(0u)
			{

			}

			InputKeyboardEvent(Type type, unsigned char code) noexcept
				:
				type(type),
				code(code)
			{

			}

			bool IsPress() const noexcept
			{
				return type == Type::Press;
			}

			bool IsRelease() const noexcept
			{
				return type == Type::Release;
			}

			bool IsValid() const noexcept
			{
				return type != Type::Invalid;
			}

			unsigned char GetCode() const noexcept
			{
				return code;
			}
		};

	public:
		class InputMouseEvent
		{
		public:
			enum class Type
			{
				LPress,
				LRelease,
				RPress,
				RRelease,
				WheelUp,
				WheelDown,
				Move,
				Invalid,
				Enter,
				Leave
			};

		private:
			Type type;
			bool isLeftPressed;
			bool isRightPressed;
			int x;
			int y;

		public:
			InputMouseEvent() noexcept
				:
				type(Type::Invalid),
				isLeftPressed(false),
				isRightPressed(false),
				x(0),
				y(0)
			{}

			InputMouseEvent(Type type) noexcept
				:
				type(type),
				isLeftPressed(Input::IsLeftPressed()),
				isRightPressed(Input::IsRightPressed()),
				x(Input::GetMousePosX()),
				y(Input::GetMousePosY())
			{}

			bool IsValid() const noexcept
			{
				return type != Type::Invalid;
			}

			Type GetType() const noexcept
			{
				return type;
			}

			std::pair<int, int> GetMousePos() const noexcept
			{
				return { x,y };
			}

			int GetMousePosX() const noexcept
			{
				return x;
			}

			int GetMousePosY() const noexcept
			{
				return y;
			}

			bool IsLeftPressed() const noexcept
			{
				return isLeftPressed;
			}

			bool IsRightPressed() const noexcept
			{
				return isRightPressed;
			}
		};
#endif

	public:
		///////////////////////
		///KEYBOARD FUNCTIONS//
		///////////////////////

		inline static bool IsKeyPressed(unsigned char keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		inline static InputKeyboardEvent ReadKey() { return s_Instance->ReadKeyImpl(); }
		inline static bool IsKeyEmpty() { return s_Instance->IsKeyEmptyImpl(); }
		inline static void FlushKey() { return s_Instance->FlushKeyImpl(); }
		inline static char ReadChar() { return s_Instance->ReadCharImpl(); }
		inline static bool IsCharEmpty() { return s_Instance->IsCharEmptyImpl(); }
		inline static void FlushChar() { return s_Instance->FlushCharImpl(); }
		inline static void FlushKeyboard()	   { return s_Instance->FlushKeyboardImpl(); }
		inline static void EnableAutorepeat() { return s_Instance->EnableAutorepeatImpl(); }
		inline static void DisableAutorepeat() { return s_Instance->DisableAutorepeatImpl(); }
		inline static bool AutorepeatIsEnabled() { return s_Instance->AutorepeatIsEnabledImpl(); }

		///////////////////////
		///MOUSE FUNCTIONS/////
		///////////////////////

		inline static std::pair<int, int> GetMousePos() { return s_Instance->GetMousePosImpl(); }
		inline static int GetMousePosX() { return s_Instance->GetMousePosXImpl(); }
		inline static int GetMousePosY() { return s_Instance->GetMousePosYImpl(); }
		inline static bool IsInWindow() { return s_Instance->IsInWindowImpl(); }
		inline static bool IsLeftPressed() { return s_Instance->IsLeftPressedImpl(); }
		inline static bool IsRightPressed() { return s_Instance->IsRightPressedImpl(); }
		inline static InputMouseEvent ReadMouse() { return s_Instance->ReadMouseImpl(); }
		inline static bool IsMouseEmpty() { return s_Instance->IsMouseEmptyImpl(); }
		inline static void FlushMouse() { return s_Instance->FlushMouseImpl(); }
		// TODO Check if TrimBuffer needs to be implemented here.

	protected:
		///////////////////////
		///KEYBOARD FUNCTIONS//
		///////////////////////

		virtual bool IsKeyPressedImpl(unsigned char keycode) = 0;
		virtual InputKeyboardEvent ReadKeyImpl() = 0;
		virtual bool IsKeyEmptyImpl() = 0;
		virtual void FlushKeyImpl() = 0;
		virtual char ReadCharImpl() = 0;
		virtual bool IsCharEmptyImpl() = 0;
		virtual void FlushCharImpl() = 0;
		virtual void FlushKeyboardImpl() = 0;
		virtual void EnableAutorepeatImpl() = 0;
		virtual void DisableAutorepeatImpl() = 0;
		virtual bool AutorepeatIsEnabledImpl() = 0;

		///////////////////////
		///MOUSE FUNCTIONS/////
		///////////////////////

		virtual std::pair<int, int> GetMousePosImpl() = 0;
		virtual int GetMousePosXImpl() = 0;
		virtual int GetMousePosYImpl() = 0;
		virtual bool IsInWindowImpl() = 0;
		virtual bool IsLeftPressedImpl() = 0;
		virtual bool IsRightPressedImpl() = 0;
		virtual InputMouseEvent ReadMouseImpl() = 0;
		virtual bool IsMouseEmptyImpl() = 0;
		virtual void FlushMouseImpl() = 0; 

	private:

		///////////////////////
		///KEYBOARD FUNCTIONS//
		///////////////////////

		inline static void OnKeyPressed(unsigned char keycode) { return s_Instance->OnKeyPressedImpl(keycode); }
		inline static void OnKeyReleased(unsigned char keycode) { return s_Instance->OnKeyReleasedImpl(keycode); }
		inline static void OnChar(char character) { return s_Instance->OnCharImpl(character); }
		inline static void ClearState() { return s_Instance->ClearStateImpl(); }
		// TODO: See if TrimBuffer is needed here


		virtual void OnKeyPressedImpl(unsigned char keycode) = 0;
		virtual void OnKeyReleasedImpl(unsigned char keycode) = 0;
		virtual void OnCharImpl(char character) = 0;
		virtual void ClearStateImpl() = 0;

		///////////////////////
		///MOUSE FUNCTIONS/////
		///////////////////////

		inline static void OnMouseMove(int x, int y) { return s_Instance->OnMouseMoveImpl(x, y); }
		inline static void OnMouseEnter() { return s_Instance->OnMouseEnterImpl(); }
		inline static void OnMouseLeave() { return s_Instance->OnMouseLeaveImpl(); }
		inline static void OnLeftPressed(int x, int y) { return s_Instance->OnLeftPressedImpl(x, y); }
		inline static void OnLeftReleased(int x, int y) { return s_Instance->OnLeftReleasedImpl(x, y); }
		inline static void OnRightPressed(int x, int y) { return s_Instance->OnRightPressedImpl(x, y); }
		inline static void OnRightReleased(int x, int y) { return s_Instance->OnRightReleasedImpl(x, y); }
		inline static void OnWheelUp(int x, int y) { return s_Instance->OnWheelUpImpl(x, y); }
		inline static void OnWheelDown(int x, int y) { return s_Instance->OnWheelDownImpl(x, y); }
		inline static void OnWheelDelta(int x, int y, int delta) { return s_Instance->OnWheelDeltaImpl(x, y, delta); }
		inline static void TrimMouseBuffer() { return s_Instance->TrimMouseBufferImpl(); }

		virtual void OnMouseMoveImpl(int x, int y) = 0;
		virtual void OnMouseEnterImpl() = 0;
		virtual void OnMouseLeaveImpl() = 0;
		virtual void OnLeftPressedImpl(int x, int y) = 0;
		virtual void OnLeftReleasedImpl(int x, int y) = 0;
		virtual void OnRightPressedImpl(int x, int y) = 0;
		virtual void OnRightReleasedImpl(int x, int y) = 0;
		virtual void OnWheelUpImpl(int x, int y) = 0;
		virtual void OnWheelDownImpl(int x, int y) = 0;
		virtual void OnWheelDeltaImpl(int x, int y, int delta) = 0;
		virtual void TrimMouseBufferImpl() = 0;;

	private:
		static Input* s_Instance;
	};

}