#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

struct AccountInfo {
    float amount;
    string edited;
    string created;
};

string getCurrentDate() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);

    ostringstream dateStream;
    dateStream << setw(2) << setfill('0') << localTime->tm_mday << '-'
               << setw(2) << setfill('0') << localTime->tm_mon + 1 << '-'
               << setw(2) << setfill('0') << (localTime->tm_year + 1900) % 100;

    return dateStream.str();
}

int main(int argc, char* argv[]) {

    string name = "cash";
    float opening_amount = 10000;
    string date = getCurrentDate();
    AccountInfo info = {opening_amount, date, date};

    unordered_map<string, AccountInfo> account_info_hash;
    account_info_hash[name] = info;
    

    ofstream out("account_info", ios::binary);
    size_t size = account_info_hash.size();
    out.write(reinterpret_cast<char*>(&size), sizeof(size));
    for (const auto& pair : account_info_hash) {
        size_t key_size = pair.first.size();
        out.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));
        out.write(pair.first.c_str(), key_size);

        out.write(reinterpret_cast<const char*>(&pair.second.amount), sizeof(pair.second.amount));
        size_t edited_size = pair.second.edited.size();
        out.write(reinterpret_cast<const char*>(&edited_size), sizeof(edited_size));
        out.write(pair.second.edited.c_str(), edited_size);
        size_t created_size = pair.second.created.size();
        out.write(reinterpret_cast<const char*>(&created_size), sizeof(created_size));
        out.write(pair.second.created.c_str(), created_size);        
    }
    out.close();

    fstream fout;
    fout.open("account_info_database.csv", ios::out | ios::app);
    fout << name << ", " << opening_amount << ", " << date << ", " << date << "\n";
    fout.close();

    return 0;
}