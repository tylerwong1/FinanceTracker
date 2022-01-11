// FinanceTracker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "WFEntry.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

string categorizeWFNote(string token) {
    vector<string> keywords = { "VENMO CASHOUT", "VENMO REPAYMENT", "ZELLE", "CAMPUS CIRCLE",
        "VANGUARD", "APPLECARD", "UNIVERSITY OF FL", "DISCOVER", "YOTTA" };

    for (const auto& keyword : keywords) {
        // transforms token to all uppercase
        transform(token.begin(), token.end(), token.begin(), ::toupper);
        auto pos = token.find(keyword);
        // keyword has been found
        if (pos != string::npos) {
            return keyword;
        }
    }
    
    return token;
}

void calcEndBal(vector<WFEntry>& entries, double inialBal) {
    
    // calculate initial ending bal
    entries[entries.size() - 1].endBalance = entries[entries.size() - 1].amount + inialBal;

    for (int i = entries.size() - 2; i > -1; i--) {
        entries[i].endBalance = entries[i].amount + entries[i + 1].endBalance;
    }
}



vector<WFEntry> readFile(string filename) {

    vector<WFEntry> entries;
    string line;
    ifstream myFile(filename);

    // check if file can be opened
    if (!myFile.is_open()) {
        throw runtime_error("Error could not open input file...");
    }

    while (getline(myFile, line)) {
        WFEntry temp;
        string token;
        stringstream ss(line);

        getline(ss, token, ',');
        // removes the " marks
        token = token.substr(1, token.size() - 2);
        temp.date = token;

        try {
            getline(ss, token, ',');
            // removes the " marks
            token = token.substr(1, token.size() - 2);
            temp.amount = stod(token);
        }
        catch (invalid_argument()) {
            throw runtime_error("Error converting string to double...");
        }

        // skip two cells
        getline(ss, token, ',');
        getline(ss, token, ',');

        getline(ss, token, ',');
        // removes the " marks
        token = token.substr(1, token.size() - 2);
        temp.note = categorizeWFNote(token);

        entries.push_back(temp);

        //cout << temp.date << " " << temp.amount << " " << temp.note << endl;
    }
    // close file
    myFile.close();

    return entries;
}

void writeFile(vector<WFEntry> entries) {
    ofstream newFile;
    newFile.open("FinanceTracker.csv");

    // set headings for new file
    newFile << "DATE,DEPOSITS,WITHDRAWALS,NOTE,,ENDING BALANCE\n";


    for (int i = entries.size() - 1; i > -1; i--) {
        // deposits
        if (entries[i].amount >= 0)
            newFile << entries[i].date << ',' << entries[i].amount << ",," << entries[i].note << ",," << entries[i].endBalance << endl;
        // withdrawals
        else
            newFile << entries[i].date << ",," <<entries[i].amount << ',' << entries[i].note << ",," << entries[i].endBalance << endl;

    }

    newFile.close();
}

void appendFile(string filename) {




}




int main()
{
    string file = "total.csv";
    double initalBal = 4787.86;


    vector<WFEntry> data = readFile(file);
    calcEndBal(data, initalBal);
    writeFile(data);
    
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
