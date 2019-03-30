#include "Board.h"
#include <QPainter>
#include "Stone.h"
#include <QMouseEvent>
#define GetRowCol(__row, __col, __id) \
    int __row = _s[__id]._row;\
    int __col = _s[__id]._col

Board::Board(QWidget *parent) :
    QWidget(parent)
{
    //初始化棋子，因为只定义了32个的数据，并不知道是什么
    //_s[0]的行是多少，列是多少...
    for(int i = 0; i<32; i++)
    {
        //为什么这么写
        _s[i].init(i);
    }
    _selectid = -1;
    _bRedTurn = true;
}

//绘制棋盘函数
void Board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //画十条横线
    int d = 40;
    _r = d/2;
    //记录被选中的棋子
    for(int i = 1; i <= 10; i++)
    {

        /*注意：按照象棋正方形的格子来讲，设棋盘最左上角的坐标是（0，0），每个
         * 梅子的长度为d,那么一条横线的长度就是8d,最右边的坐标为（8d,0）,
         * 如果顶点坐标是从（1，1）开始，那么最右边的坐标将是（9，1），可以得出如果顶点
         * 坐标是从（d,d）开始，那么最右边的坐标是（9d,d）那么可以
        得出画十条横线线的纵坐标还和i相关，由于i=0时，d*i等于，所以这里i从1开始索引
    */
        painter.drawLine(QPoint(d, i * d), QPoint(9 * d, i * d));

    }
    //画九条竖线，横不变，如果是从（0，0）开始，那么（0，10）
    for(int f = 1; f <= 9; f++)
    {
        /*画竖线时注意，最左边的一条竖线和最右边的一线竖线是从上到下没有间隔
         但是从第二条竖线开始，每一条竖线中间是有楚河汉界的，并不是一条紧画下来
        */
        if(f == 1 || f == 9)
        {
            painter.drawLine(QPoint(d * f, d), QPoint(d * f, 10 * d));
        }
        else
        {
            painter.drawLine(QPoint(f * d, d), QPoint(f * d, 5 * d));
            painter.drawLine(QPoint(f * d, 6 * d), QPoint(f * d, 10 * d));
        }
    }

    //九宫格
    painter.drawLine(QPoint(4 * d, d), QPoint(6 * d, 3 * d));
    painter.drawLine(QPoint(6 * d, d), QPoint(4 * d, 3 * d));
    painter.drawLine(QPoint(4 * d, 8 * d), QPoint(6 * d, 10 * d));
    painter.drawLine(QPoint(6 * d, 8 * d), QPoint(4 * d, 10 * d ));

    //绘制棋子
    for(int i = 0; i < 32; i++ )
    {
        drawStone(painter, i);
    }
}
//转换函数
QPoint Board::center(int row, int col)
{
    QPoint ret;
    ret.rx() = (col + 1) * _r * 2;
    ret.ry() = (row + 1) * _r * 2;

    return ret;
}
QPoint Board::center(int id)
{
    return center(_s[id]._row,_s[id]._col);
}
void Board::drawStone(QPainter& painter, int id)
{
    if(_s[id]._dead)
    {
        return;
    }
    /*
    //首先画圆圈，然后再写字，圆心怎么确定？这里涉及到像素和棋盘横纵坐标转换
    //获取棋子的行和列坐标，转换成棋盘的像素坐标，圆的心和d有关系
    //最后画字
    //由于d经常用，所以可以将d放进棋盘的成员变量中去
    //_s[32]是个行列的二维数组，其中二维数组要用窗口的像素坐标来表示
       就必须有个转化公式。
    //文字函数中第一个参数是一个框住的长方形
    c.x是属性方法要加括号

    //象棋美化一定是在绘制象棋的时候
    */
    QPoint c = center(id);
    QRect rect = QRect(c.x() - _r, c.y() - _r, _r * 2, _r * 2);

    if(id ==_selectid)
    {
        //如果是被选中的棋子，背景颜色是灰色
        painter.setBrush(QBrush(Qt :: gray));
    }
    else
    {
        //在绘制圆的时候，如果要设置背景颜色，首先要设置一个画刷
        //不是被选中的棋子是背景用黄色
        painter.setBrush(QBrush(Qt :: yellow));
    }

    //先设置黑色的画笔画矩形，
    painter.setPen(Qt :: black);
    painter.drawEllipse(center(id), _r, _r);

    //如果棋子是红色，就换成红色的画笔
    if(_s[id]._red)
    {
        painter.setPen(Qt :: red);
    }
    //设置字体大小
    painter.setFont(QFont("system", _r, _r));
    //象棋的名字是象棋的属性，应该定义在棋子类中
    painter.drawText(rect,_s[id].getText(),QTextOption(Qt :: AlignCenter));
}

