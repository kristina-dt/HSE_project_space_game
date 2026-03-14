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

TEST_F(ProductionManagerTest, ProduceAll) {
    int initialFuel = player->getAmountResource(Resource::Fuel);
    int initialDetails = player->getAmountResource(Resource::Details);
    int initialFood = player->getAmountResource(Resource::Food);

    manager->produceAll(*player);

    EXPECT_EQ(player->getAmountResource(Resource::Fuel), initialFuel + 1);
    EXPECT_EQ(player->getAmountResource(Resource::Details), initialDetails + 1);
    EXPECT_EQ(player->getAmountResource(Resource::Food), initialFood + 1);
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

TEST_F(ProductionManagerTest, ConfigurePartAssembler) {
    manager->configurePartAssembler(1, 1);

    PartAssembler* partAssembler = static_cast<PartAssembler*>(manager->getAppliance(1).get());
    EXPECT_EQ(partAssembler->getMode(), PartAssemblerMode::Decorations);

    manager->configurePartAssembler(1, 0);
    EXPECT_EQ(partAssembler->getMode(), PartAssemblerMode::Details);
}

TEST_F(ProductionManagerTest, ConfigureFoodDrinksStation) {
    auto appliance = manager->getAppliance(2);
    ASSERT_NE(appliance, nullptr);
    ASSERT_EQ(appliance->getType(), "FoodAndDrinksStation");
    Foodmaker* foodmaker = static_cast<Foodmaker*>(appliance.get());
    FoodDrinksMode initialMode = foodmaker->getMode();

    testing::internal::CaptureStdout();
    manager->configureFoodDrinksStation(2, 1);
    testing::internal::GetCapturedStdout();

    EXPECT_EQ(foodmaker->getMode(), FoodDrinksMode::Drinks);
    testing::internal::CaptureStdout();
    manager->configureFoodDrinksStation(2, 0);
    testing::internal::GetCapturedStdout();

    EXPECT_EQ(foodmaker->getMode(), FoodDrinksMode::Food);
}

TEST_F(ProductionManagerTest, GetProductName) {
    EXPECT_EQ(manager->getProductName(0), "Fuel");
    EXPECT_EQ(manager->getProductName(1), "Details");
    std::string defaultFood = manager->getProductName(2);
    EXPECT_TRUE(defaultFood == "Food" || defaultFood == "Drinks")
        << "Expected 'Food' or 'Drinks', got '" << defaultFood << "'";
    testing::internal::CaptureStdout();
    manager->configurePartAssembler(1, 1);
    testing::internal::GetCapturedStdout();
    EXPECT_EQ(manager->getProductName(1), "Decorations");
    testing::internal::CaptureStdout();
    manager->configureFoodDrinksStation(2, 1);
    testing::internal::GetCapturedStdout();
    EXPECT_EQ(manager->getProductName(2), "Drinks");
    EXPECT_EQ(manager->getProductName(5), "Unknown");
    EXPECT_EQ(manager->getProductName(-1), "Unknown");
}

TEST_F(ProductionManagerTest, GetProductPrice) {
    EXPECT_EQ(manager->getProductPrice(0), 18);
    EXPECT_EQ(manager->getProductPrice(1), 25);;

    manager->upgradeAppliance(0, *player);
    EXPECT_EQ(manager->getProductPrice(0), 28);

    manager->configurePartAssembler(1, 1);
    EXPECT_EQ(manager->getProductPrice(1), 25);

    manager->configureFoodDrinksStation(2, 1);
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