#pragma once

#include "Hazel/Input.h"

namespace Hazel {

	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(unsigned char keycode) override;
		virtual Win32KeyboardEvent ReadKeyImpl() override;
		virtual bool IsKeyEmptyImpl() override;
		virtual void FlushKeyImpl() override;
		virtual char ReadCharImpl() override;
		virtual bool IsCharEmptyImpl() override;
		virtual void FlushCharImpl() override;
		virtual void FlushImpl() override;
		virtual void EnableAutorepeatImpl() override;
		virtual void DisableAutorepeatImpl() override;
		virtual bool AutorepeatIsEnabledImpl() override;

		/*
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
		*/

	private:
		void OnKeyPressedImpl(unsigned char keycode) noexcept;
		void OnKeyReleasedImpl(unsigned char keycode) noexcept;
		void OnCharImpl(char character) noexcept;
		void ClearStateImpl() noexcept;
		template<typename T>
		static void TrimBufferImpl(std::queue<T>& buffer) noexcept;

	private:
		static constexpr unsigned int nKeys = 256u;
		static constexpr unsigned int bufferSize = 16u;
		// TODO: decide whether to keep this as false or not
		bool autorepeatEnabled = false;
		std::bitset<nKeys> keystates;
		std::queue<Win32KeyboardEvent> keybuffer;
		std::queue<char> charbuffer;
	};

}