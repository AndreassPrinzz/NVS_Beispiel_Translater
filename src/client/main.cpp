#include <asio.hpp>
//#include <clipp.h>
#include <fstream>
#include <string>
//#include <spdlog/spdlog.h>
#include <thread>
#include <iostream>



using namespace asio::ip;
//using namespace clipp;

using namespace std;


void connect_to_server(){ //Starts the connection to the server
    try{

    } catch (...) {
        cout << "Invalid json file!" << endl;
        return;
    }
    asio::io_context ctx;
    tcp::resolver resolver{ctx};
    auto results = resolver.resolve("localhost", "2400");
    tcp::socket sock{ctx};

    try {
        while (true){
            asio::connect(sock, results);
        }
        } catch (const std::exception& e) {
            cout << "lost connection to the elevator!" << endl;
        }
    
}


int main() 
{
    connect_to_server();

    return 0;
}
