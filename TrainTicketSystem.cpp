#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<iomanip>
using namespace std ;

// ============================ Queue and Linklist Node Class ===============================
class Node {
    public:
        string passengerName, CNIC, age, gender, seatNO  , city;
        Node* next;

        Node(string p, string c, string a, string g, string s ,string city) {
            passengerName = p;
            CNIC = c;
            age = a;
            gender = g;
            seatNO = s;
            this->city=city;
            next = NULL;
        }
};

// ============================ GraphNode Class ===============================
class GraphNode {
public:
    string city;
    GraphNode* next;

    GraphNode(string c) {
        city = c;
        next = NULL;
    }
};


// ============================ ShowStaions  Class ===============================
class ShowStaions {

private:
    static const int MAX = 20;        // max cities
    string cities[MAX];
    int cityCount;
    GraphNode* adjList[MAX];

public:
    // Constructor
    ShowStaions() {
        cityCount = 0;
        for (int i = 0; i < MAX; i++)
            adjList[i] = NULL;

        readFromFile();
    }

    // Get index of city
    int getIndex(string city) {
        for (int i = 0; i < cityCount; i++) {
            if (cities[i] == city)
                return i;
        }
        return -1;
    }

    // Add city if not exists
    int addCity(string city) {
        int index = getIndex(city);
        if (index == -1) {
            cities[cityCount] = city;
            return cityCount++;
        }
        return index;
    }

    // Add route (edge)
    void addRoute(string from, string to) {
        int i = addCity(from);
        addCity(to);

        GraphNode* newNode = new GraphNode(to);
        newNode->next = adjList[i];
        adjList[i] = newNode;
    }

    // Read routes from file
    void readFromFile() {
        ifstream file("routes.txt");
        string from, to;

        if (!file) {
            cout << "Error opening routes file!" << endl;
            return;
        }

        while (file >> from >> to) {
            addRoute(from, to);
        }

        file.close();
    }

    // Show stations & routes
    void showStations() {
        system("cls");
        cout << "\n--- TRAIN ROUTES (GRAPH FROM FILE) ---\n\n";

        for (int i = 0; i < cityCount; i++) {
            cout << cities[i] << " -> ";

            GraphNode* temp = adjList[i];
            while (temp != NULL) {
                cout << temp->city << " ";
                temp = temp->next;
            }
            cout << endl;
        }

        cout << "\nPress Enter to return...";
        cin.ignore();
        cin.get();
    }
};


// ============================= Admin Class============================== 
class Admin {
    private :
        string passengerName, seatNO,CNIC , age , gender , city;
        string FilePassengerName, FileSeatNO,FileCNIC , FileAge , FileGender ,FileCity, line ;
    public:
    // Constructor 
        Admin(){

        }
 // --------------------------------Admin BookSeat Function-------------------------

