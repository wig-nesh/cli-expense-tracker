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
    // account create name opening_amount
    // account list
    // account modify old_name new_name

    string name;
    float opening_amount;

    unordered_map<string, AccountInfo> account_info_hash;
    ifstream in("account_info", ios::binary);
    size_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    for (size_t i = 0; i < size; ++i) {
        size_t key_size;
        in.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));
        string key(key_size, ' ');
        in.read(&key[0], key_size);

        AccountInfo info;
        in.read(reinterpret_cast<char*>(&info.amount), sizeof(info.amount));
        size_t edited_size;
        in.read(reinterpret_cast<char*>(&edited_size), sizeof(edited_size));
        string edited_(edited_size, ' ');
        in.read(&edited_[0], edited_size);
        size_t created_size;
        in.read(reinterpret_cast<char*>(&created_size), sizeof(created_size));
        string created_(created_size, ' ');
        in.read(&created_[0], created_size);
        
        info.edited = edited_;
        info.created = created_;

        account_info_hash[key] = info;
    }
    in.close();

    if (string(argv[1]) == "create") {
        name = argv[2];
        opening_amount = stof(argv[3]);
        string date = getCurrentDate();
        AccountInfo info = {opening_amount, date, date};

        if (account_info_hash.find(name) == account_info_hash.end()) {
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
            fout << name << ", " << info.amount << ", " << info.edited << ", " << info.created << "\n";
            fout.close();
        }
        else cout << "Account name already exists.\n";
    }
    else if (string(argv[1]) == "list") {
        for (const auto& pair : account_info_hash) {
            cout << pair.first << " " << pair.second.amount << " " << pair.second.edited << " " << pair.second.created << "\n";
        }
    }
    else if (string(argv[1]) == "modify") {
        string old_name = argv[2];
        string new_name = argv[3];

        if (account_info_hash.find(old_name) != account_info_hash.end()) {
            AccountInfo info = account_info_hash[old_name];
            info.edited = getCurrentDate();
            account_info_hash[new_name] = info;
            account_info_hash.erase(old_name);

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
            fout << new_name << ", " << info.amount << ", " << info.edited << ", " << info.created << "\n";
            fout.close();
        }
        else cout << "Account name does not exist. Use create to make a new account.\n";

    }


    return 0;
}