#include<iostream>
#include<cstring>
#include<cstdlib>
#include<iomanip>
#define SIZE 81
#define DLX_R 9*SIZE
#define DLX_C 4*SIZE
using namespace std;
class Sudoku{
    public:
        Sudoku();
        Sudoku(const int init_board[]);
        
        void readIn();
        void solveDLX();//solve by using DLX(Dancing Links and Algorithm X)
        
        void setBoard(const int set_board[]);
        int getElement(int index);
        void printBoard();

    private:
        struct Node
        {
            Node *up;
            Node *down;
            Node *left;
            Node *right;
            Node *root;//the head of each col
            int row;//the location of row
            int nodesum;//the sum of each col
            int questionnum;
        };
        Node *nodes,*row,*col,*head;
        int rownum,colnum,nodecount;
        int *result,resultcount;
        void initDlx();
        void coverCol(Node *t);
        void uncoverCol(Node *t);
        bool dance(int k);
        void addNode(int r,int c);

        void printSolve();
        
        int _board[SIZE];
        int _zeronum;
        int _solvenum;
        int _solveboard[SIZE];
};
