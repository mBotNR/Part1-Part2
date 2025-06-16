#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <stack>
#include <set>

bool IsBracketSequenceValid(const std::string& expr) {
    std::stack<char> s;
    for (char c : expr) {
        if (c == '(' || c == '[' || c == '{') s.push(c);
        else if (c == ')' || c == ']' || c == '}') {
            if (s.empty()) return false;
            char open = s.top();
            if ((c == ')' && open != '(') ||
                (c == ']' && open != '[') ||
                (c == '}' && open != '{')) return false;
            s.pop();
        }
        else return false; 
    }
    return s.empty();
}
void BracketCheckMenu() {
    std::string expr;
    std::cout << "Введите скобочное выражение: ";
    std::cin >> expr;
    if (IsBracketSequenceValid(expr))
        std::cout << "Корректная скобочная последовательность!\n";
    else
        std::cout << "Некорректная скобочная последовательность!\n";
}

std::vector<std::string> SplitSentences(const std::string& text) {
    std::regex re(R"(([^.!?]+[.!?]))");
    std::sregex_iterator it(text.begin(), text.end(), re), end;
    std::vector<std::string> result;
    while (it != end) {
        result.push_back(it->str());
        ++it;
    }
    return result;
}
void SplitSentencesMenu() {
    std::cin.ignore();
    std::string text;
    std::cout << "Введите текст для разделения на предложения:\n";
    std::getline(std::cin, text);
    auto sentences = SplitSentences(text);
    std::cout << "Предложения:\n";
    for (const auto& s : sentences)
        std::cout << s << '\n';
}

std::vector<std::string> ExtractPersons(const std::string& text) {
    std::regex re(R"(([А-ЯЁA-Z][а-яёa-z]+(?:\s+[А-ЯЁA-Z][а-яёa-z]+)+))");
    std::sregex_iterator it(text.begin(), text.end(), re), end;
    std::vector<std::string> persons;
    while (it != end) {
        persons.push_back(it->str());
        ++it;
    }
    return persons;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int choice;
    do {
        std::cout << "\nМеню:\n";
        std::cout << "1. Проверка скобочной последовательности\n";
        std::cout << "2. Разделение текста на предложения\n";
        std::cout << "0. Выход\n";
        std::cout << "Ваш выбор: ";
        std::cin >> choice;
        switch (choice) {
        case 1: BracketCheckMenu(); break;
        case 2: SplitSentencesMenu(); break;
        }
    } while (choice != 0);
    std::cout << "Пока!\n";
    return 0;
}
