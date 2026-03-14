#include <gtest/gtest.h>
#include "ProductionManager.h"
#include "InformationPlayer.h"

class ProductionManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        player = std::make_unique<InformationPlayer>("TestPlayer", 0, 0);
        player->getWal().deposit(1000);
        manager = std::make_unique<ProductionManager>();
    }

    std::unique_ptr<InformationPlayer> player;
    std::unique_ptr<ProductionManager> manager;
};

TEST_F(ProductionManagerTest, Constructor) {
    EXPECT_EQ(manager->getCount(), 3);
}

TEST_F(ProductionManagerTest, GetAppliancePosition) {
    Position pos0 = manager->getAppliancePosition(0);
    EXPECT_EQ(pos0.x, 5);
    EXPECT_EQ(pos0.y, 10);

    Position pos1 = manager->getAppliancePosition(1);
    EXPECT_EQ(pos1.x, 8);
    EXPECT_EQ(pos1.y, 12);

    Position pos2 = manager->getAppliancePosition(2);
    EXPECT_EQ(pos2.x, 3);
    EXPECT_EQ(pos2.y, 7);

    Position invalid = manager->getAppliancePosition(5);
    EXPECT_EQ(invalid.x, -1);
    EXPECT_EQ(invalid.y, -1);
}

TEST_F(ProductionManagerTest, FindApplianceByType) {
    auto fuelMaker = manager->findApplianceByType("Fuel Synthesizer");
    EXPECT_NE(fuelMaker, nullptr);
    EXPECT_EQ(fuelMaker->getType(), "Fuel Synthesizer");

    auto partAssembler = manager->findApplianceByType("PartAssembler");
    EXPECT_NE(partAssembler, nullptr);
    EXPECT_EQ(partAssembler->getType(), "PartAssembler");

    auto foodmaker = manager->findApplianceByType("FoodAndDrinksStation");
    EXPECT_NE(foodmaker, nullptr);
    EXPECT_EQ(foodmaker->getType(), "FoodAndDrinksStation");
}

TEST_F(ProductionManagerTest, UpgradeAppliance) {
    auto fuelMaker = manager->findApplianceByType("Fuel Synthesizer");
    EXPECT_EQ(fuelMaker->getLevel(), 1);

    int initialBalance = player->getWal().getBal();
    int upgradeCost = fuelMaker->getUpgradeCost();

    manager->upgradeAppliance(0, *player);

    EXPECT_EQ(fuelMaker->getLevel(), 2);
    EXPECT_EQ(player->getWal().getBal(), initialBalance - upgradeCost);
}
TEST_F(ProductionManagerTest, UpgradeApplianceNotEnoughMoney) {
    int upgradeCost = manager->getAppliance(0)->getUpgradeCost();

    player->getWal().withdraw(player->getWal().getBal());
    player->getWal().deposit(upgradeCost - 1);

    auto fuelMaker = manager->findApplianceByType("Fuel Synthesizer");
    ASSERT_NE(fuelMaker, nullptr) << "Fuel Synthesizer not found";

    int initialLevel = fuelMaker->getLevel();
    int initialBalance = player->getWal().getBal();

    testing::internal::CaptureStdout();
    manager->upgradeAppliance(0, *player);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(fuelMaker->getLevel(), initialLevel);

    EXPECT_EQ(player->getWal().getBal(), initialBalance);

    EXPECT_NE(output.find("Upgrade failed"), std::string::npos);
    EXPECT_NE(output.find("You need " + std::to_string(upgradeCost) + " credits"), std::string::npos);
}




TEST_F(ProductionManagerTest, GetProductPrice) {
    EXPECT_EQ(manager->getProductPrice(0), 18);
    EXPECT_EQ(manager->getProductPrice(1), 25);;

    manager->upgradeAppliance(0, *player);
    EXPECT_EQ(manager->getProductPrice(0), 28);

    EXPECT_EQ(manager->getProductPrice(1), 25);

    EXPECT_EQ(manager->getProductPrice(2), 7);

    EXPECT_EQ(manager->getProductPrice(5), 0);
}

TEST_F(ProductionManagerTest, ShowAllAppliances) {
    testing::internal::CaptureStdout();
    manager->showAllAppliances();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("=== APPLIANCES ==="), std::string::npos);
    EXPECT_NE(output.find("0: Fuel Synthesizer"), std::string::npos);
    EXPECT_NE(output.find("1: PartAssembler"), std::string::npos);
    EXPECT_NE(output.find("2: FoodAndDrinksStation"), std::string::npos);
}