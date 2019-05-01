/*
    The MIT License (MIT)

    Copyright (c) 2019 Federico Barcelona <tembleking@gmail.com>

    Permission is hereby granted, free of charge, to any person obtaining a copy of
    this software and associated documentation files (the "Software"), to deal in the Software
    without restriction, including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
    to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or
    substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
    PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
    FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
    OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <iostream>
#include "../chan.h"

// https://gobyexample.com/range-over-channels
int main() {
    // We can use the default C++ foreach that uses iterators to
    // retrieve data from a channel until it's closed or
    // doesn't contain anything else.

    // We’ll iterate over 2 values in the queue channel.
    chan<std::string> queue(2);

    queue.put("one");
    queue.put("two");
    queue.close();

    // This range iterates over each element as it’s received from queue.
    // Because we closed the channel above, the iteration terminates
    // after receiving the 2 elements.
    for (auto elem : queue) {
        if (elem) {
            std::cout << elem.value() << std::endl;
        }
    }

    // This example also showed that it’s possible to close a non-empty
    // channel but still have the remaining values be received.
}