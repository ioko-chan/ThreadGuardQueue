#pragma once
template <typename T>
class QueueWrapper {
public:
    QueueWrapper() = default;
    ~QueueWrapper() = default;
    QueueWrapper(const QueueWrapper&) = delete;
    QueueWrapper(QueueWrapper&&) noexcept = default;
    QueueWrapper& operator=(const QueueWrapper&) = delete;
    QueueWrapper& operator=(QueueWrapper&&) noexcept = default;
   

    bool empty() const {
        std::lock_guard<std::mutex> lock(m_m);
        return m_q.empty();
    }

    int size() const {
        std::lock_guard<std::mutex> lock(m_m);
        return m_q.size();
    }

    void push(const T& item) {
        std::lock_guard<std::mutex> lock(m_m);
        m_q.push(item);
        m_cv.notify_one();
    }

    std::shared_ptr<T> extract() {
        std::unique_lock<std::mutex> lock(m_m);
        m_cv.wait_for(lock, 400ms, [&]() {
            return !m_q.empty();
            });
        if (m_q.empty())
            return nullptr;
        auto local = std::make_shared<T>(m_q.front());
        m_q.pop();
        return local;
    }

private:
    mutable std::mutex m_m;
    std::queue<T> m_q;
    std::condition_variable m_cv;
};