#include <iostream>

void take_lock()
{
    std::cout << "\tTaking a lock on a really big resource!\n";
}

void release_lock()
{
    std::cout << "\tReleasing the lock on an the BIG resource\n";
}

struct Lock
{
    bool have_lock;
    Lock()
    {
        take_lock();
        have_lock = true;
    }
    /*Lock(const Lock&) = delete;
    Lock(Lock && other)
    {
        std::cout << "Stealing the lock!\n";
        other.have_lock = false;
        have_lock = true;
    }*/

    ~Lock()
    {
        if( have_lock ) {
            release_lock();
            have_lock = false;
        }
    }
};

double get_value_from_db() {
    std::cout << "Reading the DB\n";
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

Lock lock_factory()
{
    Lock l;
    return l;

    Lock another;
    return another;
}

int main()
{
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
            Lock l; // lifetime of the lock is tied to the curly braces
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



    std::cout << "\n\n**************\nIndustrializing (Using a factory):\n\n";
    try {
        double work;
        {
            Lock l = lock_factory();
            work = get_value_from_db();
        }
    
        work = do_lots_of_work(work);
    
        {
            Lock l = lock_factory();
            save_result_to_db(work);
        }
    }
    catch( std::exception& exp)
    {
        std::cout << "Caught exception: " << exp.what() << "\n";
    }


    return 0;
}
