#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cmath>

std::vector<int> primes;
std::mutex primesMutex;

bool isPrime(int num) {
    if (num <= 1) {
        return false;
    }
    int limit = static_cast<int>(std::sqrt(num));
    for (int i = 2; i <= limit; ++i) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

void getPrimes(int start, int end) {
    for (int i = start; i <= end; ++i) {
        if (isPrime(i)) {
            std::lock_guard<std::mutex> lock(primesMutex);
            primes.push_back(i);
        }
    }
}

int main() {
    int range_start, range_end;

    std::cout << "Enter the start of the range: ";
    std::cin >> range_start;

    std::cout << "Enter the end of the range: ";
    std::cin >> range_end;

    const int num_threads = 4;
    std::vector<std::thread> threads;

    int segment_size = (range_end - range_start + 1) / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        int start = range_start + i * segment_size;
        int end = (i == num_threads - 1) ? range_end : start + segment_size - 1;
        threads.emplace_back(getPrimes, start, end);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "The prime numbers between " << range_start << " and " << range_end << " are:\n";
    for (int prime : primes) {
        std::cout << prime << " ";
    }
    std::cout << std::endl;

    return 0;
}
