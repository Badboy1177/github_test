#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
//在棋盘的头文件里包含棋子头文件，因为棋子是属于棋盘的
#include "Stone.h"

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);

    //定义了32个棋子，接下来去绘制函数中画棋子
    Stone _s[32];
    //定义棋子的半径
    int _r;
    bool _bSide;
    int _selectid;

    bool _bRedTurn;//轮到谁走，先在棋盘中给初始化
    //返回象棋棋盘行列对应的像素坐标，因为drawEILL函数要用像素坐标来画园
    QPoint center(int row, int col);
    QPoint center(int id);

    /*
     bool getRowCol(QPoint pt, int &row, int &col)中的参数
     PT是棋盘上像素坐标，两个输出参数，一个是行和死，为什么是bool是因为，如果点到
     棋盘外去了要返回false;
    */
    bool getClickRowCol(QPoint pt, int& row, int& col);
    /*画棋子还需要有个参数QPoint,之所引入是因为画棋子已经和窗口关联
    如果不引入，传参数的时候是拷贝传递，不能达到双向传递，改就原值
    */
    void drawStone(QPainter& painter, int id);

    //声明绘制公有接口，将接口声明为棋盘的成员函数
    virtual void paintEvent(QPaintEvent *);

    /*走棋：点中一个棋子，然后点到另一个地方释放,具体是情况，拿棋子来讲
    棋子是释放的时候证明走了一步，所以这里建议用下面的事件*/
    //void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    /*...............帮助判断函数........................ */

    bool red(int id);
    bool sameColor(int id1, int id2);
    int getStoneId(int row, int col); // 获取行row列col上的棋子id
    void killStone(int id);
    void reliveStone(int id);
    void moveStone(int moveid, int row, int col);
    bool isDead(int id); // 判断id棋子是否死亡



    //................每种象棋移动规则.......................
    bool canMove(int moveid, int row, int col, int killid);
    bool canMoveJiang(int moveid, int row, int col, int killid);
    bool canMoveXiang(int moveid, int row, int col, int killid);
    bool canMoveChe(int moveid, int row, int col, int killid);
    bool canMoveMa(int moveid, int row, int col, int killid);
    bool canMoveShi(int moveid, int row, int col, int killid);
    bool canMovePao(int moveid, int row, int col, int killid);
    bool canMoveBing(int moveid, int row, int col, int killid);
    //.............移动规则几个相关的帮助函数............
    int relation(int row1, int col1, int row, int col);
    // 判断id棋子是否在棋盘下方
    bool isBottomSide(int id);
    // 判断两个点是否在同一条直线上，炮和车走棋的时候需要用到
    int getStoneCountAtLine(int row1, int col1, int row2, int col2);
signals:

public slots:

};

#endif // BOARD_H
