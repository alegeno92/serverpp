#include <mutex>
#include <condition_variable>
#include <deque>


template <typename T>
class queue {
private:
    std::mutex              d_mutex;
    std::condition_variable d_condition;
    std::deque<T>           d_queue;
public:
    void push(T const& value);
    T pop();
};
