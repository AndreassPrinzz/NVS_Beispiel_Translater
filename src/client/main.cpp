#include <fstream>
#include <string>
#include <thread>
#include <iostream>
#include <vector>
#include <locale>


#include <src/includes.cpp>

using namespace asio::ip;
using namespace CLI;

using namespace std;


void connect_to_server(string wort){ //Starts the connection to the server
    string word_translate;
    /*
    asio::io_context ctx;
    tcp::resolver resolver{ctx};
    auto results = resolver.resolve("localhost", "4200");
    tcp::socket sock{ctx};
    */

    try {
        tcp::iostream strm {"localhost", "4200"};
        strm << wort << endl;
        if (strm){
            cout << "Translations of '" + wort + "': " + "\n" << endl;
        while (getline(strm, word_translate)){
            cout << word_translate << endl;
        }
        cout << endl;
        } else {
            spdlog::error("lost connection to server", 1);
        }
        strm.close();
        } catch (const std::exception& e) {
            spdlog::error("could not connect to server", 1);
        }  
}


int main(int argc, char* argv[]){
    string word;

    App CLI{"Requester for the dictionary"};
    CLI.add_option("-w", word, "word to translate")->required();

    CLI11_PARSE(CLI, argc, argv);

    locale loc;
    string temp_lower;
    for (auto c : word){
        temp_lower += tolower(c, loc);
    }

    word = temp_lower;

    connect_to_server(word);

    return 0;
}
