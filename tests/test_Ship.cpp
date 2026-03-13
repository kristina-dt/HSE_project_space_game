//
// Created by Greta Yugay on 12.03.2026.
//
#include "Ship.h"
#include "ResourceOrder.h"
#include <gtest/gtest.h>

TEST(ShipTest, DefaultConstructorSetsExpectedValues) {
    Ship ship;

    EXPECT_FLOAT_EQ(ship.getX(), Ship::START_X);
    EXPECT_FLOAT_EQ(ship.getY(), 5.0f);
    EXPECT_FLOAT_EQ(ship.getSpeed(), Ship::DEFAULT_SPEED);
    EXPECT_TRUE(ship.isActive());
    EXPECT_FALSE(ship.hasOrder());
    EXPECT_EQ(ship.getOrder(), nullptr);
    EXPECT_EQ(ship.getOrderInfo(), "No order");
}

TEST(ShipTest, CustomConstructorWithoutOrderSetsValues) {
    Ship ship(100.0f, 12.0f, 3.5f);

    EXPECT_FLOAT_EQ(ship.getX(), 100.0f);
    EXPECT_FLOAT_EQ(ship.getY(), 12.0f);
    EXPECT_FLOAT_EQ(ship.getSpeed(), 3.5f);
    EXPECT_TRUE(ship.isActive());
    EXPECT_FALSE(ship.hasOrder());
}

TEST(ShipTest, ConstructorWithOrderStoresOrder) {
    auto order = std::make_unique<ResourceOrder>(Resource::Food, 6, 80);
    Ship ship(100.0f, 10.0f, 2.0f, std::move(order));

    EXPECT_TRUE(ship.hasOrder());
    ASSERT_NE(ship.getOrder(), nullptr);
    EXPECT_EQ(ship.getOrder()->resourceType(), Resource::Food);
    EXPECT_EQ(ship.getOrder()->amount(), 6);
    EXPECT_EQ(ship.getOrder()->rewardMoney(), 80);
}

TEST(ShipTest, SetOrderAddsOrderToShip) {
    Ship ship;

    ship.setOrder(std::make_unique<ResourceOrder>(Resource::Fuel, 3, 100));

    EXPECT_TRUE(ship.hasOrder());
    ASSERT_NE(ship.getOrder(), nullptr);
    EXPECT_EQ(ship.getOrder()->resourceType(), Resource::Fuel);
    EXPECT_EQ(ship.getOrder()->amount(), 3);
}

TEST(ShipTest, ClearOrderRemovesOrder) {
    Ship ship;
    ship.setOrder(std::make_unique<ResourceOrder>(Resource::Drinks, 2, 70));

    ASSERT_TRUE(ship.hasOrder());

    ship.clearOrder();

    EXPECT_FALSE(ship.hasOrder());
    EXPECT_EQ(ship.getOrder(), nullptr);
    EXPECT_EQ(ship.getOrderInfo(), "No order");
}

TEST(ShipTest, GetOrderInfoReturnsDescriptionWhenOrderExists) {
    Ship ship;
    ship.setOrder(std::make_unique<ResourceOrder>(Resource::Fuel, 4, 120));

    EXPECT_EQ(
        ship.getOrderInfo(),
        "[Regular] Need 4 units of Fuel. Reward: 120 credits."
    );
}

TEST(ShipTest, CompleteOrderAndGetRewardReturnsRewardAndClearsOrder) {
    Ship ship;
    ship.setOrder(std::make_unique<ResourceOrder>(Resource::Details, 5, 140));

    int reward = ship.completeOrderAndGetReward();

    EXPECT_EQ(reward, 140);
    EXPECT_FALSE(ship.hasOrder());
    EXPECT_EQ(ship.getOrder(), nullptr);
}

TEST(ShipTest, CompleteOrderAndGetRewardReturnsZeroWhenNoOrder) {
    Ship ship;

    int reward = ship.completeOrderAndGetReward();

    EXPECT_EQ(reward, 0);
    EXPECT_FALSE(ship.hasOrder());
}

TEST(ShipTest, UpdateWithOrderMovesShipTowardDockAndStation) {
    Ship ship(100.0f, 0.0f, 2.0f);
    ship.setOrder(std::make_unique<ResourceOrder>(Resource::Food, 1, 50));

    ship.update(10.0f);

    EXPECT_FLOAT_EQ(ship.getX(), 98.0f);
    EXPECT_FLOAT_EQ(ship.getY(), 2.0f);
}

TEST(ShipTest, UpdateWithoutOrderMovesShipToRightInitially) {
    Ship ship(0.0f, 5.0f, 2.0f);

    ship.update(10.0f);

    EXPECT_FLOAT_EQ(ship.getX(), 2.0f);
    EXPECT_FLOAT_EQ(ship.getY(), 5.0f);
}

TEST(ShipTest, IsDockedReturnsTrueWhenAtOrBeforeDockX) {
    Ship shipAtDock(Ship::DOCK_X, 5.0f, 2.0f);
    Ship shipBeforeDock(Ship::DOCK_X - 10.0f, 5.0f, 2.0f);
    Ship shipAfterDock(Ship::DOCK_X + 10.0f, 5.0f, 2.0f);

    EXPECT_TRUE(shipAtDock.isDocked());
    EXPECT_TRUE(shipBeforeDock.isDocked());
    EXPECT_FALSE(shipAfterDock.isDocked());
}