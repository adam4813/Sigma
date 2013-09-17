#ifndef FACTORYSYSTEM_H
#define FACTORYSYSTEM_H
#include "../IFactory.h"
#include <memory>
#include <mutex>
#include <unordered_map>

class FactorySystem
{
    public:
        static FactorySystem& getInstance();
        ~FactorySystem();
        void create(const std::string type,
                    const unsigned int entityID,
                    std::vector<Property> &properties);
        void register_Factory(IFactory* Factory);
    protected:
    private:
        FactorySystem();
        FactorySystem(const FactorySystem& rhs) = delete;
        FactorySystem& operator=(const FactorySystem& rhs) = delete;
        static std::shared_ptr<FactorySystem> _instance;
        static std::once_flag only_once;
        std::unordered_map<std::string,IFactory::FactoryFunction>
                registeredFactoryFunctions;
};

#endif // FACTORYSYSTEM_H
