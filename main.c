#include<windows.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <limits.h>
#define oneLen 75



double** randmm(int rows, int cols)
{
    double** matrix = (double**)malloc(rows * sizeof(double*));

    for (int i = 0; i < rows; i++)
        matrix[i] = (double*)malloc(cols * sizeof(double));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] =  2.0 / RAND_MAX * rand();
        }
    }

    return matrix;
}

double** mulmr(double num, double **mat, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            mat[i][j] = mat[i][j] * num;

            if(mat[i][j] > 1.0)
            {
                mat[i][j] = 1;
            } else mat[i][j] = 0;
        }
    }

    return mat;
}



void zhdac ( HWND* hWnd )
{
    printf("\nWaiting for click\n");
    while (1)
    {
		if (_kbhit()) {
			char c = _getch();
			InvalidateRect(*hWnd, NULL, TRUE);
			break;
		}
	}
}

int nx[11] = { };
int ny[11] = { };
double** Amain = NULL;
void matGen (  )
{
    for ( int i = 0; i < 11; i++ ) {
        if ( i == 0 ) {
            nx[i] = oneLen*6;
            ny[i] = 800 - oneLen;
        }
        else if ( i < 3) {
            nx[i] = nx[i-1] - oneLen*2;
            ny[i] = ny[i-1];
        }
        else if ( i < 7 ) {
            nx[i] = nx[i-1] + oneLen;
            ny[i] = ny[i-1] - oneLen*2;
        }
        else {
            nx[i] = nx[i-1] + oneLen;
            ny[i] = ny[i-1] + oneLen*2;
        }
    }
    srand(0315);
    double** T = randmm(11, 11);
    double** randT = randmm(11, 11);
    double coef = 1.0 - 1*0.01 - 5*0.005 - 0.05;
    Amain = mulmr(coef, T, 11, 11);
    for ( int i = 0; i < 11; i++ )
        for ( int j = 0; j < 11; j++ )
            Amain[i][j] = round ( Amain[i][j] * randT[i][j] * 100 );
    for ( int i = 0; i < 11; i++ ) {
        for ( int j = 0; j < 11; j++ ) {
            int first = Amain[i][j] != Amain[j][i];
            int second = Amain[i][j] == Amain[j][i] && Amain[i][j] == 1;
            if ( j > i ) second *= ( Amain[i][j] > 0 );
            Amain[i][j] *= first + second;
        }
    }
    for ( int i = 0; i < 11; i++ )
        for ( int j = 0; j < 11; j++ )
            Amain[j][i] = Amain[i][j];
}

void matout (  )
{
    printf("\nMatrix:\n");
            for (int i = 0; i < 11; i++)
            {
                for (int j = 0; j < 11; j++)
                {
                    printf("%.0f ", Amain[i][j]);
                }
                printf("\n");
            }
}


int printWeight ( HDC hdc, int i, int j, int mode, int modeX, int modeY ) {
    if ( i == j ) return -1;
    int len;
    double numbe = Amain[i][j];
    if ( numbe < 10 ) len = 1;
    else if ( numbe < 100 ) len = 2;
    else if ( numbe < 1000 ) len = 3;
    else len = 4;
    int newnumb = numbe;
    char* buf;
    buf = (char *)malloc(10 * sizeof(char));
    int v = 0;
    while ( newnumb > 9 ) {
        buf[v++] = (newnumb % 10) + '0';
        newnumb = newnumb / 10;
    }
    buf[v++] = newnumb + '0';
    buf[v] = '\0';
    char t;
    for (int i = 0; i < v / 2; i++)
    {
        t = buf[i];
        buf[i] = buf[v - 1 - i];
        buf[v - 1 - i] = t;
    }
    int x;
    int y;

    if( mode == 0 ) {
        if ( nx[i] > nx[j] ) {
            x = nx[j] + ((nx[i] - nx[j]) / 2);
        }
        else if ( nx[i] < nx[j] ) {
            x = nx[i] + ((nx[j] - nx[i]) / 2);
        }
        else {
            x = nx[i];
        }

        if ( ny[i] > ny[j] ) {
            y = ny[j] + ((ny[i] - ny[j]) / 2);
        }
        else if ( ny[i] < ny[j] ) {
            y = ny[i] + ((ny[j] - ny[i]) / 2);
        }
        else {
            y = ny[i];
        }
    }
    else {
        x = modeX;
        y = modeY;
    }
    TextOut ( hdc, x, y, buf, len );
    return 0;
}

