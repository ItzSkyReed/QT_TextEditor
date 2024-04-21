#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

QT_BEGIN_NAMESPACE
namespace Ui {
class TextEditor;
}
QT_END_NAMESPACE

class TextEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit TextEditor(QWidget *parent = nullptr);
    ~TextEditor();
    Ui::TextEditor *ui;

private:
    QMenu *contextMenu;
    QString latestFilename = nullptr;

    void setupContextMenu();
    void setupActions();
    QString generateUniqueFilename(); // Функция для генерации уникального имени файла

private slots:
    void copyText();
    void pasteText();
    void cutText();
    void deleteText();
    void insertDateText();

    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void printFile();
    void exit();

    void editFont();

    void infoMessage();

    void searchString();
    void gotoLine();
    void replaceString();

    void showContextMenu(const QPoint &pos);
};

#endif // TEXTEDITOR_H
