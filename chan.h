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

#ifndef CHANNELS_CHAN_H
#define CHANNELS_CHAN_H


#include <queue>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <memory>


/*
 Channels are the pipes that connect concurrent threads.
 You can send values into channels from one thread and receive those values into another thread.
 They have been created inspired in Golang channels: https://gobyexample.com/channels
 */

/************************************************************************************************/

template<typename T>
class base_chan {
public:
    virtual bool valid() = 0;
};

/************************************************************************************************/
template<typename T>
class chan_recv : public base_chan<T> {
public:
    virtual std::optional<T> get() noexcept = 0;

    class iterator : public std::iterator<std::output_iterator_tag, std::optional<T>> {
        chan_recv<T> *ch;
    public:
        explicit iterator(chan_recv<T> *ch) : ch(ch) {}

        iterator &operator++() { return *this; }

        bool operator!=(const iterator &rhs) const { return ch->valid(); }

        std::optional<T> operator*() { return ch->get(); }
    };

    iterator begin() { return iterator(this); }

    iterator end() { return iterator(this); }
};

/************************************************************************************************/

template<typename T>
class chan_send : public base_chan<T> {
public:
    virtual void put(T item) noexcept = 0;

    virtual void close() = 0; // only the sender should close the channel
};

/************************************************************************************************/

template<typename T>
class chan : public chan_send<T>, public chan_recv<T> {
    typedef std::unique_lock<std::recursive_mutex> lock;
    std::queue<T> queue;
    uint16_t buff_size;
    std::recursive_mutex mtx;
    std::condition_variable_any cv;
    bool open;

public:
    explicit chan(uint16_t size = 0) : buff_size(size), open(true) {}

    virtual ~chan() {
        lock lck(mtx);
        open = false;

        std::queue<T> empty;
        queue.swap(empty);

        cv.notify_all();
    }

    chan(const chan &) = delete; // Delete copy constructor
    chan &operator=(chan const &) = delete; // Delete copy assignation

    void put(T item) noexcept override {
        lock lck(mtx);

        if (!open) return;

        queue.emplace(std::move(item));
        cv.notify_all();
        cv.wait(lck, [this]() { return queue.size() <= buff_size; });
    }

    std::optional<T> get() noexcept override {
        lock lck(mtx);

        cv.wait(lck, [this]() { return !open || !queue.empty(); });

        if (queue.empty()) return std::nullopt;

        T item = queue.front();
        queue.pop();
        cv.notify_all();
        return std::optional<T>{item};
    }

    bool valid() noexcept override {
        lock lck(mtx);
        return open || !queue.empty();
    }

    void close() noexcept override {
        lock lck(mtx);
        open = false;
        cv.notify_all();
    }
};

/************************************************************************************************/

template<typename T>
using shared_chan = std::shared_ptr<chan<T>>;

template <typename T>
using shared_chan_recv = std::shared_ptr<chan_recv<T>>;

template <typename T>
using shared_chan_send = std::shared_ptr<chan_send<T>>;

/************************************************************************************************/

/**
 * Create a shared_ptr for the channel. This is the suggested way to pass
 * channels between threads.
 */
template<typename T>
shared_chan<T> make_shared_chan(uint16_t buff_size = 0) {
    return std::make_shared<chan<T>>(buff_size);
}


#endif //CHANNELS_CHAN_H
