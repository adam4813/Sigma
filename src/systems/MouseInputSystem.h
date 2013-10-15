#pragma once
#ifndef MOUSEINPUTSYSTEM_H
#define MOUSEINPUTSYSTEM_H

namespace Sigma{
    namespace event{

        enum BUTTON_STATE {MOUSE_DOWN, MOUSE_UP};

        struct IMouseHandler{
            float dx, dy;
            BUTTON_STATE buttons[3]; // left, middle, right
            virtual void MouseMove() = 0;
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
                    this->registeredHandlers.push_back(handler);
                }
            private:
                std::vector<IMouseHandler*> registeredHandlers;
        };

    }
}

#endif // MOUSEINPUTSYSTEM_H
