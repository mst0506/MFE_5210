import numpy as np
import cvxpy as cp

class Optim:
    min_stock_hold = 100
    turnover_tolerance = 0.05
    open_exposure_tolerance = 0.02
    def __init__(self):
        pass

    @classmethod
    def get_shares(cls, returns: np.array, prices: np.array, wealth: float, pre_shares: np.array = None):
        num = returns.size
        inf = 2<<20

        shares = cp.Variable(num, integer=True) # 决策变量，股票交易数量
        _x = cp.Variable(num, integer=True) # 控制shares的“==0”约束与“>=100”或关系

        objective = cp.Maximize((returns * prices).T @ shares) # 最大化当期收益
        constraints = [
            prices.T @ shares <= wealth,  # 预算约束
            shares + (1-_x)*inf >= 100, # 约束>=100，_x1触发或条件
            shares + _x*inf >= 0, # 约束>=0，_x1触发或条件
            shares - _x*inf <= 0, # 约束<=0，_x1触发或条件
            _x >= 0, # 约束_x1==0
            _x <= 1, # 约束_x1==1
            shares >= 0, # 约束shares非负
            np.ones(num) @ _x >= cls.min_stock_hold # 最小股票买入数量约束
        ]
        if pre_shares is not None:
            constraints.extend([
                prices.T @ cp.abs(shares - pre_shares) / prices.T @ pre_shares <= cls.turnover_tolerance, # 换手率约束
                prices.T @ (shares - pre_shares) / prices.T @ pre_shares <= cls.open_exposure_tolerance, # 敞口约束
            ])

        prob = cp.Problem(objective, constraints)
        prob.solve()

        # return shares.value, prob.value
        return shares.value, prob.value, _x.value

if __name__ == '__main__':
    # 生成测试数据
    np.random.seed(0)  # 设置随机种子以获得可重复的结果
    returns = np.abs(np.random.rand(10))  # 随机生成10个收益率
    prices = np.abs(100 * np.random.rand(10))  # 随机生成10个价格
    pre_share = np.abs(100 * np.random.rand(10))  # 随机生成10个价格
    wealth = 100000  # 假设总财富为10000

    # 调用 get_shares 方法
    shares_val, obj_val, _x1_val = Optim.get_shares(returns, prices, wealth, pre_share)

    # 打印结果
    print("Optimal Shares:", shares_val)
    print("Objective Value:", obj_val)
    print("_x Value:", _x1_val)

    pass