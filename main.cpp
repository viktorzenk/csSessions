#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

void shiftRight(string &line, int ix);
void shiftLeft(string &line, int ix);

int main()
{
    cout << "\nWelcome to the csTimer session manager!\n--------------------------------------\nDeveloped by /u/vikktorz, shoot me a message on reddit if you have questions!" << endl;
    cout << "\nTo get started, export your times from csTimer, and save the .txt file on your computer." << endl;
    cout << "Copy the search route to the file and paste it below. The route should be in this format: C:\\Users\\vikktorz\\cstimer.txt" << endl;
    cout << "Paste here:" << endl;

    fstream file;
    char path[200];
    while(true){
        cin.getline(path,sizeof(path));
        file.open(path);
        if(file.is_open())
            break;
        else{
            cout << "\nFile not found, please try again! Remember to use backslash (\\, not /), and to include the .txt file extension" << endl;
            cout << "Paste here:" << endl;
        }
    }

    string line;
    getline(file,line);

    int ixFirst[50];
    int ixSecond[50];

    int start[50];
    int stop[50];
    string sessName[50];
    int currentIndex = 0;
    int numberOfSessions = 0;

    for(int i = 1; i < 50; i++){
        if(line.find("\"properties\"",currentIndex) < line.find("\"session",currentIndex))
            break;
        start[i] = line.find("\"session",currentIndex);
        currentIndex = start[i];
        ixFirst[i] = start[i] + 8;
        stop[i] = line.find("\"", currentIndex + 1);
        currentIndex = stop[i] +1;

        numberOfSessions++;
    }

    system("cls");

    cout << "\n\nFound " << numberOfSessions << " sessions:\n";

    currentIndex = line.find("sessionData");
    for(int i = 1; i <= numberOfSessions; i++){
        currentIndex = line.find("\"name", currentIndex) + 14;
        ixSecond[i] = currentIndex - 24;
        while(true){
            if(line[currentIndex] == '\\')
                break;
            sessName[i].push_back(line[currentIndex]);
            currentIndex++;
        }
        cout << "\n" << i << ": " << sessName[i];
    }

    cout << "\n\nPlease type the new order you want your sessions in, using the numbers before the name in the list above" << endl;
    cout << "Separate the numbers with commas" << endl;
    cout << "Example: 3,7,1,9,2,4,6,9,8,10" << endl;

    vector<int> order;
    order.push_back(0); //to offset the indexes

    while(true){
        bool cnt = false;
        cout << "Enter here: " << endl;

        string orderString;
        cin >> orderString;
        replace(orderString.begin(), orderString.end(), ',', ' ');

        stringstream ssin(orderString);
        int temp;
        while (ssin >> temp)
            order.push_back(temp);

        if(order.size() - 1 != numberOfSessions){
            cout << "Number of numbers entered does not match number of sessions, please try again and add all " << numberOfSessions << " numbers" << endl;
            continue;
        }
        for(int i = 1; i <= numberOfSessions; i++){
            if(order[i] > numberOfSessions){
                cout << "The number " << order[i] << " is too large and does not match a session, please try again" << endl;
                cnt = true;
                goto cnt;
            }

            for(int j = i+1; j <= numberOfSessions; j++){
                if(order[i] == order[j]){
                    cout << "The number " << order[i] << " was entered multiple times, please try again" << endl;
                    cnt = true;
                    goto cnt;
                }
            }
        }
        cnt:;
        if(cnt)
            continue;
        break;
    }

    for(int i = 1; i <= numberOfSessions; i++){

        if(order[i] < 10 && i < 10){
            char c = order[i] + 48;

            line[ixFirst[i]] = c;
            line[ixSecond[i]] = c;
        }
        else if(order[i] >= 10 && i >= 10){
            char first = (order[i]/10) + 48;
            char second = (order[i]%10) + 48;
            string c = {first,second};

            line[ixFirst[i]] = c[0];
            line[ixFirst[i] +1] = c[1];
        }
        else if (order[i] >= 10 && i < 10){
            char first = (order[i]/10) + 48;
            char second = (order[i]%10) + 48;
            string c = {first,second};

            shiftRight(line,ixFirst[i]);
            for(int j = 1; j <= numberOfSessions; j++){
                if(ixFirst[j] > ixFirst[i]){
                    ixFirst[j]++;
                }

                ixSecond[j]++;
            }
            line[ixFirst[i]] = c[0];
            line[ixFirst[i] +1] = c[1];

            shiftRight(line,ixSecond[i]);
            for(int j = 1; j <= numberOfSessions; j++){
                if(ixSecond[j] > ixSecond[i]){
                    ixSecond[j]++;
                }
            }
            line[ixSecond[i]] = c[0];
            line[ixSecond[i] +1] = c[1];
        }
        else if (order[i] < 10 && i >= 10){
            char c = order[i] + 48;

            shiftLeft(line, ixFirst[i]);
            for(int j = 1; j <= numberOfSessions; j++){
                if(ixFirst[j] > ixFirst[i]){
                    ixFirst[j]--;
                }

                ixSecond[j]--;
            }
            line[ixFirst[i]] = c;

            shiftLeft(line, ixSecond[i]);
            for(int j = 1; j <= numberOfSessions; j++){
                if(ixSecond[j] > ixSecond[i]){
                    ixSecond[j]--;
                }
            }
            line[ixSecond[i]-1] = c;

        }


    }

    ofstream newfile("cstimerNew.txt");
    newfile << line;

    file.close();
    newfile.close();

    cout << "\n\nSession order successfully changed, and saved to file \"cstimerNew.txt\"." << endl;
    cout << "The file is located in the same folder as the .exe file you're running" << endl;
    cout << "You can now import cstimerNew.txt into cstimer\n\n";

    getchar();
    getchar();
    return 0;
}

//shifts string one index to the right, leaving index ix empty. Elements with indexes < ix are unaffected
void shiftRight(string &line, int ix){
    for(int i = line.size() + 1; i > ix; i--){
         line[i] = line[i-1];
    }
}

//shifts string one index to the left, removing index ix. Elements with indexes < ix are unaffected
void shiftLeft(string &line, int ix){
    for(int i = ix; i < line.size(); i++){
         line[i] = line[i+1];
    }
    line.pop_back();
}
