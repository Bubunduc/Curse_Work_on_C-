#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <algorithm>
#include <windows.h>
#include <iomanip>
#pragma warning(disable : 4996)
using namespace std;

struct user {
    string login;
    string password;
    bool permission;
};

struct reader {
    string fio;
    string series;
    string number;
    string adress;
    string reader_ticket;
    string telephone;
};

struct book{
    string isbn;
    string name;
    string jenre;
    string author;
    string quantity;
};
struct debtor {
    string book_name;
    string author;
    string fio;
    string ticket_number;
    string date;
    string who_give;
};

class Actions {
public:
    virtual int search() = 0;
    virtual void add() = 0;
    virtual void reduct() = 0;
    virtual void print() = 0;
    virtual void del() = 0;
};


class Books:public Actions {
private:
    string dir = "databases\\Books.txt";
public:
    int search() override {
        string isbn;
        string name;
        string jenre;
        string author;
        string quantity;
        fstream file;
        string search;
        cout << "Введите ISBN искомой книги:\n";
        cin >> search;
        file.open(dir, ios::in);
        bool flag = false;
        int id = 1;
        while (!file.eof())
        {
            file >> isbn>>name>>jenre>>author>>quantity;
            if (isbn == search) {
                replace(name.begin(), name.end(), '_', ' ');
                cout << "Искомая книга найдена\n" <<"ID книги - "<< id<< "\nНазвание - " << name << "\nЖанр - " << jenre << "\nАвтор - " << author << "\nКоличество экземпляров в библиотеке - " << quantity << endl;
                file.close();
                return id-1;
            }
            id++;
        }

        if (flag == false) {
            file.close();
            cout << "По вашему запросу ничего не найдено" << endl;
            return -1;
        }
        
       }
    void add() override {
        ofstream file;
        string isbn;
        string name;
        string jenre;
        string author;
        string quantity;
        cout << "Введите ISBN книги:\n";
        cin >> isbn;
        cout << "Введите название книги:\n";
        cin.ignore(256, '\n');
        getline(cin,name);
        cout << "Введите жанр книги:\n";
        cin >> jenre;
        cout << "Введите фамилию и инициалы автора книги книги:\n";
        cin >> author;
        cout << "Введите количество книг:\n";
        cin >> quantity;
        replace(name.begin(), name.end(), ' ', '_');
        file.open(dir, ios::app);
        file <<isbn << " " << name << " " << jenre << " " << author << " " << quantity << "\n";
        file.close();
        cout << "Книга добавлена"<<endl;
    }
    int check_size()  {
        string isbn;
        string name;
        string jenre;
        string author;
        string quantity;
        fstream file;
        string isbn_buff;
        file.open(dir, ios::in);
        int size = 0;
        while (!file.eof())
        {
            file >> isbn >> name >> jenre >> author >> quantity;
            if (isbn == isbn_buff) {
                break;
            }
            else {
                isbn_buff = isbn;
            }
            size++; 
        }
        file.close();
        return size;
    }
    void print() override {
        string isbn;
        string name;
        string jenre;
        string author;
        string quantity;
        fstream file;
        int size = check_size();
        file.open(dir, ios::in);
        int id = 0;
        cout << "id\t  ISBN" << "\t    " << "Название книги" << '\t' << "     Жанр" << "\t\t " << "Автор" << "\t        " << "Книг в наличии\n\n";
        while (id<size)
        {
            file >> isbn >> name >> jenre >> author >> quantity;
            replace(name.begin(), name.end(), '_', ' ');
            //printf("%3d\t%10s\t%20s\t%10s\t%10s\t%4s\n",id+1,isbn.c_str(),name.c_str(),jenre.c_str(),author.c_str(),quantity.c_str());
            cout << left << setw(10) << id + 1 << setw(10) << isbn << setw(25) << name << setw(20) << jenre << setw(15) << author << setw(15) << quantity << endl;
            id++;
            
        }
    }
    void read_file(int size,struct book *books) {
        fstream file;
        int id = 0;
        file.open(dir, ios::in);
        while (id < size)
        {
            file >> books[id].isbn >> books[id].name >> books[id].jenre >> books[id].author >> books[id].quantity;
            id++;
        }
        file.close();
    }
    void del() override {
        print();
        int size = check_size();
        int counter = 0;
        struct book *books = new struct book[size];
        read_file(size, books);
        int id = search();
        ofstream file;
        file.open(dir, ios::out);
        while (counter < size) {
            if (id != counter) {
                file << books[counter].isbn << ' ' << books[counter].name << ' ' << books[counter].jenre << ' ' << books[counter].author << ' ' << books[counter].quantity << '\n';
            }
            counter++;
        }
        file.close();
        cout << "Книга удалена из базы данных" << endl;
    }
    void reduct() override {
        print();
        int size = check_size();
        int counter = 0;
        struct book* books = new struct book[size];
        read_file(size, books);
        int id = search();
        cout << "Введите новый ISBN книги:\n";
        cin >> books[id].isbn;
        cout << "Введите название книги:\n";
        cin.ignore(256, '\n');
        getline(cin, books[id].name);
        cout << "Введите новый жанр книги:\n";
        cin >> books[id].jenre;
        cout << "Введите новую фамилию и инициалы автора книги:\n";
        cin >> books[id].author;
        cout << "Введите количество книг:\n";
        cin >> books[id].quantity;
        replace(books[id].name.begin(), books[id].name.end(), ' ', '_');
        ofstream file;
        file.open(dir, ios::out);
        while (counter < size) {
           
            file << books[counter].isbn << ' ' << books[counter].name << ' ' << books[counter].jenre << ' ' << books[counter].author << ' ' << books[counter].quantity << '\n';
            
            counter++;
        }
        file.close();
        cout << "Позиция успешно изменена" << endl;
    }
};


