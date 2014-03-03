#pragma once

#include <string>
#include <memory>
#include <map>
#include <mutex>
#include <vector>
#include "Property.h"

namespace Sigma {
	namespace resource {
		// Each reflected type must specialize this.
		template <typename TYPE> const char* GetTypeName(void) { /*static_assert(0, "GetTypeName is undefined for a type.");*/ }
		template <typename TYPE> const unsigned int GetTypeID(void) { /*static_assert(0, "GetTypeID is undefined for a type.");*/ }

		class ResourceBase {
		public:
			ResourceBase() { }
			~ResourceBase() { }

			/**
			 * \brief Returns a resource with the specified name.
			 *
			 * \param[in] const std::vector<Property> &properties The creation properties for the resource.
			 * \return bool True if initialization finished with no errors.
			 */
			virtual bool Initialize(const std::vector<Property> &properties) = 0;
		};

		// Singleton approach derived from http://silviuardelean.ro/2012/06/05/few-singleton-approaches/ .
		class ResourceSystem {
		private:
			ResourceSystem() { }
			ResourceSystem(const ResourceSystem& right) {
				instance  = right.instance;
			}
			ResourceSystem& operator=(const ResourceSystem& right) {
				if (this != &right) {
					instance  = right.instance;
				}

				return *this;
			}
			static std::once_flag only_one;
			static std::shared_ptr<ResourceSystem> instance;
		public:
			static std::shared_ptr<ResourceSystem> GetInstance() {
				std::call_once(ResourceSystem::only_one, [] () { 
						ResourceSystem::instance.reset(new ResourceSystem()); 

						// Set up the default factory for an unknown type to return false.
						auto lambda = [] (const std::string& name, const std::vector<Property> &properties) {
							return nullptr;
						};

						ResourceSystem::instance->factories[0] = lambda;
					}
				);

				return ResourceSystem::instance;
			}
			~ResourceSystem() { }

			/**
			 * \brief Register a type to be available for factory calls.
			 *
			 * \return void
			 */
			template<class T>
			void Register() {
				// Store the type ID associated with the type name.
				this->resTypeID[GetTypeName<T>()] = GetTypeID<T>();

				// Create a lambda function that calls Create with the correct template type.
				auto lambda = [this] (const std::string& name, const std::vector<Property> &properties) {
					return this->Create<T>(name, properties);
				};

				this->factories[GetTypeID<T>()] = lambda;
			}

			/**
			 * \brief Returns a type ID associated with the given name.
			 *
			 * \param[in] const std::string & type_Name The name to look for a type ID.
			 * \return unsigned int Returns 0 if the name doesn't exist.
			 */
			unsigned int GetTypeIDFromName(const std::string& type_Name) {
				if (this->resTypeID.find(type_Name) == this->resTypeID.end()) {
					return 0;
				}
				return this->resTypeID.find(type_Name)->second;
			}

			/**
			 * \brief Loads a resource when the type isn't known.
			 *
			 * \param[in] const unsigned int type_ID The ID of the type to load. This is used to select the correct factory.
			 * \param[in] const std::string & name What the loaded resource will be named.
			 * \param[in] const std::vector<Property> & properties Creation properties.
			 * \return bool True if the resource loaded successfully.
			 */
			bool Load(const unsigned int type_ID, const std::string& name, const std::vector<Property> &properties) {
				if (this->factories.find(type_ID) != this->factories.end()) {
					return this->factories[type_ID](name, properties) != nullptr;
				}
				return false;
			}

			/**
			 * \brief Gets a resource by the given name.
			 *
			 * \param[in] const std::string& name Name of the resource to retrieve.
			 * \return std::shared_ptr<T> Returns nullptr if the resource hasn't been created yet, otherwise the requested resource..
			 */
			template<class T>
			std::shared_ptr<T> Get(const std::string& name) {
				unsigned int typeID = GetTypeID<T>();
				if (this->resources[typeID].find(name) == this->resources[typeID].end()) {
					return nullptr;
				}
				return std::static_pointer_cast<T>(this->resources[typeID][name]);
			}

			/**
			 * \brief Creates a resource with the given name and initializes it.
			 *
			 * \param[in] const std::string& name The name of the resource to create.
			 * \param[in] const std::vector<Property> &properties The creation properties for the resource.
			 * \return std::shared_ptr<T> Returns nullptr if it failed to be created, otherwise the created resource.
			 */
			template<class T>
			std::shared_ptr<T> Create(const std::string& name, const std::vector<Property> &properties) {
				unsigned int typeID = GetTypeID<T>();
				if (this->resources[typeID].find(name) == this->resources[typeID].end()) {
					this->resources[typeID][name] = std::make_shared<T>();
					if (!this->resources[typeID][name]->Initialize(properties)) {
						this->resources[typeID].erase(name);
						return nullptr;
					}
				}
				return std::static_pointer_cast<T>(this->resources[typeID][name]);
			}

			/**
			 * \brief Adds a resource to be managed by the system.
			 *
			 * \param[in] const std::string& name The name of the resource.
			 * \param[in] std::shared_ptr<T> r The resource to add.
			 * \return void
			 */
			template<class T>
			void Add(const std::string& name, std::shared_ptr<T> r) {
				unsigned int typeID = GetTypeID<T>();
				resources[typeID][name] = r;
			}

			/**
			 * \brief Removes a resources managed by the system.
			 * This doesn't invalidate any held pointers to the resource.
			 *
			 * \param[in] const std::string& name Name of the resource to remove.
			 * \return void
			 */
			template<class T>
			void Remove(const std::string& name) {
				unsigned int typeID = GetTypeID<T>();
				if (this->resources[typeID].find(name) != this->resources[typeID].end()) {
					this->resources[typeID].erase(name);
				}
			}

			/**
			 * \brief Checks if a resource exists with the given name.
			 *
			 * \param[in] const std::string& name Name of the resource to check if it exists.
			 * \return bool True if the resource exists.
			 */
			template<class T>
			bool Exists(const std::string& name) {
				unsigned int typeID = GetTypeID<T>();
				if (this->resources[typeID].find(name) != this->resources[typeID].end()) {
					return true;
				}
				return false;
			}
		private:
			std::map<unsigned int, std::map<std::string, std::shared_ptr<ResourceBase>>> resources; // Mapping of resource TypeID to loaded resources
			std::map<std::string, unsigned int> resTypeID; // Stores a mapping of TypeName to TypeID

			std::map<unsigned int, std::function<std::shared_ptr<ResourceBase>(const std::string& name, const std::vector<Property> &properties)>> factories; // Mapping of type ID to factory function.
		};
#ifdef _WIN32
		__declspec(selectany) std::once_flag ResourceSystem::only_one;
		__declspec(selectany) std::shared_ptr<ResourceSystem> ResourceSystem::instance = nullptr;
#endif
	}
}
