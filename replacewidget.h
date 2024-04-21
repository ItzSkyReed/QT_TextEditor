#ifndef REPLACEWIDGET_H
#define REPLACEWIDGET_H

#include <QDialog>
#include "texteditor.h"

namespace Ui {
class ReplaceWidget;
}

class ReplaceWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceWidget(TextEditor *editor, QWidget *parent = nullptr);
    ~ReplaceWidget();

private:
    Ui::ReplaceWidget *ui;
    TextEditor *editor;
private slots:
    void replaceAllPhrases();
    void replaceFirstPhrase();
};

#endif // REPLACEWIDGET_H
