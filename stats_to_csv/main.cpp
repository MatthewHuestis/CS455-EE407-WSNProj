#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;





vector<string> splitByAt(string& line) {
    vector<string> parts({});
    string buf("");
    for(uint i = 0; i < line.size(); i++) {
        char c = line.at(i);
        if(c == '@' && buf.length() > 0) {
            parts.push_back(buf); 
            buf.clear();
        } else {
            buf += c;
        }
    }
    return parts;
}

bool isStat(string& line) {
    vector<string> parts = splitByAt(line);
    if(parts.size() < 1) { return false; }
    if(parts.at(0) != "STATS") { return false; }
    return true;
}

bool isEvent(string& line) {
    vector<string> parts = splitByAt(line);
    if(parts.size() < 2) { return false; }
    if(parts.at(0) != "STATS") { return false; }
    if(parts.at(1) != "EVENT") { return false; }
    return true;
}

bool isNode(string& line) {
    vector<string> parts = splitByAt(line);
    if(parts.size() < 2) { return false; }
    if(parts.at(0) != "STATS") { return false; }
    if(parts.at(1) != "NODE") { return false; }
    return true;
}

int main() {
    cout << "TIME,ADDRESS,"
    while(!cin.eof()) {
        string line;
        getline(cin, line);
        if(!isStat(line)) { goto skip_line; }
        if(isEvent(line))
        skip_line:
    }
    
}