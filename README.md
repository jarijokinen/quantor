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
# ./research/rsi-on-bull-trend-start.c

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
./bin/rsi-on-bull-trend-start
```

## License

MIT License. Copyright (c) 2026 [Jari Jokinen](https://jarijokinen.com).  See
[LICENSE](https://github.com/jarijokinen/quantor/blob/main/LICENSE.txt)
for further details.
