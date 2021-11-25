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
#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <windows.h>
#include <chrono>
// dùng các phím mũi tên để di chuyển, phím S để chọn và phím X để thoát
// i trong mảng hai chiều tương đương vs 0y, j trong mảng hai chiều thì ox
#define KEY_LEFT 75
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_RIGHT 77
// các phím điều khiển
#define FLAG 102
#define CLICK 115
#define ESCAPE 120
#define Double_Click 100
// các size của board
#define BEGINNER 9
#define BEGINNER_BOOM 10
#define INTERMEDIATE 16
#define INTERMEDIATE_BOOM 40
#define EXPERT 21
#define EXPERT_BOOM 99

using namespace std;

typedef pair<int, int> ii;

/*struct game
{
    char array[100][100], state[100][100];
    int score = 0, size = 0, time = 0, Unlocked = 0;
};*/
struct scores
{
    vector<int> score, time;
    vector<float> rate;
};
struct allscores
{
    scores beginner, intermediate, expert;
};
void initialize(char (&array)[100][100], int &size, char (&state)[100][100]); // khởi tạo mảng 2 chiều và random mine
ii asklevel();                                                           // hỏi mức độ level

// vì có in 5 dòng để yêu cầu ng chơi chọn, nên array[y-line][x] với x=0, y=line là vị
// trí của con trỏ sau khi in số line dòng đó, phần tử mảng array[y-line][x] sẽ là phần tử đầu tiên
void randommine(char (&array)[100][100], int &size, int boom );                               //random mine nhiều vị trí khác nhau
void dfs(int i, int j, int &size, char (&array)[100][100], int &line, char (&state)[100][100], int &Unlocked); // tìm kiếm viền, nếu chọn 0 thì sẽ loang ra đến viền mine
void print(char (&array)[100][100], int &size);                                               // print mảng 2 chiều
void SetColor(int backgound_color, int text_color);                                         // set color chữ và background
void mark(int x, int y, char (&array)[100][100], int &size);                                  // đánh dấu vị trí của bom theo 8 hướng
// vi dụ
// 1 1 1
// 1 B 1
// 1 1 1
void gotoxy(short x, short y);                            // đi đến 1 vị trí nào đó trên console
void print1(char (&array)[100][100], int &size);            //print ra các kí tự * tượng trưng  cho các ô khi bắt đầu chơi
void print2(char (&array)[100][100], int &size, int &line); // print ra các mine khi chọn trúng
int options();                                            // hỏi lựa chọn người khi vào menu chính
int getch_noblock();                                      // check xem có phím nào được nhấn không
void playing(char (&array)[100][100], int &size, int &line, int &score, char (&state)[100][100], allscores &statictis, int &Unlocked, int &time_before );
void initialize_backup(char (&array)[100][100], int &size, char (&state)[100][100], int &score, int &Unlocked, int &time_before);      // khoi tao hien trang cua saved game
void newstate(char (&array)[100][100], int x, int y, int &line, int &size, bool &unchanged, char (&state)[100][100]); // trả lại trạng thái cũ của cell sau khi con trỏ đi qua
void oldstate(char (&array)[100][100], int x, int y, int &line, char (&state)[100][100]);                             // cập nhật trạng thái mới của cell khi con trỏ đang trỏ đến
void getscore(int &score, int &time, int &size, allscores &statistics);                                           // nhập điểm vào record
void printscore(allscores &statistics);                                                                           // in record bằng cách hỏi level
void printscore_level(scores &level);                                                                             // in record theo level
void Saved_game( char (&array)[100][100], char (&state)[100][100], int &score, int &size, int &time, int &Unlocked );
bool Check_Win( char (&array)[100][100], char (&state)[100][100], int &size );
void Winner();
int main()
{
    int size = 0, line = 0, score = 0, key;
    int Unlocked = 0, time_before = 0;                       // Số ô đã mở + cắm cờ
    char array[100][100], state[100][100];
    //game lastgame;
    allscores statistics;
tieptuc:
    key = options();
    switch (key)
    {

    case '1':
        line = 5;
        initialize(array, size, state);
        print1(array, size);
        playing(array, size, line, score, state, statistics, Unlocked, time_before );
        break;
    case '2':
        line = 1;
        initialize_backup(array, size, state, score, Unlocked, time_before );
        playing(array, size, line, score, state, statistics, Unlocked, time_before );
        break;
    case '3':
        printscore(statistics);
        break;
    case '4':
        return 0;
        break;
    default:
        cout << "No matching found" << endl;
        cout << "Press any key to enter again";
        getch();
        break;
    }
    goto tieptuc;
}
void SetColor(int backgound_color, int text_color)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    int color_code = backgound_color * 16 + text_color;
    SetConsoleTextAttribute(hStdout, color_code);
}
void initialize(char (&array)[100][100], int &size, char (&state)[100][100])
{
    system("cls");
    gotoxy(0, 0);
    ii size_setting = asklevel();
    size = size_setting.first;
    // gán giá trị ban đầu cho mảng array
    for (int i = 0; i <= size - 1; ++i)
    {
        for (int j = 0; j <= size - 1; ++j)
            array[i][j] = '0';
    }
    // gán giá trị ban đầu cho mảng state
    for (int i = 0; i <= size - 1; ++i)
    {
        for (int j = 0; j <= size - 1; ++j)
            state[i][j] = 'U';
    }
    // random bomb trong mảng array
    randommine( array, size, size_setting.second );
    for (int i = 0; i <= size - 1; ++i)
    {
        for (int j = 0; j <= size - 1; ++j)
        {
            if (array[i][j] == 'B')
                mark(i, j, array, size); // đánh dấu các số xung quanh ô có bomb
        }
    }
}
void initialize_backup(char (&array)[100][100], int &size, char (&state)[100][100], int &score, int &Unlocked, int &time_before )
{
    system("cls");
    ifstream fi( "Game_Data.txt" );
    fi >> size ;
    fi >> Unlocked;
    fi >> time_before;
    fi >> score;
    cout << "Welcome back to last game :D" << endl;
    // gán các thông số
    for( int i = 0; i < size; ++i )
        for( int j = 0; j < size; ++j ) fi >> array[i][j];
    for( int i = 0; i < size; ++i )
        for( int j = 0; j < size; ++j ) fi >> state[i][j];
    /*for (int i = 0; i <= size - 1; ++i)
    {
        for (int j = 0; j <= size - 1; ++j)
        {
            array[i][j] = lastgame.array[i][j];
            state[i][j] = lastgame.state[i][j];
        }
    }*/
    // in ra màn hình lại board cũ
    for (int i = 0; i <= size - 1; ++i)
    {
        for (int j = 0; j <= size - 1; ++j)
        {
            if (state[i][j] == 'U')
            {
                SetColor( 0, 7 );
                cout << "*";
            }
            if (state[i][j] == 'O')
            {
                if (array[i][j] == '0')
                {
                    SetColor(7, 7);
                }
                else
                    SetColor(0, 14);
                cout << array[i][j];
            }
            if ( state[i][j] == 'F' )
            {
                SetColor( 8, 4 );
                cout << "F";
            }
        }
        cout << endl;
    }
    fi.close();
    //score = lastgame.score;
}
void randommine(char (&array)[100][100], int &size, int boom )
{
    int cnt = 1;
    srand( time( NULL ) );
    while ( cnt <= boom )
    {
        int x = rand() % (size); // dựa trên kiến thức nếu muốn random 1 số trong [a;b]
        int y = rand() % (size); // thì lấy số rand chia cho [b+1-a] sẽ ra được số đu luôn bé hơn hoặc bằng b-1
        if (array[x][y] == '0')
        {
            array[x][y] = 'B';
            ++cnt;
        }
    }
}
void dfs(int i, int j, int &size, char (&array)[100][100], int &line, char (&state)[100][100], int &Unlocked )
{
    if( (i < 0) or (j < 0) or (i > size - 1) or (j > size - 1)) return; // nếu quá miền xác định của mảng, return
    if( state[i][j] == 'F' || state[i][j] == 'O' ) return;
    ++Unlocked;
    if( array[i][j] != 'B' ) // nếu khác ô có bom, in ra số ở ô đó
    {
        gotoxy(j, i + line);
        if (array[i][j] == '0')
            SetColor(7, 7);
        else if ( array[i][j] > '0' )
            SetColor(0, 14);
        cout << array[i][j];
        gotoxy(j, i + line);
        if (array[i][j] > 0 && state[i][j] == 'U')
            state[i][j] = 'O';
    };

    if (array[i][j] == '0') // bằng không, thì tiếp tục loang, không thì chỉ mở
    {
        state[i][j] = 'O';
        if (state[i - 1][j] == 'U')
            dfs(i - 1, j, size, array, line, state, Unlocked);
        if (state[i + 1][j] == 'U')
            dfs(i + 1, j, size, array, line, state, Unlocked);
        if (state[i][j + 1] == 'U')
            dfs(i, j + 1, size, array, line, state, Unlocked);
        if (state[i][j - 1] == 'U')
            dfs(i, j - 1, size, array, line, state, Unlocked);

        // *********************************************** new update ***********************************************************
        if (state[i - 1][j - 1] == 'U')
            dfs( i - 1, j - 1, size, array, line, state, Unlocked);
        if (state[i + 1][j - 1] == 'U')
            dfs( i + 1, j - 1, size, array, line, state, Unlocked);
        if (state[i - 1][j + 1] == 'U')
            dfs( i - 1, j + 1, size, array, line, state, Unlocked);
        if (state[ i + 1 ][j + 1] == 'U')
            dfs( i + 1, j + 1, size, array, line, state, Unlocked);
        // **********************************************************************************************************************
    }
}
void print(char (&array)[100][100], int &size)
{
    for ( int i = 0; i <= size - 1; ++i)
    {
        for ( int j = 0; j <= size - 1; ++j)
            cout << array[i][j] << " ";
        cout << endl;
    }
}
ii asklevel()
{
getkey:
    system("cls");
    cout << "Which level do you want to play? \n1.Beginner \n2.Intermediate \n3.Expert " << endl;
    cout << "Enter 1, 2, 3 : ";
    char key = ' ';
    cin >> key;
    int size_array, size_boom;
    switch (key)
    {
    case '1':
        size_array = BEGINNER;
        size_boom = BEGINNER_BOOM;
        break;
    case '2':
        size_array = INTERMEDIATE;
        size_boom = INTERMEDIATE_BOOM;
        break;
    case '3':
        size_array = EXPERT;
        size_boom = EXPERT_BOOM;
        break;
    /*case '4':
        cout << "Enter your size: "; cin >> size_array;
        cout << "Enter the number of booms: "; cin >> size_boom;
        break;*/
    default:
        cout << "No matching found" << endl;
        cout << "Press any key to enter again";
        getch();
        goto getkey;
        break;
    }
    return ii( size_array, size_boom );
}
void mark(int x, int y, char (&array)[100][100], int &size)
{

    if (x > 0)
        if (array[x - 1][y] != 'B')
            array[x - 1][y] = (char)((int)array[x - 1][y] + 1); //north
    if (x < size - 1)
        if (array[x + 1][y] != 'B')
            array[x + 1][y] = (char)((int)array[x + 1][y] + 1); //south
    if (y < size - 1)
        if (array[x][y + 1] != 'B')
            array[x][y + 1] = (char)((int)array[x][y + 1] + 1); //east
    if (y > 0)
        if (array[x][y - 1] != 'B')
            array[x][y - 1] = (char)((int)array[x][y - 1] + 1); //west
    if ((x < size - 1) && (y > 0))
        if (array[x + 1][y - 1] != 'B')
            array[x + 1][y - 1] = (char)((int)array[x + 1][y - 1] + 1); //south west
    if ((x < size - 1) & (y < size - 1))
        if (array[x + 1][y + 1] != 'B')
            array[x + 1][y + 1] = (char)((int)array[x + 1][y + 1] + 1); // south east
    if ((x > 0) && (y > 0))
        if (array[x - 1][y - 1] != 'B')
            array[x - 1][y - 1] = (char)((int)array[x - 1][y - 1] + 1); //north west
    if ((x > 0) && (y < size - 1))
        if (array[x - 1][y + 1] != 'B')
            array[x - 1][y + 1] = (char)((int)array[x - 1][y + 1] + 1); // north east
}
void gotoxy(short x, short y)
{
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = {x, y};
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}
void print1(char (&array)[100][100], int &size)
{
    for (int i = 0; i <= size - 1; ++i)
    {
        for (int j = 0; j <= size - 1; ++j)
            cout << "*";
        cout << endl;
    }
}
void print2(char (&array)[100][100], int &size, int &line)
{
    gotoxy(0, line);
    for (int i = 0; i <= size - 1; ++i)
        for (int j = 0; j <= size - 1; ++j)
            if ((array[i][j]) == 'B')
            {
                SetColor(0, 12);
                gotoxy(j, i + line);
                cout << array[i][j];
                SetColor(0, 7);
                gotoxy(j, i + line);
            }
}
int options()
{
    system("cls");
    cout << "Minesweepere demo \nWelcome! \n1.New game \n2.Continue playing \n3.Statistics \n4.Quit" << endl;
    cout << "Press 1, 2, 3 or 4 to choose: ";
    char key;
    cin >> key;
    return (int)key;
}
void playing(char (&array)[100][100], int &size, int &line, int &score, char (&state)[100][100], allscores &statictis, int &Unlocked, int &time_before )
{
    int c = 0, x = 0, y = 0 + line, time; // ký tự nhập từ bàn phím
    //int time_before = lastgame.time;
    score = 0;
    gotoxy(0, 0 + line); // x,y tọa độ
    SetColor(5, 7);
    // vị trí đầu tiên của con trỏ khi mới vào game
    // ô chưa được mở
    if (state[y - line][x] == 'U')
    {
        SetColor(5, 7); // nền tím, chữ trắng
        cout << "*";
    }
    // ô đã đoực mở nhưng không có gì
    if (array[y - line][x] == '0' && state[y - line][x] == 'O')
    {
        SetColor(5, 5); // nền trắng, chữ trắng
        cout << "*";
    }
    //case khi hàm playing được call trong last game
    // ô đã được mở và được đánh dấu
    if ( state[y - line][x] == 'O' && array[y-line][x] > '0' )
    {
        SetColor(5, 14); // nền tím chữ vàng
        cout << array[y - line][x];
    }
    if( state[y-line][x] == 'F' )
    {
        SetColor( 8, 4 );
        cout << 'F';
    }

    gotoxy(x, y);
    auto begin = std::chrono::high_resolution_clock::now(); // tính thời gian bắt dầu
    bool unchanged = false;
    // unchanged dể xác định trạng thái của con trỏ có thay đổi hay không
    // nếu quá phạm vi của bảng thì trạng thái vẫn giữ nguyên, không di chuyển
    while (c != ESCAPE)
    {
        c = -1;
        c = getch_noblock();    // hàm này để kiểm tra xem có phím nào được nhấn chưa
        tolower(c);             // Lỡ gamer có ngu ngục nhập Caplock thì nó tự chuyển về chữ thường

        // ô chưa được mở
        if (c == KEY_UP || c == KEY_DOWN || c == KEY_LEFT || c == KEY_RIGHT || c == CLICK || c == FLAG || c == Double_Click )
        {
            // trả ô về trạng thái cũ sau khi con trỏ di chuyển snang chổ khác
            if (!unchanged)
            {
                oldstate(array, x, y, line, state);
            }

            // bắt đầu nhận vị trí mới
            switch (c)
            {
            case KEY_UP:
                --y;
                newstate(array, x, y, line, size, unchanged, state);
                if (unchanged)
                {
                    ++y;
                    newstate(array, x, y, line, size, unchanged, state);
                }
                break;
            case KEY_DOWN:
                ++y;
                newstate(array, x, y, line, size, unchanged, state);
                if (unchanged)
                {
                    --y;
                    newstate(array, x, y, line, size, unchanged, state);
                }
                break;
            case KEY_LEFT:
                --x;
                newstate(array, x, y, line, size, unchanged, state);
                if (unchanged)
                {
                    ++x;
                    newstate(array, x, y, line, size, unchanged, state);
                }
                break;
            case KEY_RIGHT:
                ++x;
                newstate(array, x, y, line, size, unchanged, state);
                if (unchanged)
                {
                    --x;
                    newstate(array, x, y, line, size, unchanged, state);
                }
                break;
            }

            if (c == CLICK) // nếu c='s'

            {
                if ((array[y - line][x] != '0') && (array[y - line][x] != 'B')) // nếu phần tử được chọn là  chỉ số  mật độ
                {
                    SetColor(5, 14);                    // purple, light yellow
                    cout << array[y - line][x];
                    //gotoxy(x, y);
                    state[y - line][x] = 'O';
                    SetColor(0, 7);                     // black, white
                    // chỉ mở, không loang
                    ++score; // cộng điểm
                    ++Unlocked;
                }
                else if (array[y - line][x] == 'B') // nếu dính bom
                {
                    print2(array, size, line);
                    gotoxy(0, line + size + 1);
                    cout << "Failed!!!" << endl;
                    Unlocked = 0;
                    break;
                }
                else if (array[y - line][x] == '0') // nếu ô mở là không có gì, bắt đầu loang
                {
                    // tránh trường hợp chọn lại 1 ô, nó sẽ cho ra giá trị âm, lộ dữ liệu
                    dfs(y - line, x, size, array, line, state, Unlocked);
                    gotoxy(x, y);
                    SetColor(5, 5);
                    cout << "*";
                    gotoxy(x, y);
                    ++score;
                }
            }

            if( c == Double_Click )
            {
                if( state[y-line][x] != 'O' ) continue;
                if( array[y-line][x] == '0' ) continue;
                int count_Flag = 0;
                for( int i = -1; i <= 1; ++i )
                    for( int j = -1; j <= 1; ++j )
                    {
                        if( i == 0 && j == 0 ) continue;
                        int _x = y - line + i;
                        int _y = x + j;
                        if( _x < 0 || _x >= size || _y < 0 || _y >= size ) continue;
                        if( state[_x][_y] == 'F' ) ++count_Flag;
                    }
                if( count_Flag != ( int( array[y-line][x] ) - 48 ) ) continue;
                for( int i = -1; i <= 1; ++i )
                    for( int j = -1; j <= 1; ++j )
                    {
                        if( i == 0 && j == 0 ) continue;
                        int _x = y - line + i;
                        int _y = x + j;
                        if( _x < 0 || _x >= size || _y < 0 || _y >= size ) continue;
                        if( state[_x][_y] == 'F' || state[_x][_y] == 'O' ) continue;

                        gotoxy( x + j, y + i );
                        if ( ( array[_x][_y] > '0' ) && (array[_x][_y] != 'B') ) // nếu phần tử được chọn là  chỉ số  mật độ
                        {
                            SetColor(0, 14);                    // purple, light yellow
                            cout << array[_x][_y];
                            //gotoxy(x, y);
                            state[_x][_y] = 'O';
                            SetColor(0, 7);                     // black, white
                            // chỉ mở, không loang
                            ++score; // cộng điểm
                            ++Unlocked;
                            gotoxy( x, y );
                            //continue;
                        }
                        else if (array[_x][_y] == 'B') // nếu dính bom
                        {
                            print2(array, size, line);
                            gotoxy(0, line + size + 1);
                            cout << "Failed!!!" << endl;
                            Unlocked = 0;
                            break;
                        }
                        else if (array[_x][_y] == '0') // nếu ô mở là không có gì, bắt đầu loang
                        {
                            // tránh trường hợp chọn lại 1 ô, nó sẽ cho ra giá trị âm, lộ dữ liệu
                            dfs( _x, _y, size, array, line, state, Unlocked);
                            gotoxy( x + j, y + i);
                            SetColor(5, 5);
                            cout << "*";
                            gotoxy(x, y);
                            ++score;
                        }
                    }
            }

            if( c == FLAG )
            {
                if( state[y-line][x] == 'O' ) continue;
                if( state[y-line][x] == 'F' )
                {
                    state[y-line][x] = 'U';
                    gotoxy( x, y );
                    SetColor(5, 7); // nền tím, chữ trắng
                    cout << "*";
                    --Unlocked;
                }
                else if( state[y-line][x] == 'U' )
                {
                    state[y-line][x] = 'F';
                    gotoxy( x, y );
                    SetColor( 8, 4 ); // nền trắng, chữ đỏ
                    cout << "F";
                    ++Unlocked;
                }

            }
        }
        auto end = std::chrono::high_resolution_clock::now();                             // tính thời gian hiện tại
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin); // tính thời gian đã trôi qua
        time = time_before + (int)(elapsed.count() * 1e-9);
        gotoxy(0, line + size);
        SetColor(0, 7);
        cout << "Time measured: " << time << " seconds";
        if( Unlocked == size * size ) if( Check_Win( array, state, size ) )
            {
                Winner();       // Win roài;
                break;
            }
    }
    SetColor(0, 7);
    if (c == ESCAPE) // nếu c='x' nghĩa là đang chơi nhưng thoát ra
    {
        gotoxy(0, line + size + 1);
        cout << "You could do better!" << endl;
        cout << "Score: " << score << endl;
        cout << "Wanna save the game for playing later? y/n: ";
        char key;
        cin >> key;
        if (tolower(key) == 'y')
        {
            Saved_game( array, state, score, size, time, Unlocked );
        }
        Unlocked = 0;
        return;
        cout << "Press any key to return to menu";
        getch();
        return;
    }

    cout << "Score: " << score << endl;
    cout << "Press any key to return to menu";
    getch();
    getscore(score, time, size, statictis);
}
void newstate(char (&array)[100][100], int x, int y, int &line, int &size, bool &unchanged, char (&state)[100][100])
{
    // nếu trong phạm vi của bảng thì cho di chuyển đến
    if (0 <= x && x <= size - 1 && 0 <= (y - line) && (y - line) <= size - 1)
    {
        gotoxy(x, y);
        if (state[y - line][x] == 'U')
        {
            SetColor(5, 7);
            cout << "*";
        }
        if (array[y - line][x] == '0' && state[y - line][x] == 'O')
        {
            SetColor(5, 5);
            cout << "*";
        }
        if ( array[y-line][x] > '0' && state[y - line][x] == 'O')
        {
            SetColor(5, 14);
            cout << array[y - line][x];
        }
        if( state[y-line][x] == 'F' )
        {
            SetColor( 8, 4 );
            cout << "F";
        }
        gotoxy(x, y);
        unchanged = false;
    }
    else
        unchanged = true;
}
void oldstate(char (&array)[100][100], int x, int y, int &line, char (&state)[100][100])
{
    gotoxy(x, y);
    if( state[y - line][x] == 'U' )
    {
        SetColor(0, 7); // nền đen, chữ trắng
        cout << "*";
    }
    // ô đã mở nhưng không có gì
    if (array[y - line][x] == '0' && state[y - line][x] == 'O')
    {
        SetColor(7, 7); // nền trắng, chữ trắng
        cout << "*";
    }
    // ô được mở nhưng được đánh giấu
    if ( array[y-line][x] > '0' && state[y - line][x] == 'O')
    {
        SetColor(0, 14); // nền đen chữ vàng
        cout << array[y - line][x];
    }
    // Flag
    if( state[y-line][x] == 'F' )
    {
        SetColor( 8, 4 );
        cout << 'F';
    }
    gotoxy(x, y);
    // gotoxy ở đây để giữa con trỏ vẫn ở vị trí cũ sau khi in ra lại
    // định dạng mặc định
}
int getch_noblock() // hàm này để kiểm tra xem có phím nào được nhấn chưa
{
    if (_kbhit())
        return _getch();
    else
        return -1;
}
void getscore(int &score, int &time, int &size, allscores &statistics)
{

    switch (size)
    {
    case (BEGINNER):
        statistics.beginner.score.push_back(score);
        statistics.beginner.time.push_back(time);
        statistics.beginner.rate.push_back(score / ((float)time));
        break;
    case (INTERMEDIATE):
        statistics.intermediate.score.push_back(score);
        statistics.intermediate.time.push_back(time);
        statistics.intermediate.rate.push_back(score / ((float)time));
        break;
    case (EXPERT):
        statistics.expert.score.push_back(score);
        statistics.expert.time.push_back(time);
        statistics.expert.rate.push_back(score / ((float)time));
        break;
    }
}
void printscore(allscores &statistics)
{
tieptuc:
    system("cls");
    cout << "Which level do you want to see? " << endl;
    cout << "1. Beginner \n2. Intermediate \n3. Expert" << endl;
    cout << "Press 1, 2 or 3 to choose: ";
    char key;
    cin >> key;
    switch (key)
    {
    case '1':
        cout << " BEGINNER" << endl;
        printscore_level(statistics.beginner);
        break;
    case '2':
        cout << " INTERMEDIATE" << endl;
        printscore_level(statistics.intermediate);
        break;
    case '3':
        cout << " EXPERT" << endl;
        printscore_level(statistics.expert);
        break;
    default:
        cout << "no matching found" << endl;
        cout << "press anykey to enter again";
        getch();
        goto tieptuc;
        break;
    }
}
void printscore_level(scores &level)
{
    system("cls");
    cout << "Statistics" << endl;
    gotoxy(1, 1);
    cout << " Scores";
    gotoxy(10, 1);
    cout << " Time (s)";
    gotoxy(20, 1);
    cout << " Rate (score/s)" << endl;
    if (level.score.empty())
    {
        cout << "These record is empty, please play a new game" << endl;
        cout << "Press anykey to go to menu";
        getch();
        return;
    }
    gotoxy(1, 2);
    int j = 3, order = 1;
    for (auto i : level.score)
    {
        cout << order << ". " << i;
        gotoxy(1, j);
        ++j;
        ++order;
    }
    gotoxy(10, 2);
    j = 3;
    for (auto i : level.time)
    {
        cout << " " << i;
        gotoxy(10, j);
        ++j;
    }
    gotoxy(20, 2);
    j = 3;
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(3);
    for (auto i : level.rate)
    {
        cout << " " << i;
        gotoxy(20, j);
        ++j;
    }
    cout << endl;
    cout << "Press any key to go back to menu";
    getch();
}
void Saved_game( char (&array)[100][100], char (&state)[100][100], int &score, int &size, int &time, int &Unlocked )
{
    ofstream fo( "Game_Data.txt" );
    fo << size << " " << Unlocked << " " << time << " " << score << endl;
    for( int i = 0; i < size; ++i )
    {
        for( int j = 0; j < size; ++j ) fo << array[i][j] << " ";
        fo << endl;
    }
    for( int i = 0; i < size; ++i )
    {
        for( int j = 0; j < size; ++j ) fo << state[i][j] << " ";
        fo << endl;
    }
    fo.close();
}
bool Check_Win( char (&array)[100][100], char (&state)[100][100], int &size )
{
    for( int i = 0; i <= size - 1; ++i )
        for( int j = 0; j <= size - 1; ++j )
        {
            if( state[i][j] == 'F' && array[i][j] != 'B' ) return  false;
            if( array[i][j] == 'B' && state[i][j] != 'F' ) return false;
        }
    return true;
}
void Winner()
{
    system("cls");
    cout << "Good job bro !!!";
}
