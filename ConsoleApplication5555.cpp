#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>

using namespace std;

map <string, int> weight = { {"+", 1 },
                             {"-", 1 },
                             {"*", 2 },
                             {"/", 2 },
                             {"(", 0 },
                             {")", 0 },
                             {" ", -1}
};


struct Stack {
    string value;
    Stack* next = NULL;
};

void push(Stack*& head, string value) {
    Stack* tmp = new Stack;
    tmp->value = value;
    if (head == NULL) {
        head = tmp;
        tmp->next = NULL;
    }
    else {
        if (head->value == "") {
            head->value = value;
            return;
        }
        tmp->next = head;
        head = tmp;
    }
}

string pop(Stack*& head) {
    Stack* prev = NULL;
    string value;
    if (head == NULL) {
        exit(-1);
    }
    prev = head;
    value = prev->value;
    head = head->next;
    free(prev);
    return value;
}

string top(Stack*& head) {
    if (head == NULL) {
        cout << "Stack is empty";
        return "";
    }
    return head->value;
}


string printStack(Stack* head) {
    string res = "";
    while (!(head == NULL)) {
        res += head->value;
        res += " ";
        head = head->next;
    }
    return res;
}

int sizeStack(Stack* head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

void deleteStack(Stack*& head) {
    while (head) {
        Stack* stack = head;
        head = head->next;
        delete stack;
    }
}

bool isOperation(const string& str, map <string, int> map_) {
    auto res = map_.find(str);
    return !(res == map_.end());
}

bool checkWeight(Stack*& stack, const string& str, map <string, int> map_) {
    auto res = map_.at(str);
    auto res1 = map_.at(top(stack));
    if (res1 >= res) return true;
    else return false;
}

double calculation(double value1, double value2, string operation) {
    switch (operation[0]) {
    case '+':
        return value1 + value2;
    case '-':
        return value1 - value2;
    case '*':
        return value1 * value2;
    case '/':
        return value1 / value2;
    default:
        break;
    }
}

vector<string> newString(string str) {
    vector<string> temp;
    string str1 = "";
    for (char i : str) {
        string symvol(1, i);
        if (isOperation(symvol, weight)) {
            if (str1 != " " && str1 != "") temp.push_back(str1);
            str1 = i;
            if (str1 != " " && str1 != "") temp.push_back(str1);
            str1 = "";
        }
        else if (i >= '0' && i <= '9') {
            str1 += i;
        }
        else {
            temp.clear();
            return temp;
        }
    }
    if (str1 != "" && str1 != " ") temp.push_back(str1);
    return temp;
}

string polishNotation(Stack*& stack, string str, string& res) {
    string pStr = "";
    vector<string> temp = newString(str);

    for (int i = 0; i < temp.size(); i++) {
        if (isOperation(temp[i], weight)) {
            if (temp[i] == "(") {
                push(stack, temp[i]);
                continue;
            }
            if (temp[i] == ")") {
                while (top(stack) != "(" && !(stack == NULL)) {
                    pStr += pop(stack) + ' ';
                }
                pop(stack);
            }
            else {
                if (!((stack == NULL) || !checkWeight(stack, temp[i], weight))) {
                    while (stack && checkWeight(stack, temp[i], weight)) {
                        pStr += pop(stack) + ' ';
                    }
                }
                push(stack, temp[i]);
            }
        }
        else {
            pStr += temp[i];
            pStr += ' ';
        }
        res += "Стек выхода:" + pStr + " Стек операций:" + printStack(stack) + "\n";
    }
    while (!(stack == NULL)) {
        pStr += pop(stack);
        pStr += ' ';
    }
    return pStr;
}

string calculation(Stack*& stack, string str) {
    vector<string> temp = newString(str);
    for (int i = 0; i < temp.size(); i++) {
        if (isOperation(temp[i], weight)) {
            if (sizeStack(stack) < 2) return "Ошибка!";
            double x = calculation(atof(pop(stack).c_str()), atof(pop(stack).c_str()), temp[i]);
            push(stack, to_string(x));

        }
        else {
            push(stack, temp[i]);
        }
    }
    if (sizeStack(stack) == 1) return pop(stack);
    else {
        return "Ошибка!";
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    Stack* stack = new Stack;
    string str1, polishStr, value,
        res = "";
    bool flag = true;

    while (flag) {
        ifstream file("text.txt");
        if (file.is_open()) {
            while (getline(file, str1)) {
                res += "Выражение:" + str1 + "\n";
                str1.erase(remove_if(str1.begin(), str1.end(), ::isspace), str1.end());
                deleteStack(stack);
                polishStr = polishNotation(stack, str1, res);
                deleteStack(stack);
                value = calculation(stack, polishStr);
                res += "ОПН: " + polishStr;
                res += "\nОтвет: " + value + "\n\n";
            }
        }
        file.close();
        flag = false;
    }
    flag = true;
    while (flag) {
        ofstream outputFile("result.txt", ios::out | ios::trunc);
        if (outputFile.is_open()) {
            outputFile << res;
        }
        outputFile.close();
        flag = false;
    }
    cout << res;
    system("pause");
}
