
#include "../include/wallet.h"
#include <string>
#include "../include/NotEnoughMoney.h"
wallet::wallet(int initialAmount) noexcept : balance(initialAmount) {
    if (initialAmount<0) {
        balance = 0;
    }
}
void wallet::withdraw(int amount) {
    if (amount > balance) {
        throw NotEnoughMoney( "Not enough money. Need:" + std::to_string(amount) +
            ", available: " + std::to_string(balance));
    }
    balance -= amount;
}
void wallet::deposit(int amount) {
    balance+=amount;
}
int wallet::getBal() const {
    return balance;
}