#include "Plotting.h"


void myLabel::drawGrid()
{
    painter->setBrush(QColor(255, 0, 0, 127));
    
    for (int i=1; i< (width()-CellWidth*2) / CellWidth; i++) {
        int x = i * CellWidth + CellWidth;
        painter->drawLine(x, CellHeight, x, height()-2*CellHeight);
    }
    
    for (int i=1; i< (height()-CellHeight*2) / CellHeight; i++) {
        int y = i * CellHeight + CellHeight;
        painter->drawLine(CellWidth, y, width()-2*CellWidth, y );
    }
    
    columns = (width()-CellWidth*2) / CellWidth - 1;
    rows = (height()-CellHeight*2) / CellHeight - 1;
}

QPointF myLabel::CellCenter(int x, int y)
{
   // QMessageBox::warning(0, "", QString::number(rows));
    if(x > columns || y> rows) return QPointF(0,0);
    
    double x_ = CellWidth * x + CellWidth/2;
    double y_ = CellHeight * y + CellHeight/2;
    return QPointF(x_,y_);
}

QPointF myLabel::CellRight(int x, int y)
{
    if(x > columns || y> rows) return QPointF(0,0);
    return CellCenter(x, y) + QPointF(CellWidth/4, 0);}

QPointF myLabel::CellLeft(int x, int y)
{
    if(x > columns || y> rows) return QPointF(0,0);
    return CellCenter(x, y) - QPointF(CellWidth/4, 0);
}

QPointF myLabel::CellBottomRight(int x, int y)
{
    if(x > columns || y> rows) return QPointF(0,0);
    return CellCenter(x, y) + QPointF(CellWidth/4, CellHeight/4);
}

QPointF myLabel::CellTopLeft(int x, int y)
{
    if(x > columns || y> rows) return QPointF(0,0);
    return CellCenter(x, y) - QPointF(CellWidth/4, CellHeight/4);
}

QPointF myLabel::CellBottom(int x, int y)
{
    if(x > columns || y> rows) return QPointF(0,0);
    return CellCenter(x, y) + QPointF(0, CellHeight/4);
}

QPointF myLabel::CellTop(int x, int y)
{
    if(x > columns || y> rows) return QPointF(0,0);
    return CellCenter(x, y) - QPointF(0, CellHeight/4);}

void myLabel::drawSeq()
{
    painter->setPen(QColor(255, 0, 255, 127));
    
    for (int i=0; i< seq1.length(); i++) {
        int x = i * CellWidth + CellWidth*3 - CellWidth/2;
        painter->drawText(x-20, CellHeight-20, 40, 20, Qt::AlignTop | Qt::AlignHCenter, QString(seq1[i]));
    }
    
    for (int i=0; i< seq2.length(); i++) {
        int y = i * CellHeight + CellHeight*3 - CellHeight/2;
        painter->drawText(CellWidth-20, y, 20, 20, Qt::AlignLeft | Qt::AlignVCenter, QString(seq2[i]));
    }
}


