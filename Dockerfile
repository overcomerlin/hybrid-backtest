FROM python:3.9-slim

# 安裝編譯工具
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# 安裝 Python 依賴 (包含 pybind11)
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# 複製程式碼
COPY . .

# 編譯 C++ 模組
RUN mkdir build && cd build && \
    cmake .. && \
    make

# 設定 Python Path 讓 script 找得到 .so 檔
ENV PYTHONPATH="/app/build"

# 預設執行回測腳本
CMD ["python", "tests/run_backtest.py"]