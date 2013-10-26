#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include <memory>
#include <map>
#include <cstdio>
#include <stdexcept>
#include "IComponent.h"
#include "Property.h"

namespace Sigma{

    class NoSuchContextException : public std::range_error {
    public:
        NoSuchContextException(unsigned int context) : std::range_error(""){
            char buffer[50];
            sprintf(buffer, "Context %d does not exist!", context);
            this->_msg = std::string(buffer);
        }

        std::string getMessage(){
            return this->_msg;
        }
    private:
        std::string _msg;
    };

    class NoSuchEntityException : public std::range_error {
    public:
        NoSuchEntityException(unsigned int entity, unsigned int context) : std::range_error(""){
            char buffer[50];
            sprintf(buffer, "Entity %d does not exist in context %d!", entity, context);
            this->_msg = std::string(buffer);
        }

        std::string getMessage(){
            return this->_msg;
        }
    private:
        std::string _msg;
    };

    // forward definition of static-initialization struct
    struct static_initializer;

    /**
     * The EntityManager class owns and manages all entities and components.
     *
     * A single EntityManager can be thought of as a "context." That is, each
     *  entity must have a unique id _in that context_. Contexts are a useful
     *  conceptual framework for separating entities of different types. For
     *  example, there may be a sound context, graphics context, etc. This
     *  speeds up search because look-up is only within a given context.
     *
     * Note: you can use the default '0' context without having to create it manually
     * (it is automatically created so you never *need* to worry about contexts)
     *
     * Attempting to use a context that does not exist results in a NoSuchContextException
     *
     * Adding/Removing/Getting components from an entity that does not exist
     * results in a NoSuchEntityException being thrown.
     *
     * Creating an entity that already exists will overwrite the original entity.
     */
    class EntityManager
    {
        public:
            // note that the constructors are private
            virtual ~EntityManager() {}
            typedef unsigned int type_id;

            friend struct static_initializer;

            /** \brief Create a new EntityManager
             *
             * All entities that belong to the same EntityManager are considered
             * part of the same "context." Keep track of the return value from
             * this function because you will need the context-id for adding, removing,
             * and querying entities from this context.
             *
             * Note that there is a default '0' context created automatically. The first
             *  call to CreateContext() results in a *second* context being created.
             *
             * \return the id of the new EntityManager, i.e. the context id
             */
            static unsigned int CreateContext();

            /** \brief Check if the entity exists in the given context
             *
             * \param entity_id const type_id the entity being queried
             * \return bool true if and only if the entity exists in the given context
             */
            static bool EntityExists(const type_id entity_id);

            /** \brief Create an empty entity in the given context. Replaces any
             *  existing entity with the same id
             *
             * \param entity_id const type_id the entity id to insert
             * \return bool true if it overwrote a previous entity
             */
            static bool CreateEntity(const type_id entity_id);

            /** \brief Delete the specified entity
             *
             * \param entity_id const type_id the entity id to delete
             * \return bool true if it removed an entity (false if there is no such entity)
             */
            static bool DeleteEntity(const type_id entity_id);

            /** \brief Create the component specified by component_type and properties. Add it to
             * the given entity of the given context. Overwrites existing components of the same
             * type.
             *
             * \param entity_id const type_id the entity id to give this component to
             * \return void
             * \throw NoSuchEntityException if the entity doesn't exist in this context
             */
            static void AddComponent(const type_id entity_id, const std::string& component_type, const std::vector<Property>& properties);

             /** \brief Remove from the specified entity the component of type component_type, if it exists.
             *
             * \param entity_id const type_id the entity id to remove this component from
             * \return bool true if a component was actually removed (false if it never existed)
             * \throw NoSuchEntityException if the entity doesn't exist in this context
             */
            static bool RemoveComponent(const type_id entity_id, const std::string& component_type);

            /** \brief Get a pointer to the component of the requested type for the specified entity.
             *
             * \param entity_id const type_id the entity id to search
             * \return a pointer to the requested component, or nullptr if none exists
             * \throw NoSuchEntityException if the entity doesn't exist in this context
             */
            static IComponent* GetComponent(const type_id entity_id, const std::string& component_type);

            /** \brief Get a vector of all components for the specified entity
             *
             * \param entity_id const type_id the entity id to search
             * \return a vector of component pointers
             * \throw NoSuchEntityException if the entity doesn't exist in this context
             */
            static std::vector<IComponent*> GetAllComponents(const type_id entity_id);

            /** \brief Set the context for all subsequent actions
             *
             * \param context const type_id the context to switch into
             * \return void
             * \throw NoSuchContextException if the requested context does not exist
             */
            static void UseContext(const type_id context);

            /** \brief Gets the id of the currently active context
             *
             * \return type_id the id of the currently active context
             *
             */
            static type_id CurrentContext(){ return EntityManager::current_context_id; }
        private:
            // ctor only accessible through static CreateContext
            EntityManager(){ }
            EntityManager(const EntityManager& copy){ }

            // Each EntityManager can be thought of as a "context" that
            //  is shared by all entities in it. Here, we keep the list
            //  of already-created managers (i.e. contexts), statically
            static std::vector<std::unique_ptr<EntityManager>> existing_contexts;
            // keep track of current context
            static EntityManager* current_context;
            static type_id current_context_id;

            // here's where we keep track of the entities and components
            std::map<type_id, std::vector<std::unique_ptr<IComponent>>> entities;

            // static initialization
            static static_initializer theInitializer;
    };

    // static initialization
    struct static_initializer{
        static_initializer(){
            // set up default context
            // (this code is run once, statically)
            EntityManager::current_context_id = EntityManager::CreateContext();
            EntityManager::UseContext(EntityManager::current_context_id);
        }
    };

}

#endif // ENTITYMANAGER_H
