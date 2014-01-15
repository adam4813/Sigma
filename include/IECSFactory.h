#ifndef IECSFACTORY_INCLUDED
#define IECSFACTORY_INCLUDED

#include <functional>
#include <vector>
#include <map>
#include <memory>
#include "Property.h"
#include "Sigma.h"
#include "IECSComponent.h"

namespace Sigma {
    class IECSFactory {
    public:
        typedef std::function<std::vector<std::unique_ptr<IECSComponent>>(const id_t,
                                    const std::vector<Property>&)> FactoryFunction;
        IECSFactory(){};
        virtual ~IECSFactory(){};
        /**
         * \brief Returns the list of Factory functions and types they create
         *
         *
         * \return std::map<std::string, FactoryFunction> Contains Callbacks for different Component types that can be created by this class
         */
        virtual std::map<std::string,FactoryFunction>
                getECSFactoryFunctions() = 0;
    protected:
    private:
    };
}


#endif // IECSFACTORY_INCLUDED