myScrollArea::myScrollArea(QWidget *parent):QScrollArea(parent)
{
    setMinimumSize(QSize(450,300));
    setMaximumSize(QSize(450,300));
    setBackgroundRole(QPalette::Dark);
    
    next_ToolButton = new QToolButton(this);
    next_ToolButton->setIcon(QIcon(":/images/next.png"));
    next_ToolButton->adjustSize();
    connect(next_ToolButton, SIGNAL(clicked()), this, SLOT(next()));
    next_ToolButton->setEnabled(false);
    last_ToolButton = new QToolButton(this);
    last_ToolButton->setIcon(QIcon(":/images/last.png"));
    last_ToolButton->adjustSize();
    connect(last_ToolButton, SIGNAL(clicked()), this, SLOT(last()));
    last_ToolButton->move(width()-100, 50);
    next_ToolButton->move(width()-last_ToolButton->width()-25, 50);
    last_ToolButton->setEnabled(false);
    
    quality_slider = new QSlider(this);
    quality_slider->setRange(0, 100);
    
    quality_slider->setOrientation(Qt::Horizontal);
    quality_slider->setSliderPosition(70);
    quality_slider->setFixedWidth(100);
    connect(quality_slider, SIGNAL(sliderMoved(int)), this, SLOT(changeQuality(int)));
    
    quality_label = new QLabel("Picture Quality: ");
    quality_value_label = new QLabel("70");
    
    labelStack.append(new myLabel("ATCG","ATCGG",this));
    struct AliInfo *info = new AliInfo;
    info->algorithmID = 1;
    info->scoreStrategy = 1;
    info->d_value = 12;
    info->e_value = 0;
    info->seq1 = "ATCG";
    info->seq2 = "ATCGG";
    labelStack[0]->alignInfo = info;
    
    curLabel = 0;
    
    saveAsPicture = new QAction(tr("&Save"),this);
    saveAsPicture->setStatusTip(tr("Save it as a picture"));
    saveAsPicture->setShortcut(tr("Ctrl+S"));
    saveAsPicture->setIcon(QIcon(":/images/save.png"));
    connect(saveAsPicture, SIGNAL(triggered()), this, SLOT(saveAs()));
    
    deleteThisAlignment = new QAction(tr("&Delete"),this);
    deleteThisAlignment->setStatusTip(tr("Delete This Alinment"));
    deleteThisAlignment->setShortcut(tr("Ctrl+D"));
    deleteThisAlignment->setIcon(QIcon(":/images/delete.png"));
    connect(deleteThisAlignment, SIGNAL(triggered()), this, SLOT(deleteIt()));
    
    nextAction = new QAction(tr("&Next"),this);
    nextAction->setStatusTip(tr("Step To Next Alignment"));
    nextAction->setShortcut(tr("Ctrl+N"));
    nextAction->setIcon(QIcon(":/images/next.png"));
    connect(nextAction, SIGNAL(triggered()), this, SLOT(next()));

    lastAction = new QAction(tr("&Last"),this);
    lastAction->setStatusTip(tr("Step To Last Alignment"));
    lastAction->setShortcut(tr("Ctrl+L"));
    lastAction->setIcon(QIcon(":/images/last.png"));

    connect(lastAction, SIGNAL(triggered()), this, SLOT(last()));
    
    lastAction->setEnabled(false);
    nextAction->setEnabled(false);
    
    setWidget();
    drawSeq();
    
    contextMenu = 0;
    
    comboBox = new QComboBox(this);
    comboBox->addItem(QString::number(0));
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboAction(int)));
}

void myScrollArea::changeQuality(int quality)
{
    quality_value_label->setText(QString::number(quality));
}

void myScrollArea::comboAction(int id)
{
    setWidget(id);
}

void myScrollArea::setWidget()
{
    if(curLabel >= 0)
    {
        QScrollArea::setWidget(labelStack[curLabel]);
    }
    if( curLabel > 0 ) { last_ToolButton->setEnabled(true); lastAction->setEnabled(true);}
    else { last_ToolButton->setEnabled(false); lastAction->setEnabled(false); }
    
    if( curLabel < labelStack.count()-1 ) { next_ToolButton->setEnabled(true); nextAction->setEnabled(true);}
    else { next_ToolButton->setEnabled(false); nextAction->setEnabled(false); }
}

void myScrollArea::setWidget(int id)
{
    if (id < labelStack.count()) {
        labelStack[curLabel] = new myLabel(*labelStack[curLabel]);
        curLabel = id;
        setWidget();
        emit currentLabelChanged();
    }
}

void myScrollArea::clearLabel(int ids)
{
    if (labelStack.count() > 1)
    {
        labelStack[ids]->~myLabel();
        labelStack.remove(ids);
        curLabel = 0;
        setWidget();
        comboBox->removeItem(comboBox->count()-1);
        emit currentLabelChanged();
    }
}

void myScrollArea::contextMenuEvent(QContextMenuEvent * event)
{
    if (!contextMenu)
    {
        contextMenu = new QMenu;
        contextMenu->addAction(deleteThisAlignment);
        contextMenu->addAction(saveAsPicture);
        contextMenu->addAction(lastAction);
        contextMenu->addAction(nextAction);
    }
    contextMenu->popup(event->globalPos());
}

void myScrollArea::addLabel(QString seq1, QString seq2)
{
    labelStack[curLabel] = new myLabel(*labelStack[curLabel]);
    labelStack.append(new myLabel(seq1, seq2 ,this));
    curLabel=labelStack.count()-1;
    setWidget();
    drawSeq();
    comboBox->addItem(QString::number(comboBox->count()));
    
    disconnect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboAction(int)));
    comboBox->setCurrentIndex(comboBox->count() - 1);
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboAction(int)));
}

