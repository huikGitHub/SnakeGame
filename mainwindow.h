#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QVector>
#include <QPoint>
#include <QPaintEvent>
#include <QKeyEvent>
#define WIDTH  40
#define HEIGHT 30
class mainwindow : public QWidget
{
    Q_OBJECT
public:
    mainwindow(QWidget *parent = 0);
    ~mainwindow();
    enum direction{UP, DOWN, LEFT, RIGHT};
    enum kind{GROUND, STONE};
private:
    enum direction direc;                               //蛇头的运动方向
    enum direction direcof(QVector<QPoint>::size_type); //获取蛇身体某段的运动方向
    QVector<QPoint> snake;              //
    QPoint apple;
    QVector< QVector<enum kind> > mymap;
    QTimer *timer;
    void eat();                         //蛇吃豆子
    void move(enum direction);          //蛇移动
    void randapple();                   //随机放置豆子位置
private slots:
    void automove();                    //在定时器控制下自动移动
protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
