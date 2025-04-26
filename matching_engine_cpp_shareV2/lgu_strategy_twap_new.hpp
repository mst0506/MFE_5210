#pragma once
#include "order.hpp"

#include "hfs_oms_base_strategy.hpp"
#include "HfsTick.hpp"
#include "hfs_osc.hpp"
#include "hfs_data_api.hpp"
#include "hfs_data_common_tick.hpp"
#include "hfs_log.hpp"
#include "hfs_pos_manager.hpp"
#include "CSVparser.hpp"
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

struct TgtQtyData {
    std::string code;
    double tgt_qty;
    int begin_time;
    int end_time;
    double CSI1000;
};

 


// 定义 StockInfo 结构体
struct StockInfo {
    int preClose;  
    int askPrc1;
    int askVol1;
    int bidPrc1;
    int bidVol1;
    int lastPrc;
    int askIndex;
    int bidIndex;
    int planRatio;
    
    int PosBod;
    int PosActual;
    int lastMarketTime;
    int lastTransTime;
    int lastOrderTime;
    int latestTime;
    int lastOrderTrades;
    int lastReqTime;
    int lastCancelTime;
    int lastRejTime;
    
    int totalTradePlan;   
    int totalBuyVolume;
    int totalSellVolume;
    int currentTradePlan;
    int currentTradedVolume;
    int currentPosition;
    int unFinQty;

    int currentSliceIndex;
    
    float score;
    int signal;
    int NSlices;
    int Cut;
    int timeSlices[21];
    int toTrade[20];
    
    int group;
    int closeTime;
    int tailTime;
    int headTime;
    
    int tradedDuringMDsASK;
    int tradedDuringMDsBID;
    int targetPrice;
    int minOrderVol;
    
    bool forceCancel;
    int PosBodAdj;
    bool ifNewPlan;
    int preTargetQTY;
    bool ifClearHolding;
    bool fastTrade;
    bool hasPrice;
    bool hasOrder;

    float randomX;
    char ord_param;

    // 默认构造函数
    StockInfo() 
      : preClose(100), askPrc1(0), askVol1(0), bidPrc1(0), bidVol1(0), lastPrc(0), 
        askIndex(0), bidIndex(0), planRatio(100), PosBod(0), PosActual(0),
        lastMarketTime(0), lastTransTime(0), lastOrderTime(0), latestTime(0),
        lastOrderTrades(0), lastReqTime(0), lastCancelTime(0), lastRejTime(0),
        totalTradePlan(0), totalBuyVolume(0), totalSellVolume(0), 
        currentTradePlan(0), currentTradedVolume(0), currentPosition(0), unFinQty(0),
        currentSliceIndex(0), score(0.0f), signal(0), NSlices(0), Cut(0),
        group(0), closeTime(0), tailTime(0), headTime(0), 
        tradedDuringMDsASK(0), tradedDuringMDsBID(0), targetPrice(0), minOrderVol(100),
        forceCancel(false), PosBodAdj(0), ifNewPlan(false), preTargetQTY(0), 
        ifClearHolding(false), fastTrade(false), hasPrice(false), hasOrder(false), 
        randomX(1.0f), ord_param(0)
    {
        std::fill(std::begin(timeSlices), std::end(timeSlices), 0);
        std::fill(std::begin(toTrade), std::end(toTrade), 0);
    }
};


    struct signal_v3
    {
        long didx;
        long tidx;
        float bidScore;
        float askScore;
        int bid1p;
        int bid1q;
        int ask1p;
        int ask1q;
        long bid_cum_trade_vol;
        long bid_cum_trade_amt;
        long ask_cum_trade_vol;
        long ask_cum_trade_amt;
    };
// 定义 timeConfig 结构体
struct timeConfig {
    int normalTradeTime = 0;
    int normalOrderTimeInterval = 1000;
    int closeOrderTimeInterval = 1000;
    int cancelOrderTime = 3000;
    int slientTime = 10000;
    int delayTimeLimit = 120000;
    int timeRange = 0;
    int headTime = 93000000;
    int tailTime = 150000000;
    int closeTime = 150500000;
    int tailTimeGroup1 = 144500000;
    int closeTimeGroup1 = 145000000;
    int remainedTime = 300000;

    // 构造函数
    timeConfig() {}
};

// 定义 tradeConfig 结构体
struct tradeConfig {
    int totalVolumeToTrade[3] = {0};   // total , buy ,sell
    int totalVolumeTraded[3] = {0};
    float totalAmountToTrade[3] = {0.0f};
    float totalAmountTraded[3] = {0.0f};
    float currentAmountToTrade = 0.0f;
    float currentAmountTraded = 0.0f;
    float amountPerTrade = 0.0f;
    int volumePerTrade = 1000;
    float portExposureLimit = 1000000.0f;
    float expectedExposure = 0.0f;
    float portExposure = 0.0f;
    float baseThreshold = 0.0f;
    float maxAmount = 0.0f;
    float inputExposureLimit = 0.0f;
    float slope = 0.8f;
    float limitRatio = 1.0f;
    int nUnit = 7;
    int remainedTime = 300000;
    int lastNOrderNum = 20;
    int lastNTradeNum = 20;
    int ContinuousOrderTrades = 20;
    float maxOrderRatio = 0.23f;
    float spreadX = 1.0f;
    float ADJThresholdBps = 4.0f;
    unsigned int currSid = 0;
    bool waitG1 = false;
    bool ifMyorderbook = false;


