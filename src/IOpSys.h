#pragma  once

#include "Systems/KeyboardInputSystem.h"

class IOpSys {
public:
	IOpSys() { }
	virtual ~IOpSys() { }

	virtual void* CreateGraphicsWindow(const unsigned int width = 800, const unsigned int height = 600) = 0;
	virtual void ToggleFullscreen() = 0;

	virtual bool MessageLoop() = 0;

	virtual bool SetupTimer() = 0;

	virtual double GetDeltaTime() = 0;

	virtual bool KeyDown(int key, bool focused = false) = 0;

	virtual bool KeyUp( int key, bool focused = false ) = 0;

	virtual void Present() = 0;

	virtual unsigned int GetWindowWidth() = 0;

	virtual unsigned int GetWindowHeight() = 0;
	/**
	 * \brief Returns the current delta time.
	 *
	 * Returns the current delta time set in the last call to GetDeltaTime.
	 * \returns   double The current delta time.
	 */
	static double GetCurrentDelta() { return curDelta; }

	static Sigma::event::KeyboardInputSystem KeybaordEventSystem; // Handles keyboard events
protected:
	static double curDelta;
};