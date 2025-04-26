#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>
#include <ctime>
#include <memory>  // for std::shared_ptr




class Order {
public:
    // Constructors
    Order(int uid, bool isBuy, int qty, double price, const std::time_t& timestamp = std::time(nullptr));

    // Core Methods
    int getCumQty() ; //const;
    int getLeaveQty() ; //const;

    // Setters for updating order status
    void setPrice(double price);
    void setLeavesQuantity(int qty);
    void setActive(bool activeStatus);
    void setHasBeenMatchedByMyOrder(bool matchedStatus);

    // Linked List Pointers using shared_ptr
    std::shared_ptr<Order> getPrev() ; //const;
    void setPrev(std::shared_ptr<Order> prevOrder);
    std::shared_ptr<Order> getNext() ;// const;
    void setNext(std::shared_ptr<Order> nextOrder);

public:
    int uid;             
    bool isBuy;          
    int qty;              
    int leavesQty;      
    int cumQty;        
    double price;        
    std::time_t timestamp; 
    bool active;         
    bool hasBeenMatched;   

    // Change prev and next to shared_ptr for automatic memory management
    std::shared_ptr<Order> prev;
    std::shared_ptr<Order> next;
};



#endif // ORDER_HPP
