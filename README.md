# Hybrid Event-Driven Backtesting Framework

![C++](https://img.shields.io/badge/Core-C++17-red.svg)
![Python](https://img.shields.io/badge/Interface-Python3.9-orange.svg)
![Pybind11](https://img.shields.io/badge/Library-Pybind11-yellow.svg)

A high-performance hybrid backtesting system designed to bridge the gap between **Strategy Research (Python)** and **High-Frequency Execution (C++)**. It leverages `pybind11` to offload computationally expensive strategy logic to C++ while maintaining the flexibility of Python for data analysis.

## 🚀 Why Hybrid?

In quantitative trading, Python is excellent for data manipulation (Pandas), but slow for iterative loops required in event-driven backtesting (due to the GIL and interpreter overhead). This project solves this by:
1.  **Prototyping** strategy logic in Python.
2.  **Executing** the core loop in compiled C++.
3.  **Binding** the two seamlessly using Pybind11.

**Result:** A backtest on 1,000,000 tick data points completes in **milliseconds** in C++, compared to seconds/minutes in pure Python.

## 🛠 Tech Stack

* **Core:** C++ 17 (STL, Logic Execution)
* **Interface:** Python 3.9, Pybind11
* **Data Processing:** NumPy, Pandas
* **Build:** CMake, Docker

## 📂 Project Structure

* `src/main.cpp`: The C++ performance engine implementing SMA logic and PnL tracking.
* `tests/run_backtest.py`: Python driver script that generates data and visualizes results.
* `CMakeLists.txt`: Configuration to compile C++ code into a Python importable module.

## ⚡ Quick Start

### 1. Run with Docker (Recommended)
No local C++ setup required.

```bash
# Build the image
docker build -t hybrid-quant .

# Run the backtest simulation
docker run hybrid-quant
```

### 2. Build using CMake (Optional)

```bash
mkdir build && cd build
cmake .. && make
cd ..
python tests/run_backtest.py
```

## 👨‍💻 Author

**Jacob Lin**
_Algorithm Engineer & Full-Stack Developer_
[LinkedIn](https://www.linkedin.com/in/dachunglin) | [Email](mailto:overcomerlin@gmail.com)

_"A ranger soaring through the world of algorithms."_