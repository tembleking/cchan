# cchan

A C++17 header-based library that implements [Go](https://golang.org/)-like channels.

Go relies on a concurrency model called [CSP (Communicating Sequential Processes)](https://en.wikipedia.org/wiki/Communicating_sequential_processes), to achieve this pattern of synchronization through Channels. Go core philosophy for concurrency is

    Do not communicate by sharing memory; instead, share memory by communicating.

This library provides a set of types which methods behave like Go channels.

## Installation

Import the header `chan.h` into your project to use it.

## Usage

There are multiple examples in the `examples` folder.

```cpp
#include <iostream>
#include <thread>
#include "chan.h"

// https://gobyexample.com/channels
int main() {

    chan<std::string> messages;

    std::thread([&]() {
        messages.put("ping");
    }).detach();

    auto msg = messages.get().value();
    std::cout << msg << std::endl;
}
```

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License

[MIT](https://choosealicense.com/licenses/mit/)
