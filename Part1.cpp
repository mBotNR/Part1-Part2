#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <set>
#include <locale>

bool IsValidPassword(const std::string& password) {
    
    if (!std::regex_match(password, std::regex("^[A-Za-z0-9\\^\\$%@#&*!\\?]{8,}$"))) return false;
    
    if (!std::regex_search(password.begin(), password.end(), std::regex("[A-Z]"))) return false;
    
    if (!std::regex_search(password.begin(), password.end(), std::regex("[a-z]"))) return false;
    
    if (!std::regex_search(password.begin(), password.end(), std::regex("[0-9]"))) return false;
    
    std::set<char> specials;
    for (char ch : password)
        if (std::string("^$%@#&*!?").find(ch) != std::string::npos)
            specials.insert(ch);
    if (specials.size() < 2) return false;
    
    for (size_t i = 1; i < password.size(); ++i)
        if (password[i] == password[i - 1]) return false;
    return true;
}

void PasswordCheckMenu() {
    std::string password;
    std::cout << "Введите пароль для проверки: ";
    std::cin >> password;
    if (IsValidPassword(password))
        std::cout << "Пароль корректный!\n";
    else
        std::cout << "Пароль некорректный!\n";
}


bool IsValidWebColor(const std::string& color) {
    std::regex hex_re("^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$");
    std::regex rgb_re("^rgb\\((([0-9]{1,3}%?|100%?),\\s*){2}([0-9]{1,3}%?|100%?)\\)$");
    std::regex hsl_re("^hsl\\(([0-9]{1,3}),\\s*([0-9]{1,3})%?,\\s*([0-9]{1,3})%?\\)$");
    std::regex rgb_simple_re("^#([A-Fa-f0-9]{1})$");
    return std::regex_match(color, hex_re) ||
        std::regex_match(color, rgb_re) ||
        std::regex_match(color, hsl_re) ||
        std::regex_match(color, rgb_simple_re);
}

void WebColorCheckMenu() {
    std::string color;
    std::cout << "Введите web-цвет: ";
    std::cin >> color;
    if (IsValidWebColor(color))
        std::cout << "Корректная запись цвета!\n";
    else
        std::cout << "Некорректная запись цвета!\n";
}


struct Token {
    std::string type;
    std::pair<int, int> span;
    std::string value;
};

const std::vector<std::string> functions = {
    "sin", "cos", "tg", "ctg", "tan", "cot", "sinh", "cosh", "th", "cth",
    "tanh", "coth", "ln", "lg", "log", "exp", "sqrt", "cbrt", "abs", "sign"
};
const std::vector<std::string> constants = {
    "pi", "e", "sqrt2", "ln2", "ln10"
};

void TokenizeMathExprMenu() {
    std::cin.ignore();
    std::string expr;
    std::cout << "Введите математическое выражение: ";
    std::getline(std::cin, expr);

    std::vector<Token> tokens;
    size_t pos = 0;
    while (pos < expr.size()) {
        
        if (isspace(expr[pos])) { ++pos; continue; }
        
        if (expr[pos] == '(') {
            tokens.push_back({ "left_parenthesis", {int(pos), int(pos + 1)}, "(" });
            ++pos; continue;
        }
        if (expr[pos] == ')') {
            tokens.push_back({ "right_parenthesis", {int(pos), int(pos + 1)}, ")" });
            ++pos; continue;
        }
        
        if (std::string("+-*/^").find(expr[pos]) != std::string::npos) {
            tokens.push_back({ "operator", {int(pos), int(pos + 1)}, std::string(1,expr[pos]) });
            ++pos; continue;
        }
        
        std::smatch m;
        std::string rest = expr.substr(pos);
        std::regex number_re("^[0-9]+(\\.[0-9]+)?");
        if (std::regex_search(rest, m, number_re)) {
            int len = m.length(0);
            tokens.push_back({ "number", {int(pos), int(pos + len)}, m.str(0) });
            pos += len;
            continue;
        }
        
        std::regex ident_re("^[_A-Za-z][_A-Za-z0-9]*");
        if (std::regex_search(rest, m, ident_re)) {
            std::string ident = m.str(0);
            int len = m.length(0);
            
            bool is_func = false, is_const = false;
            for (auto& f : functions) if (ident == f) is_func = true;
            for (auto& c : constants) if (ident == c) is_const = true;
            if (is_func)
                tokens.push_back({ "function", {int(pos), int(pos + len)}, ident });
            else if (is_const)
                tokens.push_back({ "constant", {int(pos), int(pos + len)}, ident });
            else
                tokens.push_back({ "variable", {int(pos), int(pos + len)}, ident });
            pos += len;
            continue;
        }
        ++pos; 
    }

    std::cout << "Токены:\n";
    for (auto& t : tokens) {
        std::cout << "{type: " << t.type
            << ", span: [" << t.span.first << ", " << t.span.second << "]"
            << ", value: " << t.value << "}\n";
    }
}