class Reader :public Actions {
private:
    string dir = "databases\\Readers.txt";
public:
    
    int check_size() {
        fstream file;
        string surname, name, fathername, street, home, phone_num, seria, nomer, num_readerticket,surname_buff;
        file.open(dir, ios::in);
        int size = 0;
        int id = 0;
        while (!file.eof())
        {
            file >> surname >> name >> fathername >> seria >> nomer >> street >> home >> num_readerticket >> phone_num;
            if (surname == surname_buff) {
                break;
            }
            else {
                surname_buff = surname;
            }
            size++;
        }
        file.close();
        
        return size;
    }
    void read_file(int size,struct reader* readers) {
        fstream file;
        string surname, name, fathername, street,home, phone_num, seria, nomer, num_readerticket;
        file.open(dir, ios::in);
        int id = 0; 
        while (id<size)
        {
            file >> surname >> name >> fathername >> seria >> nomer >> street >> home >> num_readerticket >> phone_num;
            readers[id].fio = surname + " " + name + " " + fathername;
            readers[id].series = seria;
            readers[id].number = nomer;
            readers[id].adress = street + " " + home;
            readers[id].reader_ticket = num_readerticket;
            readers[id].telephone = phone_num;
            //printf("%d\t%25s%10s%15s   %20s %15s %15s\n\n", id+1,readers[id].fio.c_str(), readers[id].series.c_str(), readers[id].number.c_str(),readers[id].adress.c_str(), readers[id].reader_ticket.c_str(), readers[id].telephone.c_str());
            id++;
        }
        file.close();
        
    }

