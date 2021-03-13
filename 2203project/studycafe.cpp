#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <ctime> 
#include <string>
#include <Windows.h>
#include <mysql.h>

using namespace std;

#pragma comment(lib, "libmysql.lib")

void connectDB();

struct cafe
{
    int state; // 0-> 입실되어있음 1 -> 비어있음
    time_t start;
    time_t end;
    string phone;
};


MYSQL Conn;
MYSQL* ConnPtr = NULL;
MYSQL_RES* Result;
MYSQL_ROW Row;
int Stat;

class Database {
private:

public:
    void addSalesDB(int money) {
        char temp[10];
        string Query = "";
        Query += "insert into sales(price) values (";
        _itoa(money, temp, 10);
        Query += temp;
        Query += ")";
        Stat = mysql_query(ConnPtr, Query.c_str());
        if (Stat != 0) {
            cout << stderr << "error : " << mysql_error(&Conn);
        }
    }

    void printSalesDB() {
        string Query = "select date_format(payment_date, '%Y-%m-%d')as Date, price, count(id) as customer, sum(price) as total_price from sales where date_format(payment_date, '%Y-%m-%d')=curdate() group by price";
        Stat = mysql_query(ConnPtr, Query.c_str());

        if (Stat != 0)
        {
            cout << stderr << "error : " << mysql_error(&Conn);
        }
        else {
            Result = mysql_store_result(ConnPtr);
            while ((Row = mysql_fetch_row(Result)) != NULL)
            {
                cout << "\t    " << Row[0] << "\t\t " << Row[1] << "\t\t\t " << Row[2] << " \t\t " << Row[3] << endl;
            }
            mysql_free_result(Result);
        }
    }

    void totalSalesDB() {
        string Query = "select sum(price) as total_price from sales where date_format(payment_date, '%Y-%m-%d') = curdate()";
        Stat = mysql_query(ConnPtr, Query.c_str());

        if (Stat != 0)
        {
            cout << stderr << "error : " << mysql_error(&Conn);
        }
        else {
            Result = mysql_store_result(ConnPtr);
            while ((Row = mysql_fetch_row(Result)) != NULL)
            {
                cout << Row[0];
            }
            mysql_free_result(Result);
        }
    }
};

class User
{
protected:
    string user;
public:
    User() {

    }
    ~User() {

    }
};

class StudyCafe : public User
{
private:
    cafe** c;
    int a;
    int b;//멤버 변수로
    struct tm start_tm;
    struct tm end_tm;
    time_t start_t;
    time_t end_t;
    Database db;

public:
    StudyCafe() {
        user = "사용자";
        db = Database();
    }
    ~StudyCafe() {}

    void make_node(int a, int b)
    {
        int i, j;

        this->a = a;
        this->b = b;//입실공간 상태를 초기화

        //cafe **c = new cafe*[a];
        c = new cafe * [a];
        for (i = 0; i < a; i++)
        {
            c[i] = new cafe[b];
        }
        for (i = 0; i < a; i++)
        {
            for (j = 0; j < b; j++)
            {
                c[i][j].state = 1; //비어있는 곳을 1로 초기화
            }
        }
    }  // 동적할당 한 부분

    void print()
    {
        cout << endl << endl << endl;
        cout << "\t\t\t\t\t===================" << endl;
        cout << "\t\t\t\t\t 1.   좌 석  상 태 " << endl;
        cout << "\t\t\t\t\t 2.   입        실 " << endl;
        cout << "\t\t\t\t\t 3.   퇴        실 " << endl;
        cout << "\t\t\t\t\t 4.   관   리   자 " << endl;
        cout << "\t\t\t\t\t===================" << endl;
        cout << endl;
    }

