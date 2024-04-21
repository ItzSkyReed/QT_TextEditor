#include "replacewidget.h"
#include "ui_replacewidget.h"
#include "ui_texteditor.h"
#include "texteditor.h"
#include <QMessageBox>
ReplaceWidget::ReplaceWidget(TextEditor *editor, QWidget *parent)
    : QDialog(parent), editor(editor)
    , ui(new Ui::ReplaceWidget)
{
    ui->setupUi(this);

    // Подключаем кнопки
    connect(ui->replaceFirstButton, &QPushButton::clicked, this, &ReplaceWidget::replaceFirstPhrase);
    connect(ui->replaceAllButton, &QPushButton::clicked, this, &ReplaceWidget::replaceAllPhrases);

}

ReplaceWidget::~ReplaceWidget()
{
    delete ui;
}


void ReplaceWidget::replaceAllPhrases() {
    // Получаем текст для замены и текст замены из полей ввода интерфейса
    QString fromText = ui->FromLineText->text();
    QString toText = ui->ToLineText->text();

    // Проверяем, что поля ввода не пустые
    if (fromText.isEmpty() or toText.isEmpty()) {
        // Если хотя бы одно из полей пустое, выводим предупреждение и выходим из функции
        QMessageBox::warning(this, "Предупреждение", "Замена или поиск на пустые строки не возможен!");
        return;
    }

    QTextDocument *doc = editor->ui->EditorField->document(); // Получаем текст текстового редактора
    QTextCursor cursor(doc); // Создаем курсор для перемещения по тексту

    // Пока курсор не нулевой и не достиг конца документа
    while (!cursor.isNull() && !cursor.atEnd()) {
        cursor = doc->find(fromText, cursor); // Ищем текст для замены

        // Если найдено совпадение
        if (!cursor.isNull()) {
            // Перемещаем курсор к концу найденного текста
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, fromText.length() - 1);
            // Заменяем найденный текст на текст замены
            cursor.insertText(toText);
            // Перемещаем курсор к следующему месту, где может быть найдено совпадение
            cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, toText.length() - fromText.length());
        }
    }
}

void ReplaceWidget::replaceFirstPhrase() {
    // Получаем текст для замены и текст, на который будем заменять
    QString fromText = ui->FromLineText->text();
    QString toText = ui->ToLineText->text();

    // Проверяем, что поля ввода не пустые
    if(fromText.isEmpty() || toText.isEmpty()) {
        // Если хотя бы одно из полей пустое, выводим предупреждение и выходим из функции
        QMessageBox::warning(this, "Warning", "Replacement or search cannot be performed on empty strings!");
        return;
    }

    QTextDocument *doc = editor->ui->EditorField->document(); // Получаем текстовый документ из редактора
    QTextCursor cursor(doc); // Создаем курсор для поиска
    cursor = doc->find(fromText); // Ищем первое вхождение текста для замены

    // Если найдено совпадение
    if (!cursor.isNull()) {
        // Перемещаем курсор в конец найденной фразы
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, fromText.length() - 1);
        // Заменяем найденный текст на новый
        cursor.insertText(toText);
        // Перемещаем курсор вправо на разницу длин между новым и старым текстом
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, toText.length() - fromText.length());
    }
}
