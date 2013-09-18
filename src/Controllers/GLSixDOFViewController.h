#pragma once

#include "../Systems/KeyboardInputSystem.h"
#include "../Systems/OpenGLSystem.h"
#include "../IOpSys.h"

namespace Sigma {
	namespace event {
		namespace handler {
			// A type of handler. This handler controls an OpenGL 6 DOF view.
			class GLSixDOFViewController : public IKeyboardEventHandler {
			private:
				GLSixDOFViewController() { }
			public:
				GLSixDOFViewController(OpenGLSystem *gl) : glsys(gl) {
					memset(this->keys, 0, sizeof(this->keys));
					memset(this->keyState, 0, sizeof(this->keyState));
					this->keys['W'] = 1; this->keys['B'] = 1; this->keys['S'] = 1;
					this->keys['A'] = 1; this->keys['D'] = 1; this->keys['F'] = 1;
					this->keys['W'] = 1; this->keys['B'] = 1; this->keys['S'] = 1;
					this->keys['G'] = 1; this->keys['E'] = 1; this->keys['C'] = 1;
					this->keys['Q'] = 1; this->keys['Z'] = 1; this->keys['R'] = 1;
					this->keys['T'] = 1;
				}
				void KeyStateChange(const unsigned int key, const KEY_STATE state) {
					this->keyState[key] = state;
					float deltaSec = (float)IOpSys::GetCurrentDelta()/1000.0f;
					// Translation keys
					if (key == 'W') { // Move forward
						if (this->keyState['B'] == KS_DOWN) {
							this->glsys->Move(0.0f, 0.0f, 100.0f*deltaSec);
						} else {
							this->glsys->Move(0.0f, 0.0f, 10.0f*deltaSec);
						}
					} else if (key == 'S') { // Move backward
						this->glsys->Move(0.0f, 0.0f, -10.0f*deltaSec);
					}

					if (key == 'A') { 
						this->glsys->Rotate(0.0f, -90.0f*deltaSec, 0.0f); // Yaw left.
					} else if (key == 'D') {
						this->glsys->Rotate(0.0f, 90.0f*deltaSec, 0.0f); // Yaw right.
					}

					if (key == 'F') { 
						this->glsys->Move(-10.0f*deltaSec, 0.0f, 0.0f); // Strafe Left
					} else if (key == 'G') {
						this->glsys->Move(10.0f*deltaSec, 0.0f, 0.0f); // Strafe Right
					}

					if (key == 'E') { // Move up
						this->glsys->Move(0.0f, 10.0f*deltaSec, 0.0f);
					} else if (key == 'C') { // Move down
						this->glsys->Move(0.0f, -10.0f*deltaSec, 0.0f);
					}

					if (key == 'Q') { // Pitch Up
						this->glsys->Rotate(-90.0f * deltaSec, 0.0f, 0.0f);
					} else if (key == 'Z') { // Pitch Down
						this->glsys->Rotate(90.0f*deltaSec, 0.0f, 0.0f);
					}

					if (key == 'R') { // Roll left
						this->glsys->Rotate(0.0f, 0.0f, -90.0f * deltaSec);
					} else if (key == 'T') { // Roll right
						this->glsys->Rotate(0.0f, 0.0f, 90.0f*deltaSec);
					}
				}
			private:
				OpenGLSystem* glsys;
			};
		}
	}
}
