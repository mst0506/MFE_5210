CXX = g++
# 添加 -Wall -Wextra -Wshadow -Wpedantic 等编译器警告选项，以及 -g 用于调试
CXXFLAGS = -std=c++14 -O3  -DUSE_BIZ -DUSE_TIMESTAMP  $(INCLUDE_DIRS)
#-Wall -Wextra -Wshadow -Wpedantic

# Additional include directories
BOOST_INCLUDE = /home/liyl/work/oms/boost
BOOST_INCLUDE_1 = /home/liyl/work/oms

# Include directories for project headers
INCLUDE_DIRS = -I$(BOOST_INCLUDE) -I$(BOOST_INCLUDE_1) \
               -I/home/liyl/work/oms/VWAP_V35_Shm_14 \
               -I/home/liyl/work/oms/core \
               -I/home/liyl/work/oms/include \
               -I/home/liyl/work/oms/twap_strategy \
               -I/home/liyl/work/oms/matching_engine_cpp_shareV2

# Linker flags, 添加 -g 以便链接调试符号
LDFLAGS = -L/home/liyl/work/oms/libs -g

# Libraries
LIBS = -lpthread -ldl -lutil -lm -lrt \
       /home/liyl/work/oms/lib/libjsim_core.so \
       /home/liyl/work/oms/lib/gw_adaptor.so \
       /home/liyl/work/oms/lib/libstock_oms.so \
       /home/liyl/work/oms/lib/target_csv.so \
       /home/liyl/work/oms/lib/libCSVparser.so

# Source files
SRCS = main.cpp hfs_backtest_hft.cpp hfs_oms_strategy_twap_new.cpp  orderbook.cpp order.cpp pricelevel.cpp side.cpp bids.cpp asks.cpp 

# Object files
OBJS = $(SRCS:.cpp=.o)

# Output executable
TARGET = matching_engine

# Default target to build the program
all: $(TARGET)

# Link the object files to create the final executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS)

# Compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
