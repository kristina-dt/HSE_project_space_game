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

TEST_F(ProductionManagerTest, ProduceAt) {
    int initialFuel = player->getAmountResource(Resource::Fuel);
    manager->produceAt(0, *player);
    EXPECT_EQ(player->getAmountResource(Resource::Fuel), initialFuel + 1);

    int initialDetails = player->getAmountResource(Resource::Details);
    manager->produceAt(1, *player);
    EXPECT_EQ(player->getAmountResource(Resource::Details), initialDetails + 1);

    int initialFood = player->getAmountResource(Resource::Food);
    manager->produceAt(2, *player);
    EXPECT_EQ(player->getAmountResource(Resource::Food), initialFood + 1);
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
    auto fuelMaker = manager->findApplianceByType("FuelMaker");
    EXPECT_EQ(fuelMaker->getLevel(), 1);

    int initialBalance = player->getWal().getBal();
    int upgradeCost = fuelMaker->getUpgradeCost();

    manager->upgradeAppliance(0, *player);

    EXPECT_EQ(fuelMaker->getLevel(), 2);
    EXPECT_EQ(player->getWal().getBal(), initialBalance - upgradeCost);
}

TEST_F(ProductionManagerTest, UpgradeApplianceNotEnoughMoney) {
    player->getWal().withdraw(1000);

    auto fuelMaker = manager->findApplianceByType("FuelMaker");
    EXPECT_EQ(fuelMaker->getLevel(), 1);

    int initialBalance = player->getWal().getBal();
    manager->upgradeAppliance(0, *player);

    EXPECT_EQ(fuelMaker->getLevel(), 1);
    EXPECT_EQ(player->getWal().getBal(), initialBalance);
}

TEST_F(ProductionManagerTest, ConfigurePartAssembler) {
    manager->configurePartAssembler(1, 1);

    auto partAssembler = std::dynamic_pointer_cast<PartAssembler>(
        manager->findApplianceByType("PartAssembler"));
    EXPECT_EQ(partAssembler->getMode(), PartAssemblerMode::Decorations);

    manager->configurePartAssembler(1, 0);
    EXPECT_EQ(partAssembler->getMode(), PartAssemblerMode::Details);
}

TEST_F(ProductionManagerTest, ConfigureFoodDrinksStation) {
    manager->configureFoodDrinksStation(2, 1);

    auto foodmaker = std::dynamic_pointer_cast<Foodmaker>(
        manager->findApplianceByType("Foodmaker"));
    EXPECT_EQ(foodmaker->getMode(), FoodDrinksMode::Drinks);

    manager->configureFoodDrinksStation(2, 0);
    EXPECT_EQ(foodmaker->getMode(), FoodDrinksMode::Food);
}

TEST_F(ProductionManagerTest, GetProductName) {
    EXPECT_EQ(manager->getProductName(0), "Fuel");
    EXPECT_EQ(manager->getProductName(1), "Details");

    std::string defaultFood = manager->getProductName(2);
    EXPECT_TRUE(defaultFood == "Food" || defaultFood == "Drinks");

    manager->configurePartAssembler(1, 1);
    EXPECT_EQ(manager->getProductName(1), "Decorations");

    manager->configureFoodDrinksStation(2, 1);
    EXPECT_EQ(manager->getProductName(2), "Drinks");

    EXPECT_EQ(manager->getProductName(5), "Unknown");
}

TEST_F(ProductionManagerTest, GetProductPrice) {
    EXPECT_EQ(manager->getProductPrice(0), 18);
    EXPECT_EQ(manager->getProductPrice(1), 35);

    int foodPrice = manager->getProductPrice(2);
    EXPECT_TRUE(foodPrice == 7 || foodPrice == 10);

    manager->upgradeAppliance(0, *player);
    EXPECT_EQ(manager->getProductPrice(0), 27);

    manager->configurePartAssembler(1, 1);
    EXPECT_EQ(manager->getProductPrice(1), 50);

    manager->configureFoodDrinksStation(2, 1);
    EXPECT_EQ(manager->getProductPrice(2), 10);

    EXPECT_EQ(manager->getProductPrice(5), 0);
}

TEST_F(ProductionManagerTest, ShowAllAppliances) {
    testing::internal::CaptureStdout();
    manager->showAllAppliances();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("=== APPLIANCES ==="), std::string::npos);
    EXPECT_NE(output.find("0: FuelMaker"), std::string::npos);
    EXPECT_NE(output.find("1: PartAssembler"), std::string::npos);
    EXPECT_NE(output.find("2: Foodmaker"), std::string::npos);
}