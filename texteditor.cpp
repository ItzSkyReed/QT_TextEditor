#include "texteditor.h"
#include "./ui_texteditor.h"
#include <QClipboard>
#include <QDateTime>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QFontDialog>
#include <QInputDialog>


// Разработать меню. Пункты меню:
// 1) Файл (Новый, Открыть, Сохранить как, Сохранить, Печать, Выход);
// 2) Правка (Отменить, Копировать, Вставить, Повторить, Вырезать, Удалить, Вставить дату);
// 3) Вид (Шрифт, Выравнивание (Слева, По центру, Справа));
// 4) Найти (Поиск, Перейти к строке, Заменить);
// 5) Справка (О программе)
// Дополнительные баллы будут начислены за возможность переключения с
// кириллицы на латинский алфавит, или любую другую полезную возможность

TextEditor::TextEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextEditor) {

    { // Устанавливаем интерфейс
        ui->setupUi(this);
    }

    setupActions(); // Вызов функции, которая устанавливает связь всех действий
    setupContextMenu(); // Вызов функции, которая настраивает контекстное меню
}


TextEditor::~TextEditor() {
    delete ui;
}

// Создание собственного контекстного меню при нажатии ПКМ
void TextEditor::setupContextMenu() {

    contextMenu = new QMenu(this); // Создание контекстного меню

    ui->EditorField->setContextMenuPolicy(Qt::CustomContextMenu); // Установка политики контекстного меню на пользовательское

    // Добавление действий в контекстное меню
    contextMenu->addAction(ui->copySelectedAction);
    contextMenu->addAction(ui->pasteClipboardAction);
    contextMenu->addSeparator(); // Добавляем разделитель
    contextMenu->addAction(ui->cancelAction);
    contextMenu->addAction(ui->repeatAction);
    contextMenu->addSeparator(); // Добавляем разделитель
    contextMenu->addAction(ui->cutSelectedAction);
    contextMenu->addAction(ui->deleteSelectedAction);

    // Подключение сигнала запроса контекстного меню к слоту
    connect(ui->EditorField, &QTextEdit::customContextMenuRequested, this, &TextEditor::showContextMenu);
}
// Функция для отображения контекстного меню
void TextEditor::showContextMenu(const QPoint &pos) {
    contextMenu->exec(ui->EditorField->mapToGlobal(pos));
}

