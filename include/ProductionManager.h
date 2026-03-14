#pragma once

#include "Appliance.h"
#include "FuelMaker.h"
#include "PartAssembler.h"
#include "Foodmaker.h"
#include "InformationPlayer.h"
#include <vector>
#include <memory>
#include <iostream>
#include <optional>
#include <variant>
using ApplianceVariant = std::variant<
    FuelMaker,
    PartAssembler,
    Foodmaker
>;
class ProductionManager {
private:
    std::vector<std::shared_ptr<Appliance>> appliances_;
    std::vector<ApplianceVariant> applianceVariants_;

public:
    ProductionManager();
    std::shared_ptr<Appliance> getAppliance(int index);
    size_t getCount() const;
    void showAllAppliances() const;
    void upgradeAppliance(size_t index, InformationPlayer& player);

    Position getAppliancePosition(int index) const;
    std::string getProductName(int index) const;
    int getProductPrice(int index) const;

    std::shared_ptr<Appliance> findApplianceByType(const std::string& type);
};