    int search() override{
        string seria, number;
        int size = check_size();
        struct reader *readers = new struct reader[size];
        read_file(size, readers);
        cout<< "Введите серию и номер через пробел, чтобы найти пойльзователя:\n";
        cin >> seria >> number;
        int i;
        for (i = 0; i < size; i++) {
            if ((seria == readers[i].series) && (number == readers[i].number)) {
                cout << "Совпадение найдено:"<<endl;
                cout <<"ФИО - " << readers[i].fio<<endl<<"Адрес - "<<readers[i].adress<<endl<<"Номер читальского билета - "<<readers[i].reader_ticket<<endl<<"Контактный телефон - "<<readers[i].telephone<<endl;
                return i;
            }
        }
        if (i == size) {
            cout << "Совпадений не найдено"<< endl;
            return -1;
        }
        delete[] readers;
        readers = nullptr;
    }
    void print() override {
        int size = check_size();
        struct reader* readers = new struct reader[size];
        read_file(size, readers);
        
        cout << "id\t\tФИО\t\t   Серия     Номер     Адресс\t\t Номер билета\t  Номер телефона\n\n";
        for (int id = 0; id < size; id++) {
            //printf("%d\t%25s%10s%15s   %20s %15s     %15s\n\n", id+1,readers[id].fio.c_str(), readers[id].series.c_str(), readers[id].number.c_str(),readers[id].adress.c_str(), readers[id].reader_ticket.c_str(), readers[id].telephone.c_str());
            cout << left << setw(5) << id + 1 << setw(30) << readers[id].fio << setw(10) << readers[id].series << setw(10) << readers[id].number << setw(20) << readers[id].adress << setw(15) << readers[id].reader_ticket << setw(8) << readers[id].telephone << endl;
        }
        delete[] readers;
        readers = nullptr;
    }
    void add() override {
       
        ofstream file;
        string surname, name, fathername, series, number, street,home, reader_ticket, telephone;
        cout << "\nВведите ФИО - ";
        cin >> surname>>name>>fathername;
        cout << "\nВведите cерию и номер пасспорта через пробел - ";
        cin >> series;
        cin>>number;
        cout << "\nВведите адрес читателя по формату: \"Название улицы Номер дома\" - ";
        cin >> street>>home;
        cout << "\nВведите номер читальского билета (6 цифр) - ";
        cin >> reader_ticket;
        cout << "\nВведите Номер телефона читателя - ";
        cin >> telephone;
        file.open(dir, ios::app);
        file << surname<<" " << name <<" " << fathername << " " << series << " " << number << " " << street<<" "<<home<<" " << " " << reader_ticket<<" "<< telephone << "\n";
        file.close();
        cout << "Читатель успешно добавлен в систему\n";
    }
    void del() override {
        print();
        int size = check_size();
        struct reader* readers = new struct reader[size];
        read_file(size, readers);
        int id = search();
        
        string surname, name, fathername,home,street;
        ofstream file;
        file.open(dir, ios::out);
        int counter = 0;
        while (counter < size) {
            if (counter != id) {
                int pos = readers[counter].fio.find(" ");
                surname = readers[counter].fio.substr(0, pos);
                readers[counter].fio.erase(0, pos + 1);
                pos = readers[counter].fio.find(" ");
                name = readers[counter].fio.substr(0, pos);
                readers[counter].fio.erase(0, pos + 1);
                fathername = readers[counter].fio;
                pos = readers[counter].adress.find(" ");
                street = readers[counter].adress.substr(0, pos);
                readers[counter].adress.erase(0, pos + 1);
                home = readers[counter].adress.substr(0, pos);
                file << surname << " " << name << " " << fathername << " " << readers[counter].series << " " << readers[counter].number << " " << street << " " << home << " " << " " << readers[counter].reader_ticket << " " << readers[counter].telephone << "\n";
                
            }
            counter++;
        }
        cout << "Удаление завершено...\n";
        file.close();
        delete[] readers;
        readers = nullptr;
    }
    void reduct() override {
        print();
        int size = check_size();
        struct reader* readers = new struct reader[size];
        read_file(size, readers);
        int id  = search();
        if (id == -1) {
            return;
        }
        ofstream file;
        string surname, name, fathername, series, number, street, home, reader_ticket, telephone;
        cout << "\nВведите ФИО - ";
        cin >> surname >> name >> fathername;
        cout << "\nВведите cерию и номер пасспорта через пробел - ";
        cin >> series;
        cin >> number;
        cout << "\nВведите адрес читателя по формату: \"Название улицы Номер дома\" - ";
        cin >> street >> home;
        cout << "\nВведите номер читальского билета (6 цифр) - ";
        cin >> reader_ticket;
        cout << "\nВведите Номер телефона читателя - ";
        cin >> telephone;
        int counter = 0;
        file.open(dir, ios::out);
        
        while (counter <  size) {
            if (counter == id) {

                    file << surname << " " << name << " " << fathername << " " << series << " " << number << " " << street << " " << home << " " << reader_ticket << " " << telephone << "\n";
                    
            }
            
            else{

                int pos = readers[counter].fio.find(" ");
                string surname_buff = readers[counter].fio.substr(0, pos);
                readers[counter].fio.erase(0, pos + 1);
                pos = readers[counter].fio.find(" ");
                string name_buff = readers[counter].fio.substr(0, pos);
                readers[counter].fio.erase(0, pos + 1);
                string fathername_buff = readers[counter].fio;
                pos = readers[counter].adress.find(" ");
                string home_buff = readers[counter].adress.substr(0, pos);
                readers[counter].adress.erase(0, pos + 1);
                string street_buff = readers[counter].adress.substr(0, pos);
                file << surname_buff << " " << name_buff << " " << fathername_buff << " " << readers[counter].series << " " << readers[counter].number << " " << street_buff << " " << home_buff << " " << readers[counter].reader_ticket << " " << readers[counter].telephone << "\n";
                
            }
            counter++;
        }
        /*if (id - 1 == size - 1) {
            int pos = readers[id].fio.find(" ");
            string surname_buff = readers[id].fio.substr(0, pos);
            readers[id].fio.erase(0, pos + 1);
            pos = readers[id].fio.find(" ");
            string name_buff = readers[counter].fio.substr(0, pos);
            readers[id].fio.erase(0, pos + 1);
            string fathername_buff = readers[id].fio;
            pos = readers[id].adress.find(" ");
            string home_buff = readers[counter].adress.substr(0, pos);
            readers[id].adress.erase(0, pos + 1);
            string street_buff = readers[id].adress.substr(0, pos);
            file << surname_buff << " " << name_buff << " " << fathername_buff << " " << readers[id].series << " " << readers[id].number << " " << street_buff << " " << home_buff << " " << readers[counter].reader_ticket << " " << readers[id].telephone << "\n";
        }*/
        file.close();
        cout << "Пользователь успешно редактирован!\n";
        delete[] readers;
        readers = nullptr;
    }
};


