#ifndef PRICELEVEL_HPP
#define PRICELEVEL_HPP

#include "order.hpp"
#include <memory>  // for std::shared_ptr


class PriceLevel {
public:
    PriceLevel(std::shared_ptr<Order> order);  // Constructor accepting shared_ptr

    int getVolume();// const;
    bool isEmpty();// const;

    bool findOrder(std::shared_ptr<Order> order) const;
    void append(std::shared_ptr<Order> order);
    void pop();
    void popSecondOrFirst();
    void remove(std::shared_ptr<Order> order);
    std::shared_ptr<Order> getFirstOrSecondOrder();
    void setHeadOrder(std::shared_ptr<Order> order);

    PriceLevel* getPrev() ;//const;
    void setPrev(PriceLevel* prevLevel);
    PriceLevel* getNext();// const;
    void setNext(PriceLevel* nextLevel);

    double getPrice() const;//const;  // Get price level
    std::shared_ptr<Order> getHeadOrder() ;//const;  // Get head order

public:
    double price;                               // Price for this level
    std::shared_ptr<Order> headOrder;           // Pointer to the first order in the queue
    std::shared_ptr<Order> tailOrder;           // Pointer to the last order in the queue
    PriceLevel* prev;                           // Pointer to the previous price level
    PriceLevel* next;                           // Pointer to the next price level
};


#endif // PRICELEVEL_HPP