void TextEditor::setupActions() {

    ui->EditorField->setUndoRedoEnabled(true); // Включаем функции undo/redo для текстового редактора

    // Вкладка вставки
    // Устанавливаем соединение между действием отмены (функцией undo()) и функцией отмены в редакторе
    // Когда пользователь активирует cancelAction, будет отправлен сигнал triggered() и т.д
    connect(ui->cancelAction, &QAction::triggered, ui->EditorField, &QTextEdit::undo);
    connect(ui->repeatAction, &QAction::triggered, ui->EditorField, &QTextEdit::redo);
    connect(ui->copySelectedAction, &QAction::triggered, this, &TextEditor::copyText);
    connect(ui->pasteClipboardAction, &QAction::triggered, this, &TextEditor::pasteText);
    connect(ui->cutSelectedAction, &QAction::triggered, this, &TextEditor::cutText);
    connect(ui->deleteSelectedAction, &QAction::triggered, this, &TextEditor::deleteText);
    connect(ui->insertDateAction, &QAction::triggered, this, &TextEditor::insertDateText);
    // Вкладка файла
    connect(ui->newFileAction, &QAction::triggered, this, &TextEditor::newFile);
    connect(ui->openFileAction, &QAction::triggered, this, &TextEditor::openFile);
    connect(ui->saveFileAction, &QAction::triggered, this, &TextEditor::saveFile);
    connect(ui->saveFileAsAction, &QAction::triggered, this, &TextEditor::saveFileAs);
    connect(ui->printFileAction, &QAction::triggered, this, &TextEditor::printFile);
    connect(ui->exitProgramAction, &QAction::triggered, this, &TextEditor::exit);
    // Вкладка вида
    connect(ui->editFontAction, &QAction::triggered, this, &TextEditor::editFont);
    // Вкладка "о программе"
    connect(ui->infoAction, &QAction::triggered, this, &TextEditor::infoMessage);

    // Вкладка "Найти"
    connect(ui->findStringAction, &QAction::triggered, this, &TextEditor::searchString);
    connect(ui->gotoLineAction, &QAction::triggered, this, &TextEditor::gotoLine);
    connect(ui->replaceStringAction, &QAction::triggered, this, &TextEditor::replaceString);

    ui->cancelAction->setShortcut(QKeyCombination(Qt::CTRL | Qt::Key_Z)); // Горячая клавиша для отмены (Ctrl + Z)
    ui->repeatAction->setShortcut(QKeyCombination(Qt::CTRL | Qt::Key_Y)); // Горячая клавиша для повтора (Ctrl + Y)
    ui->copySelectedAction->setShortcut(QKeyCombination(Qt::CTRL | Qt::Key_C)); // Горячая клавиша для копирования (Ctrl + C)
    ui->pasteClipboardAction->setShortcut(QKeyCombination(Qt::CTRL | Qt::Key_V)); // Горячая клавиша для вставки (Ctrl + V)
    ui->cutSelectedAction->setShortcut(QKeyCombination(Qt::CTRL | Qt::Key_X)); // Горячая клавиша для вырезания (Ctrl + X)
    ui->deleteSelectedAction->setShortcut(QKeyCombination(Qt::Key_Delete)); // Горячая клавиша для вырезания (Del)

    ui->newFileAction->setShortcut(QKeyCombination(Qt::CTRL | Qt::Key_N)); // Горячая клавиша для создания чистого окна (CTRL + N)
    ui->openFileAction->setShortcut(QKeyCombination(Qt::CTRL | Qt::Key_O)); // Горячая клавиша для открытия нового файла (CTRL + N)
    ui->saveFileAction->setShortcut(QKeyCombination(Qt::CTRL | Qt::Key_S)); // Горячая клавиша для сохранения файла без диалогового окна
    ui->saveFileAsAction->setShortcut(QKeyCombination(Qt::CTRL | Qt::SHIFT | Qt::Key_S)); // Горячая клавиша для сохранения файла с диалоговым окном
    ui->printFileAction->setShortcut(QKeyCombination(Qt::CTRL | Qt::Key_P)); // Горячая клавиша для вызова окна печати
    ui->exitProgramAction->setShortcut(QKeyCombination(Qt::ALT | Qt::Key_F4)); // Горячая клавиша для закрытия приложения (на самом деле она ничего не делает и нужна только визуально)

    ui->infoAction->setShortcut(QKeyCombination(Qt::Key_F1)); // Горячая клавиша для справки о программе

    ui->findStringAction->setShortcut(QKeyCombination(Qt::CTRL | Qt::Key_F)); // Горячая клавиша для вызова окна поиска
    ui->gotoLineAction->setShortcut(QKeyCombination(Qt::CTRL | Qt::Key_G)); // Горячая клавиша для вызова окна перехода к строке
    ui->replaceStringAction->setShortcut(QKeyCombination(Qt::CTRL | Qt::Key_H)); // Горячая клавиша для вызова окна замены
}
void TextEditor::copyText() {
    QString selectedText = ui->EditorField->textCursor().selectedText(); // Получаем выделенный текст с окна редактора
    QApplication::clipboard()->setText(selectedText); // Устанавливаем выделенный текст в буфер обмена системы
}

void TextEditor::pasteText() {
    QString clipboardText = QApplication::clipboard()->text();  // Получаем буфер обмена системы
    ui->EditorField->textCursor().insertText(clipboardText); // Вставляем полученный текст из буфера обмена после курсора
}

void TextEditor::cutText() {
    QString selectedText = ui->EditorField->textCursor().selectedText(); // Получаем выделенный текст с окна редактора
    ui->EditorField->textCursor().removeSelectedText(); // Удаляем выделенный текст
    QApplication::clipboard()->setText(selectedText); // Добавляем выделенный текст в буфер обмена
}

void TextEditor::deleteText() {
    ui->EditorField->textCursor().removeSelectedText(); // Удаляем выделенный текст
}

void TextEditor::insertDateText() {
    QString CurrentDateAsText = QDateTime::currentDateTime().toString("dd.MM.yy"); // Получаем сегодняшнюю дату в формате "День.месяц.год"
    ui->EditorField->textCursor().insertText(CurrentDateAsText); // Вставляем отформатированную дату после курсора
}

// Функция создания нового файла
void TextEditor::newFile() {
    latestFilename = nullptr; // Очищаем последний файл
    ui->EditorField->clear(); // Очищаем текстовое поле
}