bool Board :: getClickRowCol(QPoint pt, int &row, int &col)
{
    for(row = 0; row <=9; row++)
    {
        for(col = 0; col <=8; col++)
        {
            //取出行列对应的中心点取出来
            QPoint c = center(row,col);
            /*计算点击的位置到中心点距离是否小于或者等于_r,
             这里的c是棋盘中心点坐标，pt是点击的坐标
             用棋盘中心点坐标对应的像素点坐标减去点击位置对应的
             像素坐标，如果小于半径那么，就属于这个点心点管
            */
            int dx = c.x() - pt.x();
            int dy = c.y() - pt.y();

            int dist = dx * dx + dy * dy;
            if(dist < _r * _r)
            {
                return true;
            }
        }
    }
    return false;
}
bool Board::isDead(int id)
{
    if (id == -1) {
        return true;
    }
    return _s[id]._dead;
}

bool Board::red(int id)
{
    return _s[id]._red;
}

bool Board::sameColor(int id1, int id2)
{
    if (id1 == -1 || id2 == -1) {
        return false;
    }
    return red(id1) == red(id2);
}

int Board::getStoneId(int row, int col)
{
    for (int i = 0; i < 32; ++i) {
        if (_s[i]._row == row &&
            _s[i]._col == col && !isDead(i)) {
            return i;
        }
    }
    return -1; // 如果不是棋子返回-1
}

void Board::killStone(int id)
{
    if (id == -1) {
        return;
    }
    _s[id]._dead = true;
}

void Board::reliveStone(int id)
{
    if (id == -1) {
        return;
    }
    _s[id]._dead = false;
}

void Board::moveStone(int moveid, int row, int col)
{
    _s[moveid]._row = row;
    _s[moveid]._col = col;

    _bRedTurn = !_bRedTurn;
}

//关系值函数
int Board :: relation(int row1, int col1, int row, int col)
{
    return qAbs(row1 - row) * 10 + qAbs(col1 - col);
}

//判断哪种棋子在下边的函数
bool Board :: isBottomSide(int id)
{

    return _bSide == _s[id]._red;

}
int Board ::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;

    // 首先判断两个棋子是否在同一条直线上，如果不在同一条直线上，直接返回-1
    if (row1 != row2 && col1 != col2) {
        return -1;
    }
    if (row1 == row2 && col1 == col2) {
        return -1;
    }

    // 计算两个棋子之间的有多少个棋子
    if (row1 == row2) { // 在同一行
        int min = col1 < col2 ? col1 : col2;
        int max = col1 > col2 ? col1 : col2;
        for (int col = min + 1; col < max; ++col) {
            if (getStoneId(row1, col) != -1) {
                ++ret;
            }
        }
    }
    else
    { // 在同一列
            int min = row1 < row2 ? row1 : row2;
            int max = row1 > row2 ? row1 : row2;
            for (int row = min + 1; row < max; ++row)
            {
                if (getStoneId(row, col1) != -1)
                {
                    ++ret;
                }
            }
    }
    return ret;
}

//.....................  7个规则函数实现.......................

//1，"将"的实现函数


bool Board :: canMoveJiang(int moveid, int row, int col, int killid);

