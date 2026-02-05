import sys
import os
import time
import numpy as np
import pandas as pd

# Ensure the compiled C++ module can be imported
# In a Docker environment, the binary is typically located in the '../build' directory
sys.path.append(os.path.join(os.path.dirname(__file__), '../build'))

try:
    import quant_engine
except ImportError:
    # Error handling for missing .so or .pyd files
    print("Error: Could not import 'quant_engine'. Please compile the C++ module first.")
    sys.exit(1)


def generate_dummy_data(n=1000000):
    # Generates random walk price data (simulating 1 million tick data points)
    print(f"Generating {n} data points in Python...")
    np.random.seed(42)
    returns = np.random.normal(0.0001, 0.01, n)
    prices = 100 * np.cumprod(1 + returns)
    return prices.tolist()


def main():
    # 1. Data Preparation
    prices = generate_dummy_data(1000000)  # 100萬筆數據

    # 2. Initialize C++ Engine
    initial_capital = 10000.0
    engine = quant_engine.BacktestEngine(initial_capital)

    # 3. Load Data (Transferring data from Python memory to C++ memory)
    # NOTE: Converting large Python lists to C++ vectors can be a bottleneck.
    # If performance lags here, consider using NumPy arrays with pybind11 buffer protocol.
    print("Loading data into C++ Engine...")
    start_load = time.time()
    engine.load_data(prices)
    print(f"Data Loaded. Time: {time.time() - start_load:.4f}s")

    # 4. Execute Strategy (Heavy calculations performed in C++)
    print("Running Strategy (SMA Crossover) in C++...")
    start_run = time.time()

    # Parameters: Fast Window = 50, Slow Window = 200
    equity_curve = engine.run_strategy(50, 200)

    end_run = time.time()

    # 5. Result Analysis
    duration = end_run - start_run
    final_equity = equity_curve[-1] if equity_curve else initial_capital
    ret = (final_equity - initial_capital) / initial_capital * 100

    print("-" * 30)
    print(f"Backtest Completed.")
    print(f"Processing Time: {duration:.4f} seconds")
    print(f"Final Equity: ${final_equity:.2f}")
    print(f"Return: {ret:.2f}%")
    print("-" * 30)

    # 這裡可以加 Matplotlib 畫圖，但在 Docker 命令行通常省略
    # import matplotlib.pyplot as plt ...


if __name__ == "__main__":
    main()
