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

// https://gobyexample.com/channel-buffering
int main() {
    // By default channels are unbuffered, meaning that they will only accept
    // sends (.put()) if there is a corresponding receive (.get()) ready to
    // receive the sent value. Buffered channels accept a limited number of values
    // without a corresponding receiver for those values.

    // We make a channel of strings buffering up to 2 values.
    chan<std::string> messages(2);


    // Because the channel is buffered, we can send these values into the channel
    // without a corresponding concurrent receive.
    messages.put("buffered");
    messages.put("channel");

    // Later we can receive these two values as usual.
    std::cout << messages.get().value() << std::endl;
    std::cout << messages.get().value() << std::endl;
}