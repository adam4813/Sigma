#pragma once

#include <unordered_map>
#include <string>
#include <vector>

class IComponent;

struct StringHasher {
	const unsigned long operator() (const char* str) const;
	const unsigned long operator() (const std::string str) const;
};

class KeyNotFoundException : public std::exception {
	virtual const char* what() const throw();
};

class Entity {
public:
	Entity(const int guid = -1, const std::string name = "");
	~Entity();

	// Get a property by name.
	std::string GetStringProperty(const std::string key);
	int GetIntProperty(const std::string key);
	double GetFloatProperty(const std::string key);
	bool GetBoolProperty(const std::string key);
	
	// Add a property by name.
	// Returns the value stored.
	std::string AddProperty(const std::string key, const std::string value);
	std::string AddProperty(const std::string key, const char* value);
	int AddProperty(const std::string key, const int value);
	double AddProperty(const std::string key, const double value);
	bool AddProperty(const std::string key, const bool value);

	// Remove a property by name
	// Throws a KeyNotFoundException on failure.
	void RemoveProperty( const std::string key );

	// Checks if the key exists.
	// Throws a KeyNotFoundException on failure, nothing on success.
	// Override the KeyNotFoundException by setting noKNFexcept to true.
	void KeyExists( const std::string key );

	// Sets whether to fail silently on KeyNotFoundException.
	// true to fail silently, false to throw KeyNotFoundException
	void NoKNFExcept(bool b = false);
private:
	std::unordered_map<const std::string, std::string, StringHasher> properties; // The properties of this Entity (hashed key => value).
	std::vector<const IComponent*> components; // Array of the components associated with this Entity.
	const int guid;
	bool noKNFexcept; // Set to true to disable the KeyNotFoundException, and instead fail silently and return a default value.
};
