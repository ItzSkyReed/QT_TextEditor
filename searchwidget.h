#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QDialog>
#include "texteditor.h"

namespace Ui {
class SearchWidget;
}

class SearchWidget : public QDialog
{
    Q_OBJECT

public:
    explicit SearchWidget(TextEditor *editor, QWidget *parent = nullptr);
    ~SearchWidget();

private:
    Ui::SearchWidget *ui;
    TextEditor *editor;
private slots:
    void nextSearch();
    void previousSearch();
};

#endif // SEARCHWIDGET_H
