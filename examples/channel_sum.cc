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
#include <vector>
#include <thread>
#include "../chan.h"

void sum(std::vector<int> s, shared_chan_send<int> c) {
    int sum = 0;
    for (auto v : s) {
        sum += v;
    }
    c->put(sum);
}

template<typename T>
auto split(const std::vector<T> &v) {
    std::vector<T> left(v.begin(), v.begin() + v.size() / 2);
    std::vector<T> right(v.begin() + v.size() / 2, v.end());

    return std::make_tuple(left, right);
}

// https://tour.golang.org/concurrency/2
int main() {
    //  Channels are a typed conduit through which you can send and receive values.
    // By default, sends and receives block until the other side is ready.
    // This allows threads to synchronize without explicit locks or condition variables.
    // The example code sums the numbers in a vector, distributing the work between two threads.
    // Once both threads have completed their computation, it calculates the final result.

    std::vector<int> s = {7, 2, 8, -9, 4, 0};

    auto c = make_shared_chan<int>();
    auto tuple = split(s);

    std::thread(sum, std::get<0>(tuple), c).detach();
    std::thread(sum, std::get<1>(tuple), c).detach();

    auto x = c->get().value();
    auto y = c->get().value();

    std::cout << x << " " << y << " " << x + y << std::endl;
}