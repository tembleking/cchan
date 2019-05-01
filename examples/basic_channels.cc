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
#include <thread>
#include "../chan.h"

// https://gobyexample.com/channels
int main() {
    // Channels are the pipes that connect concurrent threads.
    // You can send values into channels from one thread and receive
    // those values into another thread.

    // Create a new channel with chan<val-type>.
    // Channels are typed by the values they convey.
    chan<std::string> messages;

    // Send a value into a channel using put().
    // Here we send "ping" to the messages channel we made above, from a new thread.
    std::thread([&]() {
        messages.put("ping");
    }).detach();

    // The get() receives a value from the channel.
    // Here we’ll receive the "ping" message we sent above and print it out.
    auto msg = messages.get().value();
    std::cout << msg << std::endl;

    // When we run the program the "ping" message is successfully passed from
    // one thread to another via our channel.
    //
    // By default sends and receives block until both the sender and receiver are ready.
    // This property allowed us to wait at the end of our program for the "ping" message
    // without having to use any other synchronization.
}
