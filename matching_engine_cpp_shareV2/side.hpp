
#ifndef SIDE_HPP
#define SIDE_HPP

#include "pricelevel.hpp"
#include <map>
#include <vector>
#include <memory>  // for std::shared_ptr
#include <unordered_map> 


class Side {
public:
    Side();

    // Getters
    PriceLevel* getBestPriceLevel() const;
    double getBestPrice() const;
    int getBestVolume() const;
    PriceLevel* getPriceLevel(double price) const;

    // Methods
    void addOrder(std::shared_ptr<Order> order);
    void removeOrder(std::shared_ptr<Order> order);
    void removePriceLevel(double price);
    std::vector<double> getBestNPrices(int nLevels) const;
    
    bool determinePriceLevel() const;

    // Abstract Method to Determine Priority
    virtual bool price1HasHigherPriorityThanPrice2(double price1, double price2) const = 0;

protected:
    PriceLevel* newPriceLevel(std::shared_ptr<Order> order);

public:
    // 价格
    std::unordered_map<double, PriceLevel*> book;            // Price level book
    PriceLevel* headPriceLevel;                    // Pointer to the highest priority PriceLevel
    PriceLevel* tailPriceLevel;                    // Pointer to the lowest priority PriceLevel
};



#endif // SIDE_HPP
