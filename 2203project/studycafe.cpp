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
    int state; // 0-> �ԽǵǾ����� 1 -> �������
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
    int b;//��� ������
    struct tm start_tm;
    struct tm end_tm;
    time_t start_t;
    time_t end_t;
    Database db;

public:
    StudyCafe() {
        user = "�����";
        db = Database();
    }
    ~StudyCafe() {}

    void make_node(int a, int b)
    {
        int i, j;

        this->a = a;
        this->b = b;//�Խǰ��� ���¸� �ʱ�ȭ

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
                c[i][j].state = 1; //����ִ� ���� 1�� �ʱ�ȭ
            }
        }
    }  // �����Ҵ� �� �κ�

    void print()
    {
        cout << endl << endl << endl;
        cout << "\t\t\t\t\t===================" << endl;
        cout << "\t\t\t\t\t 1.   �� ��  �� �� " << endl;
        cout << "\t\t\t\t\t 2.   ��        �� " << endl;
        cout << "\t\t\t\t\t 3.   ��        �� " << endl;
        cout << "\t\t\t\t\t 4.   ��   ��   �� " << endl;
        cout << "\t\t\t\t\t===================" << endl;
        cout << endl;
    }

    void seatstate() // �Խ� ����
    {
        int i, j;
        int count = 0;
        cout << "[ " << user << " ]" << endl;
        cout << endl << endl;
        cout << "\t\t\t\t\t   <�¼� ����>" << endl << endl;
        cout << endl << endl;
        for (i = 0; i < a; i++)
        {
            cout << "\t\t\t\t\t  <<< " << (i + 1) << "�� >>>" << endl << endl;
            for (j = 0; j < b; j++)
            {
                if (!c[i][j].state == 0)
                {
                    cout << "\t[" << j + 1 << "��" << "] \t\t";
                }
                else
                {
                    cout << "\t[" << j + 1 << "��" << "] " << "�Խǵ�" << " " << "\t";
                    count++;
                }
            }
            cout << endl << endl;
            cout << "\t-----------------------------------------------------------------------------------" << endl;
            cout << endl << endl;
        }
        cout << endl;
        cout << "\t\t\t\t\t[ �ܿ��¼� : " << (a * b) - count << "�� ]" << endl;
        cout << endl;
    }

    void incoming()       //�Խ�
    {
        int i, j;

        cout << "\t\t\t\t\t�� �Է� : ";
        cin >> i;
        cout << "\t\t\t\t\t�ڸ� �Է� : ";
        cin >> j;
        cout << endl;
        cout << endl;
        if (i <= 4 && j <= 4) {
            if (c[i - 1][j - 1].state == 0) {
                system("cls");
                cout << endl << endl << endl << endl << endl << endl;
                cout << "\t\t\t\t===========================================" << endl;
                cout << "\t\t\t\t    ** �� �¼��� �̹� ��� ���Դϴ�. **" << endl << endl;
                cout << "\t\t\t\t      ** �ٸ� �¼��� �̿����ּ���. **" << endl;
                cout << "\t\t\t\t===========================================" << endl;
                cout << endl;
            }
            else if (c[i - 1][j - 1].state == 1) {
                cout << "\t\t\t\t     ��ȭ��ȣ�� �Է��ϼ��� : ";
                cin >> c[i - 1][j - 1].phone;
                cout << endl;

                start_t = time(NULL);
                c[i - 1][j - 1].start = start_t;
                localtime_s(&start_tm, &start_t);
                c[i - 1][j - 1].state = 0;
                //cout << start_tm.tm_year + 1900 << "��" << endl;
                cout << endl;
                system("cls");
                cout << endl << endl << endl << endl << endl << endl;
                cout << "\t\t\t\t ------------------------------------------" << endl;
                cout << "\t\t\t\t|\t\t  [ " << i << "�� " << j << "�� ]\t\t   | " << endl;
                cout << "\t\t\t\t|        << �Խ��� �Ϸ�Ǿ����ϴ� >>       |" << endl;
                cout << "\t\t\t\t ------------------------------------------" << endl;
            }
        }
        else {
            system("cls");
            cout << endl << endl << endl << endl << endl << endl;
            cout << "\t\t\t\t===================================" << endl;
            cout << "\t\t\t\t  ** �������� �ʴ� �¼��Դϴ�. **" << endl << endl;
            cout << "\t\t\t\t     ** �ٽ� �Է����ּ���. **" << endl;
            cout << "\t\t\t\t===================================" << endl;
            cout << endl;
        }
    }

    void outcoming()   //���
    {
        int i, j;
        int day, hour, min, sec;
        int money = 0;
        int use_time;
        string phone_check;
        cout << "\t\t\t\t\t�� �Է� : ";
        cin >> i;
        cout << "\t\t\t\t\t�ڸ� �Է� : ";
        cin >> j;
        cout << endl;

        if (i <= 4 && j <= 4) {
            if (c[i - 1][j - 1].state == 0) {
                cout << "\t\t\t\t   ��ȭ��ȣ�� �Է��ϼ��� : ";
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
                    cout << "\t\t\t\t   ** �̿� �ð� : " << hour << "�ð� " <<
                        min << "�� " << sec << "�� **" << endl << endl;
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
                    cout << "\t\t\t\t      *��� : " << money << "�� �Դϴ�.*" << endl << endl;
                    cout << "\t\t\t\t   * �̿��� �ּż� �����մϴ�. *" << endl << endl;
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
                    cout << "\t\t\t\t* ��ȭ��ȣ�� ��ġ���� �ʽ��ϴ�. *" << endl;
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
                cout << "\t\t\t\t  ** �� �¼��� �ԽǵǾ� ���� �ʽ��ϴ�. **" << endl << endl;
                cout << "\t\t\t\t          ** �ٽ� �Է����ּ���. **" << endl;
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
            cout << "\t\t\t\t  ** �������� �ʴ� �¼��Դϴ�. **" << endl << endl;
            cout << "\t\t\t\t      ** �ٽ� �Է����ּ���. **" << endl;
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
        user = "������";
    }
    ~Manager() {

    }

    int passCheck(int pass)   //������
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
        cout << "\t\t\t\t\t 1.   �� ��  Ȯ �� " << endl;
        cout << endl;
        cout << "\t\t\t\t\t 2.   ���α׷����� " << endl;
        cout << endl;
        cout << "\t\t\t\t\t 3.   �� ��  �� �� " << endl;
        cout << "\t\t\t\t\t===================" << endl;
        cout << endl;
    }

    void salesPrint() {
        cout << endl << endl << endl << endl;
        cout << "\t\t\t\t\t [ ���� ���� Ȯ�� ] " << endl << endl;
        cout << "\t================================================================================" << endl << endl;
        cout << endl << "\t    ��¥" << " \t\t " << "����" << "\t\t\t" << "����" << " \t      " << "���� �� ���� " << endl << endl;
        cout << "\t================================================================================" << endl << endl;
        db.printSalesDB();
        cout << endl;
        cout << "\t================================================================================" << endl << endl;

        cout << "\t\t\t\t***************************************" << endl;
        cout << "\t\t\t\t* ������ �� ������� ";
        db.totalSalesDB();
        cout << "�� �Դϴ�.  *" << endl;
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
        cout << "\t\t\t\t   ���ϴ� �۾��� �������ּ��� : ";
        cin >> n1;
        cout << endl;
        switch (n1)
        {
        case 1:
            system("cls");
            study.seatstate();
            cout << "\t\t\t\t���� �������� ���ư����� 0 �Է� : ";
            cin >> back;
            if (back == 0) {
                system("cls");
                break;
            }
            break;
        case 2:
            system("cls");
            study.seatstate();
            cout << "\t\t\t\t     �Խ��� �ڸ��� �Է��ϼ���." << endl;
            study.incoming();
            Sleep(4000);
            system("cls");
            cout << endl;
            break;
        case 3:
            system("cls");
            study.seatstate();
            cout << "\t\t\t\t    ����� �ڸ��� �������ּ���" << endl;
            study.outcoming();
            cout << endl;
            break;
        case 4:
            system("cls");
            int pass;
            cout << endl << endl << endl << endl << endl;
            cout << "\t\t\t\t     ������ ��й�ȣ �Է� : ";
            cin >> pass;
            if (manager.passCheck(pass) == 0) {
                cout << endl << endl << endl;
                cout << "\t\t\t\t   << ������ �������� ��ȯ�˴ϴ� >>" << endl;
                Sleep(2000);
                system("cls");
                manager.print();
                cout << endl;
                cout << "\t\t\t\t   ���ϴ� �۾��� �������ּ��� : ";
                cin >> n2;
                cout << endl << endl;
                switch (n2)
                {
                case 1:
                    system("cls");
                    manager.salesPrint();
                    cout << endl << endl;
                    cout << "\t\t\t\t���� �������� ���ư����� 0 �Է� : ";
                    cin >> back;
                    if (back == 0) {
                        system("cls");
                        break;
                    }
                case 2:
                    system("cls");
                    cout << endl << endl << endl << endl << endl;
                    cout << "\t\t\t\t<<���α׷��� ����Ǿ����ϴ�.>>" << endl;
                    exit(0);
                case 3:
                    system("cls");
                    break;
                default:
                    cout << "\t\t\t\t*** �߸��� ��ȣ�Դϴ�. ***" << endl << endl;
                    cout << "\t\t\t\t    *** ��ȣ�� �ٽ� �Է����ּ���. ***" << endl;
                    Sleep(2000);
                    system("cls");
                    break;
                }
            }
            else {
                cout << endl << endl;
                cout << "\t\t\t\t  **��й�ȣ�� ��ġ���� �ʽ��ϴ�.**" << endl;
                Sleep(2000);
                system("cls");
                break;
            }
            cout << endl;
            break;

        default:
            cout << "\t\t\t\t\t*** �߸��� ��ȣ�Դϴ�. ***" << endl << endl;
            cout << "\t\t\t\t    ***��ȣ�� �ٽ� �Է����ּ���. ***" << endl;
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