#include <asio.hpp>
#include <CLI11.hpp>
#include <fstream>
#include <string>
//#include <spdlog/spdlog.h>
#include <thread>
#include <iostream>



using namespace asio::ip;
using namespace CLI;

using namespace std;


void connect_to_server(){ //Starts the connection to the server
   
    asio::io_context ctx;
    tcp::resolver resolver{ctx};
    auto results = resolver.resolve("localhost", "2400");
    tcp::socket sock{ctx};

    try {
        while (true){
            asio::connect(sock, results);
        }
        } catch (const std::exception& e) {
            cout << "lost connection to the server!" << endl;
        }
    
}


int main(int argc, char* argv[]) 
{
    string word;

    App CLI{"Requester for the dictionary"};
    CLI.add_option("-w", word, "word to translate")->required();

    CLI11_PARSE(CLI, argc, argv);

    connect_to_server();

    return 0;
}
