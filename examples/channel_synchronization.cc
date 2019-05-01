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

#include <iostream> // cout, endl
#include <thread>   // thread
#include <chrono>   // chrono
#include "../chan.h"

// https://gobyexample.com/channels

// This is the function we’ll run in a thread. The done channel will be
// used to notify another thread that this function’s work is done.
void worker(shared_chan<bool> done) {
    std::cout << "working..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "done" << std::endl;

    // Send a value to notify that we’re done.
    done->put(true);
}

int main() {
    // We can use channels to synchronize execution across threads.
    // Here’s an example of using a blocking receive to wait for a thread to
    // finish.

    // Note that we are creating a pointer. We have to be careful while passing
    // pointers to memory located on stack to other thread, because it might be
    // possible that the variable goes out of scope before the thread tries to
    // access to it.
    // Similarly, we should not use raw heap pointers between threads, one of
    // the threads can free the memory before other tries to access to the
    // contents of the pointer.
    auto done = make_shared_chan<bool>();

    // Start a worker thread, giving it the channel to notify on.
    std::thread th(worker, done);
    th.detach();

    // Block until we receive a notification from the worker on the channel.
    done->get();

    // If you removed the done->get() line from this program, the program would
    // exit before the worker even started.
}
