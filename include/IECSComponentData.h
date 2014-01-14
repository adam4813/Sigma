#ifndef IECSCOMPONENTGETTER_H_INCLUDED
#define IECSCOMPONENTGETTER_H_INCLUDED

#include <string>
#include "Sigma.h"
#include "WeakPtrWrapper.hpp"

#define SET_GETTER(type)								\
WeakPtrWrapper<type>* get() { return &data;};

namespace Sigma{
	template<class ComponentType>
    /** \brief A class to store data of a component
     */
    class IECSComponentData {
    public:
        IECSComponentData(const WeakPtrWrapper<ComponentType>& ptr) : data(ptr) {}
        virtual ~IECSComponentData() {}

        WeakPtrWrapper<ComponentType> get() { return data; };

   		bool expired() { return data.expired(); };

	protected:
		WeakPtrWrapper<ComponentType> data;
    }; // class IECSComponentGetter
} // namespace Sigma

#endif // IECSCOMPONENTGETTER_H_INCLUDED
