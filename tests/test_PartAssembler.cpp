#include <gtest/gtest.h>
#include "PartAssembler.h"
#include "InformationPlayer.h"

class PartAssemblerTest : public ::testing::Test {
protected:
    void SetUp() override {
        player = std::make_unique<InformationPlayer>("TestPlayer", 0, 0);
        player->getWal().deposit(1000);
        assembler = std::make_unique<PartAssembler>(5, 15);
    }

    std::unique_ptr<InformationPlayer> player;
    std::unique_ptr<PartAssembler> assembler;
};

TEST_F(PartAssemblerTest, Constructor) {
    EXPECT_EQ(assembler->getName(), "PartAssembler");
    EXPECT_EQ(assembler->getType(), "PartAssembler");
    EXPECT_EQ(assembler->getPosition().x, 5);
    EXPECT_EQ(assembler->getPosition().y, 15);
    EXPECT_EQ(assembler->getLevel(), 1);
    EXPECT_EQ(assembler->getMode(), PartAssemblerMode::Details);
}

TEST_F(PartAssemblerTest, ModeMethods) {
    EXPECT_EQ(assembler->getMode(), PartAssemblerMode::Details);
    EXPECT_EQ(assembler->getModeName(), "Details");

    assembler->setMode(PartAssemblerMode::Decorations);

    EXPECT_EQ(assembler->getMode(), PartAssemblerMode::Decorations);
    EXPECT_EQ(assembler->getModeName(), "Decorations");
}

TEST_F(PartAssemblerTest, CurrentPrice) {
    EXPECT_EQ(assembler->getCurrentPrice(), 20);

    assembler->setMode(PartAssemblerMode::Decorations);
    EXPECT_EQ(assembler->getCurrentPrice(), 45);

    assembler->upgrade();
    EXPECT_EQ(assembler->getCurrentPrice(), 70);
}

TEST_F(PartAssemblerTest, UpgradeCost) {
    EXPECT_EQ(assembler->getUpgradeCost(), 50);

    assembler->upgrade();
    EXPECT_EQ(assembler->getUpgradeCost(), 100);
}

TEST_F(PartAssemblerTest, ProduceDetails) {
    int initialDetails = player->getAmountResource(Resource::Type::Details);
    int initialBalance = player->getWal().getBal();

    bool result = assembler->produce(*player);

    EXPECT_TRUE(result);
    EXPECT_EQ(player->getAmountResource(Resource::Type::Details), initialDetails + 1);
    EXPECT_EQ(player->getWal().getBal(), initialBalance);
}

TEST_F(PartAssemblerTest, ProduceDecorations) {
    assembler->setMode(PartAssemblerMode::Decorations);

    int initialDecorations = player->getAmountResource(Resource::Type::Decorations);
    int initialBalance = player->getWal().getBal();

    bool result = assembler->produce(*player);

    EXPECT_TRUE(result);
    EXPECT_EQ(player->getAmountResource(Resource::Type::Decorations), initialDecorations + 1);
    EXPECT_EQ(player->getWal().getBal(), initialBalance);
}

TEST_F(PartAssemblerTest, ProduceAfterUpgrade) {
    assembler->upgrade();

    int initialDetails = player->getAmountResource(Resource::Type::Details);

    bool result = assembler->produce(*player);

    EXPECT_TRUE(result);
    EXPECT_EQ(player->getAmountResource(Resource::Type::Details), initialDetails + 1);
}

TEST_F(PartAssemblerTest, GetResourceTypeReturnsCorrectType) {
    assembler->setMode(PartAssemblerMode::Details);
    EXPECT_EQ(assembler->getResourceType(), Resource::Type::Details);

    assembler->setMode(PartAssemblerMode::Decorations);
    EXPECT_EQ(assembler->getResourceType(), Resource::Type::Decorations);
}

TEST_F(PartAssemblerTest, ShowAvailableModes) {
    testing::internal::CaptureStdout();
    PartAssembler::showAvailableModes();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("=== Available Modes ==="), std::string::npos);
    EXPECT_NE(output.find("0. Details"), std::string::npos);
    EXPECT_NE(output.find("1. Decorations"), std::string::npos);
}

TEST_F(PartAssemblerTest, UpgradeMethod) {
    EXPECT_EQ(assembler->getLevel(), 1);

    testing::internal::CaptureStdout();
    assembler->upgrade();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(assembler->getLevel(), 2);
    EXPECT_EQ(output, "PartAssembler upgraded to level 2!\n");
}

TEST_F(PartAssemblerTest, CanInteract) {
    EXPECT_TRUE(assembler->canInteract(5, 15));
    EXPECT_TRUE(assembler->canInteract(7, 15, 2.0f));
    EXPECT_FALSE(assembler->canInteract(8, 15, 2.0f));
}