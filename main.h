#pragma once

#include <iostream>
#include <thread>
#include <chrono>

#define fail(message) std::cout << message << std::endl; std::this_thread::sleep_for(std::chrono::seconds(3)); exit(0);
