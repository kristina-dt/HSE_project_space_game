#include <gtest/gtest.h>
#include "Appliance.h"

TEST(PositionTest, Constructor) {
    Position p1, p2(3, -5);
    EXPECT_EQ(p1.x, 0); EXPECT_EQ(p1.y, 0);
    EXPECT_EQ(p2.x, 3); EXPECT_EQ(p2.y, -5);
}

TEST(PositionTest, IsNearby) {
    Position pos(5, 5);
    EXPECT_TRUE(pos.isNearby(5, 5));
    EXPECT_TRUE(pos.isNearby(6, 5));
    EXPECT_FALSE(pos.isNearby(8, 5, 2.0f));
    EXPECT_TRUE(pos.isNearby(10, 5, 5.0f));
}
