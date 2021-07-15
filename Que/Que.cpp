#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include "QueWrapper.h"
#include "ThreadW.h"

using namespace std::chrono_literals;

std::atomic_bool end(false);
QueueWrapper<int> q;

void print(const std::string& s, int x) {
    static std::mutex m;
    std::lock_guard<std::mutex> lock(m);
    static  std::ofstream out("logger.txt");
    out << s << " " << x << std::endl;
}

void producer() {
    while (!end) {
        auto t = std::rand();
        print("p", t);
        q.push(t);
    }
    std::this_thread::sleep_for(200ms);
}

void consumer() {
    while (!end || !q.empty()) {
        auto t = q.extract();
        if (t)
            print("c", *t);
        else
            print("c is empty", 0);
    }
    std::this_thread::sleep_for(300ms);
}


int main() {
    threadGuard v;
    v.AddThread(std::move(std::thread(producer)));
    v.AddThread(std::move(std::thread(consumer)));
    v.AddThread(std::move(std::thread(consumer)));
    v.AddThread(std::move(std::thread(consumer)));
    std::this_thread::sleep_for(1s);
    end = true;
    for (int i = 0; i < v.Size();i++) {
        v.Join(i);
    }

    return 0;
}
