#include <cstdio>
#include <stdexcept>

namespace acm {
    
    template<typename... T>
    void print_arg(const char * format, int value, T... args);
    template<typename... T>
    void print_arg(const char * format, double value, T... args);

    void print_arg(const char * format)
    {
        std::printf("%s", format);
    }
    
    template<typename... T>
    void printf(const char* format, T... arg)
    {
        for( int i = 0; format[i] != 0; ++i )
        {
            if( format[i] != '%' )
                std::printf("%c", format[i]);
            else {
                print_arg(format + i + 1, arg...);
                return;
            }
        }
    }

    template<typename... T>
    void print_arg(const char * format, int value, T... args)
    {
        if( format[0] != 'd' )
            throw std::runtime_error("Bad formatting string");
        std::printf("%d", value);
        acm::printf(format + 1, args...);
    }
    template<typename... T>
    void print_arg(const char * format, double value, T... args)
    {
        if( format[0] != 'f' )
            throw std::runtime_error("Bad formatting string");
        std::printf("%f", value);
        acm::printf(format + 1, args...);
    }
}

int main()
{
    std::printf("Hello world!\n");

    std::printf("Here's a number %d\n", 5);
    std::printf("Here's a double: %f\n", 6.7);

    acm::printf("We're printing %d\n", 5); 
    return 0;
}
