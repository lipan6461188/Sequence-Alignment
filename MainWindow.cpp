#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),rdock(0)
{
    
    /*初始化矩阵栈*/
    QStringList matrixList;
    matrixList.append(":/matrix/BLOSUM45");
    matrixList.append(":/matrix/BLOSUM50");
    matrixList.append(":/matrix/BLOSUM62");
    matrixList.append(":/matrix/BLOSUM80");
    matrixList.append(":/matrix/BLOSUM90");
    matrixList.append(":/matrix/PAM30");
    matrixList.append(":/matrix/PAM70");
    matrixList.append(":/matrix/PAM250");
    matrixList.append(":/matrix/DNA_matrix");
    
    for (int i = 0; i < matrixList.count(); i++)
    {
        SubMatrix *tmpMatrix = new SubMatrix(matrixList[i],this);
        myMatrixStack.append(tmpMatrix);
    }
    curMatrix = 2;
    
    
    matrixtype_comboBox = new QComboBox(this);
    matrixtype_comboBox->addItem("BLOSUM45");
    matrixtype_comboBox->addItem("BLOSUM50");
    matrixtype_comboBox->addItem("BLOSUM62");
    matrixtype_comboBox->addItem("BLOSUM80");
    matrixtype_comboBox->addItem("BLOSUM90");
    matrixtype_comboBox->addItem("PAM30");
    matrixtype_comboBox->addItem("PAM70");
    matrixtype_comboBox->addItem("PAM250");
    matrixtype_comboBox->addItem("DNA");
    connect(matrixtype_comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeMatrix(int)));
    
    scroll = new myScrollArea;
    setCentralWidget(scroll);
    
    ldock = new leftDock;
    left = new QDockWidget;
    left->setWidget(ldock);
    connect(left, SIGNAL(visibilityChanged(bool)), this, SLOT(ldockClosed(bool)));
    
    rdock = new rightDock;
    right = new QDockWidget;
    right->setWidget(rdock);
    connect(right, SIGNAL(visibilityChanged(bool)), this, SLOT(rdockClosed(bool)));
    
    rdock->my_validator = rdock->pro_validator;
    
    result = new QTextEdit;
    result->setReadOnly(true);
    result->setText("<h1>Hello</h1>\n<h1>Hello</h1>\n<h1>Hello</h1>\n");
    bottom = new QDockWidget;
    bottom->setWidget(result);
    connect(bottom, SIGNAL(visibilityChanged(bool)), this, SLOT(bdockClosed(bool)));
    
    addDockWidget(Qt::LeftDockWidgetArea, left);
    addDockWidget(Qt::RightDockWidgetArea, right);
    addDockWidget(Qt::BottomDockWidgetArea, bottom);
 
    createAction();
    
    runToolBar = addToolBar(tr("&File"));
    runToolBar -> addAction(runAction);
    runToolBar -> addAction(scroll->saveAsPicture);
    runToolBar->addAction(scroll->deleteThisAlignment);
    runToolBar -> addWidget(matrixtype_comboBox);
    comboBar = addToolBar(tr("&Index"));
    comboBar->setAllowedAreas(Qt::RightToolBarArea);
    comboBar->addAction(scroll->lastAction);
    comboBar->addAction(scroll->nextAction);
    comboBar->addWidget(scroll->comboBox);
    comboBar->addWidget(scroll->quality_label);
    comboBar->addWidget(scroll->quality_slider);
    comboBar->addWidget(scroll->quality_value_label);
    
    connect(scroll, SIGNAL(currentLabelChanged()), this, SLOT(setAliInfo()));
    
    matrixtype_comboBox->setCurrentIndex(2);
    
    statusBar();
    createMenuBar();
}

void MainWindow::ldockClosed(bool visible)
{
    showLeftDockAction->setChecked(visible);
    if (!showLeftDockAction->isChecked() &&
        !showRightDockAction->isChecked() &&
        !showBottomDockAction->isChecked())
        showAllDockAction->setChecked(false);
}

void MainWindow::rdockClosed(bool visible)
{
    showRightDockAction->setChecked(visible);
    if (!showLeftDockAction->isChecked() &&
        !showRightDockAction->isChecked() &&
        !showBottomDockAction->isChecked())
        showAllDockAction->setChecked(false);
}

void MainWindow::bdockClosed(bool visible)
{
    showBottomDockAction->setChecked(visible);
    if (!showLeftDockAction->isChecked() &&
        !showRightDockAction->isChecked() &&
        !showBottomDockAction->isChecked())
        showAllDockAction->setChecked(false);
}


