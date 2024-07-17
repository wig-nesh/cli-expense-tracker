#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    // account create name opening_amount
    // account list

    string name;
    float opening_amount;

    unordered_map<string, float> account_info_hash;
    ifstream in("account_info", ios::binary);
    size_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    for (size_t i = 0; i < size; ++i) {
        size_t key_size;
        in.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));
        string key(key_size, ' ');
        in.read(&key[0], key_size);
        float value;
        in.read(reinterpret_cast<char*>(&value), sizeof(value));
        account_info_hash[key] = value;
    }
    in.close();

    if (string(argv[1]) == "create") {
        name = argv[2];
        opening_amount = stof(argv[3]);

        if (account_info_hash.find(name) == account_info_hash.end()) {
            account_info_hash[name] = opening_amount;

            ofstream out("account_info", ios::binary);
            size_t size = account_info_hash.size();
            out.write(reinterpret_cast<char*>(&size), sizeof(size));
            for (const auto& pair : account_info_hash) {
                size_t key_size = pair.first.size();
                out.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));
                out.write(pair.first.c_str(), key_size);
                out.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
            }
            out.close();
        
            fstream fout;
            fout.open("account_info_database.csv", ios::out | ios::app);
            fout << name << ", " << opening_amount << "\n";
            fout.close();
        }
        else cout << "Account name already exists.\n";
    }
    else if (string(argv[1]) == "list") {
        for (const auto& pair : account_info_hash) {
            cout << pair.first << " " << pair.second << "\n";
        }
    }


    return 0;
}