    void seatstate() // 입실 상태
    {
        int i, j;
        int count = 0;
        cout << "[ " << user << " ]" << endl;
        cout << endl << endl;
        cout << "\t\t\t\t\t   <좌석 상태>" << endl << endl;
        cout << endl << endl;
        for (i = 0; i < a; i++)
        {
            cout << "\t\t\t\t\t  <<< " << (i + 1) << "층 >>>" << endl << endl;
            for (j = 0; j < b; j++)
            {
                if (!c[i][j].state == 0)
                {
                    cout << "\t[" << j + 1 << "번" << "] \t\t";
                }
                else
                {
                    cout << "\t[" << j + 1 << "번" << "] " << "입실됨" << " " << "\t";
                    count++;
                }
            }
            cout << endl << endl;
            cout << "\t-----------------------------------------------------------------------------------" << endl;
            cout << endl << endl;
        }
        cout << endl;
        cout << "\t\t\t\t\t[ 잔여좌석 : " << (a * b) - count << "석 ]" << endl;
        cout << endl;
    }

    void incoming()       //입실
    {
        int i, j;

        cout << "\t\t\t\t\t층 입력 : ";
        cin >> i;
        cout << "\t\t\t\t\t자리 입력 : ";
        cin >> j;
        cout << endl;
        cout << endl;
        if (i <= 4 && j <= 4) {
            if (c[i - 1][j - 1].state == 0) {
                system("cls");
                cout << endl << endl << endl << endl << endl << endl;
                cout << "\t\t\t\t===========================================" << endl;
                cout << "\t\t\t\t    ** 이 좌석은 이미 사용 중입니다. **" << endl << endl;
                cout << "\t\t\t\t      ** 다른 좌석을 이용해주세요. **" << endl;
                cout << "\t\t\t\t===========================================" << endl;
                cout << endl;
            }
            else if (c[i - 1][j - 1].state == 1) {
                cout << "\t\t\t\t     전화번호를 입력하세요 : ";
                cin >> c[i - 1][j - 1].phone;
                cout << endl;

                start_t = time(NULL);
                c[i - 1][j - 1].start = start_t;
                localtime_s(&start_tm, &start_t);
                c[i - 1][j - 1].state = 0;
                //cout << start_tm.tm_year + 1900 << "년" << endl;
                cout << endl;
                system("cls");
                cout << endl << endl << endl << endl << endl << endl;
                cout << "\t\t\t\t ------------------------------------------" << endl;
                cout << "\t\t\t\t|\t\t  [ " << i << "층 " << j << "번 ]\t\t   | " << endl;
                cout << "\t\t\t\t|        << 입실이 완료되었습니다 >>       |" << endl;
                cout << "\t\t\t\t ------------------------------------------" << endl;
            }
        }
        else {
            system("cls");
            cout << endl << endl << endl << endl << endl << endl;
            cout << "\t\t\t\t===================================" << endl;
            cout << "\t\t\t\t  ** 존재하지 않는 좌석입니다. **" << endl << endl;
            cout << "\t\t\t\t     ** 다시 입력해주세요. **" << endl;
            cout << "\t\t\t\t===================================" << endl;
            cout << endl;
        }
    }

