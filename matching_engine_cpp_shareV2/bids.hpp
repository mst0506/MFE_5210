#ifndef BIDS_HPP
#define BIDS_HPP

#include "side.hpp"
#include "pricelevel.hpp"




class Bids : public Side {
public:
    // Constructor
    Bids();

    // Determine if price1 has higher priority than price2 (higher price has priority for bids)
    bool price1HasHigherPriorityThanPrice2(double price1, double price2) const override;
};



#endif // BIDS_HPP
