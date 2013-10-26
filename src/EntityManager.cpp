#include "EntityManager.h"
#include "systems/FactorySystem.h"

namespace Sigma{

    // static initialization
    std::vector<std::unique_ptr<EntityManager>> EntityManager::existing_contexts;

    EntityManager::type_id EntityManager::CreateContext(){
        // create new entity manager
        std::unique_ptr<EntityManager> theManager(new EntityManager());
        // record the index where it is going (this will be its id)
        int next_slot = EntityManager::existing_contexts.size();
        // store this manager in the static list of contexts
        EntityManager::existing_contexts.push_back(std::move(theManager));
        // return this manager's id
        return next_slot;
    }

    bool EntityManager::EntityExists(const EntityManager::type_id context, const EntityManager::type_id entity_id){
        EntityManager* manager = EntityManager::getContext(context);
        if(manager != nullptr){
            // map.find() returns an iterator position, which is at the end if the query failed
            return manager->entities.find(entity_id) != manager->entities.end();
        } else{
            throw NoSuchContextException(context);
        }
    }

    bool EntityManager::CreateEntity(const EntityManager::type_id context, const EntityManager::type_id entity_id){
        EntityManager* manager = EntityManager::getContext(context);
        if(manager != nullptr){
            // note that the map class handles overwrite in the way we want:
            // if entity_id is already a key, it will delete the previous value
            // and replace it with this one
            manager->entities[entity_id] = std::vector<std::unique_ptr<IComponent>>();
        } else{
            throw NoSuchContextException(context);
        }
    }

    bool EntityManager::DeleteEntity(const type_id context, const type_id entity_id){
        EntityManager* manager = EntityManager::getContext(context);
        if(manager != nullptr){
            if(EntityExists(context, entity_id)){
                manager->entities.erase(entity_id);
            } else{
                return false;
            }
        } else{
            throw NoSuchContextException(context);
        }
    }

    void EntityManager::AddComponent(const EntityManager::type_id context, const EntityManager::type_id entity_id, const std::string& component_type, const std::vector<Property>& properties){
        EntityManager* manager = EntityManager::getContext(context);
        if(manager != nullptr){
            if(EntityManager::EntityExists(context, entity_id)){
                std::unique_ptr<IComponent> theComponent(FactorySystem::getInstance().create(component_type, entity_id, properties));
                (manager->entities[entity_id]).push_back(std::move(theComponent));
            } else{
                throw NoSuchEntityException(entity_id);
            }
        } else{
            throw NoSuchContextException(context);
        }
    }

    bool EntityManager::RemoveComponent(const EntityManager::type_id context, const EntityManager::type_id entity_id, const std::string& component_type){
        EntityManager* manager = EntityManager::getContext(context);
        if(manager != nullptr){
            if(EntityManager::EntityExists(context, entity_id)){
                // make an alias (reference) this entity's vector of components
                std::vector<std::unique_ptr<IComponent>>& theComponents = manager->entities[entity_id];
                // search for the specified component
                for(auto itr = theComponents.begin(); itr != theComponents.end(); ++itr){
                    if((*itr)->getComponentID() == component_type){
                        // remove it and break out since we know there is only one.
                        theComponents.erase(itr);
                        return true;
                    }
                }
                // never found it
                return false;
            } else{
                throw NoSuchEntityException(entity_id);
            }
        } else{
            throw NoSuchContextException(context);
        }
    }

    IComponent* EntityManager::GetComponent(const EntityManager::type_id context, const EntityManager::type_id entity_id, const std::string& component_type){
        EntityManager* manager = EntityManager::getContext(context);
        if(manager != nullptr){
            if(EntityManager::EntityExists(context, entity_id)){
                // make an alias (reference) this entity's vector of components
                std::vector<std::unique_ptr<IComponent>>& theComponents = manager->entities[entity_id];
                // search for the specified component
                for(auto itr = theComponents.begin(); itr != theComponents.end(); ++itr){
                    if((*itr)->getComponentID() == component_type){
                        // found! return it.
                        return (*itr).get();
                    }
                }
                // reached the end of the loop.. component not found
                return nullptr;
            } else{
                throw NoSuchEntityException(entity_id);
            }
        } else{
            throw NoSuchContextException(context);
        }
    }

    std::vector<IComponent*> EntityManager::GetAllComponents(const EntityManager::type_id context, const EntityManager::type_id entity_id){
        EntityManager* manager = EntityManager::getContext(context);
        if(manager != nullptr){
            if(EntityManager::EntityExists(context, entity_id)){
                // we have a vector of unique_ptrs, but need regular pointers.
                std::vector<std::unique_ptr<IComponent>>& theComponents = manager->entities[entity_id];
                std::vector<IComponent*> retval;
                retval.reserve(theComponents.size());
                for(auto itr = theComponents.begin(); itr != theComponents.end(); ++itr){
                    retval.push_back((*itr).get());
                }
                return retval;
            } else{
                throw NoSuchEntityException(entity_id);
            }
        } else{
            throw NoSuchContextException(context);
        }
    }

    EntityManager* EntityManager::getContext(const EntityManager::type_id context){
        if(0 <= context && context <= EntityManager::existing_contexts.size()){
            return EntityManager::existing_contexts[context].get();
        } else{
            return nullptr;
        }
    }
}
