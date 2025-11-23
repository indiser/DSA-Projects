import sys
import os
import logging
import requests
from typing import List, Optional
from dotenv import load_dotenv
import matplotlib.pyplot as plt

# --- CONFIGURATION & SETUP ---

# 1. Load Environment Variables
load_dotenv()
API_KEY = os.getenv("ALPHA_VANTAGE_KEY")

if not API_KEY:
    print("FATAL: API Key is missing. Check your .env file.")
    sys.exit(1)

# 2. Configure Professional Logging
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(message)s",
    handlers=[
        logging.FileHandler("system.log"),
        logging.StreamHandler(sys.stdout)
    ]
)
logger = logging.getLogger(__name__)

# 3. Dynamic Module Loading
def load_cpp_module():
    """Locates and loads the compiled C++ stock_analyzer module."""
    module_name = "stock_analyzer"
    current_dir = os.path.dirname(os.path.abspath(__file__))
    
    # Check common build directories first
    search_paths = [
        os.path.join(current_dir, "build", "Debug"),
        os.path.join(current_dir, "build", "Release"),
        os.path.join(current_dir, "build"),
    ]
    
    for search_path in search_paths:
        if os.path.exists(search_path):
            for file in os.listdir(search_path):
                if file.startswith(module_name) and (file.endswith(".pyd") or file.endswith(".so")):
                    if search_path not in sys.path:
                        sys.path.append(search_path)
                    return
    
    logger.critical("Could not find compiled C++ module. Did you run 'cmake --build'?")
    sys.exit(1)

load_cpp_module()

try:
    import stock_analyzer
    logger.info("C++ Analysis Engine loaded successfully.")
except ImportError as e:
    logger.critical(f"Failed to import C++ module: {e}")
    sys.exit(1)

# --- BUSINESS LOGIC ---

def fetch_stock_data(symbol: str) -> List[float]:
    """
    Fetches daily closing prices for a given symbol from Alpha Vantage.
    
    Args:
        symbol: The stock ticker (e.g., 'IBM').
        
    Returns:
        A list of closing prices (floats), sorted by date.
    """
    if not API_KEY:
        logger.error("API Key is missing. Check your .env file.")
        return []

    url = "https://www.alphavantage.co/query"
    params = {
        "function": "TIME_SERIES_DAILY",
        "symbol": symbol,
        "apikey": API_KEY,
        "outputsize": "compact"
    }

    logger.info(f"Fetching data for symbol: {symbol}")

    try:
        response = requests.get(url, params=params, timeout=10)
        response.raise_for_status()
        
        data = response.json()
        
        if "Error Message" in data:
            logger.error(f"API Error: Invalid Symbol '{symbol}'")
            return []
        
        if "Note" in data:
            logger.warning("API rate limit reached. Please wait before making more requests.")
            return []
            
        if "Time Series (Daily)" not in data:
            logger.error("Unexpected API response format.")
            return []

        time_series = data["Time Series (Daily)"]
        closing_prices = [
            float(details["4. close"]) 
            for date, details in sorted(time_series.items())
        ]
        
        logger.info(f"Successfully retrieved {len(closing_prices)} data points.")
        return closing_prices

    except (requests.exceptions.RequestException, ValueError, KeyError) as e:
        logger.error(f"Error fetching data: {e}")
        return []
def sanitize_filename(filename: str) -> str:
    """Sanitize filename to prevent path traversal attacks."""
    # Remove any path separators and keep only alphanumeric and basic chars
    import re
    return re.sub(r'[^a-zA-Z0-9_-]', '', filename)

