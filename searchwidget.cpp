#include "searchwidget.h"
#include "ui_searchwidget.h"
#include "ui_texteditor.h"

SearchWidget::SearchWidget(TextEditor *editor, QWidget *parent)
    : QDialog(parent), editor(editor)
    , ui(new Ui::SearchWidget)
{
    ui->setupUi(this);

    // Подключаем кнопки
    connect(ui->nextButton, &QPushButton::clicked, this, &SearchWidget::nextSearch);
    connect(ui->prevButton, &QPushButton::clicked, this, &SearchWidget::previousSearch);
}

SearchWidget::~SearchWidget()
{
    delete ui;
}

void SearchWidget::nextSearch()
{
    QTextCursor cursor = editor->ui->EditorField->textCursor(); // Получаем текущий курсор текста из поля редактора
    QString searchText = ui->StringToFind->text(); // Получаем текст для поиска из строки ввода

    // Ищем следующее вхождение текста для поиска, начиная с позиции курсора
    QTextCursor newCursor = editor->ui->EditorField->document()->find(searchText, cursor);

    // Если текст не найден, начинаем поиск с начала документа
    if (newCursor.isNull()) {
        newCursor = editor->ui->EditorField->document()->find(searchText);
    }

    // Если найдено вхождение текста
    if (!newCursor.isNull()) {
        editor->ui->EditorField->setTextCursor(newCursor); // Устанавливаем курсор на найденное вхождение
    }
}

void SearchWidget::previousSearch() {
    QTextCursor cursor = editor->ui->EditorField->textCursor(); // Получаем текущий курсор текста из поля редактора
    QString searchText = ui->StringToFind->text(); // Получаем текст для поиска из строки ввода

    // Ищем предыдущее вхождение текста для поиска, начиная с позиции курсора и двигаясь назад
    QTextCursor newCursor = editor->ui->EditorField->document()->find(searchText, cursor, QTextDocument::FindBackward);

    // Если текст не найден, создаем новый курсор, расположенный в конце документа
    if (newCursor.isNull()) {
        QTextCursor endCursor(editor->ui->EditorField->document());
        endCursor.movePosition(QTextCursor::End);

        // Пытаемся найти текст для поиска, начиная с конца документа и двигаясь назад
        newCursor = editor->ui->EditorField->document()->find(searchText, endCursor, QTextDocument::FindBackward);
    }

    // Если найдено вхождение текста, устанавливаем курсор на найденное вхождение
    if (!newCursor.isNull()) {
        editor->ui->EditorField->setTextCursor(newCursor);
    }
}