void myScrollArea::drawArrow(int x1,int y1,int x2, int y2)
{
    int tmp = x1; x1 = y1; y1 = tmp;
    tmp = x2; x2 = y2; y2 = tmp;
    
    if (x1 > labelStack[curLabel]->seq1.length()+1 || y1 > labelStack[curLabel]->seq2.length()+1) return;
    if (x2 > labelStack[curLabel]->seq1.length()+1 || y2 > labelStack[curLabel]->seq2.length()+1) return;
    
    QPointF left_1 = labelStack[curLabel]->CellLeft(x1, y1);
    QPointF topLeft_1 = labelStack[curLabel]->CellTopLeft(x1, y1);
    QPointF top_1 = labelStack[curLabel]->CellTop(x1, y1);
    
    QPointF right_2 = labelStack[curLabel]->CellRight(x2, y2);
    QPointF bottom_2 = labelStack[curLabel]->CellBottom(x2, y2);
    QPointF bottomRight_2 = labelStack[curLabel]->CellBottomRight(x2, y2);
    
    labelStack[curLabel]->painter->setPen(QColor(200,0,200));
    
    if( x1 > x2 )
    {
        if(y1 == y2) labelStack[curLabel]->painter->drawLine(left_1,right_2);
        if(y1 > y2 ) labelStack[curLabel]->painter->drawLine(topLeft_1,bottomRight_2);
    }
    if( x1 == x2 && y1 > y2) labelStack[curLabel]->painter->drawLine(top_1,bottom_2);
}

void myScrollArea::drawText(int x, int y, double score)
{
    int tmp = x; x = y; y = tmp; /*行列交换*/
    /*x表示横向偏移量，y表示纵向偏移*/
    /*seq1表示上面的横向序列，seq2是纵向序列*/
    if (x > labelStack[curLabel]->seq1.length()+1 || y > labelStack[curLabel]->seq2.length()+1)
    {
        return;
    }
    labelStack[curLabel]->painter->setPen(QColor(0, 0, 255));
    QPointF cell = labelStack[curLabel]->CellCenter(x, y);
    cell = cell - QPointF(5,-5);
    labelStack[curLabel]->painter->drawText(cell, QString::number(score));
}

void myScrollArea::drawSeq()
{
        labelStack[curLabel]->drawSeq();
}


void myScrollArea::next()
{
    labelStack[curLabel] = new myLabel(*labelStack[curLabel]);
    curLabel++;
    setWidget();
    
    if(curLabel != 0) { last_ToolButton->setEnabled(true); lastAction->setEnabled(true);}
    if (curLabel == labelStack.count() - 1)
    { next_ToolButton->setEnabled(false); nextAction->setEnabled(false);}
    emit currentLabelChanged();

    disconnect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboAction(int)));
    comboBox->setCurrentIndex(curLabel);
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboAction(int)));
}
void myScrollArea::last()
{
    labelStack[curLabel] = new myLabel(*labelStack[curLabel]);
    curLabel--;
    setWidget();
    
    next_ToolButton->setEnabled(true);
    nextAction->setEnabled(true);
    if (curLabel == 0)
    {last_ToolButton->setEnabled(false); lastAction->setEnabled(false);}
    emit currentLabelChanged();
    
    disconnect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboAction(int)));
    comboBox->setCurrentIndex(curLabel);
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboAction(int)));
}

void myScrollArea::deleteIt()
{
    clearLabel(curLabel);
}

void myScrollArea::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Please Give A File Name"),tr("."),tr("PNG (*.png);; XPM (*.xpm);; JPG (*.jpg);; GIF (*.gif)"));
    int quality = quality_slider->value();
    labelStack[curLabel]->pixmap.save(fileName,0,quality);
}

void myScrollArea::setAliInfo(struct AliInfo *info)
{
    labelStack[curLabel]->alignInfo = info;
}

struct AliInfo * myScrollArea::getAliInfo()
{
    return labelStack[curLabel]->alignInfo;
}


void myScrollArea::drawPixmap(QVector< QVector<alignCell *> > block)
{
    for (int row=0; row<block.count(); row++) {
        
        QVector<alignCell *> aRow = block[row];
        for (int column=0; column<aRow.count(); column++)
        {
            drawText(row+1, column+1, aRow[column]->score);
            if (aRow[column]->top) drawArrow(row+1,column+1,row, column+1);
            if (aRow[column]->topLeft) drawArrow(row+1,column+1,row, column);
            if (aRow[column]->left) drawArrow(row+1,column+1,row+1, column);
        }
    }
}





