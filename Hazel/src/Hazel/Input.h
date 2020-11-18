#pragma once

#include "hzpch.h"
#include "Hazel/Core.h"

namespace Hazel {

	class HAZEL_API Input
	{
		friend class Window;
	public:
		class Win32Event
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
			Win32Event()
				:
				type(Type::Invalid),
				code(0u)
			{

			}

			Win32Event(Type type, unsigned char code) noexcept
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
		inline static bool IsKeyPressed(unsigned char keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		inline static Win32Event ReadKey() { return s_Instance->ReadKeyImpl(); }
		inline static bool IsKeyEmpty() { return s_Instance->IsKeyEmptyImpl(); }
		inline static void FlushKey() { return s_Instance->FlushKeyImpl(); }
		inline static char ReadChar() { return s_Instance->ReadCharImpl(); }
		inline static bool IsCharEmpty() { return s_Instance->IsCharEmptyImpl(); }
		inline static void FlushChar() { return s_Instance->FlushCharImpl(); }
		inline static void Flush()	   { return s_Instance->FlushImpl(); }
		inline static void EnableAutorepeat() { return s_Instance->EnableAutorepeatImpl(); }
		inline static void DisableAutorepeat() { return s_Instance->DisableAutorepeatImpl(); }
		inline static bool AutorepeatIsEnabled() { return s_Instance->AutorepeatIsEnabledImpl(); }
		/*
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); };
		inline static std::pair<float, float> GetMousePosition()  { return s_Instance->GetMousePositionImpl(); }
		inline static bool GetMouseX()  { return s_Instance->GetMouseXImpl(); }
		inline static bool GetMouseY()  { return s_Instance->GetMouseYImpl(); }
		*/

		inline static void OnKeyPressed(unsigned char keycode) {
			return s_Instance->OnKeyPressedImpl(keycode);
		}
		inline static void OnKeyReleased(unsigned char keycode) {
			return s_Instance->OnKeyReleasedImpl(keycode);
		}
		inline static void OnChar(char character) { return s_Instance->OnCharImpl(character); }
		inline static void ClearState() { return s_Instance->ClearStateImpl(); }


	protected:
		virtual bool IsKeyPressedImpl(unsigned char keycode) = 0;
		virtual Win32Event ReadKeyImpl() = 0;
		virtual bool IsKeyEmptyImpl() = 0;
		virtual void FlushKeyImpl() = 0;
		virtual char ReadCharImpl() = 0;
		virtual bool IsCharEmptyImpl() = 0;
		virtual void FlushCharImpl() = 0;
		virtual void FlushImpl() = 0;
		virtual void EnableAutorepeatImpl() = 0;
		virtual void DisableAutorepeatImpl() = 0;
		virtual bool AutorepeatIsEnabledImpl() = 0;


		/*
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		*/

	private:


		virtual void OnKeyPressedImpl(unsigned char keycode) = 0;
		virtual void OnKeyReleasedImpl(unsigned char keycode) = 0;
		virtual void OnCharImpl(char character) = 0;
		virtual void ClearStateImpl() = 0;


		static Input* s_Instance;
	};

}