#pragma once
#ifndef MOUSEINPUTSYSTEM_H
#define MOUSEINPUTSYSTEM_H

#include <map>
#include <vector>

namespace Sigma{
    namespace event{

        enum BUTTON_STATE {MOUSE_DOWN, MOUSE_UP};
        enum BUTTON {LEFT=0, MIDDLE=1, RIGHT=2};

        struct IMouseEventHandler{
            float mouse_x, mouse_y; // current pixel locations
            BUTTON_STATE buttons[3]; // left, middle, right
            virtual void MouseMove(float x, float y, float dx, float dy) = 0; // given displacement since last call
			virtual void MouseDown(BUTTON btn, float x, float y) = 0; // called on button press.
            virtual void MouseUp(BUTTON btn, float x, float y) = 0;
        };

        class MouseInputSystem
        {
            public:
                MouseInputSystem() {}
                ~MouseInputSystem() {}

                /**
                 * \brief Registers a handler to receive mouse updates
                 *
                 * \param handler, extending IMouseHandler, to receive mouse events
                 */
                void Register(IMouseEventHandler *handler){
                    this->eventHandlers.push_back(handler);
                }

				/**
				 * \brief MouseMove event.
				 *
				 * Loops through each event handler that is registered and passes mouse movement information.
				 */
				void MouseMove(float x, float y, float dx, float dy) {
					for (auto itr = this->eventHandlers.begin(); itr != this->eventHandlers.end(); ++itr) {
						(*itr)->MouseMove(x, y, dx, dy);
					}
				}

				void MouseDown(BUTTON btn, float x, float y) {
					for (auto itr = this->eventHandlers.begin(); itr != this->eventHandlers.end(); ++itr) {
						(*itr)->MouseDown(btn, x, y);
					}
				}

				void MouseUp(BUTTON btn, float x, float y) {
					for (auto itr = this->eventHandlers.begin(); itr != this->eventHandlers.end(); ++itr) {
						(*itr)->MouseUp(btn, x, y);
					}
				}
            private:
                std::vector<Sigma::event::IMouseEventHandler*> eventHandlers;
        };
    }
}

#endif // MOUSEINPUTSYSTEM_H
