#pragma once

#include <string>
#include <memory>
#include <map>
#include <mutex>

namespace Sigma {
	namespace resource {
		// Each reflected type must specialize this.
		template <typename TYPE> const char* GetTypeName(void) { static_assert(0, "GetTypeName is undefined for a type."); }
		template <typename TYPE> const unsigned int GetTypeID(void) { static_assert(0, "GetTypeID is undefined for a type."); }

		// A simple base class used to store a non-template pointer in ResourceSystem::loaders.
		class ResourceLoaderBase {
		public:
			ResourceLoaderBase() { }
			virtual ~ResourceLoaderBase() { }
		};

		// The template parameter is the type of the resource this system is used for.
		template<class T>
		class ResourceLoader : public ResourceLoaderBase {
		public:
			ResourceLoader() { }
			~ResourceLoader() { }

			/**
			 * \brief Returns a resource with the specified name.
			 *
			 * This method will load the resource if it isn't loaded already.
			 * \param[in] std::string name Filename of the resource to load and return.
			 * \return std::shared_ptr<T> The returned resource might be empty if it failed to load.
			 */
			std::shared_ptr<T> Get(const std::string& name) {
				if (this->resources.find(name) == this->resources.end()) {
					this->resources[name].reset(new T());
					this->resources[name]->Load(name);
				}
				return this->resources[name];
			}

			/**
			 * \brief Used to add a resource created or copied from memory.
			 *
			 * \param[in] std::string name The name of the resource
			 * \param[in] std::shared_ptr<T> r The resource to store
			 * \return void
			 */
			void Add(const std::string& name, std::shared_ptr<T> r) {
				this->resources[name] = r;
			}

			/**
			 * \brief Removes a resource with the given name.
			 *
			 * \param[in] const std::string & name Name of the resource to remove.
			 * \return void
			 */
			void Remove(const std::string& name) {
				this->resources.erase(name);
			}

			/**
			 * \brief Checks if a resource with the given name exists.
			 *
			 * \param[in] const std::string & name
			 * \return bool True if it exists.
			 */
			bool Exists(const std::string& name) const {
				return this->resources.find(name) != this->resources.end();
			}
		private:
			std::map<const std::string, std::shared_ptr<T>> resources; // Mapping of resource name to resource type.
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
			static std::shared_ptr<ResourceSystem> GetInstace() {
				std::call_once(ResourceSystem::only_one, [] ()
				{ 
					ResourceSystem::instance.reset(new ResourceSystem()); 
				}
				);

				return ResourceSystem::instance;
			}
			~ResourceSystem() { }

			/**
			 * \brief Creates a new resource loader with the given type.
			 *
			 * The resource must have a static member TypeID and provide a bool Load(std::string) method.
			 * \return void
			 */
			template<class T>
			void CreateResourceLoader() {
				if (this->loaders.find(GetTypeID<T>()) == this->loaders.end()) {
					this->loaders[GetTypeID<T>()].reset(new ResourceLoader<T>);
				}
			}

			/**
			 * \brief Add a resource loader created externally to the list of loaders.
			 *
			 * The resource must have a static member TypeID and provide a bool Load(std::string) method.
			 * \param[in] std::shared_ptr<ResourceLoader<T>> rl The resource loader to add.
			 * \return void
			 */
			template<class T>
			void AddResourceLoader(std::shared_ptr<ResourceLoader<T>> rl) {
				if (this->loaders.find(GetTypeID<T>()) == this->loaders.end()) {
					this->loaders[GetTypeID<T>()] = rl;
				}
			}

			/**
			 * \brief Remove a resource loader.
			 *
			 * Removal is based on the TypeID of the resource.
			 * \return void
			 */
			template<class T>
			void RemoveResourceLoader() {
				if (this->loaders.find(GetTypeID<T>()) != this->loaders.end()) {
					this->loaders.erase(GetTypeID<T>());
				}
			}

			/**
			 * \brief Returns a resource loader.
			 *
			 * The returned loader is based on the TypeID of the resource.
			 * \return
			 */
			template<class T>
			std::shared_ptr<ResourceLoader<T>> GetResourceLoader() const {
				if (this->loaders.find(GetTypeID<T>()) != this->loaders.end()) {
					return std::static_pointer_cast<ResourceLoader<T>>(this->loaders[GetTypeID<T>()].get());
				}
			}

			/**
			 * \brief Calls ResourceLoader::Get.
			 *
			 * \param[in] const std::string& name The name of the resource to get.
			 * \return The returned resource might be empty if it failed to load
			 */
			template<class T>
			std::shared_ptr<T> Get(const std::string& name) {
				if (this->loaders.find(GetTypeID<T>()) != this->loaders.end()) {
					return static_cast<ResourceLoader<T>*>(this->loaders[GetTypeID<T>()].get())->Get(name);
				}
				return nullptr;
			}

			/**
			 * \brief Calls ResourceLoader::Add.
			 *
			 * \param[in] const std::string& name The name of the resource.
			 * \param[in] std::shared_ptr<T> r The resource to add.
			 * \return void
			 */
			template<class T>
			void Add(const std::string& name, std::shared_ptr<T> r) {
				if (this->loaders.find(GetTypeID<T>()) != this->loaders.end()) {
					static_cast<ResourceLoader<T>*>(this->loaders[GetTypeID<T>()].get())->Add(name, r);
				}
			}

			/**
			 * \brief Calls ResourceLoader::Remove
			 *
			 * \param[in] const std::string& name Name of the resource to remove.
			 * \return void
			 */
			template<class T>
			void Remove(const std::string& name) {
				if (this->loaders.find(GetTypeID<T>()) != this->loaders.end()) {
					static_cast<ResourceLoader<T>*>(this->loaders[GetTypeID<T>()].get())->Remove(name);
				}
			}

			/**
			 * \brief Calls ResourceLoader::Exists,
			 *
			 * \param[in] const std::string& name Name of the resource to check if it exists.
			 * \return bool True if the resource exists.
			 */
			template<class T>
			bool Exists(const std::string& name) const {
				if (this->loaders.find(GetTypeID<T>()) != this->loaders.end()) {
					return static_cast<ResourceLoader<T>*>(this->loaders[GetTypeID<T>()].get())->Exists(name);
				}
				return false;
			}
		private:
			std::map<unsigned int, std::shared_ptr<ResourceLoaderBase>> loaders; // Mapping of resource TypeID to loader.
		};
#ifdef _WIN32
		__declspec(selectany) std::once_flag ResourceSystem::only_one;
		__declspec(selectany) std::shared_ptr<ResourceSystem> ResourceSystem::instance = nullptr;
#endif
	}
}