#ifndef SHAREDPOINTERMAP_H_INCLUDED
#define SHAREDPOINTERMAP_H_INCLUDED

#include <memory>
#include <unordered_map>
#include <stdexcept>


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
        void operator()(V* value) {}
    } ;

    /** \brief A secured access exposer to owned resources
     *
     * This template provides secured CRUD (Create, Read, Update, Delete) functions
     * on a resource data repository.
     *
     * The resource data owner must forward its own CRUD methods to an instance
     * of this template. This template does not store the data itself, it only
     * stores pointers to the data provided by the resource data owner.
     *
     * CRUD functions are exposed in the following manner:
     * - Read operations provide a weak_ptr mapped to the data related to an entity
     * - Create and Update operations provide a non-const reference to the data
     *   related to an entity
     *
     * The properties of the exposed objects are:
     * - weak pointers are persistent and expire when data is updated. Data users must call expired()
     *   before reading data.
     * - an expired weak pointer does not mean that data was actually updated, it is
     *   only meaning that the pointer is no longer valid and a new pointer is available.
     * - non-const references are non-movable, non-copiable, non-assignable. It means
     *   that users can only use the write functions as the left argument of a instruction,
     *   i.e Write(id) = ...
     * - the data is only modifiable through the dedicated functions
     *
     * Usage:
     * - resources data owners store the resource data on their side
     * - resource data owners must store a private instance of this template
     * - owner-side CRUD implementations should forward pointers to the template-side CRUD functions
     * - owners must update pointers when necessary, for example after a vector resize operation
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
         * \param callback a data validation instance
         *
         */
        SharedPointerMap() {};
        virtual ~SharedPointerMap() {};

        // enable WriteObject(K a) = ReadObject(K b);
        SharedPointerMap& operator=(const std::weak_ptr<V>& p) {
            if(! p.expired()) {
                return *p.lock();
            }
            throw ExpiredWeakPtrException<K>(id);
        }

        // copy assignment for V
        SharedPointerMap& operator=(const V& v) {
            *vec = v;
            pointer_map.erase(id);
            pointer_map.insert({{id, vec}});
            vec.reset();
            return *this;
        }

        // move assignment for V
        SharedPointerMap& operator=(V&& v) {
            *vec = std::move(v);
            pointer_map.erase(id);
            pointer_map.insert({{id, vec}});
            vec.reset();
            return *this;
        }

        /** \brief Tells if an entity has record in the internal storage
         *
         * \param entity_id const K the id of the entity
         * \return const bool true if data exists
         *
         */
        const bool Exist(const K entity_id) const {
            return pointer_map.count(entity_id);
        }

        /** \brief Get a weak_ptr pointing on data
         *
         * \param entity_id const K the id of the entity
         * \return const std::weak_ptr<V> a weak_ptr pointing on data, or empty
         *
         */
        const std::weak_ptr<const V> Read(const K entity_id) const {
            if (pointer_map.count(entity_id)) {
                return pointer_map.find(entity_id)->second;
            }
            throw std::out_of_range("entity " + std::to_string(entity_id) + " does not exist.");
            return std::shared_ptr<const V>();
        }

        /** \brief Add an element to the map
         *
         * \param entity_id const K the id of the entity
         * \param pointer V* const the pointer to add
         * \return void
         *
         */
        void SetElement(const K entity_id, V* const pointer) {
            if (pointer_map.count(entity_id)) {
                pointer_map.erase(entity_id);
            }
            pointer_map.insert({{entity_id, std::shared_ptr<const V>(pointer, SoftSharedPointerDelete<V>())}});
        }

        /** \brief Get a non-const reference to the data
         *
         * \param entity_id const K
         * \return SharedPointerMap& non-const reference to the data
         *
         */
        SharedPointerMap& Write(const K entity_id) {
            // prepare the context before writing
            id = entity_id;
            V* ptr = const_cast<V*>(pointer_map.find(entity_id)->second.get());
            pointer_map.find(entity_id)->second.reset();
            vec.reset(ptr, SoftSharedPointerDelete<V>());
            return *this;
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
        V* Remove(const K entity_id) {
            V* ptr = const_cast<V*>(pointer_map.find(entity_id)->second.get());
            pointer_map.erase(id);
            return ptr;
        }

    private:
        // Move assignment
        SharedPointerMap& operator=(SharedPointerMap&& p) = delete;
        // Move constructor
        SharedPointerMap(SharedPointerMap&& p) = delete;
        // Copy assignment
        SharedPointerMap& operator=(SharedPointerMap& p) = delete;
        // Copy constructor
        SharedPointerMap(SharedPointerMap& p) = delete;

        // the map to store position pointers
        std::unordered_map<K,std::shared_ptr<const V>> pointer_map;

        std::shared_ptr<V> vec;
        K id;
    };
}


#endif // SHAREDPOINTERMAP_H_INCLUDED
