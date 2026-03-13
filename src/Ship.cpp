#include "../include/Ship.h"

Ship::Ship()
    : x_(START_X),
      y_(5.0f),
      speed_(DEFAULT_SPEED),
      active_(true),
      order_(std::nullopt) {}

Ship::Ship(float startX, float y, float speed)
    : x_(startX),
      y_(y),
      speed_(speed),
      active_(true),
      order_(std::nullopt) {}

Ship::Ship(float startX, float y, float speed, std::unique_ptr<Order> order)
    : x_(startX),
      y_(y),
      speed_(speed),
      active_(true),
      order_(std::move(order)) {}

void Ship::update(float station) {
    if (!active_) return;

    if (std::abs(x_ - station) > speed_) {
        x_ += (x_ < station) ? speed_ : -speed_;
    } else {
        x_ = station;
    }

    if (hasOrder()) {
        if (std::abs(y_ - DOCK_X) > speed_) {
            y_ += (y_ < DOCK_X) ? speed_ : -speed_;
        } else {
            y_ = DOCK_X;
        }
    } else {
        float cruiseY = 5.0f;
        if (std::abs(y_ - cruiseY) > speed_) {
            y_ += (y_ < cruiseY) ? speed_ : -speed_;
        } else {
            y_ = cruiseY;
        }
    }
}

bool Ship::isDocked() const noexcept {
    return std::abs(y_ - DOCK_X) < 0.1f;
}

bool Ship::hasOrder() const noexcept {
    return order_.has_value() && static_cast<bool>(*order_);
}

bool Ship::isActive() const noexcept {
    return active_;
}

float Ship::getX() const noexcept {
    return x_;
}

float Ship::getY() const noexcept {
    return y_;
}

float Ship::getSpeed() const noexcept {
    return speed_;
}

std::string Ship::getOrderInfo() const {
    if (!hasOrder()) {
        return "No order";
    }
    return (*order_)->describe();
}

Order* Ship::getOrder() const noexcept {
    if (!hasOrder()) {
        return nullptr;
    }
    return order_->get();
}

void Ship::setOrder(std::unique_ptr<Order> order) {
    order_ = std::move(order);
}

void Ship::clearOrder() {
    order_ = std::nullopt;
}

int Ship::completeOrderAndGetReward() {
    if (!hasOrder()) {
        return 0;
    }

    int reward = (*order_)->rewardMoney();
    clearOrder();
    return reward;
}