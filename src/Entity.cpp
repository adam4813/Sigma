#include "Entity.h"
#include <sstream>

Entity::Entity( const int guid, const std::string name /*= ""*/ ) : guid(guid), noKNFexcept(false) {
	this->properties["name"] = name;
}

Entity::~Entity() {

}

std::string Entity::GetStringProperty( const std::string key ) {
	try {
		KeyExists(key);
	} catch(KeyNotFoundException e) {
		if (this->noKNFexcept) {
			return "";
		}
		throw;
	}
	return this->properties[key];
}

int Entity::GetIntProperty( const std::string key ) {
	try {
		KeyExists(key);
	} catch(KeyNotFoundException) {
		if (this->noKNFexcept) {
			return 0;
		}
		throw;
	}
	return atoi(this->properties[key].c_str());
}

double Entity::GetFloatProperty( const std::string key ) {
	try {
		KeyExists(key);
	} catch(KeyNotFoundException) {
		if (this->noKNFexcept) {
			return 0.0f;
		}
		throw;
	}
	return atof(this->properties[key].c_str());
}

bool Entity::GetBoolProperty( const std::string key ) {
	try {
		KeyExists(key);
	} catch(KeyNotFoundException) {
		if (this->noKNFexcept) {
			return false;
		}
		throw;
	}
	if (this->properties[key] == "true") {
		return true;
	}
	return false;
}

std::string Entity::AddProperty( const std::string key, const std::string value ) {
	this->properties[key] = value;
	return value;
}

std::string Entity::AddProperty( const std::string key, const char* value ) {
	this->properties[key] = value;
	return value;
}

int Entity::AddProperty( const std::string key, const int value ) {
	std::stringstream ss;
	ss << value;
	this->properties[key] = ss.str();
	return value;
}

double Entity::AddProperty( const std::string key, const double value ) {
	std::stringstream ss;
	ss << value;
	this->properties[key] = ss.str();
	return value;
}

bool Entity::AddProperty( const std::string key, const bool value ) {
	std::stringstream ss;
	ss << std::boolalpha << value;
	this->properties[key] = ss.str();
	return value;
}

void Entity::RemoveProperty( const std::string key ) {
	try {
		KeyExists(key);
	} catch(KeyNotFoundException) {
		if (this->noKNFexcept) {
			return;
		}
		throw;
	}
	this->properties.erase(key);
}

void Entity::KeyExists( const std::string key ) {
	if (this->properties.find(key) == this->properties.end()) {
		throw KeyNotFoundException();
	}
}

void Entity::NoKNFExcept( bool b /*= false*/ ) {
	this->noKNFexcept = b;
}

// djb2 hashing algorithm with modification for upper-alpha only characters
// http://www.cse.yorku.ca/~oz/hash.html
const unsigned long StringHasher::operator()( const char* str ) const {
	unsigned long hash = 5381;
	int c;

	while (c = *str++) {
		c = toupper(c); // Convert to upper-alpha if lower-alpha
		if (isupper(c)) { // Check if it is a valid upper-alpha and hash it
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
		}
	}

	return hash;
}

const unsigned long StringHasher::operator() (const std::string str) const {
	return StringHasher::operator()(str.c_str());
}

const char* KeyNotFoundException::what() const throw() {
	return "Key not found in properties list.";
}
