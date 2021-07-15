#pragma once
class threadGuard {
public:
    threadGuard() {
    }
    void AddThread(std::thread th) {
        thread_.push_back(std::move(th));
    }

    int Size() {
        return thread_.size();
    }

    void Join(int i) {
        if (thread_[i].joinable())
            thread_[i].join();
    }
    ~threadGuard() {
        for (int i = 0; i < thread_.size(); i++) {
            Join(i);
        }
    }
private:
    std::vector<std::thread> thread_;
};