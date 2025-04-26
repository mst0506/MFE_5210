class BayesianLinearRegression(BaseEstimator, RegressorMixin):
    def __init__(self, alpha=1.0, beta=1.0):
        self.alpha = alpha  # 控制先验分布的超参数
        self.beta = beta    # 控制似然函数的超参数
        self.mean = None    # 后验均值参数
        self.covariance = None  # 后验协方差矩阵
        
    def fit(self, X, y):
        X = self._validate_data(X, ensure_2d=True)
        y = np.asarray(y).flatten()
        n_samples, n_features = X.shape
        X_b = np.c_[np.ones((n_samples, 1)), X]  # 添加偏置项
        
        # 计算后验协方差矩阵
        prior_term = self.alpha * np.eye(n_features + 1)
        data_term = self.beta * X_b.T @ X_b
        self.covariance = np.linalg.inv(prior_term + data_term)
        
        # 计算后验均值
        self.mean = self.beta * self.covariance @ X_b.T @ y
        return self
        
    def predict(self, X):
        X = self._validate_data(X, ensure_2d=True)
        if self.mean is None:
            raise NotFittedError("需要先调用fit方法进行模型训练")
        n_samples, _ = X.shape
        X_b = np.c_[np.ones((n_samples, 1)), X]  # 添加偏置项
        return X_b @ self.mean

# 定义贝叶斯回归模型
model = BayesianLinearRegression(alpha=1.0, beta=1.0)