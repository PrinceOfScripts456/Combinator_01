#pragma once
#include <cstdint>
#include <string>

class SizeFormatter
{
    public:
        std::string format_Bytes(std::uint64_t bytes);
};

class Math
{
    public:
        std::uint64_t intPow(std::uint64_t base, std::uint64_t power);
        std::uint64_t intPow_Ologn(std::uint64_t base, std::uint64_t power);
};

class Time
{
    public:
        std::string seconds_converter(uint64_t seconds);
        std::string format_miliseconds(uint64_t miliseconds);
};


#include <iostream>
#include <chrono>
#include <string>

class RunTime {
private:
    using clock = std::chrono::steady_clock;

public:
    explicit RunTime(std::string nameP = "") : name(std::move(nameP)) {
        start();
    }

    ~RunTime() noexcept {
        stop();
    }

    void start() noexcept {
        running = true;
        start_time = clock::now();
    }

    void stop() noexcept {
        if (!running) {
            return;
        }

        running = false;

        const auto end = clock::now();
        const auto elapsed =
            std::chrono::duration<double, std::milli>(end - start_time).count();

        if (!name.empty()) {
            std::cout << "[" << name << "] ";
        }

        std::cout << "Elapsed time: " << elapsed << " ms\n";
    }

    RunTime(const RunTime&) = delete;
    RunTime& operator=(const RunTime&) = delete;

private:
    std::string name;
    clock::time_point start_time{};
    bool running{false};
};