bool Board :: canMoveShi(int moveid, int row, int col, int killid)
{
    if(_s[moveid]._red)
    {
        if(row > 2 )
        {
            return false;
        }
    }
    else
    {
        if(row <7)
        {
            return false;
        }
    }
    //列永远都是3到5之间，但是可以等于
    if(col < 3)
    {
        return false;
    }
    if(col > 5)
    {
        return false;
    }

    /*
     * 2、计算移动步长是一个格子
     * 一、先获取一个d值，其中dr(行的d值)等于移动的棋子的行值减去目标位置的行值
     * 二、获取别一个d值，其中dc(列的d值)等于移动的棋子的列值减去目标位置的列值
     * 三、通过走一步的演算，可以得出行的差值为零的时候，列的差值一定是1或者是1-，相反亦然
     * 四，设定整数型变量d 当d取绝对值|(dr+dr)| = 1时，证明移动了一格
    */
    int dr = _s[moveid]._row - row;
    int dc = _s[moveid]._col - col;
    int d = abs(dr) * 10 + abs(dc);
    if(d == 11)
    {
        return true;

    }

    return false;
}
//相的走法
bool Board::canMoveXiang(int moveid, int, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if (r != 22)
    { // 象走田，所以r应该等于22
        return false;
    }

    // 看象眼有没有棋子
    int rEye = (row + row1) / 2;
    int cEye = (col + col1) / 2;
    if (getStoneId(rEye, cEye) != -1)
    {
        return false;
    }

    // 判断是否在棋盘的下方
    if (isBottomSide(moveid))
    {
        if (row < 4)
        {
            return false;
        }
    }
    else {
        if (row > 5)
        {
            return false;
        }
    }

    return true;
}
bool Board :: canMoveChe(int moveid, int row, int col, int killid)
{
    GetRowCol(row1, col1,moveid);
   int ret = getStoneCountAtLine(row1, col1, row, col);
   if(ret == 0)
   {
       return true;
   }

   return false;
}
bool Board :: canMoveMa(int moveid, int row, int col, int killid)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    //首先判断马要走日字
    if(r != 12 && r != 21)
    {
        return false;
    }
    //判断有没有憋马腿的情况

    if(r == 12)
    {
        //列相差等于
        if(getStoneId(row1, (col + col1) / 2) != -1)
        {
            return false;
        }

    }
    else
    {
        //行相差等于2
        if(getStoneId((row +row1)/2,col1) != -1)
        {
            return false;
        }
    }
    return true;
}
bool Board::canMovePao(int moveid, int killid, int row, int col)
{
    GetRowCol(row1, col1, moveid);
    int ret = getStoneCountAtLine(row, col, row1, col1);
    if (killid != -1) { // 如果炮要吃对方的棋子
        if (ret == 1) { // 中间有一个棋子，可以走
            return true;
        }
    }
    else { // 如果炮不吃棋子
        if (ret == 0) { // 中间没有棋子，可以走
            return true;
        }
    }

    return false;
}
bool Board :: canMoveBing(int moveid, int row, int col, int killid)
{
    GetRowCol(row1, col1, moveid);
    int r = relation(row1, col1, row, col);
    if(r != 1 && r != 10)
    {
        return false;
    }
    //下面一方的棋子
    if(isBottomSide(moveid))
    {
        //如果目标行大于原始行，相当于后退
        if(row > row1)
        {
            return false;
        }
        //还没有过何就想横着走
        if(row1 >= 5 && row == row1)
        {
            return false;
        }

    }
    //上面一方的棋子
    else
    {
        //如果目标行小于原始行，相当于兵在后退
        if(row1 > row)
        {
            return false;
        }
        //还没有过何就想横着走
        if(row <= 4 && row == row1)
        {
            return false;
        }
    }

    return true;
}




