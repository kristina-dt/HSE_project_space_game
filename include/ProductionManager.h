#pragma once

#include "Appliance.h"
#include "FuelMaker.h"
#include "PartAssembler.h"
#include "FoodMaker.h"
#include "InformationPlayer.h"
#include <vector>
#include <memory>
#include <iostream>
#include <optional>

class ProductionManager {
private:
    std::vector<std::shared_ptr<Appliance>> appliances_;

public:
    ProductionManager();
    std::shared_ptr<Appliance> getAppliance(int index);
    size_t getCount() const;
    void showAllAppliances() const;
    void produceAll(InformationPlayer& player);
    void produceAt(size_t index, InformationPlayer& player);
    void upgradeAppliance(size_t index, InformationPlayer& player);
    void configurePartAssembler(int index, int modeIndex);
    void configureFoodDrinksStation(int index, int modeIndex);

    Position getAppliancePosition(int index) const;
    std::string getProductName(int index) const;
    int getProductPrice(int index) const;

    std::shared_ptr<Appliance> findApplianceByType(const std::string& type);
};