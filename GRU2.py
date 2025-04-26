class GRUEstimator(BaseEstimator):
    def __init__(self, hidden_dim=16, output_dim=1, num_layers=3):
        self.hidden_dim = hidden_dim
        self.output_dim = output_dim
        self.num_layers = num_layers
        # 模型组件
        self.model = GRUModel(input_dim=7,  # 保持原始输入维度7
                             hidden_dim=hidden_dim,
                             output_dim=output_dim,
                             num_layers=num_layers)
        self.optimizer = torch.optim.Adam(self.model.parameters(), lr=0.001)
        self.criterion = nn.MSELoss()
        # 训练状态跟踪
        self._is_fitted = False
    def fit(self, X, y):
        # 输入验证
        X = self._validate_input(X)
        y = np.asarray(y).reshape(-1, 1)  # 确保二维形状
        # 转换为张量并添加时间步维度
        X_tensor = torch.tensor(X, dtype=torch.float32).unsqueeze(1)  # (n,1,7)
        y_tensor = torch.tensor(y, dtype=torch.float32)
        # 训练循环
        self.model.train()
        for epoch in range(100):  # 保持原始100 epochs
            self.optimizer.zero_grad()
            y_pred = self.model(X_tensor).squeeze()  # (n,1) -> (n)
            loss = self.criterion(y_pred, y_tensor)
            loss.backward()
            self.optimizer.step()
        self._is_fitted = True
        return self
    def predict(self, X):
        if not self._is_fitted:
            raise NotFittedError("需要先调用fit方法进行模型训练")
        X = self._validate_input(X)
        X_tensor = torch.tensor(X, dtype=torch.float32).unsqueeze(1)
        with torch.no_grad():
            return self.model(X_tensor).squeeze().numpy()
    def _validate_input(self, X):
        X = np.asarray(X)
        if X.ndim != 2 or X.shape[1] != 7:
            raise ValueError("输入特征形状应为(n_samples, 7)")
        return X