class Logs {
private:
    string dir = "databases\\Logs.txt";
public:

    void logging(string login) {
        ofstream file;
        file.open(dir, ios::app);
        auto now = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(now);

        file << login << "\t" << ctime(&end_time) << "\n";
    }
    void check_logs() {
        string line;
        ifstream file(dir);
        if (file.is_open())
        {
            while (getline(file, line))
            {
                cout << line << std::endl;
            }
        }
        file.close();
    }
};


class Users :public Actions,public Logs {
private:
    string dir = "databases\\Users.txt";
    bool permissions = false;
public:
   
   
    bool check_user(string login, string password) {
        cout << "Загрузка ....\n";
        ifstream file;
        file.open(dir, ios::in);
        string check_log, check_pass;

        
        while (!file.eof()) {
            file >> check_log >> check_pass >> permissions;
            if ((check_log == login) && (check_pass == password)) {
                return true;
            }
        }

        file.close();
        return false;
    }
    bool getpermissions() {
        return permissions;
    }
    int search() override {
        cout << "Введите логин человека, которого мы ищем\n\n";
        string what_search;
        cin >> what_search;
        ifstream file;
        file.open(dir, ios::in);
        string log,pass;
        bool permission;
       
        int id = 1;
        bool flag = false;
        while (!file.eof()) {
            file >> log >> pass >> permission;
            if (log == what_search) {
                cout << "По вашему запросу найден пользователь c именем "<<what_search<<" под номером "<<id << endl;
                flag = true;
                return id - 1;
            }
            id++;

        }
        if (flag == false) {
            cout << "По запросу "<<what_search <<" ничего не найдено" << endl;
            return -1;
        }
    }
    int check_size() {
        int id = 0;
        int size = 0;
        fstream file;
        string login, password;
        string login_buff;
        bool permission;
        file.open(dir, ios::in);
        
        while (!file.eof()) {
            file >> login >> password >> permission;
            if (login == login_buff) {
                break;
            }
            else {
                login_buff = login;
            }
            size++;
        }
        file.close();
        return size;
    }
    void add()override {
        ofstream file;
        string login, password;
        bool permission = false;
        cout << "\nВведите логин для нового пользователя - ";
        cin >> login;
        cout << "\nВведите пароль для нового пользователя - ";
        cin >> password;
        cout << "\nВведите пароль для нового пользователя(0 - пользователь;1 - администратор) - ";
        cin >> permission;
        file.open(dir, ios::app);
        file <<login<<" "<< password<<" "<< permission<< "\n";
        file.close();
        cout << "\nПользователь добавлен\n";
    }
    void print()override {
        int id = 1;
        fstream file;
        string login,login_buff;
        string password;
        bool permission;
        file.open(dir, ios::in);
        cout << "id" << "\t" << "Логин" << "\t       " << "Пароль" << "\t" << " Доступ" << endl;
        while (!file.eof()) {
            
            file >> login >> password >> permission;
            if (login == login_buff) {
                break;
            }
            cout <<left<<setw(8) << id << setw(15) << login << setw(10) << password << setw(10) << (permission == true ? "Aдминистратор" : "Пользователь") << endl;
            id++;
            login_buff = login;
        }
        file.close();
    }
    void reduct() override {
        print();
        int id = 0;
        int size = check_size();
        fstream file;
       
        struct user* users = new struct user[size];
        file.close();
        file.open(dir, ios::in);
        while (!file.eof()) {
            file >> users[id].login >> users[id].password >> users[id].permission;
            id++;

        }
        file.close();
        
        int choice = search();

        cout << "Введите новый логин" << endl;
        cin >> users[choice].login;
        cout << "Введите новый пароль" << endl;
        cin >> users[choice].password;
        cout << "Введите уровень доступа(0 - обычный пользователь; 1 - администратор)" << endl;
        cin >> users[choice].permission;
        file.open(dir, ios::out);
        int counter = 0;
        while (counter < size) {
            file << users[counter].login<<' '<< users[counter].password << ' ' << users[counter].permission << "\n";
            counter++;

        }
        file.close();
        cout << "Пользователь успешно редактирован!";
        delete[] users;
    }
    void del() override {
        print();
        int id =0;
        int size = check_size();
        fstream file;
        struct user* users = new struct user[size];
        
        file.open(dir, ios::in);
        //cout << "id" << "\t" << "Логин" << "\t" << "Пароль" << "\t" << "Доступ" << endl;
        while (!file.eof()) {
            file >> users[id].login >> users[id].password >> users[id].permission;
            /*if (id != size) {

                cout << id + 1 << "\t" << users[id].login << "\t" << users[id].password << "\t" << (users[id].permission == true ? "Aдминистратор" : "Пользователь") << endl;
            }*/
            id++;

        }
        
        file.close();
        id = search();
        file.open(dir, ios::out);
        int counter = 0;
        while (counter < size) {
            if (id != counter) {

                file << users[counter].login << ' ' << users[counter].password << ' ' << users[counter].permission << "\n";
                
            }
            counter++;
        }
        file.close();
        delete[] users;
        cout << "Пользователь успешно удален!";

    }
    
};

