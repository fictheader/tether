#include <tether/tether.h>
#include <memory>
#include <iostream>


struct TestHooker{
    int operator()(const std::weak_ptr<int> weakptr){ //Hooker takes std::weak_ptr of signal's type and return its type.
        auto shp = weakptr.lock();
        std::cout << "hooker invoked... +100" << std::endl;
        *shp += 100;
        return *shp;
    }
};


int main()
{
    std::cout << "Tether Test 1" << std::endl;

    TetherSig<int> sig; //create a signal object.
    TetherSig<int> sig2;
    TestHooker hooker; //create a hooker object.
    auto tether_ptr = std::make_shared<Tether<int>>(300); //create a shared pointer of Tether.

    std::cout << "default value is " << tether_ptr->get_state() << std::endl; //get a curent state.

    std::cout << "Setting up..." << std::endl;
    tether_ptr->make_tether(sig); //connect a signal to tether.
    tether_ptr->make_tether(sig2); //multiple sigs are OK.
    tether_ptr->make_hooker<TestHooker>(hooker); //connect a hooker to tether.
    sig(tether_ptr->get_state()); //send a signal.
    sig2(tether_ptr->get_state());

    std::cout << "[1] current value is " << tether_ptr->get_state() << std::endl;

    tether_ptr->resolve_hooker(); //resolve all hookers.
    tether_ptr->resolve_tether(); //resolve all signals.
    std::cout <<  "everything is resolved..." << std::endl;

    sig(tether_ptr->get_state()); //not connected
    sig2(tether_ptr->get_state()); //not connected
    std::cout << "[2] current value is " << tether_ptr->get_state() << std::endl;

    TetherSig<int> sig3; //new sig

    const auto sig_id = tether_ptr->make_tether(sig); //keep an id of sig.
    tether_ptr->make_tether(sig3);

    const auto hooker_id = tether_ptr->make_hooker<TestHooker>(hooker); //keep an id of hooker.
    std::cout << "sig and sig3 are reconnected..." << std::endl;

    std::cout << "an old sig works correctly after reconnecting..." << std::endl;
    sig(tether_ptr->get_state());

    std::cout << "new sig..." << std::endl;
    sig3(tether_ptr->get_state());

    std::cout << "[3] current value is " << tether_ptr->get_state() << std::endl;

    tether_ptr->cut_tether(sig_id); //cut a specific sig 
    std::cout << "sig is cut..." << std::endl;

    sig(tether_ptr->get_state()); //not connected
    sig3(tether_ptr->get_state()); //still connected

    std::cout << "[4] current value is " << tether_ptr->get_state() << std::endl;

    tether_ptr->make_tether(sig);
    tether_ptr->make_tether(sig2);
    std::cout << "then all signals are reconnected..." << std::endl;

    tether_ptr->cut_hooker(hooker_id); //cut a specific hooker
    std::cout << "but a hooker is not connected..." << std::endl;

    sig(tether_ptr->get_state()); //connected
    sig2(tether_ptr->get_state()); //connected
    sig3(tether_ptr->get_state()); //connected
    std::cout << "[5] current value is " << tether_ptr->get_state() << std::endl;

    tether_ptr->make_hooker<TestHooker>(hooker); //reconnect a hooker
    std::cout << "a hooker is reconnected..." << std::endl;

    sig(tether_ptr->get_state()); //connected
    sig2(tether_ptr->get_state()); //connected
    sig3(tether_ptr->get_state()); //connected
    std::cout << "[6] current value is " << tether_ptr->get_state() << std::endl;

    std::cout << "goodbye"<< std::endl;

    return 0;
}
