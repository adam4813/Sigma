#pragma once
#ifndef MOUSEINPUTSYSTEM_H
#define MOUSEINPUTSYSTEM_H

#include <map>
#include <vector>

namespace Sigma{
    namespace event{

        enum BUTTON_STATE {MOUSE_DOWN, MOUSE_UP};

        struct IMouseHandler{
            float dx, dy;
            BUTTON_STATE buttons[3]; // left, middle, right
            virtual void MouseMove(float dx, float dy) = 0;
            virtual void MouseDown() = 0;
            virtual void MouseUp() = 0;
        };

        class MouseInputSystem
        {
            public:
                MouseInputSystem();

                /**
                 * \brief Registers a handler to receive mouse updates
                 *
                 * \param handler, extending IMouseHandler, to receive mouse events
                 */
                void Register(IMouseHandler *handler){
                    this->eventHandlers.push_back(handler);
                }

				/**
				 * \brief MouseMove event.
				 *
				 * Loops through each event handler that is registered and passes mouse movement information.
				 */
				void MouseMove(float dx, float dy) {
					for (auto itr = this->eventHandlers.begin(); itr != this->eventHandlers.end(); ++ itr) {
						(*itr)->MouseMove(dx, dy);
					}
				}
            private:
                std::vector<Sigma::event::IMouseHandler*> eventHandlers;
        };
    }
}

#endif // MOUSEINPUTSYSTEM_H
