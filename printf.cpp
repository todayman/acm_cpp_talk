#include <cstdio>
#include <iostream>
#include <string>

namespace acm
{

    template<typename... Args>
    void printf(const char* format, Args... args);

    template<typename... MoreArgs>
    void printf_arg(const char * format, int val, MoreArgs... args)
    {
        if( format[0] != 'd' ) {
            std::cerr << "format = " << format;
            throw std::runtime_error("Provided integer for another format specifier");
        }
        std::printf("%d", val);
        acm::printf(format + 1, args...);
    }

    void printf_arg(const char * format) { } // base case for printf(format)

    template<typename... Args>
    void printf(const char* format, Args... args)
    {
        for( unsigned i = 0; format[i] != 0; ++i )
        {
            char ch = format[i];
            if( ch != '%' )
                std::printf("%c", ch);
            else {
                printf_arg(format + i + 1, args...);
                break;
            }
        }
    }

    void printf(const char* format)
    {
        std::printf(format);
    }
}

int main()
{
    acm::printf("Number %d\n", 5);
}