double** tree = NULL;
double spanningTree[11][11];
int visited[11] = {};
int unvisited;
int last = 0;
int checker = 1;
int ten = 1;
void risovac ( HDC hdc, double** A, int* nx, int* ny )
{
            void arrow(float fi, int px,int py){
                    fi = 3.1416*(180.0 - fi)/180.0;
                    int lx,ly,rx,ry;
                    lx = px+15*cos(fi+0.3);
                    rx = px+15*cos(fi-0.3);
                    ly = py+15*sin(fi+0.3);
                    ry = py+15*sin(fi-0.3);
                    MoveToEx(hdc, lx, ly, NULL);
                    LineTo(hdc, px, py);
                    LineTo(hdc, rx, ry);
              }
            int i;
            char *nn[11] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b"};
            int dx = 16, dy = 16, dtx = 7;
            HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
            HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));

            HPEN NOTGGPen = CreatePen(PS_SOLID, 2, RGB(194, 210, 255));
            HPEN GGPen = CreatePen(PS_SOLID, 2, RGB(158, 194, 255));
            SelectObject(hdc, KPen);
            if( checker == 0 ) TextOut(hdc,100,100,"spanning tree", 13);
            for (int i = 0; i < 11; i++)
            {
                for (int j = 0; j < 11; j++)
                {
                    if ( A[i][j] != 0 && visited[i] != 0) {
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        //printf("\n%i -> %i", i, j);
                        if ( i != ( j + 1 ) && j != ( i + 1 ) ) {
                            POINT pt[3];
                            pt[0].x = nx[i];
                            pt[0].y = ny[i];

                            pt[2].x = nx[j];
                            pt[2].y = ny[j];
                           if ( ( i <= 2 || i == 10 ) && ny[i] == ny[j] ) {
                                if( ( A[j][i] == 0 || ( A[j][i] != 0 && ( i - j > 0 ) ) ) ) {
                                    if ( i - j > 0 ) {
                                      pt[1].x = nx[i] - (nx[i] - nx[j]) / 2;
                                    }
                                    else {
                                       pt[1].x = nx[j] - (nx[j] - nx[i]) / 2;
                                    }
                                    pt[1].y = ny[i] + 5 * ( i - j );
                                    Polyline(hdc, pt, 3);
                                    printWeight(hdc, i, j, 1, pt[1].x, pt[1].y);
                                }
                           }
                           else if ( ( i >= 2 && i < 7 ) && ( j >= 2 && j < 7 ) ) {
                                if( ( A[j][i] == 0 || ( A[j][i] != 0 && ( i - j > 0 ) ) ) ) {
                                    if ( i - j > 0 ) {
                                      pt[1].y = ny[i] - (ny[i] - ny[j]) / 2;
                                    }
                                    else {
                                       pt[1].y = ny[j] - (ny[j] - ny[i]) / 2;
                                    }
                                    pt[1].x = nx[j] - 5 * ( i - j );
                                    Polyline(hdc, pt, 3);
                                    printWeight(hdc, i, j, 1, pt[1].x, pt[1].y);
                                }
                           }
                            else if ( ( i >= 6 && i < 12 ) && ( j >= 6 && j < 12 ) ) {
                                if( ( A[j][i] == 0 || ( A[j][i] != 0 && ( i - j > 0 ) ) ) ) {
                                    if ( i - j > 0 ) {
                                      pt[1].y = ny[i] - (ny[i] - ny[j]) / 2;
                                    }
                                    else {
                                       pt[1].y = ny[j] - (ny[j] - ny[i]) / 2;
                                    }
                                    pt[1].x = nx[i] + 5 * ( i - j ) / 4 + ten * 10;
                                    ten++;
                                    if ( ten > 4 ) ten = 0;
                                    Polyline(hdc, pt, 3);
                                    printWeight(hdc, i, j, 1, pt[1].x, pt[1].y);
                                }
                            }
                            else {
                                LineTo(hdc, nx[j], ny[j]);
                                printWeight(hdc, i, j, 0, 0, 0);
                            }
                        }
                        else {
                            LineTo(hdc, nx[j], ny[j]);
                            printWeight(hdc, i, j, 0, 0, 0);
                        }

                    }
                }
            }
            SelectObject(hdc, BPen);
            for ( i = 0; i <= 10; i++ ){
                if( visited[i] == 0 )
                {
                   SelectObject(hdc, BPen);
                }
                else if ( visited[i] == 1 )
                {
                    SelectObject(hdc, NOTGGPen);
                }
                else
                {
                    SelectObject(hdc, GGPen);
                }
                Ellipse ( hdc, nx[i]-dx, ny[i]-dy, nx[i]+dx, ny[i]+dy );
                TextOut ( hdc, nx[i]-dtx, ny[i]-dy/2, nn[i], 1 );
            };
}



