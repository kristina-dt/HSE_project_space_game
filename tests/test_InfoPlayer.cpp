#include <gtest/gtest.h>
#include <InformationPlayer.h>

TEST(InformationPlayerTest, Constructor) {
    InformationPlayer player("TestPlayer", 10.5f, 20.5f);
    EXPECT_EQ(player.getName(), "TestPlayer");
    EXPECT_EQ(player.getX(), 10.5f);
    EXPECT_EQ(player.getY(), 20.5f);
    EXPECT_EQ(player.getWal().getBal(), 100);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Fuel), 5);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Food), 7);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Drinks), 7);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Details), 5);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Decorations), 0);
}
TEST(InformationPlayerTest, ConstructorDefault) {
    InformationPlayer player("TestPlayer", 2.0f, 3.0f);
    EXPECT_EQ(player.getX(), 2.0f);
    EXPECT_EQ(player.getY(), 3.0f);
}
TEST(InformationPlayerTest, SellPrices) {
    InformationPlayer player("TestPlayer", 0, 0);
    EXPECT_EQ(player.getSellPrice(Resource::Type::Fuel), 18);
    EXPECT_EQ(player.getSellPrice(Resource::Type::Food), 7);
    EXPECT_EQ(player.getSellPrice(Resource::Type::Drinks), 7);
    EXPECT_EQ(player.getSellPrice(Resource::Type::Details), 20);
    EXPECT_EQ(player.getSellPrice(Resource::Type::Decorations), 45);
}
TEST(InformationPlayerTest, AddResource) {
    InformationPlayer player("TestPlayer", 0, 0);
    player.addResource(Resource::Type::Fuel, 3);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Fuel), 8);
    player.addResource(Resource::Type::Fuel, 2);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Fuel), 10);
    player.addResource(Resource::Type::Food, 5);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Food), 12);
}
TEST(InformationPlayerTest, GetResourceAmount) {
    InformationPlayer player("TestPlayer", 0, 0);
    EXPECT_EQ(player.getResourceAmount(Resource::Type::Fuel), 5);
    EXPECT_EQ(player.getResourceAmount(Resource::Type::Food), 7);
    EXPECT_EQ(player.getResourceAmount(Resource::Type::Drinks), 7);
    EXPECT_EQ(player.getResourceAmount(Resource::Type::Details), 5);
    EXPECT_EQ(player.getResourceAmount(Resource::Type::Decorations), 0);
}
TEST(InformationPlayerTest, SellResourceSu) {
    InformationPlayer player("TestPlayer", 0, 0);
    int initialFuel = player.getAmountResource(Resource::Type::Fuel);
    bool result = player.sell(Resource::Type::Fuel, 3);
    EXPECT_TRUE(result);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Fuel), initialFuel - 3);
}
