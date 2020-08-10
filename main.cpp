//Satkkeerthi Sriram H402756
#include<iostream>
#include<thread>
#include<chrono>
#include<fstream>
#include<string>

#if defined(_WIN32) || defined(WIN32) || defined(__MINGW32__) || defined(__BORLANDC__)
 #define OS_WIN
#elif defined(linux) || defined(__CYGWIN__)
  #define OS_LINUX
#endif // define os

#if defined(OS_WIN)
    #include<windows.h>
#endif

using namespace std;


void clearscreen()
{
    #if defined(OS_WIN)
        HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD count;
        DWORD cellCount;
        COORD homeCoords = { 0, 0 };

        if (hStdOut == INVALID_HANDLE_VALUE) return;

        // Get the number of cells in the current buffer
        GetConsoleScreenBufferInfo( hStdOut, &csbi );
        cellCount = csbi.dwSize.X *csbi.dwSize.Y;

        //Fill the entire buffer with spaces
        FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count);

        //Fill the entire buffer with the current colors and attributes
        FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count);

        SetConsoleCursorPosition( hStdOut, homeCoords );
    #elif defined(OS_LINUX)
        cout << "\033[2J;" << "\033[1;1H";
    #endif
}

int x[502][502]={0},y[502][502]={0};//current board configuration, new board configuration: represented by 1-alive and 0-dead

bool input(string filename, int &m, int&n)//takes in the file as input for initial configuration
{
    filename = filename + ".txt";
    ifstream patternfile (filename);
    int i=1,j=0;
    string row;
    if(patternfile.is_open())
    {
        while (getline(patternfile,row))
        {
            for(j=1;j<=row.length();j++)
            {
                x[i][j]=(int)(row[j-1]-'0');
            }
            i++;
        }
        m=i-1;
        n=j-1;
        patternfile.close();
        return 1;
    }
    else
    {
        cout<<"Error. Cannot Open.";
        return 0;
    }
}

void output(int m, int n)//prints the board configuration
{
        int i,j;
        for(i=1;i<=m;i++)
        {
            for(j=1;j<=n;j++)
            {

                if(x[i][j]==0)
                {
                    #if defined(OS_WIN)
                        cout<<(char)178;
                    #elif defined(OS_LINUX)
                        cout<<' ';
                    #endif
                }
                else
                {
                    #if defined(OS_WIN)
                        cout<<' ';
                    #elif defined(OS_LINUX)
                        cout<<'O';
                    #endif

                }
            }
            cout<<'\n';
        }

}

bool nextgen(int m, int n)
{
        int i,j,s;
        bool flag=0;
        for(i=1;i<=m;i++)
        {
            for(j=1;j<=n;j++)
            {
                s=x[i-1][j]+x[i+1][j]+x[i-1][j-1]+x[i+1][j-1]
                    +x[i-1][j+1]+x[i+1][j+1]+x[i][j-1]+x[i][j+1];
                if(s==3)            //new cell is created
                    y[i][j]=1;
                else if(s<2||s>3)   //cell dies due to over/under-population
                    y[i][j]=0;
                else                //cell is sustained
                    y[i][j]=x[i][j];

                if(x[i][j]!=y[i][j])//checks whether cell has changed, i.e configuration has changed
                    flag=1;
            }
        }
        for(i=1;i<=m;i++)
            for(j=1;j<=n;j++)
            x[i][j]=y[i][j];

        return flag;//flag checks whether the next generation is a new configuration
}

int main()
{
    int i,j,m,n;
    string filename;
    cout<<"Enter the filename for initial board configuration. (Without .extension)\n";
    cin>>filename;
    if(!input(filename,m,n))
        return 0;
    int flag, t=0;
    do              //new configuration is shown every 1 sec until stabilization
    {
        clearscreen();
        output(m,n);
        cout<<"\nTime = "<<t++<<" sec\n";
        flag=nextgen(m,n);
        this_thread::sleep_for(chrono::seconds(1));
    }while(flag!=0);
    return 0;
}
