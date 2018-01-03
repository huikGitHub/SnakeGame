#include "mainwindow.h"
#include <QPainter>
#include <QMessageBox>


mainwindow::mainwindow(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle(tr("SnakeGame"));   //主题名
    this->setFixedSize(25 * WIDTH,25 * HEIGHT);
    timer = new QTimer(this);
    direc = RIGHT;
    //起始大小
    snake.push_back(QPoint(5,4));
    snake.push_back(QPoint(5,5));
    snake.push_back(QPoint(5,6));
    snake.push_back(QPoint(5,7));
    QVector<mainwindow::kind> temp1(WIDTH,STONE);
    mymap<<temp1;
    QVector<mainwindow::kind> temp2;
    temp2<<STONE;
    for(int i=1;i<=WIDTH-2;++i)
    {
        temp2<<GROUND;
    }
    temp2<<STONE;
    for(int j=1;j<=HEIGHT-2;++j)
    {
        mymap<<temp2;
    }
    mymap<<temp1;
    randapple();
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(automove()));
    timer->start(280);
}

mainwindow::~mainwindow()
{

}

mainwindow::direction mainwindow::direcof(QVector<QPoint>::size_type index)
{
    if(snake[index].y()==snake[index-1].y()-1)
        return DOWN;
    if(snake[index].y()-1==snake[index-1].y())
        return UP;
    if(snake[index].x()==snake[index-1].x()-1)
        return RIGHT;
    if(snake[index].x()-1==snake[index-1].x())
        return LEFT;
}

void mainwindow::randapple()
{
    qsrand(QTime::currentTime().msec()+QTime::currentTime().second()*1000);
    apple.setX(qrand()%WIDTH);
    apple.setY(qrand()%HEIGHT);
    if(mymap[apple.y()][apple.x()]!=0||snake.contains(apple))
        this->randapple();
}

void mainwindow::move(enum direction a)
{
    QVector<QPoint> temp(snake.size());
    switch(a)
    {
    case UP:
        temp[0]=QPoint(snake[0].x(),snake[0].y()-1);
        qCopy(snake.begin(),snake.end()-1,temp.begin()+1);
        snake=temp;
        break;
    case LEFT:
        temp[0]=QPoint(snake[0].x()-1,snake[0].y());
        qCopy(snake.begin(),snake.end()-1,temp.begin()+1);
        snake=temp;
        break;
    case DOWN:
        temp[0]=QPoint(snake[0].x(),snake[0].y()+1);
        qCopy(snake.begin(),snake.end()-1,temp.begin()+1);
        snake=temp;
        break;
    case RIGHT:
        temp[0]=QPoint(snake[0].x()+1,snake[0].y());
        qCopy(snake.begin(),snake.end()-1,temp.begin()+1);
        snake=temp;
        break;
    }
}

void mainwindow::eat()
{
    if((snake.end()-1)->x()==(snake.end()-2)->x()&&(snake.end()-1)->y()<(snake.end()-2)->y())
        snake.push_back(QPoint((snake.end()-1)->x(),(snake.end()-1)->y()-1));
    if((snake.end()-1)->x()==(snake.end()-2)->x()&&(snake.end()-1)->y()>(snake.end()-2)->y())
        snake.push_back(QPoint((snake.end()-1)->x(),(snake.end()-1)->y()+1));
    if((snake.end()-1)->y()==(snake.end()-2)->y()&&(snake.end()-1)->x()<(snake.end()-2)->x())
        snake.push_back(QPoint((snake.end()-1)->x()-1,(snake.end()-1)->y()));
    if((snake.end()-1)->y()==(snake.end()-2)->y()&&(snake.end()-1)->x()>(snake.end()-2)->x())
        snake.push_back(QPoint((snake.end()-1)->x()+1,(snake.end()-1)->y()));
    //防止增长出来的身体在墙里
    if(mymap[(snake.end()-1)->y()][(snake.end()-1)->x()]==STONE)
        snake.pop_back();
}

