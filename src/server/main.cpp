#include <fstream>
#include <string>
#include <iostream>
#include <thread>

#include <asio.hpp>
#include <CLI11.hpp>
///#include <spdlog/spdlog.h>

using namespace std;
using namespace asio::ip;
using namespace CLI;


void start_server() { //starts the server and creates the dictionary
/*
    try{
        std::ifstream i{input_file};
        i >> j;
    } catch(...){//compiling text file failed
        cout << "Invalid text file!" << std::endl;
        return;
    }
*/
    try{
        asio::io_context ctx;
        tcp::endpoint ep{tcp::v4(), 2400};
        tcp::acceptor acceptor{ctx, ep};
        acceptor.listen();
        tcp::socket sock{ctx};
        acceptor.accept(sock);
        cout << "Server started" << endl;
        while (true) {

            }
    } catch (...) { //lost connection to the client
        cout << "error ..." << endl;
    }
}

int main(int argc, char* argv[])     {
    string filename{"de-en.txt"};
    
    App CLI{"German-English Dictionary"};
    
    CLI.add_option("-f", filename, "file for the dictionary");

    CLI11_PARSE(CLI, argc, argv);

    cout << filename << endl;

    start_server();

    return 0;
}