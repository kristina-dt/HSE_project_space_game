#include <gtest/gtest.h>
#include "Foodmaker.h"
#include "InformationPlayer.h"
#include "NotEnoughMoney.h"
#include <string>
#include <NotEnoughMoney.h>

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
    EXPECT_EQ(foodmaker->getCurrentPrice(), 7);

    foodmaker->setMode(FoodDrinksMode::Drinks);
    EXPECT_EQ(foodmaker->getCurrentPrice(), 7);

    foodmaker->upgrade();
    EXPECT_EQ(foodmaker->getCurrentPrice(), 15);
    foodmaker->setMode(FoodDrinksMode::Food);
    EXPECT_EQ(foodmaker->getCurrentPrice(), 15);

    foodmaker->upgrade();

    EXPECT_EQ(foodmaker->getCurrentPrice(), 23);
    foodmaker->setMode(FoodDrinksMode::Drinks);
    EXPECT_EQ(foodmaker->getCurrentPrice(), 23);

}

TEST_F(FoodmakerTest, UpgradeCost) {
    EXPECT_EQ(foodmaker->getUpgradeCost(), 75);
    foodmaker->upgrade();
    EXPECT_EQ(foodmaker->getUpgradeCost(), 150);
    foodmaker->upgrade();
    EXPECT_EQ(foodmaker->getUpgradeCost(), 225);
}

TEST_F(FoodmakerTest, ProduceFood) {
    int initialFood = player->getAmountResource(Resource::Type::Food);
    int initialBalance = player->getWal().getBal();

    int currentPrice = foodmaker->getCurrentPrice();
    int expectedPrice = static_cast<int>(currentPrice * 0.7);

    testing::internal::CaptureStdout();
    bool result = foodmaker->produce(*player);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(result);
    EXPECT_EQ(player->getAmountResource(Resource::Type::Food), initialFood + 1);
    EXPECT_EQ(player->getWal().getBal(), initialBalance - expectedPrice);
    EXPECT_NE(output.find("produced 1 Food"), std::string::npos);
}

TEST_F(FoodmakerTest, ProduceDrinks) {
    foodmaker->setMode(FoodDrinksMode::Drinks);
    
    int initialDrinks = player->getAmountResource(Resource::Type::Drinks);
    int initialBalance = player->getWal().getBal();

    int currentPrice = foodmaker->getCurrentPrice();
    int expectedPrice = static_cast<int>(currentPrice * 0.7);

    testing::internal::CaptureStdout();
    bool result = foodmaker->produce(*player);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(result);
    EXPECT_EQ(player->getAmountResource(Resource::Type::Drinks), initialDrinks + 1);
    EXPECT_EQ(player->getWal().getBal(), initialBalance - expectedPrice);
    EXPECT_NE(output.find("produced 1 Drinks"), std::string::npos);
}
TEST_F(FoodmakerTest, ShowAvailableModes) {
    testing::internal::CaptureStdout();
    Foodmaker::showAvailableModes();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("=== Available Modes ==="), std::string::npos);
    EXPECT_NE(output.find("0. Food"), std::string::npos);
    EXPECT_NE(output.find("1. Drinks"), std::string::npos);
}