void treeout (  )
{
    printf("\n\nSpanning tree matrix:\n");
            for (int i = 0; i < 11; i++)
            {
                for (int j = 0; j < 11; j++)
                {
                    printf("%.0f ", tree[i][j]);
                }
                printf("\n");
            }
}

//potok


DWORD WINAPI somethingcool (LPVOID lpParam)
{
    matGen ( );
    matout ( );
    for ( int i = 0; i < 11; i++ )
    {
        visited[i] = 0;
        for ( int j = 0; j < 11; j++ ) spanningTree[i][j] = 0;
    }

    //included = visited
    //tree = spanningTree

    visited[0] = 1;

    int from, to, weight;
    for ( int p = 0; p < 10; p++ ) {
        from = -1;
        to = -1;
        weight = INT_MAX;
        for ( int i = 0; i < 11; i++ ) {
            if ( visited[i] )
                for ( int j = 0; j < 11; j++ ) {
                    if ( i != j && Amain[i][j] && !visited[j] && Amain[i][j] < weight ) {
                        from = i;
                        to = j;
                        weight = Amain[i][j];
                    }
                }
        }
        if ( from != -1 && to != -1 ) {
            zhdac(lpParam);
            spanningTree[from][to] = weight;
            spanningTree[to][from] = spanningTree[from][to];
            checkanddraw(lpParam);
            visited[to] = 1;
        }
    }

    tree = randmm(11, 11);
    for ( int i = 0; i < 11; i++ )
        for ( int j = 0; j < 11; j++ )
            tree[i][j] = spanningTree[i][j];
    treeout();

    printf("\n\nSpanning tree");
    checker = 0;
    zhdac(lpParam);
    checkanddraw(lpParam);

}


void checkanddraw ( HDC hdc )
{
    if( checker ) risovac ( hdc, Amain, nx, ny );
    else risovac ( hdc, tree, nx, ny );
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
char ProgName[] = "Лабораторна робота 6";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    HWND hWnd;
    MSG lpMsg;

    WNDCLASS w;

    w.lpszClassName = ProgName;
    w.hInstance = hInstance;
    w.lpfnWndProc = WndProc;
    w.hCursor = LoadCursor(NULL, IDC_ARROW);
    w.hIcon = 0;
    w.lpszMenuName = 0;
    w.hbrBackground = LTGRAY_BRUSH;
    w.style = CS_HREDRAW|CS_VREDRAW;
    w.cbClsExtra = 0;
    w.cbWndExtra = 0;

    if( !RegisterClass(&w) )
        return 0;


    hWnd=CreateWindow(ProgName,
        "Лабораторна робота 6",
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        1920,
        1080,
        (HWND)NULL,
        (HMENU)NULL,
        (HINSTANCE)hInstance,
        (HINSTANCE)NULL);

    ShowWindow(hWnd, nCmdShow);


        DWORD dwThreadId = 1;

        HANDLE cool;
        cool = CreateThread(NULL, 0, somethingcool, &hWnd, 0, &dwThreadId);

    while(GetMessage(&lpMsg, hWnd, 0, 0)) {
            TranslateMessage(&lpMsg);
            DispatchMessage(&lpMsg);
        }
    return(lpMsg.wParam);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT messg,
                        WPARAM wParam, LPARAM lParam)
    {
    HDC hdc;
    PAINTSTRUCT ps;


    switch ( messg )
    {
        case WM_PAINT :

            hdc=BeginPaint(hWnd, &ps);

            checkanddraw( hdc );

            EndPaint(hWnd, &ps);
            break;

        case WM_KEYDOWN:
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return(DefWindowProc(hWnd, messg, wParam, lParam));
    }
    return 0;
}
