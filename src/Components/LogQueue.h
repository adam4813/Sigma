#pragma once

#include <string>
#include <queue>

#include "../IComponent.h"

class LogQueue : public IComponent {
public:
	LogQueue(const int entityID = 0);

	void SetTag(const std::string tag); // Sets the tag displayed when a log message is made.
	const std::string GetTag(); // Gets the tag for the given component.

	void AddMessage(const std::string msg); // Adds a message to the message queue to be written on the next update.
	std::string GetMessage(); // Returns the next message in the queue or nullptr if there isn't one.

	int GetSize(); // Returns the size of the message queue.
private:
	std::string tag; // The tag display before a log message.
	std::queue<std::string> msgQueue; // Queue to hold messages waiting to be written.
};