       void bookSeat() {
    system("cls");
    cin.ignore();
    cout << "1. passengerName \t 2.CNIC \t 3. Age \t 4.gender \t 5. SeatNo \t 6. City" << endl;

    cout << "Enter Passenger Name here: ";
    getline(cin, passengerName);

    cout << "Enter CNIC here: ";
    getline(cin, CNIC);

    cout << "Enter Age here: ";
    getline(cin, age);

    cout << "Enter Gender here: ";
    getline(cin, gender);

    cout << "Enter Seat No. here: ";
    getline(cin, seatNO);

    cout << "Enter City here: ";
    string city;
    getline(cin, city);

    // Check if city exists in routes
    ShowStaions stations;  // create stations object
    if (stations.getIndex(city) == -1) {
        cout << "Error: City does not exist in train routes! Booking failed." << endl;
        cout << "\nPress Enter to return...";
        cin.ignore();
        cin.get();
        return;
    }

    // If city exists, book seat
    ofstream outfile("BookSeat.txt", ios::app);
    if (!outfile) {
        cout << "Error: The Booking file is not open here !!!" << endl;
        return;
    } else {
        outfile << passengerName << '\t' << CNIC << '\t' << age << '\t'
                << gender << '\t' << seatNO << '\t' << city << endl;
    }
    outfile.close();

    cout << "Seat is booked successfully in " << city << "!" << endl;
    cout << "\nPress Enter to return...";
    cin.ignore();
    cin.get();
}

// --------------------------------Admin ViewSeat Function-------------------------
     void viewALLSeat() {
    ifstream infile("BookSeat.txt");
    if (!infile) {
        cout << "Error: Booking File is not open !!!" << endl;
        return;
    }

    Node* front = NULL;
    Node* rear = NULL;
    string line;

    // Read file & enqueue
    while (getline(infile, line)) {
        stringstream ss(line);
        string passengerName, CNIC, age, gender, seatNO, city;

        getline(ss, passengerName, '\t');
        getline(ss, CNIC, '\t');
        getline(ss, age, '\t');
        getline(ss, gender, '\t');
        getline(ss, seatNO, '\t');
        getline(ss, city, '\t');

        Node* newNode = new Node(passengerName, CNIC, age, gender, seatNO, city);

        if (front == NULL) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }
    infile.close();

    system("cls");

    // Print headers
    cout << left << setw(20) << "Passenger"
         << setw(20) << "CNIC"
         << setw(10) << "Age"
         << setw(15) << "Gender"
         << setw(10) << "SeatNo"
         << setw(15) << "City" << endl;

    cout << "-------------------------------------------------------------------------------" << endl;

    // Display data
    Node* temp = front;
    while (temp != NULL) {
        cout << left << setw(20) << temp->passengerName
             << setw(20) << temp->CNIC
             << setw(10) << temp->age
             << setw(15) << temp->gender
             << setw(10) << temp->seatNO
             << setw(15) << temp->city << endl;
        temp = temp->next;
    }

    // Free memory
    while (front != NULL) {
        temp = front;
        front = front->next;
        delete temp;
    }

    cout << "\nPress Enter to return...";
    cin.ignore();
    cin.get();
}


// --------------------------------Admin CancelSeat Function-----------------------
        void cancelSeat (){

            Node* head = NULL;
            Node* tail = NULL;

            ifstream infile("BookSeat.txt");
            if (!infile){
                cout <<"Error : Booking file is not open here "<<endl;
                return;
            }

            // ---------- Load file into Linked List ----------
            while (getline(infile, line))
            {
                stringstream ss(line);
                getline(ss, passengerName, '\t');
                getline(ss, CNIC, '\t');
                getline(ss, age, '\t');
                getline(ss, gender, '\t');
                getline(ss, seatNO, '\t');
                getline(ss, city, '\t');
                
                Node* newNode = new Node(passengerName, CNIC, age, gender, seatNO ,city);

                if (head == NULL){
                    head = tail = newNode;
                } else {
                    tail->next = newNode;
                    tail = newNode;
                }
            }
            infile.close();

            if (head == NULL){
                cout << "No records found!" << endl;
                return;
            }

            // ---------- Ask CNIC ----------
            cin.ignore();
            cout << "Enter CNIC to cancel seat: ";
            getline(cin, CNIC);

            Node* curr = head;
            Node* prev = NULL;
            bool found = false;

            // ---------- Search & Delete ----------
            while (curr != NULL){
                if (curr->CNIC == CNIC){
                    found = true;

                    // delete first node
                    if (prev == NULL){
                        head = curr->next;
                    } 
                    // delete middle or last node
                    else {
                        prev->next = curr->next;
                    }

                    delete curr;
                    break;
                }
                prev = curr;
                curr = curr->next;
            }

            if (!found){
                cout << "Passenger not found!" << endl;
                return;
            }

            // ---------- Rewrite file ----------
            ofstream outfile("BookSeat.txt", ios::out);
            if (!outfile){
                cout << "Error opening file for rewrite!" << endl;
                return;
            }

            Node* temp = head;
            while (temp != NULL){
                outfile << temp->passengerName << '\t'
                        << temp->CNIC << '\t'
                        << temp->age << '\t'
                        << temp->gender << '\t'
                        << temp->seatNO <<'\t'
                        << temp->city << endl;
                temp = temp->next;
            }
            outfile.close();

            // ---------- Free memory ----------
            while (head != NULL){
                temp = head;
                head = head->next;
                delete temp;
            }

            cout << "Seat cancelled successfully!" << endl;
            cout << "\nPress Enter to return...";
            cin.get();
        }

// --------------------------------Admin UpdateSeat Function-----------------------
        void updateSeat (){

            Node* front = NULL;
            Node* rear  = NULL;

            ifstream infile("BookSeat.txt");
            if (!infile){
                cout << "Error: Booking file not open!" << endl;
                return;
            }

            // ---------- Load file into Queue ----------
            while (getline(infile, line)){
                stringstream ss(line);
                getline(ss, passengerName, '\t');
                getline(ss, CNIC, '\t');
                getline(ss, age, '\t');
                getline(ss, gender, '\t');
                getline(ss, seatNO, '\t');
                getline(ss, city, '\t');

                Node* newNode = new Node(
                    passengerName, CNIC, age, gender, seatNO , city
                );

                if (front == NULL){
                    front = rear = newNode;
                } else {
                    rear->next = newNode;
                    rear = newNode;
                }
            }
            infile.close();

            // ---------- Remove first 20 persons ----------
            int count = 0;
            while (front != NULL && count < 20){
                Node* temp = front;
                front = front->next;
                delete temp;
                count++;
            }

            // ---------- Rewrite remaining data into file ----------
            ofstream outfile("BookSeat.txt", ios::out);
            if (!outfile){
                cout << "Error: Cannot rewrite file!" << endl;
                return;
            }

            Node* temp = front;
            while (temp != NULL){
                outfile << temp->passengerName << '\t'
                        << temp->CNIC << '\t'
                        << temp->age << '\t'
                        << temp->gender << '\t'
                        << temp->seatNO << '\t'
                        << temp->city << endl;
                temp = temp->next;
            }
            outfile.close();

            // ---------- Free remaining queue ----------
            while (front != NULL){
                temp = front;
                front = front->next;
                delete temp;
            }

            cout << "Update complete: First 20 passengers removed (FCFS)." << endl;
            cout << "\nPress Enter to return...";
            cin.ignore();
            cin.get();
        }


// --------------------------------Admin Panel Function-----------------------
        void adminPanel (){
            string choice ;
            int num ;
           do {
             cout <<"\t 1. Admin Book Seats "<<endl;
            cout <<"\t 2. Admin View Seats "<<endl;
            cout <<"\t 3. Admin Cancel Seats "<<endl;
            cout <<"\t 4. Admin Update Seats "<<endl;
            cout <<"\t 5. Exit Admin Panel "<<endl;
            cout <<"Please select an option ";
            num = atoi(choice.c_str());
            cin >> num;
                switch (num)
                {
                case 1 :
                    bookSeat();
                    break;
                case 2 :
                    viewALLSeat();
                    break;
                case 3 :
                    cancelSeat();
                    break;
                case 4 :
                    updateSeat();
                    break;
                case 5 :
                    cout <<"Good bye !!! From admin Panel"<<endl;
                    break;
                default:
                    cout <<"Invalid option Pleaase select again !"<<endl;
                    break;
                }
           }while(num !=5);
        }
};



