#ifndef __Alignment__Plotting__
#define __Alignment__Plotting__

#include <QScrollArea>
#include <QLabel>
#include <QStylePainter>
#include <QMotifStyle>
#include <QMessageBox>
#include <QToolButton>
#include <QMenu>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QComboBox>

enum{CellHeight = 30, CellWidth = 60};

struct alignCell
{
    QChar upBase;
    QChar leftBase;
    int score;
    
    bool top;
    bool topLeft;
    bool left;
    bool tracked;
};

struct AliInfo
{
    int algorithmID;
    int scoreStrategy;
    double d_value;
    double e_value;
    QString seq1;
    QString seq2;
};

class myLabel:public QLabel
{
public:
    myLabel(QString seq1, QString seq2, QWidget *parent = 0): QLabel(parent),alignInfo(0)
    {
        this->seq1 = seq1;
        this->seq2 = seq2;
        double width = (seq1.length()+4) * CellWidth;
        double height = (seq2.length()+4) * CellHeight;
        
        QSize size(width, height);
        
        setMinimumSize(size);
        setMaximumSize(size);
        pixmap = QPixmap(this->size());
        pixmap.fill(this,0,0);
        painter = new QPainter(&pixmap);
        drawGrid();
    }
    
    myLabel(myLabel const &old): alignInfo(0)
    {
        this->seq1 = old.seq1;
        this->seq2 = old.seq2;
        double width = (seq1.length()+4) * CellWidth;
        double height = (seq2.length()+4) * CellHeight;
        
        QSize size(width, height);
        
        setMinimumSize(size);
        setMaximumSize(size);

        pixmap = old.pixmap;
        QStylePainter temp_painter(this);
        temp_painter.drawPixmap(0, 0, pixmap);
        painter = 0;
        
        alignInfo = new AliInfo;
        alignInfo->algorithmID = old.alignInfo->algorithmID;
        alignInfo->scoreStrategy = old.alignInfo->scoreStrategy;
        alignInfo->d_value = old.alignInfo->d_value;
        alignInfo->e_value = old.alignInfo->e_value;
        alignInfo->seq1 = old.alignInfo->seq1;
        alignInfo->seq2 = old.alignInfo->seq2;
    }
    
    void drawGrid();
    void drawSeq();
    QPixmap pixmap;
    QPainter *painter;
    QPointF CellCenter(int x, int y);
    QPointF CellRight(int x, int y);
    QPointF CellLeft(int x, int y);
    QPointF CellBottomRight(int x, int y);
    QPointF CellTopLeft(int x, int y);
    QPointF CellBottom(int x, int y);
    QPointF CellTop(int x, int y);
    int rows;
    int columns;
    QString seq1,seq2;
    AliInfo *alignInfo;
    ~myLabel()
    {
        if(painter)
            painter->end();
        delete painter;
        if(alignInfo)
            delete alignInfo;
    }
    
protected:
    void paintEvent(QPaintEvent * )
    {
        QStylePainter painter(this);
        painter.drawPixmap(0, 0, pixmap);
    }
};



class myScrollArea: public QScrollArea
{
    Q_OBJECT
    
public:
    myScrollArea(QWidget *parent = 0);
    void drawArrow(int x1,int y1,int x2, int y2);
    void drawText(int x, int y, double score);
    void clearLabel(int ids);
    void addLabel(QString seq1, QString seq2);
    void drawSeq();
    void setWidget();
    void setWidget(int id);
    void setAliInfo(struct AliInfo *info);
    struct AliInfo *getAliInfo();
    void drawPixmap(QVector< QVector<alignCell *> > block);
    
    QComboBox *comboBox;
    QSlider *quality_slider;
    QLabel *quality_label;
    QLabel *quality_value_label;
    
private:
    QVector<myLabel *>  labelStack;
    int curLabel;
    QMenu *contextMenu;
    
    QToolButton *next_ToolButton;
    QToolButton *last_ToolButton;
    
public:
    QAction *nextAction;
    QAction *lastAction;
    QAction *saveAsPicture;
    QAction *deleteThisAlignment;
    
protected:
    void contextMenuEvent(QContextMenuEvent * event);
    
signals:
    void currentLabelChanged();
    
private slots:
    void next();
    void last();
    void saveAs();
    void deleteIt();
    void comboAction(int id);
    void changeQuality(int);
};




#endif /* defined(__Alignment__Plotting__) */