    // 构造函数
    tradeConfig() {
        std::fill(std::begin(totalVolumeToTrade), std::end(totalVolumeToTrade), 0);
        std::fill(std::begin(totalVolumeTraded), std::end(totalVolumeTraded), 0);
        std::fill(std::begin(totalAmountToTrade), std::end(totalAmountToTrade), 0.0f);
        std::fill(std::begin(totalAmountTraded), std::end(totalAmountTraded), 0.0f);
    }
};

//  float portExposure = 0.0f;  //   static 

struct AlgoParams {
    float G1, G2, G3, G4, G5;
};
struct TwapScore {
    double close, score;
    int rank, group;
};

class DataManager;

template<class DataField, class Lock>
class ShmMdReader;
class Order;
class hfs_oms_strategy_twap : public hfs_oms_base_strategy {
//protected:
public: 
    // virtual int process_state_none(hfs_oms_base_ord_trader * otrd);
    // virtual int process_state_filled(hfs_oms_base_ord_trader *otrd);
    // virtual int process_state_new(hfs_oms_base_ord_trader *otrd);
    // virtual int process_state_rejected(hfs_oms_base_ord_trader *otrd);
    // virtual int process_state_canceled(hfs_oms_base_ord_trader *otrd);
    // virtual int process_state_pending_new(hfs_oms_base_ord_trader * otrd) ;
    // virtual int process_state_pending_cancel(hfs_oms_base_ord_trader * otrd) ;
    // virtual py::dict process_state_none(int didx, int tidx);process
    std::shared_ptr<Order> process_state_none(int didx, int tidx,double bidScore ,double askScore) ;
    virtual int process_state_filled(double od_prc ,int ord_qty_left ,std::string ord_side);
    virtual int process_state_new(double od_prc ,std::string od_side);
    virtual int process_state_rejected(hfs_order_t *ord);
    virtual int process_state_canceled(double ord_prc ,int ord_qty ,std::string ord_side);
    virtual int process_state_pending_new(hfs_order_t *ord) ;
    virtual int process_state_pending_cancel(hfs_order_t *ord) ;
    virtual bool test_log() ;



    virtual bool ladder_check();
//    virtual bool init();
    virtual bool init(std::string symbol, const std::string& configFile, const int cidx);

//private:
public:
    virtual TgtQtyData* findTgtQtyData(const std::string &code);
//    virtual bool on_pos(hfs_tgt_position &pos);
    virtual bool on_pos();
//    virtual bool on_mkt(hfs_tick_t &tick);
    virtual bool on_mkt(int didx ,int tidx , int bid1p, int bid1q ,int ask1p, int ask1q );


    // 对于声明重写 
    //virtual bool on_signal(trade_signal &sig);
//    bool on_signal(signal_v2 &sig);    
    virtual bool on_signal(double bidScore ,double askScore);    

    //辅助功能函数
    //计算tm时刻离开盘的距离，以毫秒计，除去午间休息
    int abs_time(unsigned tm);
    
    void to_lower_string(string &str){
        transform(str.begin(), str.end(), str.begin(), (int (*)(int))tolower);
    }
    void to_upper_string(string &str){
        transform(str.begin(), str.end(), str.begin(), (int (*)(int))toupper);  
    }
    
    bool is_can_trade(int didx ,int tidx);
    int get_delta_qty(int &orderVolume, double &orderPrc);
    double get_order_prc();  // 获得下单价格
    void extend_trade_time();
    void reinit_slices();
    void update_holding(const hfs_holdings_t* h);

//private:
public:
  //  DataManager dm;  // 读取数据
    bool hasExtra;  // 限制只有一个外挂订单
    StockInfo stockInfo;
//    std::unordered_map<std::string, TgtQtyData> tgtQtyData; 
    std::vector<TgtQtyData> tgtQtyDataVec;  
    TgtQtyData tgtQtyData1;
    timeConfig* tmConfig = nullptr;
    tradeConfig* trdConfig = nullptr; 
    TwapScore* twapScore = nullptr;
    
    // 记录策略对应的合约信息
    std::string tradeAccount;
    int     idnum;
    string  symbol;  // 股票代码

    //记录盘口价格和量
    hfs_market_data_t *curr_tick = nullptr;
// 声明 CSMA 指针
    CSMA* avgVolInLastNTrades = nullptr;
    CSMA* avgVolInLastNOrders = nullptr;




    hfs_data_api *dpi;
    hfs_base_univ *univ;
    vector<string> tkrs; 

    // 记录alpha信号 ,
    signal_v3 signal;

    // 共享内存读取器
    ShmMdReader<signal_v3>* ptr_reader; 


    // 先从语法上，将signal进行更正
    // 再从数据传入上，对于更正的signal测试
    // trade_signal signal_buy;
    // trade_signal signal_sell; 
    
    //原始的trade_signal 不知道是从哪儿继承来的，先重写一个 struct

//    struct trade_signal {
//        int buy_signal;  // 买方信号
//        int sell_signal; // 卖方信号

//    };

// 声明 algoParaTable 为裸指针
    std::unordered_map<std::string, AlgoParams*>* algoParaTable = nullptr;





//    std::unordered_map<std::string, AlgoParams*> *algoParaTable;
    std::string configDir;
public:
    hfs_oms_strategy_twap(const a2ftool::XmlNode &_cfg, hfs_oms_base_trader *trder);
    hfs_oms_strategy_twap();
    virtual ~hfs_oms_strategy_twap();
};

extern "C" {
    hfs_oms_base_strategy *create(const a2ftool::XmlNode &_cfg, hfs_oms_base_trader *trader);
    int time2msc(int timex);  // 声明 time2msc
    int msc2time(int mscx);   // 声明 msc2time
};