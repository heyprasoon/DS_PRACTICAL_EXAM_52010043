#include<fstream>
#include<iostream>
#include<stdlib.h>
#include<string>
#include<conio.h>
using namespace std;

void menu2();
void login();
void signup();
void frontpage();

int loginflag=0;
int userid=0;
string searchuser;


string regname, regpass;  // name and password for the Registration part
string logname, logpass;  // name and password for the Log In  part
string name, password;    // name and password existing in Database
string namepost, reply;   // name of user who left a post and the reply of a friend to this
string posts;             // posts of user who is logged in
string friendname, frend; // strings for friend names
int numofusers = -1;      // initialization of the number of users


void goback()
{
    cout << endl << "Go back to main menu ? (Y/N) : ";
    string choice;
    cin >> choice;
    // User inputs "Y or y" if he wants to go back to menu2
    // else the program is being terminated
    if(choice=="Y" || choice=="y")
        menu2();
    else
        exit(1);
}

void menu3()
{
    string line;
    ofstream users("users"); // Create file to save the names (not the passwords) of all registered users

    ifstream data;
    data.open("database"); // Open database file with user names and passwords

    cout << endl << "Registered (non friends) users :" << endl << string(32, '=') << endl;
    int i = 0;
    // Save the names (not the passwords) of all registered users to "users" file
    while(!data.eof()){
        getline(data, line);
        if (i%2==0)
            users << line << endl;
        i++;
    }
    users.close();
    data.close();

    int lineCount=1;

    ifstream regUsers;
    regUsers.open("users"); // Open users file

    ofstream nonFriends("Temp");  // Create temporary file to write all non-friend users

    string logline;
    while(getline(regUsers, line) && line!="")
    {
        if(line!=logname)
        {
            ifstream logUser(logname.c_str());
            bool exists = false;
            while(getline(logUser, logline))
            {
                    // Check if other user name exists already in logged in user firends file
                    if(logline.find(line) != std::string::npos)
                        exists = true;
                    else
                        continue;
            }
            logUser.close();
            if(exists==false)
            {
                cout << "[" << lineCount << "]" << " " << line << endl;
                nonFriends << line << endl;
            }
            else
                continue;
        }
        else
            continue;
        lineCount++;
    }
    // Close opened files
    nonFriends.close();
    regUsers.close();

    cout << endl << "Send Friend Request..." << endl;

    int friendNumber;
    string moreFriends = "Y";

    // Write inside the file of requests without deleting the existing text
    ofstream requests("Requests",  ios::app);

    // Process to send a friend request
    // User will type the number that corresponds to the user he wants to send a request
    while((moreFriends!="Y") || (moreFriends!="y")){
        cout << "Choose the number that corresponds to the desired friend : ";
        cin >> friendNumber;

        // Open file with all non friend registered users
        ifstream nonFriends;
        nonFriends.open("Temp");

        int j=0;
        while(getline(nonFriends, line) && line!=""){
            j++;
            if(friendNumber == j)
            {
                // Save the requests as "user_sent_request -> user_to_accept" to the file of requests
                requests << logname + " -> " + line << endl;
            }
            else
                continue;
        }
        // Choose whether to send or not more requests
        cout << endl << "Send more requests ? (Y/N) : ";
        cin >> moreFriends;
        if((moreFriends=="Y") || (moreFriends=="y"))
            continue;
        else
        {
            goback();
            break;
        }
    }
    // Close files
    requests.close();
    nonFriends.close();
    // Remove temporary file
    remove("Temp");
}




