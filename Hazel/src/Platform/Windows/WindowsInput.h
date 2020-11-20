#pragma once

#include "Hazel/Input.h"

namespace Hazel {

	class WindowsInput : public Input
	{
	

	protected:
		///////////////////////
		///KEYBOARD FUNCTIONS//
		///////////////////////

		virtual bool IsKeyPressedImpl(unsigned char keycode) override;
		virtual InputKeyboardEvent ReadKeyImpl() override;
		virtual bool IsKeyEmptyImpl() override;
		virtual void FlushKeyImpl() override;
		virtual char ReadCharImpl() override;
		virtual bool IsCharEmptyImpl() override;
		virtual void FlushCharImpl() override;
		virtual void FlushKeyboardImpl() override;
		virtual void EnableAutorepeatImpl() override;
		virtual void DisableAutorepeatImpl() override;
		virtual bool AutorepeatIsEnabledImpl() override;

		///////////////////////
		///MOUSE FUNCTIONS/////
		///////////////////////

		virtual std::pair<int, int> GetMousePosImpl() override;
		virtual int GetMousePosXImpl() override;
		virtual int GetMousePosYImpl() override;
		virtual bool IsInWindowImpl() override;
		virtual bool IsLeftPressedImpl() override;
		virtual bool IsRightPressedImpl() override;
		virtual InputMouseEvent ReadMouseImpl() override;
		virtual bool IsMouseEmptyImpl() override
		{
			//return mouseBuffer.empty();
			return false;
		}
		virtual void FlushMouseImpl() override;



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
		void TrimKeyboardBufferImpl(std::queue<T>& buffer) noexcept;

		///////////////////////
		///MOUSE FUNCTIONS/////
		///////////////////////

		virtual void OnMouseMoveImpl(int x, int y) override;
		virtual void OnMouseEnterImpl() override;
		virtual void OnMouseLeaveImpl() override;
		virtual void OnLeftPressedImpl(int x, int y) override;
		virtual void OnLeftReleasedImpl(int x, int y) override;
		virtual void OnRightPressedImpl(int x, int y) override;
		virtual void OnRightReleasedImpl(int x, int y) override;
		virtual void OnWheelUpImpl(int x, int y) override;
		virtual void OnWheelDownImpl(int x, int y) override;
		virtual void OnWheelDeltaImpl(int x, int y, int delta) override;
		virtual void TrimMouseBufferImpl() override;


	private:
		static constexpr unsigned int nKeys = 256u;
		static constexpr unsigned int bufferSize = 16u;
		// TODO: decide whether to keep this as false or not
		bool autorepeatEnabled = false;
		std::bitset<nKeys> keystates;
		std::queue<InputKeyboardEvent> keybuffer;
		std::queue<char> charbuffer;

		///////////////////////
		///MOUSE VARIABLES/////
		///////////////////////
		int x;
		int y;
		bool isLeftPressed = false;
		bool isRightPressed = false;
		bool isInWindow = false;
		int wheelDeltaCarry = 0;


		std::queue<InputMouseEvent> mouseBuffer;
	};

}