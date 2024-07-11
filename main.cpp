#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
using namespace std;

int main() {
  cout << "no. of transactions: ";
  int num_transactions;
  cin >> num_transactions;
  cin.ignore();

  for (int i = 0; i < num_transactions; i++) {
    // 1. type
    // + -> gain money
    // - -> lose money
    // 2. amount
    // 3. date
    // dd-mm-yy
    // 4. note
    // 5. tags
    // 2, 3, 4, 5 are comma separated, end with full stop
    string entry;
    getline(cin, entry);
    
    float amount = 0;
    string date;
    string note;
    vector<string> tags;

    if (entry[0] == '+' || entry[0] == '-') {

      int idx = 1;
      bool decimal = false;
      while (entry[idx] != ',') {
        if (entry[idx] == '.') {
          decimal = true;
          idx++;
          continue;
        }
        amount *= decimal ? 10 : 0.1;
        amount += entry[idx] - 48;
        idx++;
      }
      if (entry[0] == '-') amount *= -1;
      
      idx++;
      for (int j = 0; j < 8; j++) {date += entry[idx + j];}
      
      idx++;
      idx += 8;
      while (entry[idx] != ',') {note += entry[idx++];}
      
      idx++;
      string tag;
      while (entry[idx] != '.') {
        if (entry[idx] == ',') {
          tags.push_back(tag);
        while (entry[idx] != ',') {tag += entry[idx++];}
          tag.clear();
        }
        else tag += entry[idx];
        idx++;
      }
      if (!tag.empty()) tags.push_back(tag);
    }
    else {
      cout << "invalid entry, try again\n";
      i--;
    }

    fstream fout;
    fout.open("expense_database.csv", ios::out | ios::app);
    fout << amount << ", " << date << ", " << note;
    for (string &s : tags) {
      fout << ", " << s;
    }
    fout << "\n";

  }
  
  return 0;
}
