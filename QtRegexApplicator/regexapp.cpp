#include "regexapp.h"
#include "ui_regexapp.h"
#include "QScrollBar"
#include <QDebug>
#include <QTextBlock>
#include <QRegExp>
#include <QFileDialog>

regexapp::regexapp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::regexapp)
{
    ui->setupUi(this);

    connect(ui->textOriginal, SIGNAL(textChanged()), this, SLOT(handleOriginalTextChange()));
    connect(ui->textOriginal->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(handleOriginalScrollChange(int)));
    connect(ui->textRegexed->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(handleRegexedScrollChange(int)));

    connect(ui->buttonAddRegex, SIGNAL(clicked()), this, SLOT(handleButtonAddRegexClicked()));
    connect(ui->buttonRemoveRegex, SIGNAL(clicked()), this, SLOT(handleButtonRemoveRegexClicked()));
    connect(ui->editRegexModify, SIGNAL(textChanged(QString)), this, SLOT(handleEditRegexModifyChanged()));
    connect(ui->comboBoxRegex, SIGNAL(currentIndexChanged(int)), this, SLOT(handleComboBoxRegexChanged(int)));

    connect(ui->buttonOpen, SIGNAL(clicked()), this, SLOT(handleButtonOpen()));
}

regexapp::~regexapp()
{
    delete ui;
}

void regexapp::handleOriginalTextChange()
{
    ui->textRegexed->clear();
    ui->textRegexed->append(applyRegex(ui->textOriginal->document()));
}

void regexapp::handleOriginalScrollChange(int value)
{
    ui->textRegexed->verticalScrollBar()->setSliderPosition(value);
}

void regexapp::handleRegexedScrollChange(int value)
{
    ui->textOriginal->verticalScrollBar()->setSliderPosition(value);
}

QString regexapp::applyRegex(QTextDocument *document)
{
    QList<QRegExp *> regexList;

    for(int i =0; i < ui->comboBoxRegex->count(); i++)
    {
        regexList.append(new QRegExp(ui->comboBoxRegex->itemText(i)));
    }
    QString out;
    out.append("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">");
    out.append("<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">");
    out.append("p, li { white-space: pre-wrap; }");
    out.append("</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">");
    for(int i=0; i < document->blockCount() ; i++)
    {
        bool regexmatch = false;
        foreach (QRegExp *regex,regexList)
        {
            if(regex->exactMatch(document->findBlockByNumber(i).text()))
            {
                regexmatch = true;
                //If the regex capture text we append the captured text
                if(regex->capturedTexts().count() == 2)
                   out.append(QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:orange;\">%1</span><span style=\" color:green;\">%2</span></p>").arg(regex->capturedTexts().first().remove(regex->capturedTexts().last()).toHtmlEscaped()).arg(regex->capturedTexts().last().toHtmlEscaped()));
                else
                   out.append(QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:orange;\">%1</p>").arg(regex->capturedTexts().last().toHtmlEscaped()));
                break;
            }
        }
        if(!regexmatch)
        {
               out.append(QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:red; \">%1</p>").arg(document->findBlockByNumber(i).text().toHtmlEscaped()));
        }
    out.append("</body></html>");
    }
    qDeleteAll(regexList);
    return out;
}

void regexapp::handleButtonAddRegexClicked()
{
    if(ui->editRegexAdder->text().length() == 0) return;
    ui->comboBoxRegex->addItem(ui->editRegexAdder->text());
    ui->editRegexAdder->clear();
    handleOriginalTextChange();
}

void regexapp::handleButtonRemoveRegexClicked()
{
    ui->comboBoxRegex->removeItem(ui->comboBoxRegex->currentIndex());
    handleOriginalTextChange();
}

void regexapp::handleComboBoxRegexChanged(int)
{
    ui->editRegexModify->setText(ui->comboBoxRegex->currentText());
    handleOriginalTextChange();
}

void regexapp::handleEditRegexModifyChanged()
{
    disconnect(ui->comboBoxRegex, SIGNAL(currentIndexChanged(int)), this, SLOT(handleComboBoxRegexChanged(int)));

    int current = ui->comboBoxRegex->currentIndex();

    ui->comboBoxRegex->insertItem(current, ui->editRegexModify->text());
    ui->comboBoxRegex->removeItem(current+1);

    connect(ui->comboBoxRegex, SIGNAL(currentIndexChanged(int)), this, SLOT(handleComboBoxRegexChanged(int)));
    handleOriginalTextChange();
}

void regexapp::handleButtonOpen()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open file");

    QFile *file = new QFile(filename);
    file->open(QIODevice::ReadOnly);

    ui->textOriginal->clear();
    ui->textOriginal->append(QString(file->readAll()));

    file->close();


    delete file;
}