    void outcoming()   //퇴실
    {
        int i, j;
        int day, hour, min, sec;
        int money = 0;
        int use_time;
        string phone_check;
        cout << "\t\t\t\t\t층 입력 : ";
        cin >> i;
        cout << "\t\t\t\t\t자리 입력 : ";
        cin >> j;
        cout << endl;

        if (i <= 4 && j <= 4) {
            if (c[i - 1][j - 1].state == 0) {
                cout << "\t\t\t\t   전화번호를 입력하세요 : ";
                cin >> phone_check;
                if ((c[i - 1][j - 1].phone.compare(phone_check)) == 0) {
                    cout << endl;
                    end_t = time(NULL);
                    c[i - 1][j - 1].end = end_t;
                    localtime_s(&end_tm, &end_t);
                    use_time = difftime(c[i - 1][j - 1].end, c[i - 1][j - 1].start);
                    day = use_time / (3600 * 24);
                    use_time = use_time - (day * 3600 * 24);

                    hour = use_time / (3600);
                    use_time = use_time - (hour * 3600);

                    min = use_time / 60;
                    use_time = use_time - (min * 60);

                    sec = use_time;
                    Sleep(1000);
                    system("cls");
                    cout << endl << endl << endl << endl << endl;
                    cout << "\t\t\t=================================================" << endl << endl;
                    cout << "\t\t\t\t   ** 이용 시간 : " << hour << "시간 " <<
                        min << "분 " << sec << "초 **" << endl << endl;
                    if (hour <= 1 && min < 59 && sec < 59) {
                        money = 1000;
                    }
                    else if (hour <= 2 && min <= 59 && sec <= 59) {
                        money = 2000;
                    }
                    else if (hour <= 3 && min <= 59 && sec <= 59) {
                        money = 4000;
                    }
                    else if (hour <= 5 && min <= 59 && sec <= 59) {
                        money = 7000;
                    }
                    else if (hour <= 11 && min <= 59 && sec <= 59) {
                        money = 10000;
                    }
                    cout << "\t\t\t\t      *요금 : " << money << "원 입니다.*" << endl << endl;
                    cout << "\t\t\t\t   * 이용해 주셔서 감사합니다. *" << endl << endl;
                    cout << "\t\t\t=================================================" << endl;
                    db.addSalesDB(money);
                    c[i - 1][j - 1].state = 1;
                    c[i - 1][j - 1].phone = "";
                    Sleep(6000);
                    system("cls");
                    cout << endl;
                }
                else {
                    system("cls");
                    cout << endl << endl << endl << endl << endl << endl;
                    cout << "\t\t\t\t*********************************" << endl;
                    cout << "\t\t\t\t* 전화번호가 일치하지 않습니다. *" << endl;
                    cout << "\t\t\t\t*********************************" << endl;
                    Sleep(2000);
                    system("cls");
                    cout << endl;
                }

            }
            else if (c[i - 1][j - 1].state == 1) {
                system("cls");
                cout << endl << endl << endl << endl << endl << endl;
                cout << "\t\t\t\t===========================================" << endl;
                cout << "\t\t\t\t  ** 이 좌석은 입실되어 있지 않습니다. **" << endl << endl;
                cout << "\t\t\t\t          ** 다시 입력해주세요. **" << endl;
                cout << "\t\t\t\t===========================================" << endl;
                cout << endl;
                Sleep(2000);
                system("cls");
            }
        }
        else {
            system("cls");
            cout << endl << endl << endl << endl << endl << endl;
            cout << "\t\t\t\t===================================" << endl;
            cout << "\t\t\t\t  ** 존재하지 않는 좌석입니다. **" << endl << endl;
            cout << "\t\t\t\t      ** 다시 입력해주세요. **" << endl;
            cout << "\t\t\t\t===================================" << endl;
            cout << endl;
            Sleep(2000);
            system("cls");
        }
    }
};

class Manager : public User{
private:
    int password = 2203;
    Database db;
public:
    Manager() {
        db = Database();
        user = "관리자";
    }
    ~Manager() {

    }

    int passCheck(int pass)   //관리자
    {
        if (password != pass) {
            return -1;
        }
        return 0;
    }

    void print() {
        cout << "[ " << user << " ]";
        cout << endl << endl << endl;
        cout << "\t\t\t\t\t===================" << endl;
        cout << "\t\t\t\t\t 1.   매 출  확 인 " << endl;
        cout << endl;
        cout << "\t\t\t\t\t 2.   프로그램종료 " << endl;
        cout << endl;
        cout << "\t\t\t\t\t 3.   돌 아  가 기 " << endl;
        cout << "\t\t\t\t\t===================" << endl;
        cout << endl;
    }

