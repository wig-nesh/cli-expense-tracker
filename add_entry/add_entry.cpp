#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void parseInput(const string& input, float& amount, bool& a, string& date, bool& d, string& note, bool& n, vector<string>& tags) {
    regex amountRegex(R"([+-]?\d+(\.\d+)?)");
    regex dateRegex(R"(-d\s+(\d{2}-\d{2}-\d{2}))");
    regex noteRegex(R"(-n\s+([^"]*))");
    regex tagsRegex(R"(-t\s+([\w,]+))");
    smatch match;

    if (regex_search(input, match, amountRegex)) {
        amount = stof(match[0]);
        a = true;
    } 
    else a = false;

    if (regex_search(input, match, dateRegex)) {
        date = match[1];
        d = true;
    } 
    else d = false;

    if (regex_search(input, match, noteRegex)) {
        note = match[1];
        n = true;
    } 
    else n = false;

    if (regex_search(input, match, tagsRegex)) {
        string tagsString = match[1];
        stringstream ss(tagsString);
        string tag;
        while (getline(ss, tag, ',')) {
            tags.push_back(tag);
        }
    }
}

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
  
  int num_transactions = stoi(argv[1]);

  for (int i = 0; i < num_transactions; i++) {

    // +-amount -d dd-mm-yy -n note with spaces -t tag1,tag2,tag3

    string entry;
    char c;
    while (true) {
      c = cin.get();
      if (c == '\n') break;
      if (c == 8) { // handles backspace
        if (!entry.empty()) {
          entry.pop_back();
          cout << "\b \b";
        }
      }
      else entry += c;
    }

    float amount = 0;
    string date;
    string note;
    vector<string> tags;

    bool a = false;
    bool d = false;
    bool n = false;

    parseInput(entry, amount, a, date, d, note, n, tags);


    if (!a) {
      cout << "no amount entered, try again\n";
      i--;
      continue;
    }
    if (!d) {
      date = getCurrentDate();
    }


    fstream fout;
    fout.open("expense_database.csv", ios::out | ios::app);
    fout << amount << ", " << date << ", " << note;
    for (string &s : tags) {
      fout << ", " << s;
    }
    fout << "\n";
    
    amount = 0;
    date.clear();
    note.clear();
    tags.clear();

  }
  
  return 0;
}