void mainwindow::paintEvent(QPaintEvent *event)
{
    QPainter mypainter(this);
    mypainter.setPen(QPen(Qt::black,1));
    //画地图
    for(QVector< QVector<mainwindow::kind> >::size_type i=0;i!=WIDTH;++i)
        for(QVector<mainwindow::kind>::size_type j=0;j!=HEIGHT;++j)
        {
            switch(mymap[j][i])
            {
            case GROUND:
                mypainter.drawPixmap(i*25,j*25,QPixmap(":/res/ground.png"));
                break;
            case STONE:
                mypainter.setBrush(Qt::red);
                mypainter.drawPixmap(i*25,j*25,QPixmap(":/res/stone.png"));
                break;
            }

        }
    //画苹果
    mypainter.drawPixmap(apple.x()*25,apple.y()*25,QPixmap(":/res/apple.png"));
    //画贪吃蛇蛇头
    switch(direc)
    {
    case UP:
        mypainter.drawPixmap(snake[0].x()*25,snake[0].y()*25,QPixmap(":/res/head_up.png"));
        break;
    case DOWN:
        mypainter.drawPixmap(snake[0].x()*25,snake[0].y()*25,QPixmap(":/res/head_down.png"));
        break;
    case LEFT:
        mypainter.drawPixmap(snake[0].x()*25,snake[0].y()*25,QPixmap(":/res/head_left.png"));
        break;
    case RIGHT:
        mypainter.drawPixmap(snake[0].x()*25,snake[0].y()*25,QPixmap(":/res/head_right.png"));
        break;
    }
    //画贪吃蛇蛇身
    for(QVector<QPoint>::size_type i=1;i!=snake.size();++i)
    {
        switch(direcof(i))
        {
        case UP:
            mypainter.drawPixmap(snake[i].x()*25,snake[i].y()*25,QPixmap(":/res/body_up.png"));
            break;
        case DOWN:
            mypainter.drawPixmap(snake[i].x()*25,snake[i].y()*25,QPixmap(":/res/body_down.png"));
            break;
        case LEFT:
            mypainter.drawPixmap(snake[i].x()*25,snake[i].y()*25,QPixmap(":/res/body_left.png"));
            break;
        case RIGHT:
            mypainter.drawPixmap(snake[i].x()*25,snake[i].y()*25,QPixmap(":/res/body_right.png"));
            break;
        }
    }
    event->accept();
}

void mainwindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        if(direc==RIGHT||direc==LEFT)
        {
            move(UP);
            if(snake[0]==apple)
            {
                eat();
                randapple();
            }
            if(mymap[snake[0].y()][snake[0].x()]==STONE)
            {
                QMessageBox::information(this,tr("Game over"),tr("You eat the stone!"));
                exit(EXIT_SUCCESS);
            }
            if(qFind(snake.begin()+1,snake.end(),snake[0])!=snake.end())
            {
                QMessageBox::information(this,tr("Game over"),tr("You eat yourself!"));
                exit(EXIT_SUCCESS);
            }
            direc=UP;
            update();
        }
        break;


    case Qt::Key_Down:
        if(direc==RIGHT||direc==LEFT)
        {
            move(DOWN);
            if(snake[0]==apple)
            {
                eat();
                randapple();
            }
            if(mymap[snake[0].y()][snake[0].x()]==STONE)
            {
                QMessageBox::information(this,tr("Game over"),tr("You eat the stone!"));
                exit(EXIT_SUCCESS);
            }
            if(qFind(snake.begin()+1,snake.end(),snake[0])!=snake.end())
            {
                QMessageBox::information(this,tr("Game over"),tr("You eat yourself!"));
                exit(EXIT_SUCCESS);
            }
            direc=DOWN;
            update();
        }
        break;


    case Qt::Key_Right:
        if(direc==UP||direc==DOWN)
        {
            move(RIGHT);
            if(snake[0]==apple)
            {
                eat();
                randapple();
            }
            if(mymap[snake[0].y()][snake[0].x()]==STONE)
            {
                QMessageBox::information(this,tr("Game over"),tr("You eat the stone!"));
                exit(EXIT_SUCCESS);
            }
            if(qFind(snake.begin()+1,snake.end(),snake[0])!=snake.end())
            {
                QMessageBox::information(this,tr("Game over"),tr("You eat yourself!"));
                exit(EXIT_SUCCESS);
            }
            direc=RIGHT;
            update();
        }
        break;


    case Qt::Key_Left:
        if(direc==UP||direc==DOWN)
        {
            move(LEFT);
        if(snake[0]==apple)
        {
            eat();
            randapple();
        }
        if(mymap[snake[0].y()][snake[0].x()]==STONE)
        {
            QMessageBox::information(this,tr("Game over"),tr("You eat the stone!"));
            exit(EXIT_SUCCESS);
        }
        if(qFind(snake.begin()+1,snake.end(),snake[0])!=snake.end())
        {
            QMessageBox::information(this,tr("Game over"),tr("You eat yourself!"));
            exit(EXIT_SUCCESS);
        }
        direc=LEFT;
        update();
        }
        break;
    }

    event->accept();
}

void mainwindow::automove()
{
    move(direc);
    if(snake[0]==apple)
    {
        eat();
        randapple();
    }
    if(mymap[snake[0].y()][snake[0].x()]==STONE)
    {
        QMessageBox::information(this,tr("Game over"),tr("You eat the stone!"));
        exit(EXIT_SUCCESS);
    }
    if(qFind(snake.begin()+1,snake.end(),snake[0])!=snake.end())
    {
        QMessageBox::information(this,tr("Game over"),tr("You eat yourself!"));
        exit(EXIT_SUCCESS);
    }
    update();
}
