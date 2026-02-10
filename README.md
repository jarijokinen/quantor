# Quantor

Quantitative analysis and backtesting framework for algorithmic trading. 
Written in pure C and optimized for speed.

## Features

* Import historical intraday data (1-min bars) from Tiingo API
* Store data in binary format and mmap it into memory for maximum performance
* Research functionality to create studies based on historical data

## Installation

```bash
git clone https://github.com/jarijokinen/quantor
cd quantor
make clean; make
```

You'll need also some libraries if these are not installed yet.

```bash
# Homebrew
brew install pkg-config curl

# APT
apt install pkg-config libcurl4-openssl-dev
```

## Configuration

Configure Tiingo API token in the .env file.

```bash
TIINGO_API_TOKEN=secret
```

## Usage

Import historical intraday data from Tiingo:

```bash
./quantor import AAPL
```

Check the data file (and print some info like bar count):

```bash
./quantor check AAPL
```

## Research

Create your own study inside the `research` directory:

```c
// research/rsi_on_spy_bull_trend_start.c

#include "research.h"

void on_init(Quantor *q)
{
  // Initialize your study
}

void on_bar(Quantor *q, const char *symbol, const Bar *bar)
{
  // Process bar data
}

void on_done(Quantor *q)
{
  // Print results
}
```

Compile and run the study:


```bash
make research
./bin/rsi_on_spy_bull_trend_start
```
```
Probability study: RSI(14) when SPY close crosses above SMA(200)
Samples: 3014
Mean RSI at bull-start: 55.51
Most probable RSI bin: [50.0, 60.0)  count=1134
RSI [0.0, 10.0): 0.00% (0)
RSI [10.0, 20.0): 0.00% (0)
RSI [20.0, 30.0): 0.23% (7)
RSI [30.0, 40.0): 5.47% (165)
RSI [40.0, 50.0): 24.12% (727)
RSI [50.0, 60.0): 37.62% (1134)
RSI [60.0, 70.0): 25.38% (765)
RSI [70.0, 80.0): 6.37% (192)
RSI [80.0, 90.0): 0.63% (19)
RSI [90.0, 100.0): 0.17% (5)
```

## License

MIT License. Copyright (c) 2026 [Jari Jokinen](https://jarijokinen.com).  See
[LICENSE](https://github.com/jarijokinen/quantor/blob/main/LICENSE.txt)
for further details.