def plot_data(ticker, prices, sma_value, trade_result):
    """Visualizes the Price, SMA, and Best Trade."""
    days = range(len(prices))
    
    plt.figure(figsize=(12, 6))
    
    # 1. Plot Price
    plt.plot(days, prices, label=f'{ticker} Price', color='black', linewidth=1)
    
    # 2. Plot SMA as horizontal line (since it's a single value)
    plt.axhline(y=sma_value, label='SMA (5-day)', color='blue', linestyle='--')

    # 3. Plot Buy/Sell Signals (Green Up, Red Down)
    if (trade_result.buy_day_index != -1 and 
        0 <= trade_result.buy_day_index < len(prices) and 
        0 <= trade_result.sell_day_index < len(prices)):
        
        buy_day = trade_result.buy_day_index
        sell_day = trade_result.sell_day_index
        
        plt.scatter(buy_day, prices[buy_day], color='green', s=100, label='Buy Signal', zorder=5)
        plt.scatter(sell_day, prices[sell_day], color='red', s=100, label='Sell Signal', zorder=5)
        
        # Draw a line connecting the trade
        plt.plot([buy_day, sell_day], [prices[buy_day], prices[sell_day]], color='green', alpha=0.3, linewidth=3)

        # Annotation
        plt.annotate(f'Profit: ${trade_result.max_profit:.2f}', 
                     xy=(sell_day, prices[sell_day]), 
                     xytext=(sell_day+5, prices[sell_day]+5),
                     arrowprops=dict(facecolor='black', shrink=0.05))

    plt.title(f"{ticker} Analysis: Optimal Algo Trading Strategy")
    plt.xlabel("Trading Days")
    plt.ylabel("Price ($)")
    plt.legend()
    plt.grid(True, alpha=0.3)
    
    # Sanitize filename to prevent path traversal
    safe_ticker = sanitize_filename(ticker)
    filename = f"{safe_ticker}_analysis.png"
    plt.savefig(filename)
    logger.info(f"Chart saved to {filename}")
    plt.show()
# --- ENTRY POINT ---

if __name__ == "__main__":
    ticker_input = input("Enter Stock Symbol (e.g., IBM): ").strip().upper()
    
    if not ticker_input:
        logger.warning("No ticker provided. Exiting.")
        sys.exit(0)

    prices = fetch_stock_data(ticker_input)
    
    if prices:
        window_size = 5
        
        logger.info(f"Running SMA({window_size}) on C++ Engine...")
        sma_result = stock_analyzer.calculate_sma(prices, window_size)
        
        logger.info("Calculating EMA (20) and RSI (14)...")
        ema_values = stock_analyzer.calculate_ema(prices, 20)
        rsi_values = stock_analyzer.calculate_rsi(prices, 14)
        
        ema_latest = ema_values[-1] if ema_values else 0.0
        rsi_latest = rsi_values[-1] if rsi_values else 0.0

        # Top K Gains Analysis
        k_days = 3
        logger.info(f"Finding top {k_days} daily gains using C++ Min-Heap...")
        top_gains = stock_analyzer.find_top_k_gains(prices, k_days)
        
        # Sort them for display (since heap output order depends on extraction)
        top_gains.sort(reverse=True)

        logger.info("Calculating optimal trade entry/exit...")
        trade_result = stock_analyzer.find_max_profit(prices)

        # Display results
        logger.info("\n" + "="*40)
        logger.info(f" ANALYSIS REPORT: {ticker_input}")
        logger.info("="*40)
        logger.info(f" Latest Price:      ${prices[-1]:.2f}")
        logger.info(f" SMA (5-day):       ${sma_result:.2f}")
        logger.info(f" EMA (20-day):      ${ema_latest:.2f}")
        logger.info(f" RSI (14-day):      {rsi_latest:.2f}")
        logger.info(f" Top {k_days} Daily Jumps:  {['${:.2f}'.format(g) for g in top_gains]}")
        logger.info("="*40)
        
        if (0 <= trade_result.buy_day_index < len(prices) and 
            0 <= trade_result.sell_day_index < len(prices)):
            logger.info(f" Max Profit: ${trade_result.max_profit:.2f}")
            logger.info(f"   Buy Day:   Index {trade_result.buy_day_index} (@ ${prices[trade_result.buy_day_index]:.2f})")
            logger.info(f"   Sell Day:  Index {trade_result.sell_day_index} (@ ${prices[trade_result.sell_day_index]:.2f})")
        else:
            logger.warning("Invalid trade indices returned.")

        # Launch Visualization
        logger.info("Generating visualization...")
        plot_data(ticker_input, prices, sma_result, trade_result)
        

        
    else:
        logger.warning("Analysis aborted due to lack of data.")
        sys.exit(1)