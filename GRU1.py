class GRUModel(nn.Module):
    def __init__(self, input_dim, hidden_dim, output_dim, num_layers):
        super(GRUModel, self).__init__()
        self.gru_layers = nn.ModuleList()
        self.fc_layers = nn.ModuleList()
        # 构建GRU层
        for layer_idx in range(num_layers):
            input_size = input_dim if layer_idx == 0 else hidden_dim
            self.gru_layers.append(
                nn.GRU(input_size=input_size, 
                       hidden_size=hidden_dim, 
                       batch_first=True)
            )
            # 每个GRU层后接全连接层
            self.fc_layers.append(nn.Linear(hidden_dim, hidden_dim))
            
        self.output_layer = nn.Linear(hidden_dim, output_dim)
        self.activation = nn.ReLU()
    def forward(self, x):
        for gru_layer, fc_layer in zip(self.gru_layers, self.fc_layers):
            x, _ = gru_layer(x)  # GRU层输出形状：(batch, seq_len, hidden_dim)
            x = fc_layer(x)      # 全连接层形状保持不变
            x = self.activation(x)
        # 取最后一个时间步输出
        return self.output_layer(x[:, -1, :])