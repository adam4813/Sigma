#pragma once

#include "EntityManager.h"
#include <string>

using Sigma::EntityManager;

namespace {
    // test context creation
	TEST(EntityManagerTest, EntityManagerContexts) {
        ASSERT_EQ(0, EntityManager::CurrentContext()) << "Default context not created";
        auto newctx = EntityManager::CreateContext();
        ASSERT_EQ(1, newctx);
        EntityManager::UseContext(newctx);
        ASSERT_EQ(newctx, EntityManager::CurrentContext()) << "Error switching contexts";
	}

	// test entity creation/deletion/exists
	TEST(EntityManagerTest, EntityManagerBasicEntities) {
        EntityManager::CreateEntity(0);
        EXPECT_FALSE(EntityManager::CreateEntity(1));
        EXPECT_TRUE(EntityManager::CreateEntity(1));
        // testing EntityExists
        ASSERT_TRUE(EntityManager::EntityExists(0)) << "Error creating entity 0";
        EXPECT_FALSE(EntityManager::EntityExists(2)) << "WTF is entity 2 doing here?";
        // testing deletion + return values
        EXPECT_TRUE(EntityManager::DeleteEntity(0));
        ASSERT_FALSE(EntityManager::EntityExists(0)) << "Error deleting entity 0";
        ASSERT_TRUE(EntityManager::EntityExists(1)) << "Entity 1 should still exist but doesnt";
        EXPECT_FALSE(EntityManager::DeleteEntity(2));
	}

	// test switching between contexts
	TEST(EntityManagerTest, EntityManagerContextSwitches){
        auto newctx = EntityManager::CreateContext();
        EntityManager::CreateEntity(0); // creates entity in context 0
        EntityManager::UseContext(newctx);
        EXPECT_FALSE(EntityManager::EntityExists(0)) << "Entity 0 should only exist in context 0";
        EXPECT_THROW(EntityManager::AddComponent(0, "", std::vector<Property>()), Sigma::NoSuchEntityException);
        EXPECT_THROW(EntityManager::UseContext(1000), Sigma::NoSuchContextException);
	}
}  // namespace