void MainWindow::changeMatrix(int matrixID)
{
    curMatrix = matrixID;
    if (rdock)
    {
        if (matrixID == 8)
            rdock->my_validator = rdock->dna_validator;
        else
            rdock->my_validator = rdock->pro_validator;
    }
    int existence = 0;
    switch (matrixID) {
        case 0:case 1:
            existence = -5;
            break;
            
        case 2:
            existence = 0;
            break;
            
        case 3:case 4:
            existence = -6;
            break;
            
        case 5:case 6:
            existence = -17;
            break;
        
        case 7:
            existence = -8;
            break;
            
        default:
            break;
    }
    ldock->d_value1_line_edit->setText(QString::number(abs(existence)));
    ldock->d_value2_line_edit->setText(QString::number(abs(existence)));
    ldock->e_value_line_edit->setText(QString::number(2));
}


void MainWindow::createMenuBar()
{
    QMenu *run = menuBar()->addMenu(tr("&Opeartion"));
    run->addAction(runAction);
    run->addAction(scroll->deleteThisAlignment);
    run->addSeparator();
    run->addAction(scroll->lastAction);
    run->addAction(scroll->nextAction);
    run->addAction(scroll->saveAsPicture);
    run->addSeparator();
    run->addAction(aboutAction);
    run->addAction(quitAction);
    
    QMenu *view = menuBar()->addMenu(tr("&Window"));
    view->addAction(showLeftDockAction);
    view->addAction(showRightDockAction);
    view->addAction(showBottomDockAction);
    view->addSeparator();
    view->addAction(showAllDockAction);
    
    QMenu *matrix = menuBar()->addMenu(tr("&Matrix"));
    matrix->addAction(showMatrixAction);
}


leftDock::leftDock(QWidget *parent):QWidget(parent)
{
    alg_label = new QLabel("<h3>Algorithm: </h3>");
    AlgorithmGroup = new QButtonGroup;
    alg1_button = new QRadioButton(tr("Needman-wunch"));
    alg1_button->setChecked(true);
    alg2_button = new QRadioButton("Smith-waterman");
    threshold_line_edit = new QLineEdit("5");
    threshold_line_edit->setEnabled(false);
    AlgorithmGroup->addButton(alg1_button);
    AlgorithmGroup->addButton(alg2_button);
    AlgorithmGroup->setId(alg1_button, 1);
    AlgorithmGroup->setId(alg2_button, 2);
    
    validator = new QRegExpValidator(QRegExp("^-?[0-9]+"),this);
    
    gap_label = new QLabel("<h3>gap Scoring: </h3>");
    ScoringGroup = new QButtonGroup;
    linear_button = new QRadioButton("liner");
    special_button = new QRadioButton("special");
    linear_button->setChecked(true);
    ScoringGroup->addButton(linear_button);
    ScoringGroup->addButton(special_button);
    ScoringGroup->setId(linear_button, 1);
    ScoringGroup->setId(special_button, 2);
    d_value1_line_edit = new QLineEdit("12");
    d_value1_line_edit->setEnabled(true);
    d_value2_line_edit = new QLineEdit("8");
    e_value_line_edit = new QLineEdit("2");
    d_value1_label = new QLabel("d-value: ");
    d_value1_label->setBuddy(d_value1_line_edit);
    d_value2_label = new QLabel("d-value: ");
    d_value2_label->setBuddy(d_value2_line_edit);
    e_value_label = new QLabel("e-value: ");
    e_value_label->setBuddy(e_value_line_edit);
    d_value2_line_edit->setEnabled(false);
    e_value_line_edit->setEnabled(false);
    d_value1_line_edit->setValidator(validator);
    d_value2_line_edit->setValidator(validator);
    e_value_line_edit->setValidator(validator);
    
    layout = new QGridLayout;
    layout->addWidget(alg_label, 1, 1);
    layout->addWidget(alg1_button,2,1);
    layout->addWidget(alg2_button,3,1);
    layout->addWidget(threshold_line_edit,3,2);
    layout->addWidget(gap_label,4,1);
    layout->addWidget(linear_button,5,1);
    layout->addWidget(d_value1_label,6,1);
    layout->addWidget(d_value1_line_edit,6,2);
    layout->addWidget(special_button,7,1);
    layout->addWidget(d_value2_label,8,1);
    layout->addWidget(e_value_label,8,2);
    layout->addWidget(d_value2_line_edit,9,1);
    layout->addWidget(e_value_line_edit,9,2);
    
    connect(linear_button, SIGNAL(clicked(bool)), this, SLOT(enable_edit_line()));
    connect(special_button, SIGNAL(clicked(bool)), this, SLOT(enable_edit_line()));
    connect(alg2_button, SIGNAL(toggled(bool)), this, SLOT(enable_threshold(bool)));
    
    setLayout(layout);
    setFixedSize(sizeHint());
}

void leftDock::enable_threshold(bool clicked)
{
    if (clicked)
    {
        threshold_line_edit->setEnabled(true);
    }else{
        threshold_line_edit->setEnabled(false);
    }
}