    void salesPrint() {
        cout << endl << endl << endl << endl;
        cout << "\t\t\t\t\t [ 일일 매출 확인 ] " << endl << endl;
        cout << "\t================================================================================" << endl << endl;
        cout << endl << "\t    날짜" << " \t\t " << "가격" << "\t\t\t" << "수량" << " \t      " << "가격 별 매출 " << endl << endl;
        cout << "\t================================================================================" << endl << endl;
        db.printSalesDB();
        cout << endl;
        cout << "\t================================================================================" << endl << endl;

        cout << "\t\t\t\t***************************************" << endl;
        cout << "\t\t\t\t* 오늘의 총 매출액은 ";
        db.totalSalesDB();
        cout << "원 입니다.  *" << endl;
        cout << "\t\t\t\t***************************************" << endl;
    }

};


int main()
{
    system("mode con lines=45 cols=100");
    connectDB();
    int n1, n2;
    int back;
    StudyCafe study;
    Manager manager;

    study.make_node(4, 4);

    while (1)
    {
        study.print();
        cout << "\t\t\t\t   원하는 작업을 선택해주세요 : ";
        cin >> n1;
        cout << endl;
        switch (n1)
        {
        case 1:
            system("cls");
            study.seatstate();
            cout << "\t\t\t\t이전 페이지로 돌아가려면 0 입력 : ";
            cin >> back;
            if (back == 0) {
                system("cls");
                break;
            }
            break;
        case 2:
            system("cls");
            study.seatstate();
            cout << "\t\t\t\t     입실할 자리를 입력하세요." << endl;
            study.incoming();
            Sleep(4000);
            system("cls");
            cout << endl;
            break;
        case 3:
            system("cls");
            study.seatstate();
            cout << "\t\t\t\t    퇴실할 자리를 선택해주세요" << endl;
            study.outcoming();
            cout << endl;
            break;
        case 4:
            system("cls");
            int pass;
            cout << endl << endl << endl << endl << endl;
            cout << "\t\t\t\t     관리자 비밀번호 입력 : ";
            cin >> pass;
            if (manager.passCheck(pass) == 0) {
                cout << endl << endl << endl;
                cout << "\t\t\t\t   << 관리자 페이지로 전환됩니다 >>" << endl;
                Sleep(2000);
                system("cls");
                manager.print();
                cout << endl;
                cout << "\t\t\t\t   원하는 작업을 선택해주세요 : ";
                cin >> n2;
                cout << endl << endl;
                switch (n2)
                {
                case 1:
                    system("cls");
                    manager.salesPrint();
                    cout << endl << endl;
                    cout << "\t\t\t\t이전 페이지로 돌아가려면 0 입력 : ";
                    cin >> back;
                    if (back == 0) {
                        system("cls");
                        break;
                    }
                case 2:
                    system("cls");
                    cout << endl << endl << endl << endl << endl;
                    cout << "\t\t\t\t<<프로그램이 종료되었습니다.>>" << endl;
                    exit(0);
                case 3:
                    system("cls");
                    break;
                default:
                    cout << "\t\t\t\t*** 잘못된 번호입니다. ***" << endl << endl;
                    cout << "\t\t\t\t    *** 번호를 다시 입력해주세요. ***" << endl;
                    Sleep(2000);
                    system("cls");
                    break;
                }
            }
            else {
                cout << endl << endl;
                cout << "\t\t\t\t  **비밀번호가 일치하지 않습니다.**" << endl;
                Sleep(2000);
                system("cls");
                break;
            }
            cout << endl;
            break;

        default:
            cout << "\t\t\t\t\t*** 잘못된 번호입니다. ***" << endl << endl;
            cout << "\t\t\t\t    ***번호를 다시 입력해주세요. ***" << endl;
            Sleep(2000);
            system("cls");
            break;
        }
    }
    mysql_close(&Conn);

    return 0;

}

void connectDB()
{

    mysql_init(&Conn);

    ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "mirim", "studycafe", 3307, NULL, 0);

    if (ConnPtr == NULL) {
        cout << "error : " << mysql_error(&Conn);
    }
    else {
    }
}