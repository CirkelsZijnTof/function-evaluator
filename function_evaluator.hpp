#ifndef FUNCTION_EVALUATOR_HPP
#define FUNCTION_EVALUATOR_HPP

// made by Jörgen 30-09-2026

#include <iostream>
#include <string>
#include <cassert> // for assert
#include <typeinfo> // for typeid
#include <chrono>
#include <functional>
#include <array>

// a method for evaluating a function's speed and function
// includes unit test and time complexity support

class Probe 
{
private:

const std::array<int, 3> color_green = {0, 180, 0};
const std::array<int, 3> color_red = {180, 0, 0};

void
printRGBColored(std::array<int, 3> RGB, const char& text) 
{
    std::cout << "\x1b[38;2;"
            << RGB[0] << ";"
            << RGB[1] << ";"
            << RGB[2] << "m"
            << text;

    std::cout << "\x1b[0m";
};

void
printRGBColored(std::array<int, 3> RGB, const std::string& text) 
{
    std::cout << "\x1b[38;2;"
            << RGB[0] << ";"
            << RGB[1] << ";"
            << RGB[2] << "m"
            << text;

    std::cout << "\x1b[0m";
};

public:

    std::string getTypeName(const std::type_info& typeInfo)
    {
    
    return typeInfo.name();
    };

    // analyse the time complexity of a given function with the option of adding args.
    // returns the runtime in seconds. To smoothe out noise in the measurement, there's an option to
    // run the function multiple times and return the average execution time.
    // use of lambda requires [&](){YOUR_FUNCTION(args)}; wrapper.
    template <typename Func>
    double
    time_complexity(Func&& func)
    {
        
        auto start = std::chrono::steady_clock::now();
        
        func();
                
        auto end = std::chrono::steady_clock::now();

    return std::chrono::duration<double>(end - start).count();
    };

    template <typename Func>
    double
    time_complexity(int amount_of_probes, Func&& func)
    {
        
        auto start = std::chrono::steady_clock::now();
        
        for (int i = 0; i < amount_of_probes; i++)
        {
            func();
        };
        
        auto end = std::chrono::steady_clock::now();

    return std::chrono::duration<double>(end - start).count();
    };

    // use of lambda requires [&](){YOUR_FUNCTION(args)}; wrapper.
    template <typename Func, typename... Args>
    double
    time_complexity(Func&& func, Args&&... args)
    {
        
        auto start = std::chrono::steady_clock::now();
        
        std::invoke(func, args...);
        
        auto end = std::chrono::steady_clock::now();

    return std::chrono::duration<double>(end - start).count();
    };
    
    template <typename Func, typename... Args>
    double
    time_complexity(int amount_of_probes, Func&& func, Args&&... args)
    {
        
        auto start = std::chrono::steady_clock::now();
        
        for (int i = 0; i < amount_of_probes; i++)
        {
            std::invoke(func, args...);
        }
        
        auto end = std::chrono::steady_clock::now();

        return std::chrono::duration<double>(end - start).count() / amount_of_probes;
    };

    bool evaluate_output(bool condition, const std::string& test_name)
    {
        
        if (condition)
        {

            std::string str = "[CONDITION PASSED] " + test_name + "\n";

            printRGBColored(color_green, str);

            return true;

        } else {

            std::string str = "[CONDITION FAILED] " + test_name + "\n";

            printRGBColored(color_red, str);
        
            return false;
        }

    };

    template <typename Func>
    bool evaluate_crash(const std::string& test_name, Func&& func)
    {
        try
        {
            func();

            std::string str = "[CRASH-TEST PASSED] " + test_name + "\n";

            printRGBColored(color_green, str);

            return true;
        }
        catch (...)
        {
            std::string str = "[CRASH-TEST FAILED] " + test_name + " Exception: " + error.what() + "\n";

            printRGBColored(color_red, str);

            return false;
        }
    };

    template <typename Func, typename... Args>
    bool evaluate_exception(const std::string& test_name, Func&& func, Args&&... args)
    {
        
        try
        {
            std::invoke(
                std::forward<Func>(func),
                std::forward<Args>(args)...
            );

            std::string str = "[CRASH-TEST PASSED] " + test_name + "\n";

            printRGBColored(color_green, str);

            return true;
        }
        catch (const std::exception& error)
        {
            std::string str = "[CRASH-TEST FAILED] " + test_name + " Exception: " + error.what() + "\n";

            printRGBColored(color_red, str);

            return false;
        }
    };

};

#endif