#pragma once
#ifndef FACTORYSYSTEM_H
#define FACTORYSYSTEM_H
#include "IFactory.h"
#include "IECSFactory.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>
#include "Sigma.h"

namespace Sigma {

    class FactorySystem {
        public:
            static FactorySystem& getInstance();
            ~FactorySystem();
            /**
             * \brief Create a new components of a given type.
             *
             * A factory method to create various components and add them to the system. These components will be used during the system update method
             * \param[in] const std::string type The type of componenet to create
             * \param[in] const id_t entityID The ID of the entity this component belongs to.
             * \param[in] std::vector<Property> &properties A vector containing the properties to apply to the created component.
             * \return a pointer to the newly created component
             */
            IComponent* create(const std::string& type,
                        const id_t entityID,
                        const std::vector<Property> &properties);

			std::vector<std::unique_ptr<IECSComponent>> createECS(const std::string& type,
                               const id_t entityID,
                               const std::vector<Property> &properties);

			/**
             * \brief add the given Factory to the central list
             *
             *  All factory-functions returned by getFactoryFunctions() are
             *  added (registered) to the central list of functions
             * \param[in] IFactory& Factory the factory whose functions will be registered
             */
            void register_Factory(IFactory& Factory);

            /**
             * \brief add the given ECSFactory to the central list (ECS style)
             *
             *  All factory-functions returned by getFactoryFunctions() are
             *  added (registered) to the central list of functions
             * \param[in] IECSFactory& Factory the factory whose functions will be registered
             */
            void register_ECSFactory(IECSFactory& Factory);

        protected:
        private:
            // Hide all constructors and the assignment operator to enforce the singleton pattern
            FactorySystem();
            FactorySystem(const FactorySystem& rhs);
            FactorySystem& operator=(const FactorySystem& rhs);
            // the singleton instance
            static std::shared_ptr<FactorySystem> _instance;
            // the map of name-->factory
            std::unordered_map<std::string,IFactory::FactoryFunction>
                    registeredFactoryFunctions;

			// the map of name-->factory for the ECS style components
            std::unordered_map<std::string,IECSFactory::FactoryFunction>
                    registeredECSFactoryFunctions;
    }; // class FactorySystem

} // namespace Sigma

#endif // FACTORYSYSTEM_H
