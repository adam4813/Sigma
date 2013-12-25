#pragma once

#include <map>
#include <vector>

namespace Sigma {
	namespace event {
		enum KEY_STATE {KS_UP, KS_DOWN};

		static const unsigned int MAX_KEY = 348;

		static int KEY_ESCAPE;

		class KeyboardInputSystem;

		// A keyboard event handler interface. Handlers can be controllers, loggers, etc.
		struct IKeyboardEventHandler {
			unsigned int keys[MAX_KEY]; // The keys this handler triggers off.
			unsigned int chars[MAX_KEY]; // The keys this handler triggers off.
			KEY_STATE keyState[MAX_KEY]; // State of the keys.
			/**
			 * \brief Called when on the keys reported during register has a state change.
			 *
			 * \param[in] unsigned int key The key that has had a state change
			 * \param[in] KEY_STATE state The new state the key is in
			 */
			virtual void KeyStateChange(const unsigned int key, const KEY_STATE state) = 0;
			virtual void CharDown(const unsigned int c) { }
			virtual void LostKeyboardFocus() { }
			KeyboardInputSystem* system;
		};

		class KeyboardInputSystem {
		public:
			KeyboardInputSystem() : focusLock(nullptr) { }
			~KeyboardInputSystem() { }

			/**
			 * \brief Called to request focus lock.
			 *
			 * By requesting focus lock all future keyboard events will go to that handler until it calls ReleaseFocusLock.
			 * \param[in/out] IKeyboardEventHandler * e
			 * \return bool True is focus was locked or if this handler already has focus lock otherwise false.
			 */
			bool RequestFocusLock(IKeyboardEventHandler* e) {
				if ((this->focusLock == nullptr) || (this->focusLock == e)) {
					this->focusLock = e;
					LostKeyboardFocus(); // Since we are now locking focus we should inform other components about the lose of focus.
					return true;
				}
				return false;
			}

			/**
			 * \brief Called when keyboard focus has been lost.
			 *
			 * When a focus lock is requested or the window simply loses focus lock, this will notify each even handler that focus has been lost.
			 * \return void
			 */
			void LostKeyboardFocus() {
				for (auto itr = this->eventHandlers.begin(); itr != this->eventHandlers.end(); ++itr) {
					for (auto citr = itr->second.begin(); citr != itr->second.end(); ++citr) {
						(*citr)->LostKeyboardFocus();
					}
				}
			}

			/**
			 * \brief Releases focus lock if the handler currently has the lock.
			 *
			 * \param[in/out] IKeyboardEventHandler * e
			 * \return void No return indication if the lock was successfully released.
			 */
			void ReleaseFocusLock(IKeyboardEventHandler* e) {
				if (this->focusLock == e) {
					this->focusLock = nullptr;
				}
			}

			/**
			 * \brief Register a KeyboardEventHandler.
			 *
			 * \param[in] IKeyboardEventHandler* e The keyboard even handler to register.
			 * \return void
			 */
			void Register(IKeyboardEventHandler* e) {
				e->system = this;
				for (unsigned int i = 0; i < MAX_KEY; i++) {
					if (e->keys[i] > 0) {
						this->eventHandlers[i].push_back(e);
					}
				}
				for (unsigned int i = 0; i < MAX_KEY; i++) {
					if (e->chars[i] > 0) {
						this->charHandlers[i].push_back(e);
					}
				}
			}

			/**
			 * \brief Key Up event.
			 *
			 * Loops through each event handler that is registered to the supplied key and calls its KeyStateChange method.
			 * \param[in] const unsigned int key The key the is now up.
			 * \return void
			 */
			void KeyUp(const unsigned int key) {
				if (this->eventHandlers.find(key) != this->eventHandlers.end()) {
					for (auto itr = this->eventHandlers[key].begin(); itr != this->eventHandlers[key].end(); ++ itr) {
						if (focusLock != nullptr) {
							if ((*itr) == focusLock) {
								(*itr)->KeyStateChange(key, KS_UP);
							}
						}
						else {
							(*itr)->KeyStateChange(key, KS_UP);
						}
					}
				}
			}

			/**
			 * \brief Called when a character event occurs.
			 *
			 * These are human readable characters already modified such as shift-a is A or numpad_9 is 9.
			 * \param[in] const unsigned int c The character code.
			 * \return void
			 */
			void CharDown(const unsigned int c) {
				if (this->charHandlers.find(c) != this->charHandlers.end()) {
					for (auto itr = this->charHandlers[c].begin(); itr != this->charHandlers[c].end(); ++ itr) {
						if (focusLock != nullptr) {
							if ((*itr) == focusLock) {
								(*itr)->CharDown(c);
							}
						}
						else {
							(*itr)->CharDown(c);
						}
					}
				}
			}

			/**
			 * \brief Key Down event.
			 *
			 * Loops through each event handler that is registered to the supplied key and calls its KeyStateChange method.
			 * \param[in] const unsigned int key The key the is now down.
			 */
			void KeyDown(const unsigned int key) {
				if (this->eventHandlers.find(key) != this->eventHandlers.end()) {
					for (auto itr = this->eventHandlers[key].begin(); itr != this->eventHandlers[key].end(); ++ itr) {
						if (focusLock != nullptr) {
							if ((*itr) == focusLock) {
								(*itr)->KeyStateChange(key, KS_DOWN);
							}
						}
						else {
							(*itr)->KeyStateChange(key, KS_DOWN);
						}
					}
				}
			}
		private:
			std::map<unsigned int, std::vector<IKeyboardEventHandler*> > eventHandlers;
			std::map<unsigned int, std::vector<IKeyboardEventHandler*> > charHandlers;
			IKeyboardEventHandler* focusLock;
		};
	}
}
