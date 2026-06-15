#include <fstream>
#include <stdexcept>
#include <string>

// ─────────────────────────────────────────────
// Шаг 1. Разбиваем «толстый» интерфейс на
//         узкоспециализированные (ISP)
// ─────────────────────────────────────────────

class PrintableAsHTML
{
public:
    virtual ~PrintableAsHTML() = default;
    virtual std::string printAsHTML() const = 0;
};

class PrintableAsText
{
public:
    virtual ~PrintableAsText() = default;
    virtual std::string printAsText() const = 0;
};

class PrintableAsJSON
{
public:
    virtual ~PrintableAsJSON() = default;
    virtual std::string printAsJSON() const = 0;
};

// ─────────────────────────────────────────────
// Шаг 2. Конкретные классы реализуют только
//         нужный им интерфейс (LSP, ISP)
// ─────────────────────────────────────────────

class DataHTML : public PrintableAsHTML
{
public:
    explicit DataHTML(std::string data)
        : data_(std::move(data)) {
    }

    std::string printAsHTML() const override
    {
        return "<html>" + data_ + "</html>";
    }

private:
    std::string data_;
};

class DataText : public PrintableAsText
{
public:
    explicit DataText(std::string data)
        : data_(std::move(data)) {
    }

    std::string printAsText() const override
    {
        return data_;
    }

private:
    std::string data_;
};

class DataJSON : public PrintableAsJSON
{
public:
    explicit DataJSON(std::string data)
        : data_(std::move(data)) {
    }

    std::string printAsJSON() const override
    {
        return "{ \"data\": \"" + data_ + "\"}";
    }

private:
    std::string data_;
};

// ─────────────────────────────────────────────
// Шаг 3. Функции сохранения принимают только
//         нужный интерфейс — saveTo ничего не
//         знает о форматах (OCP, SRP)
// ─────────────────────────────────────────────

void saveToAsHTML(std::ofstream& file, const PrintableAsHTML& printable)
{
    file << printable.printAsHTML();
}

void saveToAsText(std::ofstream& file, const PrintableAsText& printable)
{
    file << printable.printAsText();
}

void saveToAsJSON(std::ofstream& file, const PrintableAsJSON& printable)
{
    file << printable.printAsJSON();
}

// ─────────────────────────────────────────────
// Шаг 4. Проверка использования
// ─────────────────────────────────────────────

int main()
{
    DataHTML  htmlData("<h1>Hello</h1>");
    DataText  textData("Hello");
    DataJSON  jsonData("Hello");

    std::ofstream htmlFile("out.html");
    std::ofstream textFile("out.txt");
    std::ofstream jsonFile("out.json");

    saveToAsHTML(htmlFile, htmlData);
    saveToAsText(textFile, textData);
    saveToAsJSON(jsonFile, jsonData);

    // Компилятор не позволит передать не тот тип:
    // saveToAsHTML(htmlFile, textData); // <- ошибка компиляции

    return 0;
}