// ============================= Admin Registration Class============================== 
class AdminRegistration {
    private:
        string choice ;
        int num ;
        string username, password , CNIC , Qualification ;
        bool isFound = false;
        string FileUsername , FileCNIC , FileQualification , FilePassword  , line ;
    public:

        AdminRegistration() {
            username = "";
            password = "";
            CNIC = "";
            Qualification = "";
        }
// --------------------------------Admin SignUp Function----------------------------

        void signUp (){
            system("cls");
            cout <<"---------------------Admin Sig Up --------------------"<<endl;
            cout <<"1. Enter Admin username \t 2. Entetr Admin CNIC  \t Enter Password of Admin \t Enter Qualification as BS , MS or PHD "<<endl;
            cout <<"Enter Username of Admin "<<endl;
            cin.ignore();
            getline(cin, username);
            cout << "Enter CNIC of Admin" <<endl;
            getline(cin,CNIC);
            cout <<"Enter Qualification of Admin "<<endl;
            getline(cin,Qualification);
            cout <<"Enter Password of Admin "<<endl;
            getline(cin,password);
            if(Qualification == "BS" || Qualification == "MS" || Qualification =="PHD" || Qualification == "bs" || Qualification == "ms" || Qualification == "phd"){
                ofstream outfile("AdminRegistration.txt" , ios::app);
                if (!outfile){
                    cout <<"Error ! Outfile is not open yet "<<endl;
                    return;
                }else{
                    system("cls");
                    outfile<<username <<"\t" <<CNIC << "\t" << Qualification << "\t" <<password<<endl;
                }
                outfile.close();
                cout <<"Admin is Registered !!!"<<endl;
                return;
            }else{
                cout <<"You enter wrong qualification "<<endl;
            }
        }
// --------------------------------Admin Login Function----------------------------
        void login (){
            system("cls");
            ifstream infile("AdminRegistration.txt");
            if (!infile){
                cout <<"Error : The file is not open !!!"<<endl;
                return;
            }else{
                cin.ignore();
                cout <<"Enter Username of Admin"<<endl;
                getline(cin ,username);
                cout <<"Enter CNIC of Admin"<<endl;
                getline(cin ,CNIC);
                cout <<"Enter Password of Admin"<<endl;
                getline(cin ,password);
                while (getline(infile,line))
                {
                    stringstream ss(line); 
                    getline(ss,FileUsername,'\t');
                    getline(ss,FileCNIC,'\t');
                    getline(ss,FileQualification,'\t');
                    getline(ss,FilePassword,'\t');

                    if (FileUsername == username && FileCNIC == CNIC  && FilePassword == password){
                        isFound = true;
                        break;
                    }
                }
                if (isFound){
                    cout <<"Admin Account is login welcome sir !!!"<<endl;
                    Admin admin ;
                    admin.adminPanel();
                }else{
                    cout <<"Please Enter correct account details" <<endl;
                    return ;
                }
                
                
            }
            infile.close();

        }

// --------------------------------Admin Registration Panel Function-----------------------
        void registrationPanel (){
            system("cls");
            do {
                 cout <<"\t 1. Admin Sign Up "<<endl;
                cout <<"\t 2. Admin Login "<<endl;
                cout <<"\t 3. Exit the Admin Authentication "<<endl;
                cout <<"Please select an option ";
                num = atoi(choice.c_str());
                cin >> num;
                switch (num)
                    {
                    case 1 :
                        signUp();
                        break;
                    case 2 :
                        login() ;
                        break;
                    case 3 :
                        cout <<"Good bye!!! From Admin Authentication Panel"<<endl;
                        break;
                    default:
                        cout <<"Invalid option Pleaase select again !"<<endl;
                        break;
                    }
            }while(num !=3);
        }
};



// ============================= Show List Class================================
class ShowList:Admin {
    public:
    // Constructor 
        ShowList(){
        }
// --------------------------------Show  List Function-----------------------
        void showList (){
            viewALLSeat();
        }
};
// --------------------------------Main Function-----------------------
int main(){
    string choice ;
    int num ;
        do {
        cout << "\n--- TRAIN RESERVATION SYSTEM ---\n";
        cout << "1. View Train Stations \n";
        cout << "2. Admin \n";
        cout << "3. View List\n";
        cout << "4. Exit the system \n";
        cout << "Please select an option  ";
        num = atoi(choice.c_str());
        cin >> num;   
            
            switch (num )
            {
            case 1 :{
                ShowStaions stations ;
                stations.showStations();
                break;
            }
                
            case 2: {
                AdminRegistration Adminregister ;
                Adminregister.registrationPanel();
                break;
            }  
            case 3 :{
                ShowList list ;
                list.showList();
            }
            case 4 :
                cout <<"Good bye!!! The system is exit !"<<endl;
                break;
            
            default :
                cout <<"Invalid number so enter correct number"<<endl;
                break;
            }
        }while(num != 4);
    return 0;
}