void leftDock::enable_edit_line()
{
    if (linear_button->isChecked()) {
        d_value1_line_edit->setEnabled(true);
        d_value2_line_edit->setEnabled(false);
        e_value_line_edit->setEnabled(false);
    }else{
        d_value1_line_edit->setEnabled(false);
        d_value2_line_edit->setEnabled(true);
        e_value_line_edit->setEnabled(true);
    }
}

rightDock::rightDock(QWidget *parent):QDialog(parent)
{
    seq1_label = new QLabel("<h3><font color='green'>Sequence1: </font></h3>");
    seq2_label = new QLabel("<h3><font color='green'>Sequence2: </font></h3>");
    seq1_TextEdit = new QTextEdit("IADARLLLN");
    seq1_TextEdit->setFixedSize(180, 90);
    seq2_TextEdit = new QTextEdit("TVLGILKAYT");
    seq2_TextEdit->setFixedSize(180, 90);
    layout = new QGridLayout;
    layout->addWidget(seq1_label,1,1);
    layout->addWidget(seq1_TextEdit,2,1);
    layout->addWidget(seq2_label,3,1);
    layout->addWidget(seq2_TextEdit,4,1);
    
    pro_validator = new QRegExpValidator(QRegExp("[ARNDCQEGHILKMFPSTWYVBZXarndcqeghilkmfpstwyvbzx]+"),this);
    dna_validator= new
        QRegExpValidator(QRegExp("[atcgATGC]+"),this);
    
    setLayout(layout);
    setFixedSize(sizeHint());
}

bool rightDock::validateString(QString &seq1_, QString &seq2_)
{
    QString seq1 = seq1_TextEdit->toPlainText().toUpper();
    QString seq2 = seq2_TextEdit->toPlainText().toUpper();
    if(seq1[0] == '>')
    {
        int index = seq1.indexOf('\n');
        seq1.remove(0, index+1);
    }
    if(seq2[0] == '>')
    {
        int index = seq2.indexOf('\n');
        seq2.remove(0, index+1);
    }
    seq1.remove(QRegExp("[\n\t ]"));
    seq2.remove(QRegExp("[\n\t ]"));
    
    seq1_ = seq1;
    seq2_ = seq2;
    
    int id=0;
    if( my_validator->validate(seq1, id) != QValidator::Acceptable
       || my_validator->validate(seq2, id) != QValidator::Acceptable)
    {
        return false;
    }
    return true;
}

void MainWindow::createAction()
{
    runAction = new QAction(tr("&RUN"),this);
    runAction->setStatusTip(tr("Alighmn Two Sequence Now!"));
    runAction->setShortcut(tr("Ctrl+R"));
    runAction->setIcon(QIcon(":/images/run.png"));
    connect(runAction, SIGNAL(triggered()), this, SLOT(alignment()));
    
    showAllDockAction = new QAction(tr("&AllDocks"),this);
    showAllDockAction->setCheckable(true);
    showAllDockAction->setChecked(true);
    showAllDockAction->setStatusTip("Show All the Dock Window");
    connect(showAllDockAction, SIGNAL(triggered()), this, SLOT(showAllDock()));
    
    showLeftDockAction = new QAction(tr("&leftDock"),this);
    showLeftDockAction->setCheckable(true);
    showLeftDockAction->setChecked(true);
    showLeftDockAction->setStatusTip("Show or not show left dock");
    connect(showLeftDockAction, SIGNAL(triggered()), this, SLOT(leftdock()));

    showRightDockAction = new QAction(tr("&rightDock"),this);
    showRightDockAction->setCheckable(true);
    showRightDockAction->setChecked(true);
    showRightDockAction->setStatusTip("Show or not show right dock");
    connect(showRightDockAction, SIGNAL(triggered()), this, SLOT(rightdock()));

    showBottomDockAction = new QAction(tr("&bottomDock"),this);
    showBottomDockAction->setCheckable(true);
    showBottomDockAction->setChecked(true);
    showBottomDockAction->setStatusTip("Show or not show bottom dock");
    connect(showBottomDockAction, SIGNAL(triggered()), this, SLOT(bottomdock()));

    quitAction = new QAction(tr("&quit"),this);
    quitAction->setIcon(QIcon(":/images/quit.png"));
    quitAction->setStatusTip("quit this application");
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    showMatrixAction = new QAction(tr("&Matrix"),this);
    showMatrixAction->setIcon(QIcon(":/images/matrix.png"));
    showMatrixAction->setShortcut(tr("Ctrl+M"));
    showMatrixAction->setStatusTip("show and set the substitude matrix");
    connect(showMatrixAction, SIGNAL(triggered()), this, SLOT(showMatrix()));
    
    aboutAction = new QAction(tr("&About"),this);
    aboutAction->setIcon(QIcon(":/images/abount.png"));
    aboutAction->setStatusTip("The Inforation of this programe");
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

}

