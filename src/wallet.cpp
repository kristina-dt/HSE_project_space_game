
#include "../include/wallet.h"

wallet::wallet(int initialAmount) noexcept : balance(initialAmount) {
    if (initialAmount<0) {
        balance = 0;
    }
}