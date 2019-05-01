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

// https://gobyexample.com/channel-directions

// This ping function only accepts a channel for sending values.
// It would be a compile-time error to try to receive on this channel.
void ping(chan_send<std::string> &pings, std::string msg) {
    pings.put(std::move(msg));
}

// The pong function accepts one channel for receives (pings) and a second
// for sends (pongs).
void pong(chan_recv<std::string> &pings, chan_send<std::string> &pongs) {
    auto msg = pings.get().value();
    pongs.put(std::move(msg));
}

int main() {
    // When using channels as function parameters, you can specify if a
    // channel is meant to only send or receive values.
    // This specificity increases the type-safety of the program.

    chan<std::string> pings(1);
    chan<std::string> pongs(1);

    ping(pings, "passed message");
    pong(pings, pongs);

    std::cout << pongs.get().value() << std::endl;
}
