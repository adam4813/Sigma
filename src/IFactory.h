#pragma once
#ifndef IFACTORY_H
#define IFACTORY_H

#include <functional>
#include <vector>
#include <map>
#include "Property.h"

namespace Sigma {
    class IFactory {
    public:
        typedef std::function<void( const unsigned int,
                                    std::vector<Property>&)> FactoryFunction;
        IFactory(){};
        virtual ~IFactory(){};
        /**
         * \brief Returns the list of Factory functions and types they create
         *
         *
         * \return std::map<std::string, FactoryFunction> Contains Callbacks for different Component types that can be created by this class
         */
        virtual std::map<std::string,FactoryFunction>
                getFactoryFunctions() = 0;
    protected:
    private:
    };
}

#endif // IFACTORY_H
