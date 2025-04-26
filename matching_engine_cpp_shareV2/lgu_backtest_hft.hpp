#pragma once
#ifndef HFS_BACKTEST_HFT_HPP
#define HFS_BACKTEST_HFT_HPP
#include <unordered_map>
#include <memory>
#include <string>
#include <numeric>
#include "xml_parser.hpp"
#include "hfs_oms_strategy_twap_new.hpp"
#include "hfs_data_api.hpp"
#include "orderbook.hpp"
#include "shmmd_read.h"



struct LastScoreData {
    double bidScore;
    double askScore;
    int bid1p;
    int bid1q;
    int ask1p;
    int ask1q;
    long bid_cum_trade_vol;
    long bid_cum_trade_amt;
    long ask_cum_trade_vol;
    long ask_cum_trade_amt;
};


class hfs_oms_strategy_twap;
class hfs_backtest_hft {
public:
    // 构造函数，初始化策略和 orderbook
    hfs_backtest_hft(std::string tkr,
                     int didx, const a2ftool::XmlNode &cfg,  std::shared_ptr<hfs_oms_strategy_twap>, double size) ;


    struct inference_data {
    long bid_cum_trade_vol;
    long bid_cum_trade_amt;
    long ask_cum_trade_vol;
    long ask_cum_trade_amt;
    int nTime;
    std::string source;
    int bid1p;
    int ask1p;
};




    // void do_backtest(int didx, int tidx, double bidScore ,double askScore ,int bid1p ,int bid1q ,int ask1p ,int ask1q,
    // long bid_cum_trade_vol ,long bid_cum_trade_amt ,long ask_cum_trade_vol, long ask_cum_trade_amt);

    void do_backtest(int didx, int tidx, LastScoreData& lastData);
    void update_order_state(int nTime);

    // 获取或初始化 orderbook 对象
    std::shared_ptr<OrderBook> get_orderbook();

    // 集合竞价和开盘期间订单和交易的处理函数
    void pre_opening_auction(hfs_transaction_t *ts);
    void pre_opening_sz_order(hfs_tdf_order_t *od);
    void pre_opening_sz_trade(hfs_transaction_t *ts);
    void pre_opening_sh(hfs_tdf_order_t *od);

    // 正常竞价期间订单和交易的处理函数
    void process_sh_trade(hfs_transaction_t *ts);
    void process_sh_order(hfs_tdf_order_t *od);
    void process_sz_trade(hfs_transaction_t *ts);
    void process_sz_order(hfs_tdf_order_t *od);
//    void gen_order(int didx ,int tidx ,int nTime );
 //   void gen_order(int didx ,int tidx ,int nTime ,int is_trade);
    void gen_order(int didx, int tidx, int nTime, int is_trade ,double bidScore ,double askScore);
//    void gen_sh_order(int didx ,int tidx ,int nTime);
    void on_mkt_f(int nTime ,int didx ,int tidx, double bidScore ,double askScore,int bid1p ,int bid1q ,int ask1p ,int ask1q);

    // 生成vwap和myvwap
    double get_vwap();
    double get_my_vwap();
    double get_my_tradesvol();
    double get_tradesvol();
    double get_twap();
    std::string symbol;
    std::shared_ptr<Order> order_info;
    void send_sz_order();

    inference_data current;
    inference_data prev;
   // bool is_inference(double sizeFilter,std::string side);
    bool is_inference();
    bool infer_filter();

    bool szMbd_filter(hfs_transaction_t *ts, LastScoreData &lastData);
    bool szMbd_filter(hfs_tdf_order_t *od, LastScoreData &lastData);

    std::string side;
    long size ;
    double sizeFilter;
    std::string ticker;

private:
    
    void leave_temp_order();
    void send_sh_order(int uid , int is_trade);
    ShmMdReader<signal_v3>* ptr_reader;

    hfs_oms_strategy_twap* twap_strategy;
    hfs_data_api* dpi;
    std::string filename;
    std::string exchange;


    
    // double bidScore;
    // double askScore;

    std::shared_ptr<OrderBook> orderbook;

    bool on_pos_called;
    
    int tidx;
    int didx;
//    int nTime;
    std::shared_ptr<hfs_oms_strategy_twap> strategy;

    // 更新策略订单状态所需的成员变量
    int my_leavesqty_last;
    int my_uid_last;
    bool TEST;

};


#endif // HFS_BACKTEST_HFT_HPP
