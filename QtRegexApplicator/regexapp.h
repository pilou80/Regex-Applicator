#ifndef REGEXAPP_H
#define REGEXAPP_H

#include <QWidget>
#include <QTextDocument>

namespace Ui {
class regexapp;
}

class regexapp : public QWidget
{
    Q_OBJECT
    
public:
    explicit regexapp(QWidget *parent = 0);
    ~regexapp();
    
private:
    Ui::regexapp *ui;
    QString applyRegex(QTextDocument *document);

private slots:
    void handleOriginalTextChange();
    void handleOriginalScrollChange(int value);
    void handleRegexedScrollChange(int value);

    void handleButtonAddRegexClicked();
    void handleButtonRemoveRegexClicked();
    void handleComboBoxRegexChanged(int);
    void handleEditRegexModifyChanged();

    void handleButtonOpen();

};

#endif // REGEXAPP_H
