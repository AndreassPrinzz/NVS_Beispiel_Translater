#include <asio.hpp>
#include <CLI11.hpp>
#include <fstream>
#include <string>
//#include <spdlog/spdlog.h>
#include <thread>
#include <iostream>
#include <vector>



using namespace asio::ip;
using namespace CLI;

using namespace std;


void connect_to_server(string wort){ //Starts the connection to the server
   
    vector<string> translated_words;
    string word_temp;
    asio::io_context ctx;
    tcp::resolver resolver{ctx};
    auto results = resolver.resolve("localhost", "4200");
    tcp::socket sock{ctx};

    try {
        tcp::iostream strm (asio::connect(sock, results));
        strm << wort;
        strm >> word_temp;
        translated_words.push_back(word_temp);
        for (size_t i = 0; i < translated_words.size(); i++){
            cout << translated_words[i] + "; " << endl; 
        }
        strm.close();
        } catch (const std::exception& e) {
            cout << e.what() << endl;
        }  
}


int main(int argc, char* argv[]) 
{
    string word;

    App CLI{"Requester for the dictionary"};
    CLI.add_option("-w", word, "word to translate")->required();

    CLI11_PARSE(CLI, argc, argv);

    connect_to_server(word);

    return 0;
}
