#ifndef IFACTORY_H
#define IFACTORY_H

#include <functional>
#include <vector>
#include <map>
#include "Property.h"

class IFactory
{
    public:
        typedef std::function<void( const std::string,
                                    const unsigned int,
                                    std::vector<Property>&)> FactoryFunction;
        IFactory(){};
        virtual ~IFactory(){};
        virtual std::map<std::string,FactoryFunction>
                getFactoryFunctions()=0;
    protected:
    private:
};

#endif // IFACTORY_H
