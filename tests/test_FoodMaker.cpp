#include <gtest/gtest.h>
#include "Foodmaker.h"
#include "InformationPlayer.h"

class FoodmakerTest : public ::testing::Test {
protected:
    void SetUp() override {
        player = std::make_unique<InformationPlayer>("TestPlayer", 0, 0);
        player->getWal().deposit(1000);
        foodmaker = std::make_unique<Foodmaker>(10, 20);
    }
    
    std::unique_ptr<InformationPlayer> player;
    std::unique_ptr<Foodmaker> foodmaker;
};

TEST_F(FoodmakerTest, Constructor) {
    EXPECT_EQ(foodmaker->getName(), "Food & Drinks Station");
    EXPECT_EQ(foodmaker->getType(), "FoodAndDrinksStation");
    EXPECT_EQ(foodmaker->getPosition().x, 10);
    EXPECT_EQ(foodmaker->getPosition().y, 20);
    EXPECT_EQ(foodmaker->getLevel(), 1);
}

TEST_F(FoodmakerTest, ModeMethods) {
    EXPECT_EQ(foodmaker->getMode(), FoodDrinksMode::Food);
    EXPECT_EQ(foodmaker->getModeName(), "Food");
    EXPECT_EQ(foodmaker->getResourceType(), Resource::Type::Food);

    testing::internal::CaptureStdout();
    foodmaker->setMode(FoodDrinksMode::Drinks);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(foodmaker->getMode(), FoodDrinksMode::Drinks);
    EXPECT_EQ(foodmaker->getModeName(), "Drinks");
    EXPECT_EQ(foodmaker->getResourceType(), Resource::Type::Drinks);
    EXPECT_EQ(output, "Food & Drinks Station switched to: Drinks\n");
}

TEST_F(FoodmakerTest, CurrentPrice) {
    EXPECT_EQ(foodmaker->getCurrentPrice(), 30);
    foodmaker->setMode(FoodDrinksMode::Drinks);
    EXPECT_EQ(foodmaker->getCurrentPrice(), 20);

    foodmaker->upgrade();
    EXPECT_EQ(foodmaker->getCurrentPrice(), 42);
    
    foodmaker->setMode(FoodDrinksMode::Drinks);
    EXPECT_EQ(foodmaker->getCurrentPrice(), 28);

    foodmaker->upgrade();
    EXPECT_EQ(foodmaker->getCurrentPrice(), 54);
}

TEST_F(FoodmakerTest, UpgradeCost) {
    EXPECT_EQ(foodmaker->getUpgradeCost(), 180);
    foodmaker->upgrade();
    EXPECT_EQ(foodmaker->getUpgradeCost(), 360);
    foodmaker->upgrade();
    EXPECT_EQ(foodmaker->getUpgradeCost(), 540);
}

TEST_F(FoodmakerTest, ProduceFood) {
    int initialFood = player->getAmountResource(Resource::Type::Food);
    int initialBalance = player->getWal().getBal();
    
    testing::internal::CaptureStdout();
    bool result = foodmaker->produce(*player);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(result);
    EXPECT_EQ(player->getAmountResource(Resource::Type::Food), initialFood + 1);
    EXPECT_EQ(player->getWal().getBal(), initialBalance - 30);
    EXPECT_THAT(output, testing::HasSubstr("produced 1 Food"));
}

TEST_F(FoodmakerTest, ProduceDrinks) {
    foodmaker->setMode(FoodDrinksMode::Drinks);
    
    int initialDrinks = player->getAmountResource(Resource::Type::Drinks);
    int initialBalance = player->getWal().getBal();
    
    testing::internal::CaptureStdout();
    bool result = foodmaker->produce(*player);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(result);
    EXPECT_EQ(player->getAmountResource(Resource::Type::Drinks), initialDrinks + 1);
    EXPECT_EQ(player->getWal().getBal(), initialBalance - 20);
    EXPECT_THAT(output, testing::HasSubstr("produced 1 Drinks"));
}

TEST_F(FoodmakerTest, ProduceNotEnoughMoney) {
    player->getWal().withdraw(1000);
    
    int initialFood = player->getAmountResource(Resource::Type::Food);
    int initialBalance = player->getWal().getBal();
    
    testing::internal::CaptureStdout();
    bool result = foodmaker->produce(*player);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_FALSE(result);
    EXPECT_EQ(player->getAmountResource(Resource::Type::Food), initialFood);
    EXPECT_EQ(player->getWal().getBal(), initialBalance);
    EXPECT_THAT(output, testing::HasSubstr("Not enough money!"));
}

TEST_F(FoodmakerTest, ShowAvailableModes) {
    testing::internal::CaptureStdout();
    Foodmaker::showAvailableModes();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_THAT(output, testing::HasSubstr("=== Available Modes ==="));
    EXPECT_THAT(output, testing::HasSubstr("0. Food"));
    EXPECT_THAT(output, testing::HasSubstr("1. Drinks"));
}