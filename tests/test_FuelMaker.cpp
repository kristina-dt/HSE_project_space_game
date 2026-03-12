#include <gtest/gtest.h>
#include "FuelMaker.h"
#include "InformationPlayer.h"

class FuelMakerTest : public ::testing::Test {
protected:
    void SetUp() override {
        player = std::make_unique<InformationPlayer>("TestPlayer", 0, 0);
        player->getWal().deposit(1000);
        fuelMaker = std::make_unique<FuelMaker>(15, 25);
    }

    std::unique_ptr<InformationPlayer> player;
    std::unique_ptr<FuelMaker> fuelMaker;
};

TEST_F(FuelMakerTest, Constructor) {
    EXPECT_EQ(fuelMaker->getName(), "Fuel Synthesizer");
    EXPECT_EQ(fuelMaker->getType(), "Fuel Synthesizer");
    EXPECT_EQ(fuelMaker->getPosition().x, 15);
    EXPECT_EQ(fuelMaker->getPosition().y, 25);
    EXPECT_EQ(fuelMaker->getLevel(), 1);
}

TEST_F(FuelMakerTest, CurrentPrice) {
    EXPECT_EQ(fuelMaker->getCurrentPrice(), 20);
    EXPECT_EQ(fuelMaker->getFuelPrice(), 20);

    fuelMaker->upgrade();
    EXPECT_EQ(fuelMaker->getCurrentPrice(), 30);
    EXPECT_EQ(fuelMaker->getFuelPrice(), 30);

    fuelMaker->upgrade();
    EXPECT_EQ(fuelMaker->getCurrentPrice(), 40);
    EXPECT_EQ(fuelMaker->getFuelPrice(), 40);
}

TEST_F(FuelMakerTest, UpgradeCost) {
    EXPECT_EQ(fuelMaker->getUpgradeCost(), 150);

    fuelMaker->upgrade();
    EXPECT_EQ(fuelMaker->getUpgradeCost(), 300);

    fuelMaker->upgrade();
    EXPECT_EQ(fuelMaker->getUpgradeCost(), 450);
}

TEST_F(FuelMakerTest, Produce) {
    int initialFuel = player->getAmountResource(Resource::Fuel);
    int initialBalance = player->getWal().getBal();

    testing::internal::CaptureStdout();
    bool result = fuelMaker->produce(*player);
    testing::internal::GetCapturedStdout();

    EXPECT_TRUE(result);
    EXPECT_EQ(player->getAmountResource(Resource::Fuel), initialFuel + 1);
    EXPECT_EQ(player->getWal().getBal(), initialBalance);
}

TEST_F(FuelMakerTest, ProduceAfterUpgrade) {
    fuelMaker->upgrade();

    int initialFuel = player->getAmountResource(Resource::Fuel);

    testing::internal::CaptureStdout();
    bool result = fuelMaker->produce(*player);
    testing::internal::GetCapturedStdout();

    EXPECT_TRUE(result);
    EXPECT_EQ(player->getAmountResource(Resource::Fuel), initialFuel + 1);
}

TEST_F(FuelMakerTest, UpgradeMethod) {
    EXPECT_EQ(fuelMaker->getLevel(), 1);

    testing::internal::CaptureStdout();
    fuelMaker->upgrade();
    testing::internal::GetCapturedStdout();

    EXPECT_EQ(fuelMaker->getLevel(), 2);
}

TEST_F(FuelMakerTest, CanInteract) {
    EXPECT_TRUE(fuelMaker->canInteract(15, 25));
    EXPECT_TRUE(fuelMaker->canInteract(17, 25, 2.0f));
    EXPECT_FALSE(fuelMaker->canInteract(18, 25, 2.0f));
}