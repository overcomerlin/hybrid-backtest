#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <numeric>
#include <iostream>

namespace py = pybind11;

// Simple Trading Signals
enum class Signal
{
    HOLD,
    BUY,
    SELL
};

class BacktestEngine
{
private:
    double cash;
    double holdings;
    std::vector<double> prices;
    std::vector<double> equity_curve;

public:
    // Constructor: Initialize starting capital and empty holdings
    BacktestEngine(double initial_capital) : cash(initial_capital), holdings(0.0) {}

    /**
     * @brief Receive price data from Python.
     * NOTE: Currently uses std::vector, which involves a data copy.
     * For large datasets, consider using py::array_t (NumPy) to achieve zero-copy.
     */
    void load_data(const std::vector<double> &input_prices)
    {
        prices = input_prices;
        equity_curve.reserve(prices.size());
    }

    /**
     * @brief Calculate Simple Moving Average (SMA).
     * PERFORMANCE NOTE: C++ implementation avoids Python's Global Interpreter Lock (GIL)
     * and loop overhead, making sliding window operations significantly faster.
     */
    std::vector<double> calculate_sma(int window)
    {
        std::vector<double> sma;
        if (prices.size() < window)
            return sma;

        // Initial window sum
        double sum = 0.0;
        for (int i = 0; i < window; ++i)
            sum += prices[i];
        sma.push_back(sum / window);

        /**
         * Sliding window optimization:
         * Instead of re-summing, subtract the element leaving the window
         * and add the new element entering it. Time complexity: O(N).
         */
        for (size_t i = window; i < prices.size(); ++i)
        {
            sum += prices[i] - prices[i - window];
            sma.push_back(sum / window);
        }
        return sma;
    }

    /**
     * @brief Execute Backtest: Dual Moving Average Strategy (Fast SMA vs. Slow SMA).
     * @param fast_window Period for the short-term average.
     * @param slow_window Period for the long-term average.
     * @return A vector representing the total portfolio value (Equity Curve) over time.
     */
    std::vector<double> run_strategy(int fast_window, int slow_window)
    {
        auto sma_fast = calculate_sma(fast_window);
        auto sma_slow = calculate_sma(slow_window);

        // Strategy logic: Start processing only when the longest window (slow SMA) is ready.
        size_t start_idx = slow_window;

        for (size_t i = start_idx; i < prices.size(); ++i)
        {
            double current_price = prices[i];

            /**
             * LOGIC:
             * Fast SMA > Slow SMA -> Bullish Cross (Buy)
             * Fast SMA < Slow SMA -> Bearish Cross (Sell)
             * * ALIGNMENT NOTE:
             * sma_fast starts at index [fast_window - 1] of the price series.
             * sma_slow starts at index [slow_window - 1] of the price series.
             * To access the SMA value corresponding to price[i]:
             * Fast SMA index = i - (fast_window - 1)
             * Slow SMA index = i - (slow_window - 1)
             */
            double f_val = sma_fast[i - fast_window];
            double s_val = sma_slow[i - slow_window];

            if (f_val > s_val && cash > 0)
            {
                // Buy: Convert all cash to holdings (all-in)
                double quantity = cash / current_price;
                holdings = quantity;
                cash = 0;
            }
            else if (f_val < s_val && holdings > 0)
            {
                // Sell: Liquidate all holdings back to cash
                cash = holdings * current_price;
                holdings = 0;
            }

            // Record current total portfolio value (Mark-to-Market)
            double total_equity = cash + (holdings * current_price);
            equity_curve.push_back(total_equity);
        }

        return equity_curve;
    }
};

// Pybind11 Module Binding
PYBIND11_MODULE(quant_engine, m)
{
    m.doc() = "C++ Accelerated Backtesting Engine";

    py::class_<BacktestEngine>(m, "BacktestEngine")
        .def(py::init<double>())
        .def("load_data", &BacktestEngine::load_data, "Load price data into engine")
        .def("run_strategy", &BacktestEngine::run_strategy, "Run SMA crossover strategy");
}