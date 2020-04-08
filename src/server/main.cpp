#include <fstream>
#include <string>
#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <regex>
#include <utility>
#include <locale>


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
    string temp_string = "";
    try{
        string line;
        ifstream in(filename);
        while (getline(in, line)){
            if (line[0] == '#'){
                continue;
            }
            
            vector<string> temp = split(line, " :: "); // [0] = deutsch | [1] = englisch
            
            vector<string> parts_de_with_semi;
            vector<string> parts_en_with_semi;

            vector<string> parts_de;
            vector<string> parts_en;
            
            parts_de_with_semi = split(temp[0], "\\|"); 
            
            parts_en_with_semi = split(temp[1], "\\|"); 
            
            for (size_t i{0}; i < parts_de_with_semi.size(); i++) {
                
            
                /*
                for (size_t i = 0; i < parts_de_with_semi.size(); i++){
                    cout << parts_de_with_semi[i] << endl;
                }

                for (size_t i = 0; i < parts_en_with_semi.size(); i++){
                    cout << parts_en_with_semi[i] << endl;
                }
                */

                parts_de = split(parts_de_with_semi[i], ";"); 
                parts_en = split(parts_en_with_semi[i], ";"); 
                /*
                for (size_t i = 0; i < parts_de.size(); i++){
                    cout << parts_de[i] << endl;
                }

                for (size_t i = 0; i < parts_en.size(); i++){
                    cout << parts_en[i] << endl;
                }
                */

                for (string part : parts_de){
                    for (size_t i = 0; i < part.length(); i++){
                        if (part[i] != '[' && part[i] != '{' && part[i] != '('){
                            if ((i + 1) < part.length())
                                temp_string += part[i];
                        } else {
                            if (i < part.length()){
                                i++;
                            }
                            while ((i) < part.length() && (part[i] != ']' || part[i] != '}' || part[i] != ')')){
                                i++;
                            }
                        }
                    }
                    if (temp_string[temp_string.length() - 1] == ' '){
                        temp_string.pop_back();
                    }

                    if (temp_string[0] ==  ' '){
                        temp_string.erase(temp_string.begin());
                    }


                    locale loc;
                    string temp_lower;
                    for (auto c : temp_string){
                        temp_lower += tolower(c, loc);
                    }

                    part = temp_lower;
                    temp_string = "";
                    auto search = dict.find(part);
                    if (search == dict.end()){
                        dict.insert(pair(part, vector<string> {}));
                    }
                    for (size_t i = 0; i < parts_en.size(); i++){
                        dict[part].push_back(parts_en[i]);
                    }
                } 
            }
            
            
        }
        in.close();
    } catch(const exception& e){//compiling text file failed
        cerr << e.what() << endl;
    }
    
    return dict;
}

vector<string> get_word(string word, map<string, vector<string>> &dictionary){
    try {
        return dictionary.at(word);
    } catch (...) {
        return vector <string> {};
    }
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
                if (word_translate.size() == 0){
                    strm << "no translations found" << endl;
                } else {
                    for (size_t i = 0; i < word_translate.size(); i++){
                        strm << word_translate[i] << endl;;
                    }
                }
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
    /* 
    for(auto ii=dictionary.begin(); ii!=dictionary.end(); ++ii){ //output of the dictionary
        cout << (*ii).first << ": ";
        vector <string> inVect = (*ii).second;
        for (unsigned j=0; j<inVect.size(); j++){
            cout << inVect[j] << " ";
        }
        cout << endl;
    }  
    */
    start_server(dictionary);

    return 0;
}