void friendrequest()
{
    cout << "\tACCEPT/REJECT FRIEND REQUEST\t" << endl;
    string line;
    string moreRequests = "Y";

    // Create file to save names of users who have sent requests to the logged in user
    ofstream tempreq("TempReq");

    ifstream freq;
    freq.open("Requests"); // Open file with friends' requests

    // Create temporary file to remake Requests text file
    ofstream mytemp;
    mytemp.open("Temp");

    // Process to save names of users who have sent requests to the logged in user
    while(getline(freq, line))
    {
        // Check if logname exists in line (find method) and also not to be at the beggining of the line
        if((line.find(logname) != std::string::npos) && ((line.substr(0, line.find(" "))!=logname)))
            tempreq << line.substr(0, line.find(" ")) << endl;
        else
            mytemp << line << endl;
    }
    mytemp.close();
    remove("Requests");
    rename("Temp", "Requests");
    tempreq.close();

    // Process to choose whether to accept or not a friend request
    while(moreRequests!="Y" || moreRequests!="y"){
        ifstream temppreq;
        temppreq.open("TempReq"); // Open temporary file

        cout << "Requests from" << endl << string(14,'=') << endl;
        // Print a numbered list of users who have sent frined request to the logged in user
        int lineCount=1;
        while(getline(temppreq, line) && line!=""){
            cout << "[" << lineCount << "] " << line << endl;
            lineCount++;
        }
        temppreq.close();

        int friendNumber;
        cout << endl << "\e(Type the number of user you want to accept)\e" << endl;
        cout << "Choice : ";
        cin >> friendNumber;

        ifstream tempReq;
        tempReq.open("TempReq"); // Open file of users who have sent request

        int j=0;
        // Add the user who have sent the request to the file of the logged in user
        // without deleting the existing text - (ios::app)
        ofstream userFile(logname.c_str(), ios::app);
        ofstream temporary("MyTemp");
        while(getline(tempReq, line) && line!=""){
            j++;
            if(friendNumber == j)
            {
                // Add friend's name to the file of user who have sent a request that has been accepted
                userFile << line << endl;
                // Add the logged in user's name in the file of user who have sent the request
                ofstream otherfile(line.c_str(), ios::app);
                otherfile << logname << endl;
                otherfile.close();
                // Replace name of accepted friend's request with empty string
                temporary << " " << endl;
            }
            else
                temporary << line << endl;
        }
        temporary.close();
        tempReq.close();
        remove("TempReq");
        rename("MyTemp", "TempReq");
        userFile.close();

        cout << endl << "Accept more requests ? (Y/N) : ";
        cin >> moreRequests;
        // Choose whether to accept or not more friend requests
        if(moreRequests=="Y" || moreRequests=="y")
            continue;
        else
        {
            goback();
            break;
        }
    }
}
void friends()
{

}

void menu2()
{
    int choice;
    system("cls");
    cout<<"********************************************************************************************************************";
    cout<<endl<<endl<<endl<<endl;
    cout<<"               Welcome to NITgram.com , the Best Social Media site to connect with your closest friends!!!";
    cout<<endl<<endl<<endl<<endl;
    cout<<"********************************************************************************************************************";
    cout<<endl<<endl<<endl<<endl;

    cout << "[1] Send friend request" << endl;
    cout << "[2] Accept/Reject friend request" << endl;
    cout << "[3] My friends" << endl;
    cout << "[4] Back" << endl;
    cout << "[5] Log out" << endl;
    cout << "[6] Exit the program" << endl;
    cout << endl;

    // Choose from the available numbers (1,...,9)
    int menuChoice;
    cout << "Choice : ";
    cin >> menuChoice; // User gives choice number
    switch (menuChoice)
    {
        case 1:
            menu3();
            break;
        case 2:
            friendrequest();
            break;
        case 3:
            friends();
            break;
        case 4:
            frontpage();
            break;
        case 5:
            cout << "You have successfully logged out!" << endl;
            frontpage();
            break;
        case 6:
            exit(1);
            break;
        default:
            cout << "Wrong choice! Try again..." << endl;
            menu2();
            break;
    }


}

void frontpage()
{
    int choice;

    cout<<"********************************************************************************************************************";
    cout<<endl<<endl<<endl<<endl;
    cout<<"               Welcome to NITgram.com , the Best Social Media site to connect with your closest friends!!!";
    cout<<endl<<endl<<endl<<endl;
    cout<<"********************************************************************************************************************";
    cout<<endl<<endl<<endl<<endl;
    cout<<"1.Login";
    cout<<"\n2.Signup";
    cout<<"\nEnter your choice :";
    cin>>choice;
    switch(choice)
    {
    case 1:
         login();
         break;
    case 2:
         signup();
         break;
    default:
        cout<<"Sorry,wrong choice selected\n";
        frontpage();
    }
}
void signup()
{
  string reguser,regpass,dateofbirth;
  system("cls");
  cout<<"Enter the username :";
  cin>>reguser;
  cout<<"Enter the password :";
  cin>>regpass;
  cout<<"Enter dateofbirth :";
  cin>>dateofbirth;
  ifstream input("database.txt");
  string u,p,d;
  int i;
  ofstream reg("database.txt",ios::app);
  userid++;
  reg<<userid<<' '<<reguser<<' '<<regpass<<' '<<dateofbirth<<endl;
  system("cls");
  cout<<"Registration is successfull\n";
  frontpage();
}
void login()
{
    string user,pass,u,p,d;
    int i;
    system("cls");
    cout<<"Enter the username :";
    cin>>user;
    cout<<"Enter the password :";
    cin>>pass;

    ifstream input("database.txt");

    while(input>>i>>u>>p>>d)
    {
        if(u == user&& p == pass)
        {
            loginflag=1;
            searchuser=u;
        }
    }
    input.close();
    if(loginflag == 1)
    {
      cout<<"Hello "<<user<<"\nWe are glad that you are here\n";
      getch();
      menu2();
    }
    else
    {
        cout<<"Sorry,Login error. ";
        getch();
        frontpage();
    }
}

int main()
{
   frontpage();
   return 0;
}