// Функция открытия файла
void TextEditor::openFile() {
    // Открываем диалоговое окно для выбора файла, пользователь выбирает файл. Фиксируем дополнительно название файла
    // Объект, заголовок сверху, стандартное название файла, возможные опции выбора файла
    QString filename = QFileDialog::getOpenFileName(this, tr("Открыть Файл"), "", tr("Текстовые файлы (*.txt);;Все файлы (*)"));

    if (!filename.isEmpty()) { // Если файл не пустой, открываем его
        QFile file(filename);
        // Если файл невозможно открыть для чтения выводим отформатированное сообщение
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Ошибка"), tr("Невозможно прочитать файл %1:\n%2.").arg(filename, file.errorString()));
            latestFilename = nullptr; // Устанавливаем имя последнего открыго файла на "ничего"
            return;
        }
        QTextStream in(&file); // Прочитываем содержимое файла
        ui->EditorField->setText(in.readAll()); // Устанавливаем его в поле редактора
        latestFilename = filename; // Устанавливаем имя последнего открыго файла
    }
}

// Функция для "Сохранить"
void TextEditor::saveFile() {
    QString content = ui->EditorField->toPlainText(); // Получите содержимое из TextEditor
    QString filename; // Путь файла

    if (latestFilename != nullptr) { // Проверяем был ли ранее загружен какой-либо файл.
        filename = latestFilename; // Устанавливаем путь до файла который был открыт через "Открыть файл"
    }
    else {
        filename = generateUniqueFilename(); // Генерируем уникальное имя файла если никакой файл до этого не был открыт
    }

    // Создаем файл с уникальным именем
    QFile file(filename);
    // Если файл невозможно открыть для чтения, то выводим ошибку
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Невозможно создать файл %1:\n%2.").arg(filename, file.errorString()));
        return;
    }

    // Записываем содержимое в файл
    QTextStream out(&file);
    out << content;

    file.close(); // Закрываем файл
    latestFilename = filename; // Устанавливаем путь до последнего открытого файла.
}

// Функция для генерации уникального названия файла
QString TextEditor::generateUniqueFilename() {
    QString baseName = "text_file"; // Базовое название файла
    QString path = QDir::currentPath(); // Получаем текущую рабочую директорию
    int counter = 1;
    QString filename;

    // Генерируем название файлов увеличивая счётчик на +1 пока такие файлы существуют
    do {
        filename = QString("%1/%2%3.txt").arg(path, baseName, QString::number(counter));
        counter++;
    } while (QFile::exists(filename));

    return filename;
}

// Функция для "Сохранить как"
void TextEditor::saveFileAs() {
    // Получаем содержимое из TextEditor
    QString content = ui->EditorField->toPlainText();

    // Открываем диалоговое окно для выбора места для сохранения файла
    QString filename = QFileDialog::getSaveFileName(this, tr("Сохранить файл как"), "", tr("Текстовые файлы (*.txt);;Все файлы (*)"));

    if (!filename.isEmpty()) { // Если файл не пустой, создаем его
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) { // Если файл недоступен для записи, то вызываем окно с ошибкой
            QMessageBox::warning(this, tr("Ошибка"), tr("Невозможно создать файл %1:\n%2.").arg(filename, file.errorString()));
            return;
        }

        // Записываем содержимое в файл
        QTextStream out(&file);
        out << content;

        file.close();  // Закрываем файл
        latestFilename = filename;  // Устанавливаем путь до последнего открытого файла.
    }
}

// Функция для вызова принтера
void TextEditor::printFile() {
    QPrinter printer(QPrinter::HighResolution); // Создание объекта принтера с высоким разрешением

    // Создание диалогового окна для печати и передача объекта принтера в качестве параметра конструктору диалога.
    QPrintDialog printDialog(&printer, this);

    // Отображение диалогового окна для печати и ожидание результата
    if (printDialog.exec() == QDialog::Accepted) {
        // Если пользователь нажал "ОК" в диалоговом окне печати,
        // то выполняется печать содержимого редактора (EditorField) на принтере
        ui->EditorField->print(&printer);
    }
}

// Функция для закрытия приложения
void TextEditor::exit() {
    QApplication::quit(); // закрываем Главное окно
}


