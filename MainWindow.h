#ifndef __Alignment__MainWindow__
#define __Alignment__MainWindow__

#include <QMainWindow>
#include "Plotting.h"
#include <QTextEdit>
#include <QLayout>
#include <QDockWidget>
#include <QLineEdit>
#include <QRadioButton>
#include <QButtonGroup>
#include <QAction>
#include <QMenuBar>
#include <qtoolbutton>
#include <QToolBar>
#include <QRegExpValidator>
#include <QTextStream>
#include <cmath>
#include <QPushButton>


class leftDock;
class rightDock;
class SubMatrix;
class Align;

class MainWindow: public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    QVector<SubMatrix *> myMatrixStack;
    int curMatrix;
    
private:
    myScrollArea *scroll;
    leftDock *ldock;
    QDockWidget *left;
    rightDock *rdock;
    QDockWidget *right;
    QTextEdit *result;
    QDockWidget *bottom;
    QComboBox *matrixtype_comboBox;
    
    QAction *showAllDockAction;
    QAction *runAction;
    QAction *showLeftDockAction;
    QAction *showRightDockAction;
    QAction *showBottomDockAction;
    QAction *quitAction;
    QAction *showMatrixAction;
    QAction *aboutAction;
    
    QToolBar *runToolBar;
    QToolBar *comboBar;
    
    
    void createAction();
    void createMenuBar();
    
private slots:
    void alignment();
    void setAliInfo();
    void showAllDock();
    void leftdock();
    void rightdock();
    void bottomdock();
    void showMatrix();
    void about();
    void changeMatrix(int);
    void ldockClosed(bool);
    void rdockClosed(bool);
    void bdockClosed(bool);
};

class leftDock: public QWidget
{
    
    Q_OBJECT
public:
    leftDock(QWidget *parent = 0);
    
public:
    QLabel *alg_label;
    QRadioButton *alg1_button;
    QRadioButton *alg2_button;
    QLineEdit *threshold_line_edit;
    QButtonGroup *AlgorithmGroup;
    QRegExpValidator *validator;
    
    QLabel *gap_label;
    QRadioButton *linear_button;
    QRadioButton *special_button;
    QLineEdit *d_value1_line_edit;
    QLineEdit *d_value2_line_edit;
    QLineEdit *e_value_line_edit;
    QGridLayout *layout;
    QLabel *d_value1_label;
    QLabel *d_value2_label;
    QLabel *e_value_label;
    QButtonGroup *ScoringGroup;
    
public slots:
    void enable_edit_line();
    void enable_threshold(bool);
};

class rightDock: public QDialog
{
    Q_OBJECT
    
public:
    rightDock(QWidget* parent = 0);
    
public:
    QLabel *seq1_label;
    QLabel *seq2_label;
    QTextEdit *seq1_TextEdit;
    QTextEdit *seq2_TextEdit;
    QGridLayout *layout;
    
    QRegExpValidator *pro_validator;
    QRegExpValidator *my_validator;
    QRegExpValidator *dna_validator;
    
public slots:
    bool validateString(QString &seq1, QString &seq2);
};

class SubMatrix: public QDialog
{
    Q_OBJECT

public:
    SubMatrix(QString matrix_path,QWidget *parent = 0);
    
    QVector<QLineEdit *> lineEdits;
    QVector<QLabel *> HLabels;
    QVector<QLabel *> VLabels;
    QVector<QChar> index;
    QMap<QString, int> scoreMap;
    int itemNum;
    QGridLayout *leftLayout;
    QGridLayout *layout;
    void show();
    int getAlignmScore(QChar base1, QChar base2);

private:
    void createLabels();
    void createLineEdits();
    void createLayout();
    QPushButton *confirm_PushButton;
    QPushButton *cancel_PushButton;
    QRegExpValidator *validator;
    
protected:
    void closeEvent(QCloseEvent * event);
    
    private slots:
    void save();
    void cancel();
    
};

/*====================================*/
/*原本是写在另一个头文件中的声明*/

/*传送给比对类的所有信息*/
struct AlignInfo
{
    int alignAlgId;
    int scoreStrategyId;
    int d_value;
    int e_value;
    int threshold;
    QString seq1;
    QString seq2;
    SubMatrix *matrix;
};

//namespace Align {
/*变量*/
class Align{
public:
    Align(struct AlignInfo *temp_info){ runAlignment(temp_info);  };
    ~Align(){ alignBlock.clear(); delete info; };
    
    /*函数*/
    void minElement(int elem1, int elem2, int elem3, bool &elem1_small,
                    bool &elem2_small, bool &elem3_small);
    int affinePenalty(int x, int y, bool left_direction, int d_value, int e_value);
    void buildAlignBlock();
    void Needman_Wunch();
    void Smith_Waterman();
    void runAlignment(struct AlignInfo *temp_info);
    QVector< QVector<alignCell *> > getAlignBlock(){ return alignBlock; };
    void backTracking1();
    void backTracking2();
    void getDipth(QVector< QPolygon > &prequelSeq, int &times);
    void putIntoScreen(int times);
    QString strForWidget;
    
private:
    struct AlignInfo *info;
    QVector< QVector<alignCell *> > alignBlock;
    QVector< QPolygon > prequelSeq;
    
};

//};

/*====================================*/



#endif /* defined(__Alignment__MainWindow__) */
