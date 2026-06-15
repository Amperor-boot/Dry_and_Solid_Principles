#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <memory>

// --- Интерфейсы (ISP) ---
// Разделяем один большой интерфейс на несколько маленьких и специализированных.

class TextPrintable {
public:
    virtual ~TextPrintable() = default;
    virtual std::string printAsText() const = 0;
};

class HTMLPrintable {
public:
    virtual ~HTMLPrintable() = default;
    virtual std::string printAsHTML() const = 0;
};

class JSONPrintable {
public:
    virtual ~JSONPrintable() = default;
    virtual std::string printAsJSON() const = 0;
};

// --- Реализации (SRP, LSP) ---
// Теперь каждый класс отвечает только за свой формат. 
// Больше никаких runtime_error при вызове "не того" метода.

class TextData : public TextPrintable {
public:
    explicit TextData(std::string data) : data_(std::move(data)) {}

    std::string printAsText() const override {
        return data_;
    }
private:
    std::string data_;
};

class HTMLData : public HTMLPrintable {
public:
    explicit HTMLData(std::string data) : data_(std::move(data)) {}

    std::string printAsHTML() const override {
        return "<html>" + data_ + "</html>";
    }
private:
    std::string data_;
};

class JSONData : public JSONPrintable {
public:
    explicit JSONData(std::string data) : data_(std::move(data)) {}

    std::string printAsJSON() const override {
        return "{ \"data\": \"" + data_ + "\"}";
    }
private:
    std::string data_;
};

// --- Функции сохранения (OCP) ---
// Теперь функции сохранения зависят только от нужного интерфейса.
// Если появится новый формат (например, XML), мы просто создадим новую функцию 
// и новый интерфейс, не меняя старый код.

void saveToAsText(std::ofstream& file, const TextPrintable& printable) {
    file << printable.printAsText();
}

void saveToAsHTML(std::ofstream& file, const HTMLPrintable& printable) {
    file << printable.printAsHTML();
}

void saveToAsJSON(std::ofstream& file, const JSONPrintable& printable) {
    file << printable.printAsJSON();
}
/*
// Пример использования
int main(int argc, char** argv)
{
    std::ofstream file("output.txt");
    if (!file.is_open()) return 1;

    TextData text("Hello World");
    HTMLData html("Hello HTML");
    JSONData json("Hello JSON");

    saveToAsText(file, text);
    file << "\n";
    saveToAsHTML(file, html);
    file << "\n";
    saveToAsJSON(file, json);

    file.close();
    return 0;
}
*/