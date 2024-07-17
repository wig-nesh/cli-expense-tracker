#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

    string name = "cash";
    float opening_amount = 10000;

    unordered_map<string, float> account_info_hash;
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

    return 0;
}