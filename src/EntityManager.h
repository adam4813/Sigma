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
        NoSuchEntityException(unsigned int entity) : std::range_error(""){
            char buffer[50];
            sprintf(buffer, "Entity %d does not exist!", entity);
            this->_msg = std::string(buffer);
        }

        std::string getMessage(){
            return this->_msg;
        }
    private:
        std::string _msg;
    };

    /**
     * The EntityManager class owns and manages all entities and components.
     *
     * A single EntityManager can be thought of as a "context." That is, each
     *  entity must have a unique id _in that context_. Contexts are a useful
     *  conceptual framework for separating entities of different types. For
     *  example, there may be a sound context, graphics context, etc. This
     *  speeds up search because look-up is only within a given context.
     *
     * Querying a context that does not exist results in a NoSuchContextException
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

            /** \brief Create a new EntityManager
             *
             * All entities that belong to the same EntityManager are considered
             * part of the same "context." Keep track of the return value from
             * this function because you will need the context-id for adding, removing,
             * and querying entities from this context.
             *
             * \return the id of the new EntityManager, i.e. the context id
             */
            static unsigned int CreateContext();

            /** \brief Check if the entity exists in the given context
             *
             * \param context const type_id the context/EntityManager to search
             * \param entity_id const type_id the entity being queried
             * \return bool true if and only if the entity exists in the given context
             * \throw NoSuchContextException
             */
            static bool EntityExists(const type_id context, const type_id entity_id);

            /** \brief Create an empty entity in the given context. Replaces any
             *  existing entity with the same id
             *
             * \param context const type_id the context/EntityManager to insert into
             * \param entity_id const type_id the entity id to insert
             * \return bool true if it overwrote a previous entity
             * \throw NoSuchContextException if there is no matching context
             */
            static bool CreateEntity(const type_id context, const type_id entity_id);

            /** \brief Delete the specified entity
             *
             * \param context const type_id the context/EntityManager to use
             * \param entity_id const type_id the entity id to delete
             * \return bool true if it removed an entity (false if there is no such entity)
             * \throw NoSuchContextException if there is no matching context
             */
            static bool DeleteEntity(const type_id context, const type_id entity_id);

            /** \brief Create the component specified by component_type and properties. Add it to
             * the given entity of the given context. Overwrites existing components of the same
             * type.
             *
             * \param context const type_id the context/EntityManager to insert into
             * \param entity_id const type_id the entity id to give this component to
             * \return void
             * \throw NoSuchContextException if there is no matching context
             * \throw NoSuchEntityException if the context exists but the entity doesn't
             */
            static void AddComponent(const type_id context, const type_id entity_id, const std::string& component_type, const std::vector<Property>& properties);

             /** \brief Remove from the specified entity the component of type component_type, if it exists.
             *
             * \param context const type_id the context/EntityManager to use
             * \param entity_id const type_id the entity id to remove this component from
             * \return bool true if a component was actually removed (false if it never existed)
             * \throw NoSuchContextException if there is no matching context
             * \throw NoSuchEntityException if the context exists but the entity doesn't
             */
            static bool RemoveComponent(const type_id context, const type_id entity_id, const std::string& component_type);

            /** \brief Get a pointer to the component of the requested type for the specified entity.
             *
             * \param context const type_id the context/EntityManager to use
             * \param entity_id const type_id the entity id to search
             * \return a pointer to the requested component, or nullptr if none exists
             * \throw NoSuchContextException if there is no matching context
             * \throw NoSuchEntityException if the context exists but the entity doesn't
             */
            static IComponent* GetComponent(const type_id context, const type_id entity_id, const std::string& component_type);

            /** \brief Get a vector of all components for the specified entity
             *
             * \param context const type_id the context/EntityManager to use
             * \param entity_id const type_id the entity id to search
             * \return a vector of component pointers
             * \throw NoSuchContextException if there is no matching context
             * \throw NoSuchEntityException if the context exists but the entity doesn't
             */
            static std::vector<IComponent*> GetAllComponents(const type_id context, const type_id entity_id);

        private:
            // ctor only accessible through static CreateContext
            EntityManager(){ }
            EntityManager(const EntityManager& copy){ }

            // Each EntityManager can be thought of as a "context" that
            //  is shared by all entities in it. Here, we keep the list
            //  of already-created managers (i.e. contexts), statically
            static std::vector<std::unique_ptr<EntityManager>> existing_contexts;
            // get context or throw NoSuchContextException
            static EntityManager* getContext(const type_id context);

            // here's where we keep track of the entities and components
            std::map<type_id, std::vector<std::unique_ptr<IComponent>>> entities;
    };

}

#endif // ENTITYMANAGER_H
