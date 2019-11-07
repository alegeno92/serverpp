#pragma once
#include <mutex>
#include <condition_variable>
#include <deque>


template<typename T>
class blocking_queue {
private:
    std::mutex d_mutex;
    std::condition_variable d_condition;
    std::deque<T> d_queue;
public:
    void push(T const &value);

    T pop();
};

typedef enum class message_type{
    memory,
    loads,
    storage,
    people
}t_message_type;

typedef struct message {
    t_message_type message_type;

    struct memory{
        int free;
        int total;
    }memory;

    struct loads{
        int free;
        int total;
    }loads;

    struct storage{
        int free;
        int total;
    }storage;

    int people;
} t_message;



template<typename T>
void blocking_queue<T>::push(T const &value) {
    {
        std::unique_lock<std::mutex> lock(this->d_mutex);
        d_queue.push_front(value);
    }
    this->d_condition.notify_one();
}

template<typename T>
T blocking_queue<T>::pop() {
    std::unique_lock<std::mutex> lock(this->d_mutex);
    this->d_condition.wait(lock, [=] { return !this->d_queue.empty(); });
    T rc(std::move(this->d_queue.back()));
    this->d_queue.pop_back();
    return rc;
}
