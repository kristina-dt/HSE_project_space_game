#include <gtest/gtest.h>
#include "wallet.h"

TEST(WalletTest, ConstructorWithNegativeAmount) {
    wallet w(-50);
    EXPECT_EQ(w.getBal(), 0);
}