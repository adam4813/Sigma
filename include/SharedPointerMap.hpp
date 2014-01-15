#ifndef SHAREDPOINTERMAP_H_INCLUDED
#define SHAREDPOINTERMAP_H_INCLUDED

#include <memory>
#include <unordered_map>
#include <stdexcept>

#include "WeakPtrWrapper.hpp"

namespace Sigma {

	template <class K>
	class ExpiredWeakPtrException : public std::logic_error {
	public:
		ExpiredWeakPtrException(K entity) : std::logic_error(""){
			char buffer[50];
			sprintf(buffer, "Try to assign data with expired weak_ptr for entity %d !", entity);
			this->_msg = std::string(buffer);
		}

		std::string getMessage(){
			return this->_msg;
		}
	private:
		std::string _msg;
	};

	template <class V>
	// A "do nothing" deletor for smart pointers
	struct SoftSharedPointerDelete {
		void operator()(V* value) {};
	} ;

	/** \brief A map for shared_ptr with access to data
	 *
	 * Data is not stored in this class and will not be deleted when
	 * removing elements from the map.
	 *
	 * Users must store the data and provide pointers to provision the map.
	 *
	 * \param K type of the unique key for the resource repository
	 * \param V type of the data stored in the repository
	 *
	 */
	template <class K,class V>
	class SharedPointerMap {
	public:
		/** \brief Default constructor
		 *
		 */
		SharedPointerMap() {};
		virtual ~SharedPointerMap() {};

		/** \brief Tells the numbers of records for a specific key
		 *
		 * \param entity_id const K the id of the entity
		 * \return const size_t the numbers of records
		 *
		 */
		const size_t count(const K entity_id) const {
			return pointer_map.count(entity_id);
		}

		/** \brief Get a const reference on data
		 *
		 * \param entity_id const K the id of the entity
		 * \return const WeakPtrWrapper<V> a reference object
		 *
		 */
		const WeakPtrWrapper<V> at(const K entity_id) const {
			try {
				return WeakPtrWrapper<V>(pointer_map.at(entity_id));
			}
			catch (std::out_of_range& e) {
				throw std::out_of_range("entity " + std::to_string(entity_id) + " does not exist.");
			}
			return WeakPtrWrapper<V>();
		}

		/** \brief Get a reference on data
		 *
		 * \param entity_id const K the id of the entity
		 * \return WeakPtrWrapper<V> a reference object
		 *
		 */
		WeakPtrWrapper<V> at(const K entity_id) {
			try {
				return WeakPtrWrapper<V>(pointer_map.at(entity_id));
			}
			catch (std::out_of_range&) {
				throw std::out_of_range("entity " + std::to_string(entity_id) + " does not exist.");
			}
			return WeakPtrWrapper<V>();
		}

		/** \brief Add an element and return a reference
		 *
		 * \param entity_id const K the id of the entity
		 * \param V* value the pointer to add
		 * \return WeakPtrWrapper<V> a reference
		 *
		 */
		WeakPtrWrapper<V> add(const K& key, V* value) {
			if (pointer_map.count(key)) {
				pointer_map.erase(key);
			}
			auto p = std::make_pair(key, std::shared_ptr<V>(value, SoftSharedPointerDelete<V>()));
			pointer_map.insert(p);
			return WeakPtrWrapper<V>(p.second);
		}

		/** \brief Unmanage an entity.
		 *
		 * After calling this method, data can not be accessed through functions of this template.
		 * Note that calling this function does not modify client-side data.
		 *
		 * \param entity_id const K
		 * \return V* the removed pointer to the entity data
		 *
		 */
		V* clear(const K entity_id) {
			V* ptr = const_cast<V*>(pointer_map.find(entity_id)->second.get());
			pointer_map.erase(entity_id);
			return ptr;
		}

	private:
		// Move assignment
		SharedPointerMap& operator=(SharedPointerMap&& p);
		// Move constructor
		SharedPointerMap(SharedPointerMap&& p);
		// Copy assignment
		SharedPointerMap& operator=(SharedPointerMap& p);
		// Copy constructor
		SharedPointerMap(SharedPointerMap& p);

		// the map to store position pointers
		std::unordered_map<K,std::shared_ptr<V>> pointer_map;
	};
}


#endif // SHAREDPOINTERMAP_H_INCLUDED