// Редактор шрифта
void TextEditor::editFont() {
    bool ok; // Устанавливаем флаг, что всё выбрано правильно
    QFont font = QFontDialog::getFont(&ok, ui->EditorField->currentFont(), this);
    if (ok) { // Если флаг true, то устанавливаем шрифт
        ui->EditorField->setCurrentFont(font);
    }
}

// Справка
void TextEditor::infoMessage() {
    QMessageBox::information(this, tr("Текстовый редактор"), tr("Данный текстовый редактор позволяет что-то делать..."));
}

// Открытие и создание диалога поиска
void TextEditor::searchString() {
    bool ok; // Переменная для проверки успешности открытия диалогового окна

    // Запрашиваем у пользователя строку для поиска с помощью диалогового окна
    QString searchString = QInputDialog::getText(this, tr("Найти строку"), tr("Введите строку для поиска:"), QLineEdit::Normal, "", &ok);

    // Проверяем, была ли нажата кнопка "ОК" и не пуста ли введенная строка
    if (ok && !searchString.isEmpty()) {
        // Ищем первое вхождение строки поиска

        // Получаем доступ к документу редактора и создаем курсор для поиска
        QTextDocument *document = ui->EditorField->document();
        QTextCursor cursor(document);
        cursor.beginEditBlock();

        // Поиск первого вхождения строки поиска, начиная с текущей позиции курсора
        QTextCursor findCursor = document->find(searchString, ui->EditorField->textCursor().position());

        // Проверяем, была ли найдена строка
        if (!findCursor.isNull()) {
            // Если строка найдена, перемещаем курсор к найденной позиции
            ui->EditorField->setTextCursor(findCursor);
        } else {
            // Если строка не найдена, отображаем информационное сообщение
            QMessageBox::information(this, tr("Поиск"), tr("Строка не найдена."));
        }
        // Завершаем блок редактирования
        cursor.endEditBlock();
    }
}

void TextEditor::gotoLine() {
    // Отображаем диалоговое окно для ввода номера строки
    bool ok;
    // Ссылка на объект, Заголовок, Текст в диалоге, начальное значение, минимальное значение, максимальное значение, шаг, переменная для изменения
    int lineNumber = QInputDialog::getInt(this, tr("Перейти к строке"), tr("Введите номер строки, к которой хотите перейти:"),
                                          1, 1, ui->EditorField->document()->lineCount(), 1, &ok);

    // Если пользователь подтвердил ввод
    if (ok) {
        QTextEdit *editorField = ui->EditorField; // Получаем указатель на редактор текста
        QTextCursor cursor = editorField->textCursor(); // Получаем текущий курсор текста
        cursor.movePosition(QTextCursor::Start); // Перемещаем курсор в начало текста
        cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, lineNumber - 1); // Перемещаем курсор к указанной строке
        editorField->setTextCursor(cursor); // Устанавливаем курсор в редакторе текста
    }
}
void TextEditor::replaceString() {
    // Отображаем диалоговое окно для ввода строки поиска и замены
    bool ok;
    QString searchString = QInputDialog::getText(this, tr("Заменить строку"), tr("Введите строку которая будет заменена:"), QLineEdit::Normal, "", &ok);

    // Проверяем, была ли нажата кнопка "ОК" и не пуста ли введенная строка замены
    if (ok && !searchString.isEmpty()) {
        // Запрашиваем строку замены
        QString replaceString = QInputDialog::getText(this, tr("Заменить строку"), tr("Введите строку на которую будет проведена замена:"), QLineEdit::Normal, "", &ok);

        // Проверяем, была ли нажата кнопка "ОК" и не пуста ли введенная строка замены
        if (ok && !replaceString.isEmpty()) {
            // Ищем первое вхождение строки поиска

            // Получаем доступ к документу редактора и создаем курсор для поиска
            QTextDocument *document = ui->EditorField->document();
            QTextCursor cursor(document);
            cursor.beginEditBlock();

            // Поиск первого вхождения строки поиска, начиная с текущей позиции курсора
            QTextCursor findCursor = document->find(searchString, ui->EditorField->textCursor().position());

            // Проверяем, была ли найдена строка
            if (!findCursor.isNull()) {
                // Если строка найдена, заменяем ее
                findCursor.insertText(replaceString);
            } else {
                // Если строка не найдена, отображаем информационное сообщение
                QMessageBox::information(this, tr("Поиск и замена"), tr("Строка не найдена."));
            }
            // Завершаем блок редактирования
            cursor.endEditBlock();
        }
    }
}
