#ifndef VECTORMAP_H_INCLUDED
#define VECTORMAP_H_INCLUDED

#include "SharedPointerMap.hpp"
//#include "AlignedVectorAllocator.hpp"
#include <memory>
#include <unordered_map>
#include <vector>
#include <iterator>

namespace Sigma {
	// Visual Studio doesn't support template aliases until 2013
    //template<class V>
    //using aligned_vector_t = std::vector<V, AlignedVectorAllocator<V>>;

    /** \brief A map whose elements are stored in a vector
     *
     */
    template<class K, class V>
    class VectorMap {
    public:
        /** Default constructor */
        VectorMap(const size_t& n = 256) {
            data_vector.reserve(n);
        };

        /** Default destructor */
        virtual ~VectorMap() {};

        /** \brief Get a const reference for a specific key
         *
         * throw an exception is key does not exist
         *
         * \param key the key
         * \return a weak pointer on the value
         *
         */
        const WeakPtrWrapper<V> at(const K& key) const { return data_map.at(key); };

        /** \brief Get a reference for a specific key
         *
         * throw an exception is key does not exist
         *
         * \param key the key
         * \return a weak pointer on the value
         *
         */
        WeakPtrWrapper<V> at(const K& key) { return data_map.at(key); };

        /** \brief Get a reference for a specific key
         *
         * Insert the element if it does not exist
         *
         * \param key K key of the element
         * \return a reference on the value
         *
         */
        WeakPtrWrapper<V> operator[](const K& key) {
            if (! data_map.count(key) && (key_vector.empty() || key_vector.back() != key)) {
                // check if we will resize
                if (data_vector.size() == data_vector.capacity()) {
                    Resize();
                }
                // if this is a new entity, create its position in the storage vector
                V my_vec;
                data_vector.emplace_back(my_vec);
                // save id of last object
                key_vector.push_back(key);
                return data_map.add(key, &data_vector.back());
            }
            // return a callback containing unique_ptr
            return data_map.at(key);
        };

        /** \brief Tell if a key exist
         *
         * \param key the key to test
         * \return bool true if the key exist
         *
         */
        size_t count(K key) const { return data_map.count(key); }

        /** \brief Remove an element
        *
        * \param key K the key of the element
        * \return void
        *
        */
        void clear(K key) {
            V* addr = data_map.clear(key);
            size_t index = addr - data_vector.data();
            K last_key = key_vector.back();
            if (last_key != key) {
                    // swap with last element
                    *addr = std::move(data_vector.back());
                    data_map.add(last_key, addr);
                    key_vector[index] = std::move(last_key);
            }
            // remove last element
            data_vector.pop_back();
            key_vector.pop_back();
        };

        /** \brief Get the vector
         *
         * \return const aligned_vector_t<V>* the vector
         *
         */
//        std::vector<V, AlignedVectorAllocator<V>>* getVector() { return &data_vector; }
        std::vector<V>* getVector() { return &data_vector; }

        /** \brief Get an iterator on the key
         *
         * \return std::vector<K>::const_iterator the iterator
         *
         */
        typename std::vector<K>::const_iterator IteratorKeyBegin() const { return key_vector.cbegin(); }

        /** \brief Get an iterator on the key at the end
         *
         * \return std::vector<K>::const_iterator the iterator
         *
         */
        typename std::vector<K>::const_iterator IteratorKeyEnd() const { return key_vector.cend(); }

    private:

        /** \brief Resize the vector
         *
         * \return void
         *
         */
        void Resize() {
            // make a copy of the vector with a double size capacity
//            std::vector<V, AlignedVectorAllocator<V>> v_copy(data_vector.size() << 1);
            std::vector<V> v_copy(data_vector.size() << 1);
            v_copy = data_vector;
            // replace the pointers in the map
            auto it = key_vector.begin();
            for (V& element : v_copy) {
                data_map.add(*(it++), &element);
            }
            // swap the original and the copy
            std::swap(data_vector, v_copy);
        };

        /** \brief Find the index of an element in the vector
         *
         * \param key K the key to find
         * \return size_t the index of the element in the vector
         *
         */
        const size_t FindIdElement(const K& key) const {
            auto ptr = data_map.at(key);
            if (! ptr.expired()) {
                auto p = ptr.lock().get();
                return (p - data_vector.data());
            }
            return -1;
        };

        // the maps containing pointers to values
        SharedPointerMap<K, V> data_map;

        // the vectors containing values
        std::vector<V> data_vector;
//        std::vector<V, AlignedVectorAllocator<V>> data_vector;

        // the vector of keys
        std::vector<K> key_vector;
    };
}

#endif // VECTORMAP_H_INCLUDED