void MainWindow::about()
{
    QMessageBox::about(this, tr("Short Sequence Alignment Program"),
                       tr("<p><font color='gray'>This small programe is developed by <i>Lee</i>.</p>"
                          "<p>If You find Some Bugs in it, Please contace Me</p>"
                          "<p><font color='red'>email: <i>hnsfyfyzlp@126.com</i></font></p>"));
}

void MainWindow::showAllDock()
{
    if( showAllDockAction->isChecked() )
    {
        left->show();
        right->show();
        bottom->show();
    }else
    {
        left->hide();
        right->hide();
        bottom->hide();
    }
   
}

void MainWindow::leftdock()
{
    if(showLeftDockAction->isChecked())
    {left->setVisible(true);}
    else
    {left->setVisible(false);}
}

void MainWindow::rightdock()
{
    if(showRightDockAction->isChecked())
        right->show();
    else
        right->hide();
}

void MainWindow::bottomdock()
{
    if(showBottomDockAction->isChecked())
        bottom->show();
    else
        bottom->hide();
}

void MainWindow::showMatrix()
{
    myMatrixStack[curMatrix]->show();
}

void MainWindow::alignment()
{
    QString seq1;
    QString seq2;
    if(rdock->validateString(seq1,seq2))
    {
        scroll->addLabel(seq1, seq2);
    }else{
        QMessageBox::warning(this, tr("ERROR"),
                             tr("the sequence must be fasta file"));
    }
    struct AliInfo *info = new AliInfo;
    info->algorithmID = ldock->AlgorithmGroup->checkedId();
    info->scoreStrategy = ldock->ScoringGroup->checkedId();
    if(info->scoreStrategy == 1)
        info->d_value = ldock->d_value1_line_edit->text().toDouble();
    else
    {
        info->d_value = ldock->d_value2_line_edit->text().toDouble();
        info->e_value = ldock->e_value_line_edit->text().toDouble();
    }
    info->seq1 = rdock->seq1_TextEdit->toPlainText();
    info->seq2 = rdock->seq2_TextEdit->toPlainText();
    scroll->setAliInfo(info);
    
    struct AlignInfo *myAlignInfo = new AlignInfo;
    myAlignInfo->alignAlgId = ldock->AlgorithmGroup->checkedId();
    myAlignInfo->scoreStrategyId = ldock->ScoringGroup->checkedId();
    if(myAlignInfo->scoreStrategyId == 1)
    {
        myAlignInfo->d_value = ldock->d_value1_line_edit->text().toInt();
    }else{
        myAlignInfo->d_value = ldock->d_value2_line_edit->text().toInt();
        myAlignInfo->e_value = ldock->e_value_line_edit->text().toInt();
    }
    myAlignInfo->threshold = ldock->threshold_line_edit->text().toInt();
    myAlignInfo->seq1 = seq1;
    myAlignInfo->seq2 = seq2;
    myAlignInfo->matrix = myMatrixStack[curMatrix];
    Align align(myAlignInfo);
    QVector< QVector<alignCell *> > block = align.getAlignBlock();
    scroll->drawPixmap(block);
    
    result->clear();
    result->setHtml(align.strForWidget);
}

/*
 struct AlignInfo
 {
 int alignAlgId;
 int scoreStrategyId;
 int d_value;
 int e_value;
 QString seq1;
 QString seq2;
 SubMatrix *matrix;
 };
 */

void MainWindow::setAliInfo()
{
    struct AliInfo *info = scroll->getAliInfo();
    ldock->AlgorithmGroup->button(info->algorithmID)->setChecked(true);
    ldock->ScoringGroup->button(info->scoreStrategy)->setChecked(true);
    if(info->scoreStrategy == 1)
    {
        ldock->d_value1_line_edit->setText(QString::number(info->d_value));
        ldock->d_value1_line_edit->setEnabled(true);
        ldock->d_value2_line_edit->setEnabled(false);
        ldock->e_value_line_edit->setEnabled(false);
    }
    else{
        ldock->d_value2_line_edit->setText(QString::number(info->d_value));
        ldock->e_value_line_edit->setText(QString::number(info->e_value));
        ldock->d_value1_line_edit->setEnabled(false);
        ldock->d_value2_line_edit->setEnabled(true);
        ldock->e_value_line_edit->setEnabled(true);

    }
    rdock->seq1_TextEdit->setText(info->seq1);
    rdock->seq2_TextEdit->setText(info->seq2);
}


void SubMatrix::createLabels()
{
    for (int i=0; i<itemNum; i++)
    {
        HLabels.append(new QLabel(index[i]));
        VLabels.append(new QLabel(index[i]));
    }
}

