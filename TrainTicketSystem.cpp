#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<iomanip>
using namespace std ;

// ============================ Queue and Linklist Node Class ===============================
class Node {
    public:
        int ticketNo;
        string passengerName, CNIC, age, gender, seatNO;
        string fromCity, toCity;
        int price;
        Node* next;

        Node(int t, string p, string c, string a, string g,
            string s, string f, string to, int pr) {

            ticketNo = t;
            passengerName = p;
            CNIC = c;
            age = a;
            gender = g;
            seatNO = s;
            fromCity = f;
            toCity = to;
            price = pr;
            next = NULL;
        }
};


// ============================ GraphNode Class ===============================
class GraphNode {
    public:
        string city;
        int km;
        GraphNode* next;

        GraphNode(string c, int d) {
            city = c;
            km = d;
            next = NULL;
        }
};



// ============================ ShowStaions  Class ===============================
class ShowStaions {
    private:
        static const int MAX = 20;
        string cities[MAX];
        int cityCount;
        GraphNode* adjList[MAX];

    public:
        ShowStaions() {
            cityCount = 0;
            for (int i = 0; i < MAX; i++)
                adjList[i] = NULL;
            readFromFile();
        }

        int getIndex(string city) {
            for (int i = 0; i < cityCount; i++)
                if (cities[i] == city)
                    return i;
            return -1;
        }

        int addCity(string city) {
            int idx = getIndex(city);
            if (idx == -1) {
                cities[cityCount] = city;
                return cityCount++;
            }
            return idx;
        }

        void addRoute(string from, string to, int km) {
            int i = addCity(from);
            addCity(to);  // ensure 'to' city exists

            GraphNode* newNode = new GraphNode(to, km);
            if (!adjList[i]) {
                adjList[i] = newNode;
            } else {
                GraphNode* temp = adjList[i];
                while (temp->next)
                    temp = temp->next;
                temp->next = newNode;
            }
        }

     void readFromFile() {
    ifstream file("routes.txt");
    if (!file) {
        cout << "Error: routes.txt not found!" << endl;
        return;
    }

    string from, to;
    int km;

    while (file >> from >> to >> km) {
        addRoute(from, to, km);
    }

    file.close();
}


        int getDistance(string from, string to) {
            int i = getIndex(from);
            if (i == -1) return -1;

            GraphNode* temp = adjList[i];
            while (temp) {
                if (temp->city == to)
                    return temp->km;
                temp = temp->next;
            }
            return -1;
        }

        int calculatePrice(string from, string to) {
            int km = getDistance(from, to);
            if (km == -1) return -1;
            return km * 5; // Rs 5 per km
        }

