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

//afafaadfaf afaf
#include <iostream>
#include <conio.h>

#include <windows.h>
#define KEY_UP 72   // dùng các phím mũi tên để di chuyển, phím a để chọn và phím x để thoát
#define KEY_DOWN 80 // i trong mảng hai chiều tương đương vs 0y, j trong mảng hai chiều thì ox
#define KEY_LEFT 75
#define KEY_RIGHT 77
using namespace std;

void initialize(int (&array)[20][20], int &size); // khởi tạo mảng 2 chiều và random mine
int asklevel();                                   // hỏi mức độ level

// vì có in 5 dòng để yêu cầu ng chơi chọn, nên array[y-5][x] với x=0, y=5 là vị
// trí của con trỏ sau khi in 5 dòng đó, phần tử mảng array[y-5][x] sẽ là phần tử đầu tiên
void randommine(int (&array)[20][20], int &size);                   //random mine nhiều vị trí khác nhau
void dfs(int i, int j, int &size, int (&array)[20][20], int &line); // tìm kiếm viền, nếu chọn 0 thì sẽ loang ra đến viền mine
void print(int (&array)[20][20], int &size);                        // print mảng 2 chiều
void SetColor(int backgound_color, int text_color);                 // set color chữ và background
void mark(int x, int y, int (&array)[20][20], int &size);           // đánh dấu vị trí của bom theo 8 hướng
// vi dụ
// 1 1 1
// 1 8 1
// 1 1 1
void gotoxy(short x, short y);                           // đi đến 1 vị trí nào đó trên console
void print1(int (&array)[20][20], int &size);            //print ra các kí tự * tượng trưng  cho các ô khi bắt đầu chơi
void print2(int (&array)[20][20], int &size, int &line); // print ra các mine khi chọn trúng
int options();
void playing(int (&array)[20][20], int &size, int &line, int &score, int (&lastgame)[20][20], int &size_lastgame);
//a afa  asg
void initialize_backup(int (&array)[20][20], int (&lastgame)[20][20], int &size_lastgame, int &size); // khoi tao hien trang cua saved game
void newstate(int (&array)[20][20], int x, int y, int &line, int &size, bool &unchanged);
void oldstate(int (&array)[20][20], int x, int y, int &line);
int main()
{
    int size = 0;
    int array[20][20];
    int size_lastgame = 0;
    int line = 0, score = 0;
    int scores[10];
    int lastgame[20][20];
tieptuc:
    switch (options())
    {

    case 1:
        line = 5;
        initialize(array, size);
        print1(array, size);
        playing(array, size, line, score, lastgame, size_lastgame);
        break;
    case 2:
        line = 1;
        initialize_backup(array, lastgame, size_lastgame, size);
        playing(array, size, line, score, lastgame, size_lastgame);
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
void initialize(int (&array)[20][20], int &size)
{
    system("cls");
    gotoxy(0, 0);
    size = asklevel();
    for (int i = 0; i <= size - 1; ++i)
    {
        for (int j = 0; j <= size - 1; ++j)
            array[i][j] = 0;
    }
    randommine(array, size);
    for (int i = 0; i <= size - 1; ++i)
    {
        for (int j = 0; j <= size - 1; ++j)
        {
            if (array[i][j] == 8)
                mark(i, j, array, size);
        }
    }
}
void initialize_backup(int (&array)[20][20], int (&lastgame)[20][20], int &size_lastgame, int &size)
{
    system("cls");
    size = size_lastgame;
    cout << "welcome back to last game :D" << endl;
    for (int i = 0; i <= size - 1; ++i)
    {
        for (int j = 0; j <= size - 1; ++j)
            array[i][j] = lastgame[i][j];
    }
    for (int i = 0; i <= size - 1; ++i)
    {
        for (int j = 0; j <= size - 1; ++j)
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
void randommine(int (&array)[20][20], int &size)
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
void dfs(int i, int j, int &size, int (&array)[20][20], int &line)
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
            dfs(i - 1, j, size, array, line);
        if (array[i + 1][j] >= 0)
            dfs(i + 1, j, size, array, line);
        if (array[i][j + 1] >= 0)
            dfs(i, j + 1, size, array, line);
        if (array[i][j - 1] >= 0)
            dfs(i, j - 1, size, array, line);
    }
}
void print(int (&array)[20][20], int &size)
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
void mark(int x, int y, int (&array)[20][20], int &size)
{
    /* 
    int column[3] = { -1; 0; 1 };
    int row[3] = { -1; 0; 1 };
    for( int i = 0; i < 3; ++i ) for( int j = 0; j < 3; ++j )
    {
        if( column[i] == 0 && row[j] == 0 ) continue;
        int _x = x + row[i], _y = y + column[j];
        if( _x < 0 || _x >= size || _y < 0 || _y >= size ) continue;
        if( array[_x][_y] == 8 ) continue;
        ++array[_x][_y];
    }
    */

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
void print1(int (&array)[20][20], int &size)
{

    for (int i = 0; i <= size - 1; ++i)
    {
        for (int j = 0; j <= size - 1; ++j)
            cout << "*";
        cout << endl;
    }
}
void print2(int (&array)[20][20], int &size, int &line)
{
    gotoxy(0, line);
    for (int i = 1; i <= size - 1; ++i)
        for (int j = 1; j <= size - 1; ++j)
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
void playing(int (&array)[20][20], int &size, int &line, int &score, int (&lastgame)[20][20], int &size_lastgame)
{
    int c = 0; // ký tự nhập từ bàn phím
    int x = 0, y = 0 + line;
    gotoxy(0, 0 + line); // x,y tọa độ
    SetColor(5, 7);
    // vị trí đầu tiên của con trỏ khi mới vào game
    // ô chưa được mở
    if (array[y - line][x] >= 0)
    {
        SetColor(5, 7); // nền tím, chữ trắng
        cout << "*";
    }
    // ô đã đoực mở nhưng không có gì
    else if (array[y - line][x] == -1)
    {
        SetColor(5, 5); // nền trắng, chữ trắng
        cout << "*";
    }
    //case khi hàm playing được call trong last game
    // ô đã được mở và được đánh dấu
    if (array[y - line][x] < -1)
    {
        SetColor(5, 14); // nền tím chữ vàng
        cout << -(array[y - line][x] + 1);
    }
    gotoxy(x, y);
    bool unchanged = false;
    // unchanged dể xác định trạng thái của con trỏ có thay đổi hay không
    // nếu quá phạm vi của bảng thì trạng thái vẫn giữ nguyên, không di chuyển
    while (c != 120)
    {
        c = getch();
        // ô chưa được mở
        while (c != 72 && c != 80 && c != 75 && c != 77 && c != 97)
            c = getch();
        // trả ô về trạng thái cũ sau khi con trỏ di chuyển snang chổ khác
        if (!unchanged)
        {
            oldstate(array, x, y, line);
        }

        // bắt đầu nhận vị trí mới
        switch (c)
        {
        case KEY_UP:
            --y;
            newstate(array, x, y, line, size, unchanged);
            if (unchanged)
            {
                ++y;
                newstate(array, x, y, line, size, unchanged);
            }
            break;
        case KEY_DOWN:
            ++y;
            newstate(array, x, y, line, size, unchanged);
            if (unchanged)
            {
                --y;
                newstate(array, x, y, line, size, unchanged);
            }
            break;
        case KEY_LEFT:
            --x;
            newstate(array, x, y, line, size, unchanged);
            if (unchanged)
            {
                ++x;
                newstate(array, x, y, line, size, unchanged);
            }
            break;
        case KEY_RIGHT:
            ++x;
            newstate(array, x, y, line, size, unchanged);
            if (unchanged)
            {
                --x;
                newstate(array, x, y, line, size, unchanged);
            }
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
                print2(array, size, line);
                gotoxy(0, line + size);
                cout << "failed!!!" << endl;
                break;
            }
            else if (array[y - line][x] == 0) // nếu ô mở là không có gì, bắt đầu loang
            {                                 // tránh trường hợp chọn lại 1 ô, nó sẽ cho ra giá trị âm, lộ dữ liệu
                dfs(y - line, x, size, array, line);
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
void newstate(int (&array)[20][20], int x, int y, int &line, int &size, bool &unchanged)
{
    // nếu trong phạm vi của bảng thì cho di chuyển đến
    if (0 <= x && x <= size - 1 && 0 <= (y - line) && (y - line) <= size - 1)
    {
        gotoxy(x, y);
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
        unchanged = false;
    }
    else
        unchanged = true;
}
void oldstate(int (&array)[20][20], int x, int y, int &line)
{

    if (array[y - line][x] >= 0)
    {
        SetColor(0, 7); // nền đen, chữ trắng
        cout << "*";
    }
    // ô đã mở nhưng không có gì
    else if (array[y - line][x] == -1)
    {
        SetColor(7, 7); // nền trắng, chữ trắng
        cout << "*";
    }
    // ô được mở nhưng được đánh giấu
    if (array[y - line][x] < -1)
    {
        SetColor(0, 14); // nền đen chữ vàng
        cout << -(array[y - line][x] + 1);
    }
    gotoxy(x, y);
    // gotoxy ở đây để giữa con trỏ vẫn ở vị trí cũ sau khi in ra lại
    // định dạng mặc định
}