class Login: public Users{
private:
    string login_safe;
public:
    
    bool sing_in() {
       
        cout << "Программа для работы  библиотекой\n\nСтудента 1 курса БИСТ-224 Румянцева Данилы\n\n";
        string password,login;
        cout << "Введите логин\n";
        cin >> login;
        login_safe = login;
        cout << "Введите пароль\n";
        cin >> password;
        if (check_user(login, password) == true) {
            cout << "Добро пожаловать " << login << " !\n";
            logging(login);
        }
        else {
            cout << "Неправильный логин или пароль\n";
            exit(0);
        }
    }
    string get_login() {
        return login_safe;
    }
    int search() { return 0; }
    void add() {}
    void reduct() {}
    void print() {}
    void del() {}

    
};
class Debtor :public Books, public Reader, public Login {
private:
    string dir = "databases\\Debtor.txt";
    string who_give;
public:
    Debtor(string login) {
        who_give = login;
    }
    int check_size() {
        fstream file;
        string book_name, author, fio, ticket_num, date, who_give, book_name_buff;
       
        int size = 0;
        file.open(dir, ios::in);
        while (!file.eof()) {
            
            file>> book_name>>author>>fio>>ticket_num>>date>>who_give>>book_name;
            size++;
            if (book_name == book_name_buff) {
                break;
            }
            else {
                book_name_buff = book_name;
            }
            
        }
        file.close();
        return size+1;
    }
    void read_file(int size, struct debtor *debtors) {
        fstream file;
        
        file.open(dir, ios::in);
        for (int i = 0; i < size; i++) {
            file >> debtors[i].book_name >> debtors[i].author >> debtors[i].fio >> debtors[i].ticket_number >> debtors[i].date >> debtors[i].who_give;
        }
        file.close();
    }
    int search() {
        string ticket;
        cout << "Введите номер читальского билета искомого должника"<< endl;
        cin >> ticket;
        
        int id = 0;
        
        fstream file;
        string book_name, author, fio,ticket_num, date, who_give;
        file.open(dir, ios::in);
        while(!file.eof()){
            file >>book_name>>author>>fio>>ticket_num>>date>>who_give;
            if (ticket == ticket_num) {
                replace(book_name.begin(), book_name.end(), '_', ' ');
                replace(fio.begin(), fio.end(), '_', ' ');
                cout << "Совпадение найдено:\n" <<"ID : "<< id+1 << "\nНазвание книги : " << book_name << "\nАвтор: " << author << "\nФИО Должника : " << fio << "\nДата выдачи : " << date << "\nЛогин выдавшего : " << who_give << endl;
                file.close();
                return id;
            }
            id++;
        }
        cout << "Совпадений не найдено...\n";
        file.close();
        return -1;
    }
    void add() {
        cout << "Поиск нужного читалеля:\n\n";
        Reader::print();
        int reader_id = Reader::search();
        if (reader_id == -1) {
            return;
        }
       
        cout << "Укажите книгу, которую забрал должник:\n";
        Books::print();
        int book_id = Books::search();
        if (book_id == -1) {
            return;
        }
        cout << "Введите дату выдачи в формате - ДД.ММ.ГГГГ\n";
        string date;
        cin >> date;
        int reader_size = Reader::check_size();
        struct reader* readers = new struct reader[reader_size];
        Reader::read_file(reader_size, readers);
        string fio = readers[reader_id].fio;
        string ticket = readers[reader_id].reader_ticket;
        delete[] readers;
        int books_size = Books::check_size();
        struct book* books = new struct book[books_size];
        Books::read_file(books_size, books);
        string book_name = books[book_id].name;
        string author = books[book_id].author;
        delete[] books;
        
        ofstream file;
        file.open(dir, ios::app);
        replace(fio.begin(), fio.end(), ' ', '_');
        file << book_name << " " << author << " " << fio <<" "<< ticket << " " << date << " " << who_give << "\n";
        file.close();
        cout << "Должник добавлен" << endl;
    }
    void print() {
        fstream file;
        string book_name, author, fio, ticket_num, date, who_give;
        cout << "id\tНазвание книги                Автор книги                ФИО\t       Номер билета  Дата выдачи    Кем выдана\n\n";
        int id = 0;
        int size = Debtor::check_size();
        file.open(dir, ios::in);
        while (id <size-1) {
            file >> book_name >> author >> fio >> ticket_num >> date >> who_give;
            replace(book_name.begin(), book_name.end(), '_', ' ');
            replace(fio.begin(), fio.end(), '_', ' ');
            //printf("%4d  %15s    %10s  %5s  %7s\t   %9s %12s\n",id+1, book_name.c_str(), author.c_str(), fio.c_str(), ticket_num.c_str(), date.c_str(), who_give.c_str());
            cout<<id+1<<'\t' << left << setw(30) << book_name << setw(20) << author << setw(25) << fio << setw(10) << ticket_num << setw(15) <<date << setw(20) << who_give << endl;
            id++;
        }
        file.close();
    }
    void del() {
        print();
        int size = check_size();
        struct debtor* debtors = new struct debtor[size];
        read_file(size, debtors);
        cout << "Поиск должника для последующего удаления\n";
        int id = search();
        if (id == -1) {
            return;
        }
        fstream file;
        file.open(dir, ios::out);
        int counter = 0;
        while (counter < size-1) {
            if (counter != id) {
                file << debtors[counter].book_name << " " << debtors[counter].author << " " << debtors[counter].fio << " " << debtors[counter].ticket_number << " " << debtors[counter].date << " " << debtors[counter].who_give << "\n";
            }
            counter++;
        }
        file.close();
        cout << "Должник успешно удален из списка\n";
    }
    void reduct() {}
};
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Login log;
   
    log.sing_in();
    string login = log.get_login();
    int choice;
    if (log.getpermissions() == true) {
        system("cls");
        while (true)
        {

            cout << "\n\nBыберите раздел:\n   1)Логи\n   2)Пользователи\n   3)Читатели\n   4)Книги\n   5)Должники\n   6)Выход\n";
            cin >> choice;
            switch (choice)
            {
            case 1: {
                system("cls");
                Logs logs;
                cout << "Выберите действие:\n   1)Вывести таблицу логов\n   2)Назад\n\n";
                cin >> choice;
                switch (choice)
                {
                case 1: system("cls"); logs.check_logs(); break;
                case 2: break;
                default:
                    cout << "Неизвестная команда\n";
                    break;
                }
            } break;
            case 2: {
                system("cls");
                Users users;
                cout << "Выберите действие:\n   1)Вывести таблицу\n   2)Редактировать\n   3)Удалить\n   4)Функия поиска\n   5)Добавить новую строку\n   6)Назад\n\n";
                cin >> choice;
                switch (choice)
                {
                case 1: system("cls"); users.print(); break;
                case 2: system("cls"); users.reduct(); break;
                case 3: system("cls"); users.del(); break;
                case 4: system("cls"); users.search(); break;
                case 5: system("cls"); users.add(); break;
                case 6: system("cls"); break;
                default:
                    cout << "Неизвестная команда\n";
                    break;
                }
            }break;
            case 3: {
                system("cls");
                Reader readers;
                cout << "Выберите действие:\n   1)Вывести таблицу\n   2)Редактировать\n   3)Удалить\n   4)Функия поиска\n   5)Добавить новую строку\n   6)Назад\n\n";
                cin >> choice;
                switch (choice)
                {
                case 1:system("cls"); readers.print(); break;
                case 2:system("cls"); readers.reduct(); break;
                case 3:system("cls"); readers.del(); break;
                case 4:system("cls"); readers.search(); break;
                case 5:system("cls"); readers.add(); break;
                case 6:system("cls"); break;
                default:
                    cout << "Неизвестная команда\n";
                    break;
                }
            }break;
            case 4: {
                system("cls");
                Books books;
                cout << "Выберите действие:\n   1)Вывести таблицу\n   2)Редактировать\n   3)Удалить\n   4)Функия поиска\n   5)Добавить новую строку\n   6)Назад\n\n";
                cin >> choice;
                switch (choice)
                {
                case 1:system("cls"); books.print(); break;
                case 2:system("cls"); books.reduct(); break;
                case 3:system("cls"); books.del(); break;
                case 4:system("cls"); books.search(); break;
                case 5:system("cls"); books.add(); break;
                case 6:system("cls"); break;
                default:
                    cout << "Неизвестная команда\n";
                    break;
                }
            }break;
            case 5: {
                system("cls");
                Debtor debtors(login);
                cout << "Выберите действие:\n   1)Вывести таблицу\n   2)Удалить\n   3)Функия поиска\n   4)Добавить новую строку\n   5)Назад\n\n";
                cin >> choice;
                switch (choice)
                {
                case 1:system("cls"); debtors.print(); break;
                
                case 2:system("cls"); debtors.del(); break;
                case 3:system("cls"); debtors.search(); break;
                case 4:system("cls"); debtors.add(); break;
                case 5:system("cls"); break;
                default:
                    cout << "Неизвестная команда\n";
                    break;
                }
            }break;
            case 6: {
                system("cls");
                cout << "\n\n\nЗавершение работы....\nДля выхода нажмите любую клавишу";
                exit(0);
            }break;
            default:
                cout << "Неизвестная команда\n";
                break;
            }

        }
    }
    else {
        
        system("cls");
        while (true)
        {

            cout << "Bыберите раздел:\n   1)Читатели\n   2)Книги\n   3)Должники\n   4)Выход\n";
            cin >> choice;
            switch (choice)
            {
            case 1: {
                system("cls");
                Reader readers;
                cout << "Выберите действие:\n   1)Вывести таблицу\n   2)Редактировать\n   3)Удалить\n   4)Функия поиска\n   5)Добавить новую строку\n   6)Назад\n\n";
                cin >> choice;
                switch (choice)
                {
                case 1:system("cls"); readers.print(); break;
                case 2:system("cls"); readers.reduct(); break;
                case 3:system("cls"); readers.del(); break;
                case 4:system("cls"); readers.search(); break;
                case 5:system("cls"); readers.add(); break;
                case 6:system("cls"); break;
                default:
                    cout << "Неизвестная команда\n";
                    break;
                }
            }break;
            case 2: {
                system("cls");
                Books books;
                cout << "Выберите действие:\n   1)Вывести таблицу\n   2)Редактировать\n   3)Удалить\n   4)Функия поиска\n   5)Добавить новую строку\n   6)Назад\n\n";
                cin >> choice;
                switch (choice)
                {
                case 1:system("cls"); books.print(); break;
                case 2:system("cls"); books.reduct(); break;
                case 3:system("cls"); books.del(); break;
                case 4:system("cls"); books.search(); break;
                case 5:system("cls"); books.add(); break;
                case 6:system("cls"); break;
                default:
                    cout << "Неизвестная команда\n";
                    break;
                }
            }break;
            case 3: {
                system("cls");
                Debtor debtors(login);
                cout << "Выберите действие:\n   1)Вывести таблицу\n   2)Удалить\n   3)Функия поиска\n   4)Добавить новую строку\n   5)Назад\n\n";
                cin >> choice;
                switch (choice)
                {
                case 1:system("cls"); debtors.print(); break;

                case 2:system("cls"); debtors.del(); break;
                case 3:system("cls"); debtors.search(); break;
                case 4:system("cls"); debtors.add(); break;
                case 5:system("cls"); break;
                default:
                    cout << "Неизвестная команда\n";
                    break;
                }
            }break;
            case 4: {
                system("cls");
                cout << "\n\n\nЗавершение работы....\nДля выхода нажмите любую клавишу";
                exit(0);
            }break;
            default:
                cout << "Неизвестная команда\n";
                break;
            }

        }
    }
}