void SubMatrix::createLineEdits()
{
    for (int i=0; i<itemNum; i++) {
        for (int j=0; j<itemNum; j++) {
            QLineEdit *line = new QLineEdit;
            line->setValidator(validator);
            int value = scoreMap[ QString(index[i])+QString(index[j]) ];
            line->setText(QString::number(value));
            lineEdits.append(line);
        }
    }
}

void SubMatrix::createLayout()
{
    leftLayout = new QGridLayout;
    
    leftLayout->setVerticalSpacing(5);
    leftLayout->setHorizontalSpacing(12);
    
    for (int i=0; i<itemNum; i++) {
        leftLayout->addWidget(HLabels[i], 1, i+2);
        leftLayout->addWidget(VLabels[i], i+2, 1);
    }
    
    for (int k=1; k<=lineEdits.count(); k++)
    {
        int x = std::ceil(double(k)/itemNum);
        int y = k%itemNum;
        if(y==0) y = itemNum;
        x++;
        y++;
        leftLayout->addWidget(lineEdits[k-1],x,y);
    }
    QGridLayout *rightLayout = new QGridLayout;
    rightLayout->addWidget(confirm_PushButton, 1, 1);
    rightLayout->addWidget(cancel_PushButton,2,1);
    
    layout = new QGridLayout;
    layout->addLayout(leftLayout,1,1);
    layout->addLayout(rightLayout,1,2);
}

void SubMatrix::closeEvent(QCloseEvent *event)
{
    event->ignore();
    hide();
}

void SubMatrix::save()
{
    for (int k=1; k<=lineEdits.count(); k++)
    {
        int x = std::ceil(double(k)/itemNum);
        int y = k%itemNum;
        if(y==0) y = itemNum;
        scoreMap[ QString(index[x-1]) + QString(index[y-1]) ] =
        lineEdits[k-1]->text().toInt();
    }
    hide();
}

void SubMatrix::show()
{
    for (int k=1; k<=lineEdits.count(); k++)
    {
        int x = std::ceil(double(k)/itemNum);
        int y = k%itemNum;
        if(y==0) y = itemNum;
        QString myScore = QString::number(scoreMap[ QString(index[x-1])
                                                   + QString(index[y-1]) ]) ;
        lineEdits[k-1]->setText(myScore);
    }
    QWidget::show();
}

void SubMatrix::cancel()
{
    hide();
}

int SubMatrix::getAlignmScore(QChar base1, QChar base2)
{
    return scoreMap[ QString(base1) + QString(base2) ];
}