        void showStations() {
        system("cls");
        cout << "\n--- TRAIN ROUTES ---\n\n";

        for (int i = 0; i < cityCount; i++) {
            GraphNode* temp = adjList[i];
            while (temp) {
                cout << cities[i] << " -> " 
                    << temp->city << ": " 
                    << temp->km << " km" << endl;
                temp = temp->next;
            }
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

            string fromCity, toCity;
            cout << "Passenger Name: ";
            getline(cin, passengerName);
            cout << "CNIC: ";
            getline(cin, CNIC);
            cout << "Age: ";
            getline(cin, age);
            cout << "Gender: ";
            getline(cin, gender);
            cout << "Seat No: ";
            getline(cin, seatNO);
            cout << "From City: ";
            getline(cin, fromCity);
            cout << "To City: ";
            getline(cin, toCity);

            ShowStaions stations;
            int price = stations.calculatePrice(fromCity, toCity);

            if (price == -1) {
                cout << "Route not found!" << endl;
                cin.get();
                return;
            }

            static int ticketCounter = 1;

            ofstream outfile("BookSeat.txt", ios::app);
            outfile << ticketCounter++ << '\t'
                    << passengerName << '\t'
                    << CNIC << '\t'
                    << age << '\t'
                    << gender << '\t'
                    << seatNO << '\t'
                    << fromCity << '\t'
                    << toCity << '\t'
                    << price << endl;

            outfile.close();

            cout << "Ticket Booked! Price: Rs " << price << endl;
            cout << "\nPress Enter to return...";
            cin.ignore();
            cin.get();
        }


// --------------------------------Admin ViewSeat Function-------------------------
        void viewALLSeat() {
            ifstream infile("BookSeat.txt");
            if (!infile) {
                cout << "Error: Booking file not found!" << endl;
                return;
            }

            Node* front = NULL;  // queue front
            Node* rear  = NULL;  // queue rear
            string line;

            // ---------- Load data into queue ----------
            while (getline(infile, line)) {
                stringstream ss(line);

                int ticketNo, price;
                string passengerName, CNIC, age, gender, seatNO, fromCity, toCity;

                ss >> ticketNo;
                ss.ignore(); // ignore tab or space after ticketNo

                getline(ss, passengerName, '\t');
                getline(ss, CNIC, '\t');
                getline(ss, age, '\t');
                getline(ss, gender, '\t');
                getline(ss, seatNO, '\t');
                getline(ss, fromCity, '\t');
                getline(ss, toCity, '\t');
                ss >> price;

                Node* newNode = new Node(ticketNo, passengerName, CNIC, age,
                                        gender, seatNO, fromCity, toCity, price);

                // enqueue operation
                if (!front) front = rear = newNode;
                else {
                    rear->next = newNode;
                    rear = newNode;
                }
            }
            infile.close();

            system("cls");
            cout << left
                << setw(6)  << "TNo"
                << setw(15) << "Name"
                << setw(15) << "CNIC"
                << setw(6)  << "Age"
                << setw(8)  << "Gender"
                << setw(8)  << "Seat"
                << setw(15) << "From"
                << setw(15) << "To"
                << setw(8)  << "Price" << endl;

            cout << "--------------------------------------------------------------------------\n";

            // ---------- Dequeue and display ----------
            while (front) {
                Node* temp = front;
                cout << left
                    << setw(6)  << temp->ticketNo
                    << setw(15) << temp->passengerName
                    << setw(15) << temp->CNIC
                    << setw(6)  << temp->age
                    << setw(8)  << temp->gender
                    << setw(8)  << temp->seatNO
                    << setw(15) << temp->fromCity
                    << setw(15) << temp->toCity
                    << setw(8)  << temp->price << endl;

                // dequeue operation
                front = front->next;
                delete temp; // free memory
            }

            cout << "\nPress Enter to return...";
            cin.ignore();
            cin.get();
        }


// --------------------------------Admin CancelSeat Function-----------------------
        void cancelSeat() {
            ifstream infile("BookSeat.txt");
            if (!infile) {
                cout << "File not found!" << endl;
                return;
            }

            Node* head = NULL;
            Node* tail = NULL;
            string line;

            // ---------- Load file into linked list ----------
            while (getline(infile, line)) {
                stringstream ss(line);

                int ticketNo, price;
                string passengerName, CNIC, age, gender, seatNO, fromCity, toCity;

                ss >> ticketNo;
                ss.ignore(); // skip tab

                getline(ss, passengerName, '\t');
                getline(ss, CNIC, '\t');
                getline(ss, age, '\t');
                getline(ss, gender, '\t');
                getline(ss, seatNO, '\t');
                getline(ss, fromCity, '\t');
                getline(ss, toCity, '\t');
                ss >> price;

                Node* newNode = new Node(
                    ticketNo, passengerName, CNIC, age,
                    gender, seatNO, fromCity, toCity, price
                );

                if (!head)
                    head = tail = newNode;
                else {
                    tail->next = newNode;
                    tail = newNode;
                }
            }
            infile.close();

            // ---------- Ask CNIC ----------
            cin.ignore();
            string searchCNIC;
            cout << "Enter CNIC to cancel ticket: ";
            getline(cin, searchCNIC);

            Node* curr = head;
            Node* prev = NULL;
            bool found = false;

            // ---------- Search & delete ----------
            while (curr) {
                if (curr->CNIC == searchCNIC) {
                    found = true;

                    if (!prev)
                        head = curr->next;
                    else
                        prev->next = curr->next;

                    delete curr;
                    break;
                }
                prev = curr;
                curr = curr->next;
            }

            if (!found) {
                cout << "No ticket found with this CNIC!" << endl;
                return;
            }

            // ---------- Rewrite updated data ----------
            ofstream outfile("BookSeat.txt");
            Node* temp = head;
            while (temp) {
                outfile << temp->ticketNo << '\t'
                        << temp->passengerName << '\t'
                        << temp->CNIC << '\t'
                        << temp->age << '\t'
                        << temp->gender << '\t'
                        << temp->seatNO << '\t'
                        << temp->fromCity << '\t'
                        << temp->toCity << '\t'
                        << temp->price << endl;
                temp = temp->next;
            }
            outfile.close();

            // ---------- Free memory ----------
            while (head) {
                temp = head;
                head = head->next;
                delete temp;
            }

            cout << "Ticket cancelled successfully using CNIC!" << endl;
            cout << "\nPress Enter to return...";
            cin.ignore();
            cin.get();
        }


// --------------------------------Admin UpdateSeat Function-----------------------
        void updateSeat() {
            ifstream infile("BookSeat.txt");
            if (!infile) return;

            Node* front = NULL;
            Node* rear = NULL;
            string line;

            while (getline(infile, line)) {
                stringstream ss(line);

                int ticketNo, price;
                string passengerName, CNIC, age, gender, seatNO, fromCity, toCity;

                ss >> ticketNo;
                ss.ignore();

                getline(ss, passengerName, '\t');
                getline(ss, CNIC, '\t');
                getline(ss, age, '\t');
                getline(ss, gender, '\t');
                getline(ss, seatNO, '\t');
                getline(ss, fromCity, '\t');
                getline(ss, toCity, '\t');
                ss >> price;

                Node* newNode = new Node(ticketNo, passengerName, CNIC, age,
                                        gender, seatNO, fromCity, toCity, price);

                if (!front) front = rear = newNode;
                else {
                    rear->next = newNode;
                    rear = newNode;
                }
            }
            infile.close();

            int count = 0;
            while (front && count < 20) {
                Node* temp = front;
                front = front->next;
                delete temp;
                count++;
            }

            ofstream outfile("BookSeat.txt");
            Node* temp = front;
            while (temp) {
                outfile << temp->ticketNo << '\t'
                        << temp->passengerName << '\t'
                        << temp->CNIC << '\t'
                        << temp->age << '\t'
                        << temp->gender << '\t'
                        << temp->seatNO << '\t'
                        << temp->fromCity << '\t'
                        << temp->toCity << '\t'
                        << temp->price << endl;
                temp = temp->next;
            }
            outfile.close();

            cout << "First 20 tickets removed (FCFS)." << endl;
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