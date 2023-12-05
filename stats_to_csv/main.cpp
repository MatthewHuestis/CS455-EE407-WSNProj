#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

vector<string> splitByAt(string& line) {
    vector<string> parts({});
    string buf("");
    for(uint i = 0; i < line.size(); i++) {
        char c = line.at(i);
        if(c == '@') {
            if(buf.length() > 0) {
                parts.push_back(buf); 
                buf.clear();
            }
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
    if(parts.size() < 4) { return false; }
    if(parts.at(0) != "STATS") { return false; }
    if(parts.at(3) != "EVENT") { return false; }
    return true;
}

bool isNode(string& line) {
    vector<string> parts = splitByAt(line);
    if(parts.size() < 4) { return false; }
    if(parts.at(0) != "STATS") { return false; }
    if(parts.at(3) != "NODE") { return false; }
    return true;
}

int main() {
    cout << "TIME,ADDRESS,HOP_TABLE_SIZE,POSITION_X,POSITION_Y,ERROR_X,ERROR_Y,EVENTCODE";
    while(!cin.eof()) {
        string line;
        getline(cin, line);
        if(isStat(line)) {
            if(isEvent(line)) {
                vector<string> parts = splitByAt(line);
                string time = parts.at(2);
                string event_code = parts.at(4);
                cout << time << ",,,,,,," << event_code << "\n"; 
            } else if(isNode(line)) {
                vector<string> parts = splitByAt(line);
                string time = parts.at(2);
                string address = parts.at(4);
                string hop_table_size = parts.at(6);
                string position_x = parts.at(8);
                string position_y = parts.at(10);
                string eventcode = "NONE";
                cout << time << "," << address << "," << hop_table_size << ",";
                cout << position_x << "," << position_y << "," << eventcode << "\n";
            }
        }
    }
}