bool Board :: canMove(int moveid, int row, int col, int killid)
{
    //如果moveid和killid颜色相同,不能走
    if(_s[moveid]._red ==_s[killid]._red)
    {
        //换选择dc
        _selectid = killid;

        update();

        return false;
    }
    switch(_s[moveid]._type)
    {
    case Stone ::JIANG:
        return canMoveJiang(moveid, row, col, killid);
        break;
    case Stone ::SHI:
        return canMoveShi(moveid, row, col, killid);
        break;
    case Stone ::XIANG:
        return canMoveXiang(moveid, row, col, killid);
        break;
    case Stone ::CHE:
        return canMoveChe(moveid, row, col, killid);
        break;
    case Stone ::MA:
        return canMoveMa(moveid, row, col, killid);
        break;
    case Stone ::PAO:
        return canMovePao(moveid, row, col, killid);
        break;
    case Stone ::BING:
        return canMoveBing(moveid, row, col, killid);
        break;
    }
    return true;
}

//为什么加上ev,ev中带了我点的位置
void Board :: mouseReleaseEvent(QMouseEvent *ev)
{
    /*
     第一步：获得我点的位置，或者说得到鼠标点击的位置
     第二步：看没有象棋被点南，那么要将pt（像素值）转化成行列值
     第三步：判断这个行列值中有不有棋子，如果有就选中
    */
    //1、
    /*
     * 特别说明的是：简单地认为“->”的左边是一个指针过于简单了。
     *这个符号是用来代替*和.的合成，因为一般情况下通过“指针”访问结构体或对象成员，
     * 是“先求值，再取分量”，即上述的(*p).set( )，因为用两个符号过于麻烦，所以才使用“->”来代替，这样直观、简单得多。
        */
    QPoint pt = /*(*ev).pos();*/ev ->pos();
    //2
    int row, col;

    bool bRet = getClickRowCol(pt, row, col);

    if(bRet == false)
    {
        return;
    }
    int i;
    //每次点击都能找到一个这时候点击的id
    int clickid = -1;
    for(i = 0; i<32;++i)
    {
        //判断行和列上是否有棋子
        if(_s[i]._row == row && _s[i]._col == col && _s[i]._dead ==false )
        {
            //continue就不执行本次循环后面的语句，直接进入下一次循
            //break也不执行本次循环后面的语句，并且跳出循环，不再执行了
            break;
        }

        //此时已经选中了棋子，用记录i，选其保存到_select中

    }

    if(i < 32)
    {
        clickid = i;
        //update();//会重新调用painterEvent函数

    }
    //之前如果没有棋子被选中，那么就选中棋子
    if(_selectid == -1)
    {


        /*
         *
        int i;
        for(i = 0; i<32; i++)
        {
            //判断行和列上是否有棋子
            if(_s[i]._row == row && _s[i]._col == col && _s[i]._dead ==false )
            {
                //continue就不执行本次循环后面的语句，直接进入下一次循
                //break也不执行本次循环后面的语句，并且跳出循环，不再执行了
                break;
            }
            //此时已经选中了棋子，用记录i，选其保存到_select中

        }
        */
        //选中棋子的时候，比如是红的，那么你就不能走黑的
        if(clickid != -1)
        {
            //轮到谁走，跟棋子的颜色一致的时候，才被让它被选中
            if(_bRedTurn ==_s[clickid]._red)
            {
                _selectid = clickid;
                //bRedTurn 只有在走棋的时候会发生变化，这是一个记录走棋的标识
                //走完之后，轮到谁走要对调
                _bRedTurn = !_bRedTurn;
                update();
            }
        }


        /*if(i < 32)
        {
            _selectid = i;
            //update会重新调用painterEvent函数
            update();
        }
        */

    }
    else//如果之前有棋子被选中，那么就移动棋子
    {
        //判断棋子是不是能走
            if(canMove(_selectid, row, col, clickid)==true)
            {
                //上一次选中的棋子的行和列
                _s[_selectid]._row = row;
                _s[_selectid]._col = col;
                if(clickid != -1)
                {
                    //棋子被杀了，不应该显示出来，在绘制棋子的地方先判断_s[id].dead的值
                    _s[clickid]._dead = true;
                }
                //因为有棋子的位置移动了，要调用update重绘棋盘
                update();
                //棋子吃了别的棋子颜色没有变是因为_selectid没有等于-1
                _selectid = -1;
            }
    }

}