bool IsValidDate(const std::string& date) {
    std::vector<std::regex> formats = {
        
        std::regex("^\\d{1,2}\\.\\d{1,2}\\.\\d{4}$"),
        
        std::regex("^\\d{1,2}/\\d{1,2}/\\d{4}$"),
        
        std::regex("^\\d{1,2}-\\d{1,2}-\\d{4}$"),
        
        std::regex("^\\d{4}\\.\\d{1,2}\\.\\d{1,2}$"),
        
        std::regex("^\\d{4}/\\d{1,2}/\\d{1,2}$"),
        
        std::regex("^\\d{4}-\\d{1,2}-\\d{1,2}$"),
        
        std::regex("^\\d{1,2} (января|февраля|марта|апреля|мая|июня|июля|августа|сентября|октября|ноября|декабря) \\d{4}$"),
        
        std::regex("^(January|February|March|April|May|June|July|August|September|October|November|December) \\d{1,2}, \\d{4}$"),
        
        std::regex("^(January|February|March|April|May|June|July|August|September|October|November|December) \\d{1,2} \\d{4}$"),
        
        std::regex("^\\d{1,2} (January|February|March|April|May|June|July|August|September|October|November|December) \\d{4}$"),
        
        std::regex("^\\d{4}, (January|February|March|April|May|June|July|August|September|October|November|December) \\d{1,2}$"),
        
        std::regex("^\\d{1,2} (января|февраля|марта|апреля|мая|июня|июля|августа|сентября|октября|ноября|декабря), \\d{4}$"),
        
        std::regex("^\\d{4}$"),
        
        std::regex("^\\d{1,2}\\.\\d{1,2}\\.\\d{1,4}$"),
        
        std::regex("^\\d{1,2} (January|February|March|April|May|June|July|August|September|October|November|December) \\d{4}$"),
        
        std::regex("^\\d{4}, (January|February|March|April|May|June|July|August|September|October|November|December) \\d{1,2}$"),
        
        std::regex("^\\d{1,2}/\\d{1,2}/\\d{4}$")
    };

    for (auto& re : formats)
        if (std::regex_match(date, re)) return true;
    return false;
}

void DateCheckMenu() {
    std::cin.ignore();
    std::string date;
    std::cout << "Введите дату: ";
    std::getline(std::cin, date);
    if (IsValidDate(date))
        std::cout << "Дата корректна!\n";
    else
        std::cout << "Дата некорректна!\n";
}


int main() {
    setlocale(LC_ALL, "Russian");
    int choice;
    do {
        std::cout << "\nМеню:\n";
        std::cout << "1. Проверка пароля\n";
        std::cout << "2. Проверка web-цвета\n";
        std::cout << "3. Токенизация мат. выражения\n";
        std::cout << "4. Проверка даты\n";
        std::cout << "0. Выход\n";
        std::cout << "Ваш выбор: ";
        std::cin >> choice;
        switch (choice) {
        case 1: PasswordCheckMenu(); break;
        case 2: WebColorCheckMenu(); break;
        case 3: TokenizeMathExprMenu(); break;
        case 4: DateCheckMenu(); break;
        }
    } while (choice != 0);
    std::cout << "Пока!\n";
    return 0;
}
