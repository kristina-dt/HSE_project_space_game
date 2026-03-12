//
// Created by Greta Yugay on 12.03.2026.
//
#include <gtest/gtest.h>
#include "ResourceOrder.h"
TEST(ResourceOrderTest, ConstructorStoresValuesCorrectly) {
    ResourceOrder order(Resource::Fuel, 5, 120);

    EXPECT_EQ(order.resourceType(), Resource::Fuel);
    EXPECT_EQ(order.amount(), 5);
    EXPECT_EQ(order.rewardMoney(), 120);
}

TEST(ResourceOrderTest, TypeNameReturnsRegular) {
    ResourceOrder order(Resource::Food, 3, 50);

    EXPECT_EQ(order.typeName(), "Regular");
}

TEST(ResourceOrderTest, DescribeReturnsExpectedStringForFuel) {
    ResourceOrder order(Resource::Fuel, 4, 200);

    EXPECT_EQ(
        order.describe(),
        "[Regular] Need 4 units of Fuel. Reward: 200 credits."
    );
}

TEST(ResourceOrderTest, DescribeReturnsExpectedStringForDecorations) {
    ResourceOrder order(Resource::Decorations, 2, 90);

    EXPECT_EQ(
        order.describe(),
        "[Regular] Need 2 units of Decorations. Reward: 90 credits."
    );
}

TEST(ResourceOrderTest, CloneCreatesIndependentCopy) {
    ResourceOrder original(Resource::Drinks, 7, 150);

    std::unique_ptr<Order> cloned = original.clone();

    ASSERT_NE(cloned, nullptr);
    EXPECT_EQ(cloned->typeName(), original.typeName());
    EXPECT_EQ(cloned->describe(), original.describe());
    EXPECT_EQ(cloned->rewardMoney(), original.rewardMoney());
    EXPECT_EQ(cloned->resourceType(), original.resourceType());
    EXPECT_EQ(cloned->amount(), original.amount());
}