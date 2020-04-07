#include <fstream>
#include <string>
#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <regex>
#include <utility>


#include <asio.hpp>
#include <CLI11.hpp>
//#include <spdlog/spdlog.h>

using namespace std;
using namespace asio::ip;
using namespace CLI;

vector<string> split(const string & s, string rgx_str) {


      vector<string> elems;

      regex rgx (rgx_str);

      sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
      sregex_token_iterator end;

      while (iter != end)  {
          elems.push_back(*iter);
          ++iter;
      }

      return elems;

  }

map<string, vector<string>> create_dic(string filename){
    map<string, vector<string>> dict;
    string reg = "[}]";
    try{
        string line;
        ifstream in(filename);
        while (getline(in, line)){
            if (line[0] == '#'){
                continue;
            }
            
            vector<string> temp = split(line, " :: "); // [0] = deutsch | [1] = englisch
            /*
            for (size_t i = 0; i < temp.size(); i++){
                cout << temp[i] << endl;
            }
            */

            vector<string> parts_de_with_pipe_semicolon;
            vector<string> parts_en_with_pipe_semicolon;

            vector<string> parts_de_with_semi;
            vector<string> parts_en_with_semi;

            vector<string> parts_de;
            vector<string> parts_en;
            
            parts_de_with_pipe_semicolon = split(temp[0], reg); 
            parts_en_with_pipe_semicolon = split(temp[1], reg);

            for (size_t i = 0; i < parts_de_with_pipe_semicolon.size(); i++){
                parts_de_with_semi = split(parts_de_with_pipe_semicolon[i], "\\|"); 
            }
            for (size_t i = 0; i < parts_en_with_pipe_semicolon.size(); i++){
                parts_en_with_semi = split(parts_en_with_pipe_semicolon[i], "\\|"); 
            }
            /*
            for (size_t i = 0; i < parts_de_with_semi.size(); i++){
                cout << parts_de_with_semi[i] << endl;
            }

            for (size_t i = 0; i < parts_en_with_semi.size(); i++){
                cout << parts_en_with_semi[i] << endl;
            }
            */

            for (size_t i = 0; i < parts_de_with_semi.size(); i++){
                parts_de = split(parts_de_with_semi[i], ";"); 
            }

            for (size_t i = 0; i < parts_en_with_semi.size(); i++){
                parts_en = split(parts_en_with_semi[i], ";"); 
            }
            /*
            for (size_t i = 0; i < parts_de.size(); i++){
                cout << parts_de[i] << endl;
            }

            for (size_t i = 0; i < parts_en.size(); i++){
                cout << parts_en[i] << endl;
            }
*/
/*
            for (size_t i = 0; i < parts_de.size(); i++){
                dict.insert(pair(parts_de[i], vector<string> {}));
            }

            for(auto ii=dict.begin(); ii!=dict.end(); ++ii){
                cout << (*ii).first << ": ";
                vector <string> inVect = (*ii).second;
                for (unsigned j=0; j<inVect.size(); j++){
                    cout << inVect[j] << " ";
                }
                cout << endl;
            }
            */
            for (string part : parts_de){
                auto search = dict.find(part);
                if (search == dict.end()){
                    dict.insert(pair(part, vector<string> {}));
                } if (part[0] == '[' || part[0] == '{'){
                    continue;
                }
                for (size_t i = 0; i < parts_en.size(); i++){
                    if (parts_en[i][0] == '[' || parts_en[i][0] == '{'){
                        continue;
                    }
                    dict[part].push_back(parts_en[i]);
                }
            }       
            /*
            for(auto ii=dict.begin(); ii!=dict.end(); ++ii){
                cout << (*ii).first << ": ";
                vector <string> inVect = (*ii).second;
                for (unsigned j=0; j<inVect.size(); j++){
                    cout << inVect[j] << " ";
                }
                cout << endl;
            }
            */
            
        }
        in.close();
    } catch(const exception& e){//compiling text file failed
        cerr << e.what() << endl;
    }
    
    return dict;
}

vector<string> get_word(string word, map<string, vector<string>> &dictionary){
    for (auto i = dictionary.begin(); i != dictionary.end(); i++){
        if (word == (*i).first){
            return (*i).second;
        }
    }
    return vector<string> {};
}

void start_server(map<string, vector<string>> &dictionary) { //starts the server
    string word;
    vector<string> word_translate;
    try{
        cout << "Server running" << endl;
        asio::io_context ctx;
        tcp::endpoint ep{tcp::v4(), 4200};
        tcp::acceptor acceptor{ctx, ep};
        while (true) {
            tcp::iostream strm;
            acceptor.accept(strm.socket());
            if (strm){
                getline(strm, word);
                word_translate = get_word(word, dictionary);
                for (size_t i = 0; i < word_translate.size(); i++){
                    strm << word_translate[i];
                }
                strm << word_translate;
            }
            strm.close();
        }
    } catch (...) { //lost connection
        cout << "error ..." << endl;
    }
}

int main(int argc, char* argv[]) {

    string filename{"../de-en.txt"};

    App CLI{"German-English Dictionary"};
    
    CLI.add_option("-f", filename, "file for the dictionary");

    CLI11_PARSE(CLI, argc, argv);

    map<string, vector<string>> dictionary {create_dic(filename)};
    start_server(dictionary);

    return 0;
}