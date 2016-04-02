#include"Sudoku.h"

Sudoku::Sudoku()
{
    int empty[SIZE];
    memset(empty,0,sizeof(empty));
    setBoard(empty);
}
Sudoku::Sudoku(const int init_board[])
{
    setBoard(init_board);
}
void Sudoku::setBoard(const int set_board[])
{
    int i;
    for(i=0;i<SIZE;i++)
    {
        _board[i]=set_board[i];
    }
}
int Sudoku::getElement(int index)
{
    return _board[index];
}
void Sudoku::printBoard()
{//print out the _board[]
    int i;
    for(i=0;i<SIZE;i++)
    {
        cout<<_board[i];
        cout<<(((i+1)%9==0)?'\n':' ');
    }
}
void Sudoku::printSolve()
{//print out the answer which store in _solveboard[]
    int i;
    for(i=0;i<SIZE;i++)
    {
        cout<<_solveboard[i];
        cout<<(((i+1)%9==0)?'\n':' ');
    }
}
void Sudoku::readIn()
{
    int i;
    int in_board[SIZE];
    _zeronum=0;
    for(i=0;i<SIZE;i++)
    {
        cin>>in_board[i];
        if(in_board[i]==0)//count the zero's number
        {
            _zeronum++;
        }
    }
    setBoard(in_board);
}
void Sudoku::initDlx()
{//use to initialize
    //cout<<DLX_R<<" "<<DLX_C<<endl;
    nodes=new Node[DLX_R*DLX_C];//create DLX board
    row=new Node[DLX_R];
    col=new Node[DLX_C+1];
    result=new int[DLX_R];
    int i;
    rownum=DLX_R;
    colnum=DLX_C;
    for(i=0;i<=colnum;i++)
    {
        col[i].up=col[i].down=col+i;
        col[i].left=col+(i+colnum)%(colnum+1);
        col[i].right=col+(i+1)%(colnum+1);
        col[i].nodesum=0;
    }
    head=col+colnum;
    for(i=0;i<rownum;i++)
    {
        row[i].up=row[i].down=row[i].left=row[i].right=row[i].root=row+i;
    }
    nodecount=0;
}
void Sudoku::addNode(int r,int c)
{//add node which is transformed from sudoke to Exact cover
    //cout<<r<<" "<<c<<endl;
    nodes[nodecount].up=col[c].up;
    nodes[nodecount].down=col+c;
    nodes[nodecount].left=row[r].left;
    nodes[nodecount].right=row+r;
    nodes[nodecount].row=r;
    nodes[nodecount].root=col+c;
    col[c].up=col[c].up->down=row[r].left=row[r].left->right=nodes+nodecount++;
    col[c].nodesum++;
}
void Sudoku::coverCol(Node *t)
{//temporarily remove one col
    //cout<<t->up<<"."<<t->down<<"."<<t->left<<"."<<t->right<<"."<<t->root<<endl;
    Node *p,*q;
    t->left->right=t->right;
    t->right->left=t->left;
    for(p=t->down;p!=t;p=p->down)
    {
        for(q=p->right;q!=p;q=q->right)
        {
            q->up->down=q->down;
            q->down->up=q->up;
            q->root->nodesum--;//the number of this col decrease
        }
    }
}
void Sudoku::uncoverCol(Node *t)
{//restore the col which has removed temporarily
    Node *p,*q;
    for(p=t->up;p!=t;p=p->up)
    {
        for(q=p->left;q!=p;q=q->left)
        {
            q->up->down=q;
            q->down->up=q;
            q->root->nodesum++;//the number of this col increases
        }
    }
    t->left->right=t;
    t->right->left=t;
}
bool Sudoku::dance(int k)
{
    if(head->right==head)
    {//get one solution
        int i,j;
        for(i=0;i<SIZE;i++)
        {//store first solution to check have any other solution
            j=result[i];
            _solveboard[j/9]=1+j%9;
        }
        _solvenum++;
        if(_solvenum>1)
        {//more than 1 solution
            return true;
        }
        else
        {//check is there have any other solution
            //cout<<k<<endl;
            resultcount=k;
            return false;
        }
    }
    Node *pmin,*p,*q;
    for(pmin=head->right,p=pmin->right;p!=head;p=p->right)
    {//find the col which has the minimal nodes
        if(pmin->nodesum>p->nodesum)
            pmin=p;
    }
    coverCol(pmin);//start cover from min col
    for(p=pmin->down;p!=pmin;p=p->down)
    {
        result[k]=p->row;
        for(q=p->right;q!=p;q=q->right)
        {
            coverCol(q->root);
        }
        if(dance(k+1))
        {
            return true;
        }
        for(q=p->left;q!=p;q=q->left)
        {
            uncoverCol(q->root);//back to up one level's for loop
        }
    }
    uncoverCol(pmin);
    return false;
}
int getRowIndex(int row_num)
{
    int num=row_num%9;
    int row_index=row_num/ 81;
    return 81+row_index*9+num;
}
int getColIndex(int row_num)
{
    int num=row_num%9;
    int index=row_num/9;
    int col_index=index%9;
    return 162+col_index*9+num;
}
int getCellIndex(int row_num)
{
    int num=row_num%9;
    int index=row_num/9;
    int row_index=index/9;
    int col_index=index%9;
    int cell_index=int(row_index/3)*3+col_index/3;
    return 243+cell_index*9+num;
}
void Sudoku::solveDLX()//solve by using DLX(Dancing Links and Algorithm X)
{
    int i,j,nodecount=0;
    initDlx();
    int index;
    for(i=0;i<DLX_R;i++)
    {
        index=static_cast<int>(i/9);
        //cout<<index<<":"<<_board[index]<<" "<<i%9<<endl;
        if(_board[index]-1==i%9)//add question first
        {//if _board[index] has been inserted number then add 4 corresponds in DLX board.
            nodecount++;
            //cout<<nodecount<<endl;
            int rowindex=i,colindex=index;
            addNode(rowindex,colindex);
            addNode(rowindex,getRowIndex(i));
            addNode(rowindex,getColIndex(i));
            addNode(rowindex,getCellIndex(i));
            col[i].questionnum++;//record the number of question in each col
        }
    }
    for(i=0;i<DLX_R;i++)
    {
        index=static_cast<int>(i/9);
        //cout<<col[i].questionnum<<endl;
        if(_board[index]==0&&col[i].questionnum==0)//add empty place
        {//if _board[index]==0 then add all possibility,
         //i.e. assume insert form 1 to 9 in this grid, so it will have 9*4 correspond in DLX board.
         //BUT if this col has had question number then it do not need to insert again or it will waste time and space.
            nodecount++;
            //cout<<nodecount<<endl;
            int rowindex=i,colindex=index;
            addNode(rowindex,colindex);
            addNode(rowindex,getRowIndex(i));
            addNode(rowindex,getColIndex(i));
            addNode(rowindex,getCellIndex(i));
        }
    }
    _solvenum=0;
    dance(0);
    switch(_solvenum)
    {
        case 0:
            cout<<0;
            break;
        case 1:
            cout<<1<<endl;
            printSolve();
            break;
        case 2:
            cout<<2;
            break;
    }
}
