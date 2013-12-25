#pragma once
#ifndef MOUSEINPUTSYSTEM_H
#define MOUSEINPUTSYSTEM_H

#include <map>
#include <vector>

namespace Sigma{
	namespace event{
		enum BUTTON_STATE {MOUSE_DOWN, MOUSE_UP};
		enum BUTTON {LEFT = 0, MIDDLE = 1, RIGHT = 2};

		class MouseInputSystem;

		struct IMouseEventHandler {
			float mouse_x, mouse_y; // current pixel locations
			BUTTON_STATE buttons[3]; // left, middle, right
			virtual void MouseMove(float x, float y, float dx, float dy) = 0; // given displacement since last call
			virtual void MouseDown(BUTTON btn, float x, float y) = 0; // called on button press.
			virtual void MouseUp(BUTTON btn, float x, float y) = 0;
			virtual void LostMouseFocus() { }
			MouseInputSystem* mouseSystem;
		};

		class MouseInputSystem {
		public:
			MouseInputSystem() : focusLock(nullptr) {}
			~MouseInputSystem() {}

			/**
				* \brief Called to request focus lock.
				*
				* By requesting focus lock all future keyboard events will go to that handler until it calls ReleaseFocusLock.
				* \param[in/out] IMouseEventHandler* e
				* \return bool True is focus was locked or if this handler already has focus lock otherwise false.
				*/
			bool RequestFocusLock(IMouseEventHandler* e) {
				if ((this->focusLock == nullptr) || (this->focusLock == e)) {
					this->focusLock = e;
					LostMouseFocus(); // Since we are now locking focus we should inform other components about the lose of focus.
					return true;
				}
				return false;
			}

			/**
				* \brief Called when mouse focus has been lost.
				*
				* When a focus lock is requested or the window simply loses focus lock, this will notify each even handler that focus has been lost.
				* \return void
				*/
			void LostMouseFocus() {
				for (auto itr = this->eventHandlers.begin(); itr != this->eventHandlers.end(); ++itr) {
					(*itr)->LostMouseFocus();
				}
			}

			/**
				* \brief Releases focus lock if the handler currently has the lock.
				*
				* \param[in/out] IMouseEventHandler* e
				* \return void No return indication if the lock was successfully released.
				*/
			void ReleaseFocusLock(IMouseEventHandler* e) {
				if (this->focusLock == e) {
					this->focusLock = nullptr;
				}
			}

			/**
				* \brief Registers a handler to receive mouse updates
				*
				* \param handler, extending IMouseHandler, to receive mouse events
				* \return void
				*/
			void Register(IMouseEventHandler *handler) {
				this->eventHandlers.push_back(handler);
				handler->mouseSystem = this;
			}

			/**
				* \brief MouseMove event.
				*
				* Loops through each event handler that is registered and passes mouse movement information.
				* \param[in] float x, y The new position of the mouse in normalized screen coordinates.
				* \param[in] float dx, dy The change in mouse position in normalized screen coordinates.
				* \return void
				*/
			void MouseMove(float x, float y, float dx, float dy) {
				for (auto itr = this->eventHandlers.begin(); itr != this->eventHandlers.end(); ++itr) {
					if (focusLock != nullptr) {
						if ((*itr) == focusLock) {
							(*itr)->MouseMove(x, y, dx, dy);
						}
					}
					else {
						(*itr)->MouseMove(x, y, dx, dy);
					}
				}
			}

			/**
				* \brief Mouse down event.
				*
				* \param[in/out] BUTTON btn The button that was pressed. Any from BUTTON.
				* \param[in/out] float x, y Normalized screen coordinates of the click event.
				* \return void
				*/
			void MouseDown(BUTTON btn, float x, float y) {
				for (auto itr = this->eventHandlers.begin(); itr != this->eventHandlers.end(); ++itr) {
					if (focusLock != nullptr) {
						if ((*itr) == focusLock) {
							(*itr)->MouseDown(btn, x, y);
						}
					}
					else {
						(*itr)->MouseDown(btn, x, y);
					}
				}
			}

			/**
				* \brief Mouse up event.
				*
				* \param[in/out] BUTTON btn The button that was released. Any from BUTTON.
				* \param[in/out] float x, y Normalized screen coordinates of the release event.
				* \return void
				*/
			void MouseUp(BUTTON btn, float x, float y) {
				for (auto itr = this->eventHandlers.begin(); itr != this->eventHandlers.end(); ++itr) {
					if (focusLock != nullptr) {
						if ((*itr) == focusLock) {
							(*itr)->MouseUp(btn, x, y);
						}
					}
					else {
						(*itr)->MouseUp(btn, x, y);
					}
				}
			}
		private:
			std::vector<IMouseEventHandler*> eventHandlers;
			IMouseEventHandler* focusLock;
		};
	}
}

#endif // MOUSEINPUTSYSTEM_H
