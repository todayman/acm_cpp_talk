#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using std::cout;

struct AThing
{
    int number;
    std::string name;

    AThing(int n, std::string na)
        : number(n), name(na)
    { }

    // Make it non-copyable
    AThing(const AThing& other) = delete;
    AThing(AThing && other) = delete;

    ~AThing() = default;
};

std::ostream& operator<<(std::ostream& output, const AThing& thing)
{
    return output << "(" << thing.name << ": " << thing.number << ")";
}

void take_lock()
{
    std::cout << "Taking a lock on a really big resource!\n";
}

void release_lock()
{
    std::cout << "Releasing the lock on an the BIG resource\n";
}

struct Lock
{
    bool have_lock;
    Lock()
    {
        take_lock();
        have_lock = true;
    }

    ~Lock()
    {
        if( have_lock ) {
            release_lock();
            have_lock = false;
        }
    }
};

double get_value_from_db() {
    return 4; // See http://xkcd.com/221/
}

double do_lots_of_work(double input)
{
    std::cout << "Hardly working\n";
    return input;
}

void save_result_to_db(double result)
{
    throw std::runtime_error("Oh no!  Saving the result failed!");
    std::cout << "writng the result (" << result << ") to stdout\n";
}

template<typename T>
std::string toString(T val)
{
    // outputs result to a string
    std::ostringstream strm;
    strm << val;
    return strm.str();
}

int main()
{
    cout << "Hi there!\n";
    
    // write to an actual file
    std::ofstream output("test_output");
    // also std::ifstream
    // these are subclasses of std::ostream and std::istream

    output << "write some stuff to the file\n";
    AThing thing(5, "mine");
    output << thing << "\n";

    char important_customer_data[] = { 1, 2, 3, 4, 5 };
    //char * cust_data = new char[5];
    output.write(important_customer_data, 5);

    /*for( char ch : important_customer_data ) {
        std::cout << "*looks both ways* Here's some customer data: " << static_cast<int>(ch) << "\n";
        cust_data[ch - 1] = ch;
    }

    for( char ch : cust_data )
        std::cout << "*looks both ways* Here's some customer data: " << static_cast<int>(ch) << "\n";*/


    // really should do:
    std::array<char, 5> super_important_customer_data { 6, 7, 8, 9, 0};
    output.write(super_important_customer_data.data(), super_important_customer_data.size());

    // we could do
    output.close();
    // or we could just let the stream go out of scope

    std::cout << "\n\n**************\nDoing it badly:\n\n";
    try {
        // take lock on DB -- don't do this
        take_lock();   
        double work = get_value_from_db();
        release_lock();

        work = do_lots_of_work(work);

        take_lock();
        save_result_to_db(work);
        release_lock();
    }
    catch( std::exception& exp)
    {
        std::cout << "Caught exception: " << exp.what() << "\n";
    }

    std::cout << "\n\n**************\nDoing it better:\n\n";
    try {
        double work;
        // Using RAII
        {
            Lock l;
            work = get_value_from_db();
        }
    
        work = do_lots_of_work(work);
    
        {
            Lock l;
            save_result_to_db(work);
        }
    }
    catch( std::exception& exp)
    {
        std::cout << "Caught exception: " << exp.what() << "\n";
    }
}
