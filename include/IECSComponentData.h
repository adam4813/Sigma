#ifndef IECSCOMPONENTDATA_H_INCLUDED
#define IECSCOMPONENTDATA_H_INCLUDED

#include <string>
#include <memory>
#include "Sigma.h"

#define SET_GETTER(type)								\
std::weak_ptr<type>* get() { return &data;};

namespace Sigma{
	template<class ComponentType>
    /** \brief A class to store data of a component
     */
    class IECSComponentData {
    public:
        IECSComponentData(const std::weak_ptr<ComponentType>& ptr) : data(ptr) {}
        virtual ~IECSComponentData() {}

        std::weak_ptr<ComponentType> get() { return data; };

   		bool expired() { return data.expired(); };

	protected:
		std::weak_ptr<ComponentType> data;
    }; // class IECSComponentData
} // namespace Sigma

#endif // IECSCOMPONENTDATA_H_INCLUDED
