#pragma  once

#include "systems/KeyboardInputSystem.h"

class IOpSys {
public:
	IOpSys() { }
	virtual ~IOpSys() { }

	/**
	 * \brief Creates an OpenGLContext capable window.
	 *
	 * Creates a window with a modern OpenGL context.
	 * \param[in] const unsigned int width The client window width.
	 * \param[in] const unsigned int height The client window height.
	 * \returns   void* A castable context. The type various by the IOpSys
	 */
	virtual void* CreateGraphicsWindow(const unsigned int width = 800, const unsigned int height = 600) = 0;

	/**
	 * \brief Get the client area width of the window.
	 *
	 * \returns   unsigned int The client area width.
	 */
	virtual unsigned int GetWindowWidth() = 0;
	
	/**
	 * \brief Get the client area height of the window.
	 *
	 * \returns   unsigned int The client area height.
	 */
	virtual unsigned int GetWindowHeight() = 0;

	/**
	 * \brief Toggles between fullscreen and windowed mode.
	 *
	 */
	virtual void ToggleFullscreen() = 0;

	/**
	 * \brief Flips the context buffers
	 *
	 */
	virtual void Present() = 0;

	/**
	 * \brief Does the OS specific message looping.
	 *
	 * Handles various OS level messages such as input events and quit messages.
	 * \returns   bool Returns false if the quit message as received.
	 */
	virtual bool MessageLoop() = 0;

	/**
	 * \brief Polls to see if the specified key is down.
	 *
	 * \param[in/out] int key The key to poll.
	 * \param[in/out] bool focused If the check should be made only when the window has focus.
	 * \returns   bool True if the key is down, false otherwise.
	 */
	virtual bool KeyDown(int key, bool focused = false) = 0;

	/**
	 * \brief Polls to see if the specified key was released since the last call to MessageLoop().
	 *
	 * \param[in/out] int key The key to poll.
	 * \param[in/out] bool focused If the check should be made only when the window has focus.
	 * \returns   bool True if the key was released, false otherwise.
	 */
	virtual bool KeyReleased( int key, bool focused = false ) = 0;

	/**
	 * \brief Sets up the OS specific timer.
	 * 
	 * \returns   bool If the setup was successful or not. If not then alternative timing should be used.
	 */
	virtual bool SetupTimer() = 0;

	/**
	 * \brief Returns the change in time since the last call to GetDeltaTime.
	 *
	 * Uses the OS specific timing mechanisms to return how many milliseconds have passed since the last call.
	 * \returns   double The amount of milliseconds (can be sub-millisecond) that have passed.
	 */
	virtual double GetDeltaTime() = 0;

	/**
	 * \brief Returns the current delta time.
	 *
	 * Returns the current delta time set in the last call to GetDeltaTime.
	 * \returns   double The current delta time.
	 */
	static double GetCurrentDelta() { return curDelta; }

	static Sigma::event::KeyboardInputSystem KeyboardEventSystem; // Handles keyboard events
protected:
	static double curDelta; // Current delta time
};
