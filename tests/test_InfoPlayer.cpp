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
    EXPECT_EQ(player.getAmountResource(Resource::Type::Fuel), 5);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Food), 7);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Drinks), 7);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Details), 5);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Decorations), 0);
}
TEST(InformationPlayerTest, SellResourceSu) {
    InformationPlayer player("TestPlayer", 0, 0);
    int initialFuel = player.getAmountResource(Resource::Type::Fuel);
    bool result = player.sell(Resource::Type::Fuel, 3);
    EXPECT_TRUE(result);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Fuel), initialFuel - 3);
}
TEST(InformationPlayerTest, SellResourceNotEnough) {
    InformationPlayer player("TestPlayer", 0, 0);
    int initialFuel = player.getAmountResource(Resource::Type::Fuel);
    bool result = player.sell(Resource::Type::Fuel, 10);
    EXPECT_FALSE(result);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Fuel), initialFuel);
}
TEST(InformationPlayerTest, SellAllResources) {
    InformationPlayer player("TestPlayer", 0, 0);
    bool result = player.sell(Resource::Type::Fuel, 5);
    EXPECT_TRUE(result);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Fuel), 0);
}
TEST(InformationPlayerTest, SetPosition) {
    InformationPlayer player("TestPlayer", 10.5f, 20.5f);
    EXPECT_EQ(player.getX(), 10.5f);
    EXPECT_EQ(player.getY(), 20.5f);
    player.SetPosition(30.5f, 40.5f);
    EXPECT_EQ(player.getX(), 30.5f);
    EXPECT_EQ(player.getY(), 40.5f);
}
TEST(InformationPlayerTest, GetWallet) {
    InformationPlayer player("TestPlayer", 0, 0);
    wallet& w = player.getWal();
    EXPECT_EQ(w.getBal(), 100);
    w.deposit(50);
    EXPECT_EQ(w.getBal(), 150);
    EXPECT_EQ(player.getWal().getBal(), 150);
}
TEST(InformationPlayerTest, ErrorChangeResources) {
    InformationPlayer player("TestPlayer", 0, 0);
    int bFuel = player.getAmountResource(Resource::Type::Fuel);
    int bFood = player.getAmountResource(Resource::Type::Food);
    player.sell(Resource::Type::Fuel, 100);
    player.sell(Resource::Type::Food, 100);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Fuel), bFuel);
    EXPECT_EQ(player.getAmountResource(Resource::Type::Food), bFood);
}