/* 
bảng màu
0 = Black      8 = Gray
1 = Blue       9 = Light Blue
2 = Green      10 = Light Green
3 = Aqua       11 = Light Aqua
4 = Red        12 = Light Red
5 = Purple     13 = Light Purple
6 = Yellow     14 = Light Yellow
7 = White      15 = Bright White
*/
//afafaadf 
#include <iostream>
#include <conio.h>
#include <windows.h>
#define KEY_UP 72   // dùng các phím mũi tên để di chuyển, phím a để chọn và phím x để thoát
#define KEY_DOWN 80 // i trong mảng hai chiều tương đương vs 0y, j trong mảng hai chiều thì ox
#define KEY_LEFT 75
#define KEY_RIGHT 77
using namespace std;
int size = 0;
int array[20][20];
int size_lastgame = 0;
void initialize(); // khởi tạo mảng 2 chiều và random mine
int asklevel();    // hỏi mức độ level
int line = 0, score = 0;
int scores[10];
int lastgame[20][20];
// vì có in 5 dòng để yêu cầu ng chơi chọn, nên array[y-5][x] với x=0, y=5 là vị
// trí của con trỏ sau khi in 5 dòng đó, phần tử mảng array[y-5][x] sẽ là phần tử đầu tiên
void randommine();                                  //random mine nhiều vị trí khác nhau
void dfs(int i, int j);                             // tìm kiếm viền, nếu chọn 0 thì sẽ loang ra đến viền mine
void print();                                       // print mảng 2 chiều
void SetColor(int backgound_color, int text_color); // set color chữ và background
void mark(int x, int y);                            // đánh dấu vị trí của bom theo 8 hướng
// vi dụ
// 1 1 1
// 1 8 1
// 1 1 1
void gotoxy(short x, short y); // đi đến 1 vị trí nào đó trên console
void print1();                 //print ra các kí tự * tượng trưng  cho các ô khi bắt đầu chơi
void print2();                 // print ra các mine khi chọn trúng
int options();
void playing();
void initialize1();
int main()
{

tieptuc:
    switch (options())
    {

    case 1:
        line = 5;
        initialize();
        print1();
        playing();
        break;
    case 2:
        line = 1;
        initialize1();
        playing();
    default:
        break;
    }

    goto tieptuc;

    getch();
    return 0;
}

