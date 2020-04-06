#include <fstream>
#include <string>
#include <iostream>
#include <thread>
#include <map>

#include <asio.hpp>
#include <CLI11.hpp>
///#include <spdlog/spdlog.h>

using namespace std;
using namespace asio::ip;
using namespace CLI;
/*
map<char, char> create_dic(string filename){
    map <char, char> temp;

    try{
        ifstream i;
        i.open(filename);
        string line;
        while (getline(i, line)){
            cout << line << endl;
        }
        i.close();
    } catch(...){//compiling text file failed
        cout << "Cant open text file!" << std::endl;
        return;
    }
    
    return temp;
}

*/
/*
void create_dic(string filename){

    try{
        string line;
        ifstream in('"' + filename + '"');
        cout << "before while" << endl;
        while (getline(in, line)){
            cout << line << endl;
        }
        cout << "after while" << endl;
        in.close();
    } catch(...){//compiling text file failed
        cout << "Cant open text file!" << std::endl;
        return;
        }
    }
*/

void start_server() { //starts the server

    try{
        asio::io_context ctx;
        tcp::endpoint ep{tcp::v4(), 2400};
        tcp::acceptor acceptor{ctx, ep};
        acceptor.listen();
        tcp::socket sock{ctx};
        cout << "Server started" << endl;
        while (true) {
            acceptor.accept(sock);
        }
    } catch (...) { //lost connection
        cout << "error ..." << endl;
    }
}

int main(int argc, char* argv[]) {

    string filename{"test.txt"};
    //map <char, char> dic;

    App CLI{"German-English Dictionary"};
    
    CLI.add_option("-f", filename, "file for the dictionary");

    CLI11_PARSE(CLI, argc, argv);

    create_dic(filename);
    start_server();

    return 0;
}