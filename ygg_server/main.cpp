#include "manager.h"
#include "localbox_manager.h"
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <vector>


int main() {

    LocalBoxManager m;
    m.Start();

    try {
        IOService io_service;
        Manager manager(io_service);
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    m.Stop();

    return 0;
}