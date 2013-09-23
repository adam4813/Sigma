#ifndef FACTORYSYSTEM_H
#define FACTORYSYSTEM_H
#include "../IFactory.h"
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

class FactorySystem
{
    public:
        static FactorySystem& getInstance();
        ~FactorySystem();
        /**
         * \brief Create a new components of a given type.
         *
         * A factory method to create various components and add them to the system. These components will be used during the system update method
         * \param[in] const std::string type The type of componenet to create
         * \param[in] const int entityID The ID of the entity this component belongs to.
         * \param[in] std::vector<Property> &properties A vector containing the properties to apply to the created component.
         * \returns   IComponent* The newly create component
         */
        void create(const std::string type,
                    const unsigned int entityID,
                    std::vector<Property> &properties);
        void register_Factory(IFactory* Factory);
    protected:
    private:
        FactorySystem();
        FactorySystem(const FactorySystem& rhs);
        FactorySystem& operator=(const FactorySystem& rhs);
        static std::shared_ptr<FactorySystem> _instance;
        static std::once_flag only_once;
        std::unordered_map<std::string,IFactory::FactoryFunction>
                registeredFactoryFunctions;
};

#endif // FACTORYSYSTEM_H
