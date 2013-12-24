#pragma once

#include <map>
#include <vector>

namespace Sigma {
	namespace event {
		enum KEY_STATE {KS_UP, KS_DOWN};
    static const unsigned int MAX_KEY = 348;

		static int KEY_ESCAPE;

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
		};

		class KeyboardInputSystem {
		public:
			KeyboardInputSystem() { }
			~KeyboardInputSystem() { }

			/**
			 * \brief Register a KeyboardEventHandler.
			 *
			 * \param[in] Sigma::event::IKeyboardEventHandler * e The keyboard even handler to register.
			 */
			void Register( Sigma::event::IKeyboardEventHandler *e ) {
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
			 */
			void KeyUp(const unsigned int key) {
				if (this->eventHandlers.find(key) != this->eventHandlers.end()) {
					for (auto itr = this->eventHandlers[key].begin(); itr != this->eventHandlers[key].end(); ++ itr) {
						(*itr)->KeyStateChange(key, KS_UP);
					}
				}
			}

			void CharDown(const unsigned int c) {
				if (this->charHandlers.find(c) != this->charHandlers.end()) {
					for (auto itr = this->charHandlers[c].begin(); itr != this->charHandlers[c].end(); ++ itr) {
						(*itr)->CharDown(c);
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
						(*itr)->KeyStateChange(key, KS_DOWN);
					}
				}
			}
		private:
			std::map<unsigned int, std::vector<Sigma::event::IKeyboardEventHandler*> > eventHandlers;
			std::map<unsigned int, std::vector<Sigma::event::IKeyboardEventHandler*> > charHandlers;
		};
	}
}
