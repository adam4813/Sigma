#ifndef WEAKPTRWRAPPER_HPP_INCLUDED
#define WEAKPTRWRAPPER_HPP_INCLUDED

template<class V>
/** \brief A reference object returned to modify data
 */
class WeakPtrWrapper {
public:
	WeakPtrWrapper() : ptr(std::make_shared<V>()) {};
	WeakPtrWrapper(const std::shared_ptr<V>& ptr) : ptr(ptr) {};
	WeakPtrWrapper(std::shared_ptr<V>&& ptr) : ptr(std::move(ptr)) {};
	virtual ~WeakPtrWrapper() {};

	// copy assignment for shared_ptr
	WeakPtrWrapper& operator=(std::shared_ptr<V>& p) { ptr = p; };
	// move assignment for shared_ptr
	WeakPtrWrapper& operator=(std::shared_ptr<V>&& p) { ptr = std::move(p); };

	// copy assignment for V
	WeakPtrWrapper& operator=(V& v) {
		if (! this->expired()) {
			*this->lock() = v;
		};
		return *this;
	}

	// move assignment for V
	WeakPtrWrapper& operator=(V&& v) {
		if (! this->expired()) {
			*this->lock() = std::move(v);
		};
		return *this;
	}

	// copy assignment
	WeakPtrWrapper& operator=(const WeakPtrWrapper<V>& p) {
		if (! this->expired() && ! p.expired()) {
			*this->lock() = *p.lock();
		};
		return *this;
	}

	operator const std::weak_ptr<V>() const { return ptr; };
	operator const V() const {
		return *ptr.lock();
	}

	bool operator==(const WeakPtrWrapper<V>& rhs) const {
		return  (!this->expired()) && (!rhs.expired()) && *this->ptr.lock() == *rhs.ptr.lock();
	}

	bool operator==(const V& rhs) const {
		return  (!this->expired()) && *this->ptr.lock() == rhs;
	}

	bool expired() const { return ptr.expired(); };

	std::shared_ptr<V> lock() const { return ptr.lock(); };
private:
	std::weak_ptr<V> ptr;
};


#endif // WEAKPTRWRAPPER_HPP_INCLUDED
