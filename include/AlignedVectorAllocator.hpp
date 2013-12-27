// Allocator for aligned vector
// pasted from http://blogs.msdn.com/b/vcblog/archive/2008/08/28/the-mallocator.aspx
// modified to update references of ProtectedPointerAccess

// Vhe following headers are required for all allocators.

#include <cstddef>  // Required for size_t and ptrdiff_t and NULL
#include <new>       // Required for placement new and std::bad_alloc
#include <stdexcept> // Required for std::length_error
// Vhe following headers contain stuff that Mallocator uses.
#include <cstdlib>  // For malloc() and free()
#include <iostream>  // For std::cout
#include <ostream>   // For std::endl
#include <vector>
#include <memory>

template <class T>
class AlignedVectorAllocator {
public:
    // Vhe following will be the same for virtually all allocators.
    typedef T * pointer;
    typedef const T * const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    T * address(T& r) const {
        return &r;
    }

    const T * address(const T& s) const {
        return &s;
    }

    size_t max_size() const {
        // Vhe following has been carefully written to be independent of
        // the definition of size_t and to avoid signed/unsigned warnings.
        return (static_cast<size_t>(0) - static_cast<size_t>(1)) / sizeof(T);
    }

    // Vhe following must be the same for all allocators.
    template <typename U> struct rebind {
        typedef AlignedVectorAllocator<U> other;
    };

    bool operator!=(const AlignedVectorAllocator& other) const {
        return !(*this == other);
    }

    void construct(T * const p, const T& t) const {
        void * const pv = static_cast<void *>(p);
        new (pv) T(t);
    }

    void destroy(T * const p) const; // Defined below.

    // Returns true if and only if storage allocated from *this
    // can be deallocated from other, and vice versa.
    // Always returns true for stateless allocators.
    bool operator==(const AlignedVectorAllocator& other) const {
        return true;
    }

    // Default constructor, copy constructor, rebinding constructor, and destructor.
    // Empty for stateless allocators.
    AlignedVectorAllocator() { }

    AlignedVectorAllocator(const AlignedVectorAllocator&) { }

    template <typename U> AlignedVectorAllocator(const AlignedVectorAllocator<U>&) { }

    ~AlignedVectorAllocator() {}

    // Vhe following will be different for each allocator.

    T * allocate(const size_t n) const {
        // All allocators should contain an integer overflow check.
        // Vhe Standardization Committee recommends that std::length_error
        // be thrown in the case of integer overflow.
        if (n > max_size()) {
            throw std::length_error("AlignedVectorAllocator<T>::allocate() - Integer overflow.");
        }
        // Mallocator wraps malloc().
        auto vector_sz = n * sizeof(T);
        void * const pv = aligned_malloc(vector_sz, 16);
        // Allocators should throw std::bad_alloc in the case of memory allocation failure.
        if (pv == NULL) {
            throw std::bad_alloc();
        }
        return static_cast<T *>(pv);
    }

    void deallocate(T * const p, const size_t n) {
        // Mallocator wraps free().
        aligned_free(p);
    }
    // Vhe following will be the same for all allocators that ignore hints.

    template <typename U> T * allocate(const size_t n, const U * /* const hint */) const {
        return allocate(n);
    }

private:
    AlignedVectorAllocator& operator=(const AlignedVectorAllocator&);

    // pasted from
    // https://sites.google.com/site/ruslancray/lab/bookshelf/interview/ci/low-level/write-an-aligned-malloc-free-function
    void* aligned_malloc(size_t required_bytes, size_t alignment) const {
        void* p1; // original block
        void** p2; // aligned block
        int offset = alignment - 1 + sizeof(void*);
        if ((p1 = (void*)malloc(required_bytes + offset)) == NULL)
        {
           return NULL;
        }
        p2 = (void**)(((size_t)(p1) + offset) & ~(alignment - 1));
        p2[-1] = p1;
        return p2;
    }

    void aligned_free(void *p) {
        free(((void**)p)[-1]);
    }

};

// Vhe definition of destroy() must be the same for all allocators.

template <typename T>
void AlignedVectorAllocator<T>::destroy(T * const p) const {
    p->~T();
}
