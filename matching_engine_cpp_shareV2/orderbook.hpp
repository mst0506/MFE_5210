#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP
#include "hfs_oms_strategy_twap_new.hpp"
#include <unordered_map>
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <stdexcept>
#include <memory>  // 引入智能指针支持
#include "order.hpp"
#include "side.hpp"
#include "asks.hpp"
#include "bids.hpp"
#include "pricelevel.hpp"


class OrderBook {
public:
    // Constructor
    OrderBook(const std::string& ticker ,std::shared_ptr<hfs_oms_strategy_twap> twap);

    // Order-related functions
    void sendOrder(bool isBuy, int qty, double price, int uid, bool isMine = false, const std::time_t& timestamp = std::time(nullptr));
    void sendOrder(std::shared_ptr<Order> newOrder);
    int cancelOrder(int uid);
    void modifyOrder(int uid, int qtyDown);

    // Price sweeping functions
    //void sweepBestPrice(std::shared_ptr<Order> order);
    void sweepBestPrice(std::shared_ptr<Order> order, int orderAggressiveCount);
    void sweepBestPriceMyUid(std::shared_ptr<Order> order, int determineAggressiveCount);

    // Aggressiveness determination
    bool isAggressive(const std::shared_ptr<Order>& order) const;
    bool isAggressive_orderbook(const std::shared_ptr<Order>& order) const;  
    bool isAggressiveMyUid(const std::shared_ptr<Order>& order, const PriceLevel* myuid_best) const;

    // 小部件功能支持
    std::pair<double, int> getBestBid() const;
    std::pair<double, int> getBestAsk() const;
    void removePrice(bool isBuy, double price);
    std::vector<double> topBidPrices(int nlevels) const;
    std::vector<double> topAskPrices(int nlevels) const;
    std::pair<double, double> topNPriceLevelVolume(int nlevels, const std::string& side) const;
    std::pair<double, double> topBidsCumVol(int nlevels) const;
    std::pair<double, double> topAsksCumVol(int nlevels) const;
    std::pair<std::vector<double>, std::vector<double>> topBids(int nlevels) const;
    std::pair<std::vector<double>, std::vector<double>> topAsks(int nlevels) const;


    // 集合竞价相关函数
    void updateLimitOrderBook(std::shared_ptr<Order> order);


    // 统计量
    struct TradeStats {
        double price;
        int tradeQty;
        int orderUid;
        int bestUid;
        bool isBuy;
        std::time_t timestamp;
    };

    struct myTradeStats {
        double price;
        int tradeQty;
        int myUid;
        std::time_t timestamp;
    };

    double computeVWAP(const std::vector<TradeStats>& trades) const;
    double computeVWAP(const std::vector<myTradeStats>& myTrades) const;
    double getVWAP() const;
    double getMyVWAP() const;
    double getTWAP() const ;
    double getMyPOV() const;
    double computeTWAP(const std::vector<TradeStats>& trades) const;
    std::vector<double> getTradesVol() const;
    std::vector<double> getTradesPx() const;
    std::vector<std::time_t> getTradesTime() const;
    std::vector<double> getMyTradesVol() const;
    std::vector<double> getMyTradesPx() const;
    std::vector<std::time_t> getMyTradesTime() const;

    void cancelNotOnBook(int uid);

    // Snapshot recording
    void recordSnapshot(const std::time_t& timestamp);

    // Trade statistics structure
    std::vector<TradeStats> trades;
    std::vector<TradeStats> lastTrades;
    std::vector<myTradeStats> myTrades;
    std::vector<myTradeStats> myLastTrades;

    // py::dict get(int uid) const;
    std::shared_ptr<Order> get(int uid) const;
    void addOrder(std::shared_ptr<Order> order);

public:
    // Helper functions
    void processOrder(std::shared_ptr<Order> newOrder, bool isBuy);
    void processMyOrder(std::shared_ptr<Order> newOrder, bool isBuy);

    // Trade management functions
    void updateLastTrades(const TradeStats& stats, int pos);
    void updateMyLastTrades(const myTradeStats& myStats, int pos);
    void updateTrades(int pos);
    void updateMyTrades(int pos);

    // Data members
    std::string ticker;
    double upLimit;
    double downLimit;
    int nMyOrders;
    int nTrades;
    int nMyTrades;
    double cumVol;
    double myCumVol;
    double marketImpact;
    double lastPrice;
    double myCumVolSent;

    int my_uid_last;
    int my_leavesqty_last;

    PriceLevel* myuid_best;
    PriceLevel* orderbook_best;

    Bids bids;
    Asks asks;

    std::unordered_map<int, std::shared_ptr<Order>> orders;

    std::shared_ptr<Order> market_order_temp;
    std::shared_ptr<Order> limit_order_temp;
    int sh_order_temp_uid;
};

#endif // ORDERBOOK_2_HPP
