import os
import pandas as pd
import pickle as pkl

from optim import Optim

# 读取数据
real_ret_df = pd.read_parquet(r'D:\pycharm_project\MFE5210\data\y.parquet')
pred_ret_df = pd.read_parquet(r'D:\pycharm_project\MFE5210\data\yhat.parquet')
price_df = pd.read_parquet("D:\pycharm_project\MFE5210\data\price.parquet")

# 交易设定
start_time = pd.Timestamp("20230703093900")
# start_time = pd.Timestamp("20231211093900")
end_time = pd.Timestamp("20241231133900")
daily_start_hour = 9
daily_start_min = 39
trade_offset = pd.DateOffset(hours=1)

# 参数定义
group = 10
top_n = 1
wealth = 1e8

# 滚动优化
wealth_log = []
stock_log = []
sgn_df = (pred_ret_df.T > pred_ret_df.quantile(1-(top_n/group), axis=1)).T.astype('int') # 信号生成
time_idx = start_time
while True:
    print(f"当前时间：{str(time_idx)}")
    end_trade_am = pd.Timestamp(year=time_idx.year, month=time_idx.month, day=time_idx.day, hour=11, minute=30)
    start_trade_pm = pd.Timestamp(year=time_idx.year, month=time_idx.month, day=time_idx.day, hour=13, minute=0)
    end_trade_pm = pd.Timestamp(year=time_idx.year, month=time_idx.month, day=time_idx.day, hour=15, minute=0)

    try:
        _sgn_s = sgn_df.loc[time_idx]
        if _sgn_s[_sgn_s == 1].size == 509:  # 预测值分组正常时调仓
            stocks = list(_sgn_s[_sgn_s == 1].index)
            pred_returns = pred_ret_df.loc[time_idx][stocks].values
            real_returns = real_ret_df.loc[time_idx][stocks].values
            prices = price_df.loc[time_idx][stocks].values

            if time_idx == start_time:
                shares, _, _ = Optim.get_shares(pred_returns, prices, wealth)
            else:
                shares, _, _ = Optim.get_shares(pred_returns, prices, wealth, shares)
            wealth += (prices * real_returns) @ shares
        else: # 预测值分组异常时保留仓位
            real_returns = real_ret_df.loc[time_idx][stocks].values
            prices = price_df.loc[time_idx][stocks].values

            wealth += (prices * real_returns) @ shares

        wealth_log.append(wealth)
        stock_log.append(stocks)

        if (time_idx + trade_offset) > end_trade_am and (time_idx + trade_offset) < start_trade_pm:
            time_idx += pd.DateOffset(minutes=150)
        elif (time_idx + 2 * trade_offset) > end_trade_pm:
            time_idx = pd.Timestamp(year=time_idx.year, month=time_idx.month, day=time_idx.day, hour=daily_start_hour,
                                    minute=daily_start_min) + pd.DateOffset(days=1)
        else:
            time_idx += trade_offset

    except KeyError as e:
        time_idx += pd.DateOffset(days=1)
        continue

    if time_idx == end_time: break

print(wealth)
pkl.dump(wealth_log, open(r'results\wealth.pkl', 'wb'))
pkl.dump(stock_log, open(r'results\stock.pkl', 'wb'))