SubMatrix::SubMatrix(QString matrix_path,QWidget *parent): QDialog(parent)
{
    QFile file(matrix_path);
    if( !file.open(QIODevice::ReadOnly) ){ QMessageBox::warning(0, "", matrix_path +" Open Failed"); }
    QTextStream in(&file);
    QMap<QString, int> scores;
    for(int i=0;!in.atEnd();i++){
        QString line = in.readLine();
        QStringList items = line.split(QRegExp("[\t ]"),QString::SkipEmptyParts);
        if(i == 0)
        {
            for(int j=0;j<items.count();j++)
            {
                index.append(items[j].at(0));
            }
            itemNum = index.count();
            continue;
        }
        for (int j=0; j<itemNum; j++)
        {
            scoreMap[items[0]+index[j]] = items[j+1].toInt();
        }
    }
    file.close();
    
    confirm_PushButton = new QPushButton("Confirm",this);
    cancel_PushButton = new QPushButton("cancel",this);
    connect(confirm_PushButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(cancel_PushButton, SIGNAL(clicked()), this, SLOT(cancel()));
    setBackgroundRole(QPalette::Background);
    createLabels();
    validator = new QRegExpValidator(QRegExp("^-?[0-9]+"),this);
    createLineEdits();
    createLayout();
    setLayout(layout);
    
    setMinimumWidth(1050);
}






/*=================================================================*/
//using namespace Align;

void Align::buildAlignBlock()
{
    int seq1_len = info->seq1.length();
    int seq2_len = info->seq2.length();

    alignBlock.clear();
    
    for (int i=0; i<=seq2_len; i++) {
        QVector<alignCell *> row;
        for (int j=0; j<=seq1_len; j++) {
            struct alignCell *cell = new alignCell;
            if (i > 0)
            {
                cell->leftBase = info->seq2[i-1];
            }
            if (j > 0) {
                cell->upBase = info->seq1[j-1];
            }
            cell->score = 0;
            cell->top = cell->topLeft = cell->left = false;
            cell->tracked = false;
            row.append(cell);
        }
        alignBlock.append(row);
    }
    
}

void Align::Needman_Wunch()
{
    buildAlignBlock();
    
 //   QMessageBox::warning(0, "", QString::number(alignBlock.count()) +"\t" + QString::number(alignBlock[1].count()));
    
    int d_value = info->d_value;
    int e_value = info->e_value;
    SubMatrix *matrix = info->matrix;
    
    /*采用线性罚分方式*/
    if(info->scoreStrategyId == 1)
    {
        e_value = d_value;
    }
    
    for(int row = 0;row<alignBlock.count();row++)
    {
        QVector<alignCell *> eachRow = alignBlock[row];
        for (int column=0; column<eachRow.count(); column++)
        {
            if (row == 0)
            {
                if (column == 0)
                {
                    /*第一行，第一个*/
                    alignBlock[row][column]->score = 0;
                    continue;
                }else{
                /*第一行，非第一个*/
                alignBlock[row][column]->score = affinePenalty(row,column,true,d_value,e_value);
                alignBlock[row][column]->left = true;
                continue;
                }
            }
            if (column == 0)
            {
                /*非第一行，第一列*/
                alignBlock[row][column]->score = affinePenalty(row,column,false,d_value,e_value);
                alignBlock[row][column]->top = true;
                continue;
            }
            else
            {
                /*非第一行，也非第一列*/
                int top = affinePenalty(row,column,false,d_value,e_value);
                
                int topLeft = alignBlock[row-1][column-1]->score +
                matrix->getAlignmScore(alignBlock[row][column]->upBase,
                                       alignBlock[row][column]->leftBase);
                
                int left = affinePenalty(row,column,true,d_value,e_value);
         
                bool top_large,left_large,topLeft_large;
                minElement(top, topLeft, left, top_large, topLeft_large, left_large);
              
              
                
                if(top_large == true)
                {
                    alignBlock[row][column]->top = true;
                    alignBlock[row][column]->score = top;
                }
                if(topLeft_large == true)
                {
                    alignBlock[row][column]->topLeft = true;
                    alignBlock[row][column]->score = topLeft;
                }
                if(left_large == true)
                {
                    alignBlock[row][column]->left = true;
                    alignBlock[row][column]->score = left;
                }
            }
        }
    }
}


int Align::affinePenalty(int x, int y, bool left_direction, int d_value, int e_value)
{
    if (left_direction)
    {
        if (alignBlock[x][y-1]->left)
        {
            /*左边扣过一次分了*/
            return alignBlock[x][y-1]->score - e_value;
        }
        else
        {
            /*第一次扣分*/
            return alignBlock[x][y-1]->score - d_value;
        }
    }
    else
    {
        /*从上面看下来*/
        if (alignBlock[x-1][y]->top)
        {
            /*上面的元素扣过一次分了*/
            return alignBlock[x-1][y]->score - e_value;
        }
        else
        {
            /*第一次扣分*/
            return alignBlock[x-1][y]->score - d_value;
        }
    }
}


void Align::minElement(int elem1, int elem2, int elem3, bool &elem1_large, bool &elem2_large, bool &elem3_large)
{
    elem1_large = elem2_large = elem3_large = false;
    /*首先排序*/
    int max = (elem1 > elem2) ? elem1 : elem2;
    max = (max > elem3) ? max : elem3;
    if(elem1 == max)
    {
        elem1_large = true;
        if(max == elem2) elem2_large = true;
        if(max == elem3) elem3_large = true;
        return;
    }
    if(elem2 == max)
    {
        elem2_large = true;
        if(max == elem3) elem3_large = true;
        return;
    }
    if(elem3 == max)
    {
        if(max == elem3) elem3_large = true;
    }
}

void Align::runAlignment(struct AlignInfo *temp_info)
{
    info = temp_info;
    
    switch (info->alignAlgId)
    {
        case 1:
            Needman_Wunch();
            backTracking1();
            break;
        case 2:
            Smith_Waterman();
            backTracking2();
            break;
            
        default:
            break;
    }
}

void Align::Smith_Waterman()
{
    buildAlignBlock();
    
    int d_value = info->d_value;
    int e_value = info->e_value;
    SubMatrix *matrix = info->matrix;
    
    /*采用线性罚分方式*/
    if(info->scoreStrategyId == 1)
    {
        e_value = d_value;
    }
    
    for(int row = 0;row<alignBlock.count();row++)
    {
        QVector<alignCell *> eachRow = alignBlock[row];
        for (int column=0; column<eachRow.count(); column++)
        {
            if (row == 0)
            {
                if (column == 0)
                {
                    /*第一行，第一个*/
                    alignBlock[row][column]->score = 0;
                    continue;
                }else{
                    /*第一行，非第一个*/
                    alignBlock[row][column]->score = 0;
                    alignBlock[row][column]->left = true;
                    continue;
                }
            }
            if (column == 0)
            {
                /*非第一行，第一列*/
                alignBlock[row][column]->score = 0;
                alignBlock[row][column]->top = true;
                continue;
            }
            else
            {
                /*非第一行，也非第一列*/
                int top = affinePenalty(row,column,false,d_value,e_value);
                
                int topLeft = alignBlock[row-1][column-1]->score +
                matrix->getAlignmScore(alignBlock[row][column]->upBase,
                                       alignBlock[row][column]->leftBase);
                
                int left = affinePenalty(row,column,true,d_value,e_value);
                
                top = (top > 0)?top:0;
                topLeft = (topLeft > 0)?topLeft:0;
                left = (left > 0)?left:0;
                
                bool top_large,left_large,topLeft_large;
                minElement(top, topLeft, left, top_large, topLeft_large, left_large);
                
                if(top_large == true && top != 0)
                {
                    alignBlock[row][column]->top = true;
                    alignBlock[row][column]->score = top;
                }
                if(topLeft_large == true && topLeft != 0)
                {
                    alignBlock[row][column]->topLeft = true;
                    alignBlock[row][column]->score = topLeft;
                }
                if(left_large == true && left != 0)
                {
                    alignBlock[row][column]->left = true;
                    alignBlock[row][column]->score = left;
                }
            }
        }
    }
}

void Align::putIntoScreen(int times)
{
    /*比对的结果输出到屏幕中*/
    QString finalScore = QString::number( alignBlock.last().last()->score );
    QString showInScreen;
    
    for (int i=0; i<prequelSeq.count(); i++)
    {
        QString seq1;
        QString seq2;
        
        for (int j=prequelSeq[i].count()-1; j >= 0; j--)
        {
            int x = prequelSeq[i][j].x();
            int y = prequelSeq[i][j].y();
            QChar upbase = alignBlock[x][y]->upBase;
            QChar leftbase = alignBlock[x][y]->leftBase;
            
            if (j == prequelSeq[i].count()-1)
            {
                continue;
            }else
            {
                if (x == prequelSeq[i][j+1].x())
                    seq2 += '-';
                else
                    seq2 += leftbase;
                
                if (y == prequelSeq[i][j+1].y())
                    seq1 += '-';
                else
                    seq1 += upbase;
            }
        }
        
        QString gap;
        for (int i=0; i<seq1.length(); i++)
        {
            if (seq1[i] == seq2[i])
                gap += seq1[i];
            else
                gap += "<font color='green'>*</font>";
        }
        int currentScore = alignBlock[ prequelSeq[i][0].x() ][ prequelSeq[i][0].y() ]->score;
        
        showInScreen += seq1 + "<br><font color='red'>" +gap +
                        "</font><br>" + seq2 + "<br><font size=5>Score: " + QString::number(currentScore) + "</font><br><br>";
    }

    showInScreen = "<font size=10><h2><font color='gray'>Find possible alignments: "+
    QString::number(times)+"</font></h2><br>"+showInScreen+"</font>";
    strForWidget = showInScreen;
}


/*Needleman-Wunch算法专用回朔法*/
void Align::backTracking1()
{
    int times = 1;
    QPolygon startPoint;
    
    int row = alignBlock.count();
    int column = alignBlock[0].count();
    
    startPoint << QPoint(row-1,column-1);
    prequelSeq.clear();
    prequelSeq << startPoint;

    getDipth(prequelSeq, times);

    putIntoScreen(times);
}

void Align::backTracking2()
{
    int threshold = info->threshold;
    
    prequelSeq.clear();
    int times = 0;
    int max_row = alignBlock.count()-1;
    int max_column = alignBlock[0].count()-1;
    
    while (max_column >0 && max_row > 0)
    {
        for (int current_row=max_row; current_row>0; current_row--)
        {
            if( alignBlock[current_row][max_column]->tracked == false &&
               alignBlock[current_row][max_column]->score >= threshold)
            {
                QVector< QPolygon > tmpPrequalSeq;
                QPolygon newAlign;
                newAlign << QPoint(current_row,max_column);
                tmpPrequalSeq.push_back(newAlign);
                getDipth(tmpPrequalSeq, times);
                times++;
                for (int i=0; i<tmpPrequalSeq.count(); i++)
                    for (int j=0; j<tmpPrequalSeq[i].count(); j++)
                        alignBlock[tmpPrequalSeq[i][j].x()][tmpPrequalSeq[i][j].y()]->tracked =true;
                prequelSeq += tmpPrequalSeq;
            }
        }
        
        for (int current_column=max_column; current_column>0; current_column--)
        {
            if( alignBlock[max_row][current_column]->tracked == false &&
               alignBlock[max_row][current_column]->score >= threshold)
            {
                QVector< QPolygon > tmpPrequalSeq;
                QPolygon newAlign;
                newAlign << QPoint(max_row,current_column);
                tmpPrequalSeq.push_back(newAlign);
                getDipth(tmpPrequalSeq, times);
                times++;
                for (int i=0; i<tmpPrequalSeq.count(); i++)
                    for (int j=0; j<tmpPrequalSeq[i].count(); j++)
                        alignBlock[tmpPrequalSeq[i][j].x()][tmpPrequalSeq[i][j].y()]->tracked =true;
                prequelSeq += tmpPrequalSeq;
            }
        }
        max_row--;
        max_column--;
    }
    
    putIntoScreen(times);
}

void Align::getDipth(QVector< QPolygon > &prequelSeq, int &times)
{
    /*首先拿到最顶上的一个点*/
    int lastItemId = prequelSeq.count() - 1;
    QPoint startPoint = prequelSeq[lastItemId].last();
    bool left = alignBlock[startPoint.x()][startPoint.y()]->left;
    bool topLeft = alignBlock[startPoint.x()][startPoint.y()]->topLeft;
    bool top = alignBlock[startPoint.x()][startPoint.y()]->top;
    
   // int score = alignBlock[startPoint.x()][startPoint.y()]->score;
   // QMessageBox::warning(0, "", QString::number(score));
    
    if (left && topLeft && top) {
        times += 2;
        
        /*重新定义两条路径*/
        QPolygon anotherPath_1, anotherPath_2, anotherPath_3;
        anotherPath_1 = anotherPath_2 = anotherPath_3 = prequelSeq[lastItemId];
        
        /*去除上一条路径，因为已经拷贝过来了*/
        prequelSeq.remove(lastItemId);
        
        /*加入三个点，分别是三条路径*/
        anotherPath_1 << startPoint + QPoint(-1,0);
        anotherPath_2 << startPoint + QPoint(-1,-1);
        anotherPath_3 << startPoint + QPoint(0, -1);
        
        /*把优先级高的路径压入以后，直接进入递归*/
        prequelSeq.push_back(anotherPath_1);
        getDipth(prequelSeq, times);
        
        /*优先级中的路径后压入，然后再递归*/
        prequelSeq.push_back(anotherPath_2);
        getDipth(prequelSeq, times);
        
        /*优先级低的路径后压入，然后再递归*/
        prequelSeq.push_back(anotherPath_3);
        getDipth(prequelSeq, times);
        
        return;
    }
    
    if ( left && topLeft)
    {
        times++;
        
        /*重新定义两条路径*/
        QPolygon anotherPath_1, anotherPath_2;
        anotherPath_1 = anotherPath_2 = prequelSeq[lastItemId];
        
        /*去除上一条路径，因为已经拷贝过来了*/
        prequelSeq.remove(lastItemId);
        
        /*加入两个可行得点*/
        anotherPath_1 << startPoint + QPoint(-1,-1);
        anotherPath_2 << startPoint + QPoint(0, -1);
        
        /*把优先级高的路径压入以后，直接进入递归*/
        prequelSeq.push_back(anotherPath_1);
        getDipth(prequelSeq, times);
       
        /*优先级低的路径后压入，然后再递归*/
        prequelSeq.push_back(anotherPath_2);
        getDipth(prequelSeq, times);
        
        return;
    }
    if (left && top) {
        times++;
        
        QPolygon anotherPath_1, anotherPath_2;
        anotherPath_1 = anotherPath_2 = prequelSeq[lastItemId];
        
        prequelSeq.remove(lastItemId);
        
        anotherPath_1 << startPoint + QPoint(-1,0);
        anotherPath_2 << startPoint + QPoint(0, -1);
        
        prequelSeq.push_back(anotherPath_1);
        getDipth(prequelSeq, times);
        
        prequelSeq.push_back(anotherPath_2);
        getDipth(prequelSeq, times);
        
        return;
    }
    if (top &&topLeft) {
        times++;
        
        QPolygon anotherPath_1, anotherPath_2;
        anotherPath_1 = anotherPath_2 = prequelSeq[lastItemId];
        
        prequelSeq.remove(lastItemId);
        
        anotherPath_1 << startPoint + QPoint(-1,0);
        anotherPath_2 << startPoint + QPoint(-1, -1);
        
        prequelSeq.push_back(anotherPath_1);
        getDipth(prequelSeq, times);
        
        prequelSeq.push_back(anotherPath_2);
        getDipth(prequelSeq, times);
        
        return;
    }
    if( top )
    {
        prequelSeq[lastItemId] << QPoint(startPoint.x()-1,startPoint.y());
        getDipth(prequelSeq, times);
        return;
    }
    if( left )
    {
        prequelSeq[lastItemId] << QPoint(startPoint.x(),startPoint.y()-1);
        getDipth(prequelSeq, times);
        return;
    }
    if( topLeft )
    {
        prequelSeq[lastItemId] << QPoint(startPoint.x()-1,startPoint.y()-1);
        getDipth(prequelSeq, times);
        return;
    }
    if (!top && !left && !topLeft )
    {
        /*第一行，第一个，左边与上面都没有碱基，所以不需要加入*/
        return;
    }
    QMessageBox::warning(0, "", "不应该出现");
}

























