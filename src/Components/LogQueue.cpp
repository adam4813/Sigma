#include "LogQueue.h"

LogQueue::LogQueue( const int entityID /*= 0*/ ) : IComponent(entityID) {

}

void LogQueue::SetTag(const std::string tag) {
	this->tag = tag;
}

const std::string LogQueue::GetTag() {
	return this->tag;
}

void LogQueue::AddMessage(const std::string msg) {
	this->msgQueue.push(msg);
}

std::string LogQueue::GetMessage() {
	if (this->msgQueue.size() > 0) {
		std::string ret = this->msgQueue.front();
		this->msgQueue.pop();
		return ret;
	}

	return "";
}

int LogQueue::GetSize() {
	return this->msgQueue.size();
}