void SetColor(int backgound_color, int text_color)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    int color_code = backgound_color * 16 + text_color;
    SetConsoleTextAttribute(hStdout, color_code);
}
void initialize()
{
    int i, j;
    system("cls");
    gotoxy(0, 0);
    size = asklevel();
    for (i = 0; i <= size - 1; ++i)
    {
        for (j = 0; j <= size - 1; ++j)
            array[i][j] = 0;
    }
    randommine();
    for (i = 0; i <= size - 1; ++i)
    {
        for (j = 0; j <= size - 1; ++j)
        {
            if (array[i][j] == 8)
                mark(i, j);
        }
    }
}
void initialize1()
{
    system("cls");
    size = size_lastgame;
    cout << "welcome back to last game :D" << endl;
    int i, j;
    for (i = 0; i <= size - 1; ++i)
    {
        for (j = 0; j <= size - 1; ++j)
            array[i][j] = lastgame[i][j];
    }
    for (i = 0; i <= size - 1; ++i)
    {
        for (j = 0; j <= size - 1; ++j)
            if (array[i][j] >= 0)
                cout << "*";
            else if (array[i][j] < 0)
            {
                if (array[i][j] == -1)
                {
                    SetColor(7, 7);
                }
                else
                    SetColor(0, 14);
                cout << -(array[i][j] + 1);
            }

        cout << endl;
    }
    
}
void randommine()
{
    int i = 1, x, y;

    while (i <= size)
    {
        x = rand() % (size); // dựa trên kiến thức nếu muốn random 1 số trong [a;b]
        y = rand() % (size); // thì lấy số rand chia cho [b+1-a] sẽ ra được số đu luôn bé hơn hoặc bằng b-1
        if (array[x][y] == 0)
        {
            array[x][y] = 8;
            ++i;
        }
    }
}
void dfs(int i, int j)
{
    if ((i < 0) or (j < 0) or (i > size - 1) or (j > size - 1))
        return; // nếu quá miền xác định của mảng, return

    if (array[i][j] != 8) // nếu khác ô có bom, in ra số ở ô đó
    {
        gotoxy(j, i + line);
        if (array[i][j] == 0)
            SetColor(7, 7);
        else if (array[i][j] > 0)
            SetColor(0, 14);
        cout << array[i][j];
        gotoxy(j, i + line);
        if (array[i][j] > 0)
            array[i][j] = -array[i][j] - 1;
    };

    if (array[i][j] == 0) // bằng không, thì tiếp tục loang, không thì chỉ mở
    {
        array[i][j] -= 1;
        if (array[i - 1][j] >= 0) // phải lớn hơn không vì có thể sé xét vào ô đã được mở
            dfs(i - 1, j);
        if (array[i + 1][j] >= 0)
            dfs(i + 1, j);
        if (array[i][j + 1] >= 0)
            dfs(i, j + 1);
        if (array[i][j - 1] >= 0)
            dfs(i, j - 1);
    }
}
void print()
{
    int i, j;
    for (i = 0; i <= size - 1; ++i)
    {
        for (j = 0; j <= size - 1; ++j)
        {
            if (array[i][j] == 8)
            {
                SetColor(0, 12);
                cout << array[i][j] << " ";
                SetColor(0, 7);
            }
            else if (array[i][j] > 0)
            {
                SetColor(0, 14);
                cout << array[i][j] << " ";
                SetColor(0, 7);
            }
            else
                cout << array[i][j] << " ";
        }

        cout << endl;
    }
}
int asklevel()
{
    cout << " which level do you want to play? \n1.beginner \n2.intermediate \n3.advanced" << endl;
    cout << "enter 1,2 or 3: ";
    char key = ' ';
    cin >> key;
    int size_array = 0;
    switch (key)
    {
    case '1':
        size_array = 12;
        break;
    case '2':
        size_array = 16;
        break;
    case '3':
        size_array = 20;
        break;
    default:
        cout << " no matching found, enter again" << endl;
        break;
    }
    return size_array;
}
void mark(int x, int y)
{

    if (x > 0)
        if (array[x - 1][y] != 8)
            array[x - 1][y] += 1; //north
    if (x < size - 1)
        if (array[x + 1][y] != 8)
            array[x + 1][y] += 1; //south
    if (y < size - 1)
        if (array[x][y + 1] != 8)
            array[x][y + 1] += 1; //east
    if (y > 0)
        if (array[x][y - 1] != 8)
            array[x][y - 1] += 1; //west
    if ((x < size - 1) && (y > 0))
        if (array[x + 1][y - 1] != 8)
            array[x + 1][y - 1] += 1; //south west
    if ((x < size - 1) & (y < size - 1))
        if (array[x + 1][y + 1] != 8)
            array[x + 1][y + 1] += 1; // south east
    if ((x > 0) && (y > 0))
        if (array[x - 1][y - 1] != 8)
            array[x - 1][y - 1] += 1; //north west
    if ((x > 0) && (y < size - 1))
        if (array[x - 1][y + 1] != 8)
            array[x - 1][y + 1] += 1; // north east
}
void gotoxy(short x, short y)
{
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = {x, y};
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}
void print1()
{
    int i, j;

    for (i = 0; i <= size - 1; ++i)
    {
        for (j = 0; j <= size - 1; ++j)
        {
            cout << "*";
        }
        cout << endl;
    }
}
void print2()
{
    gotoxy(0, line);
    int i, j;
    for (i = 1; i <= size - 1; ++i)
        for (j = 1; j <= size - 1; ++j)
            if ((array[i][j]) == 8)
            {
                SetColor(0, 12);
                gotoxy(j, i + 5);
                cout << array[i][j];
                SetColor(0, 7);
                gotoxy(j, i + 5);
            }
}
int options()
{
    system("cls");
    cout << "Minesweepere demo \nWelcome! \n1.New game \n2.Continue playing \n3.Highest score \n4.Quit" << endl;
    cout << "press 1, 2, 3 or 4 to choose: ";
    int key;
    cin >> key;
    return key;
}
void playing()
{
    int c = 0; // ký tự nhập từ bàn phím
    int x = 0, y = 0 + line;
    gotoxy(0, 0 + line); // x,y tọa độ
    SetColor(5, 7);
    if (array[y - line][x] >= 0)
    {
        SetColor(5, 7);
        cout << "*";
    }
    else if (array[y - line][x] == -1)
    {
        SetColor(5, 5);
        cout << "*";
    }
    if (array[y - line][x] < -1)
    {
        SetColor(5, 14);
        cout << -(array[y - line][x] + 1);
    }
    gotoxy(x, y);

    while (c != 120)
    {
        c = getch();
        if (array[y - line][x] >= 0)
        {
            SetColor(0, 7);
            cout << "*";
        }
        else if (array[y - line][x] == -1)
        {
            SetColor(7, 7);
            cout << "*";
        }
        if (array[y - line][x] < -1)
        {
            SetColor(0, 14);
            cout << -(array[y - line][x] + 1);
        }
        gotoxy(x, y);
        switch (c)
        {

        case KEY_UP:

            --y;

            gotoxy(x, y); //key up
            if (array[y - line][x] >= 0)
            {
                SetColor(5, 7);
                cout << "*";
            }
            else if (array[y - line][x] == -1)
            {
                SetColor(5, 5);
                cout << "*";
            }
            if (array[y - line][x] < -1)
            {
                SetColor(5, 14);
                cout << -(array[y - line][x] + 1);
            }
            gotoxy(x, y);
            break;
        case KEY_DOWN:

            ++y;

            gotoxy(x, y); // key down
            if (array[y - line][x] >= 0)
            {
                SetColor(5, 7);
                cout << "*";
            }
            else if (array[y - line][x] == -1)
            {
                SetColor(5, 5);
                cout << "*";
            }
            if (array[y - line][x] < -1)
            {
                SetColor(5, 14);
                cout << -(array[y - 5][x] + 1);
            }
            gotoxy(x, y);
            break;
        case KEY_LEFT:

            --x;

            gotoxy(x, y); // key left
            if (array[y - line][x] >= 0)
            {
                SetColor(5, 7);
                cout << "*";
            }
            else if (array[y - line][x] == -1)
            {
                SetColor(5, 5);
                cout << "*";
            }
            if (array[y - line][x] < -1)
            {
                SetColor(5, 14);
                cout << -(array[y - 5][x] + 1);
            }
            gotoxy(x, y);
            break;
        case KEY_RIGHT:

            ++x;

            gotoxy(x, y); // key right
            if (array[y - line][x] >= 0)
            {
                SetColor(5, 7);
                cout << "*";
            }
            else if (array[y - line][x] == -1)
            {
                SetColor(5, 5);
                cout << "*";
            }
            if (array[y - line][x] < -1)
            {
                SetColor(5, 14);
                cout << -(array[y - line][x] + 1);
            }
            gotoxy(x, y);
            break;
        }

        if (c == 97) // nếu c='a'

        {
            if ((array[y - line][x] > 0) && (array[y - line][x] != 8)) // nếu phần tử được chọn là  chỉ số  mật độ
            {
                SetColor(5, 14);
                cout << array[y - line][x];
                gotoxy(x, y);
                array[y - line][x] = -array[y - line][x] - 1; // gán giá trị mới cho phần tử đó để khi lưu lại
                SetColor(0, 7);                               // phân biệt được các ô đã mở hay chưa mở bằng cách
                // chỉ mở, không loang                  // kiểm tra dương hay âm, dương là chưa mở, âm là mở rồi
                ++score; // cộng điểm
            }
            else if (array[y - line][x] == 8) // nếu dính bom
            {
                print2();
                gotoxy(0, line + size);
                cout << "failed!!!" << endl;
                break;
            }
            else if (array[y - line][x] == 0) // nếu ô mở là không có gì, bắt đầu loang
            {                                 // tránh trường hợp chọn lại 1 ô, nó sẽ cho ra giá trị âm, lộ dữ liệu
                dfs(y - line, x);
                gotoxy(x, y);
                SetColor(5, 5);
                cout << "*";
                gotoxy(x, y);
                ++score;
            }
        }
        
    }
    SetColor(0, 7);
    if (c == 120) // nếu c='x' nghĩa là đang chơi nhưng thoát ra
    {
        gotoxy(0, line + size);
        cout << "you could do better!" << endl;
        cout << "score: " << score << endl;
        cout << "wanna save the game for playing later? y/n: ";
        char key;
        cin >> key;
        if (tolower(key) == 'y')
        {
            int i, j;
            for (i = 0; i <= size - 1; ++i)
                for (j = 0; j <= size - 1; ++j)
                    lastgame[i][j] = array[i][j];
        }
        size_lastgame = size;
        
        
        return;
        cout << "press any key to return to menu";
        getch();
        return;
    }
    gotoxy(0, line + size);
    cout << "failed!!!" << endl;
             cout << "press any key to return to menu";
    getch();
}
