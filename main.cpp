#include <iostream>
#include <string>
#include "db.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 5) {
        cout << "./dbms --file <file> --query '<query>'" << endl;
        return 1;
    }

    string filePath = "";
    string queryStr = "";

    //аргументы
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) filePath = argv[++i];
        else if (arg == "--query" && i + 1 < argc) queryStr = argv[++i];
    }

    if (filePath == "" || queryStr == "") {
        cout << "Error: Invalid arguments" << endl;
        return 1;
    }

    //Инициализация бд
    Database db;
    
    //Загрузка
    db.loadData(filePath);
    
    //Выполнение запроса
    string res = db.executeQuery(queryStr);
    
    if (res != "") cout << res << endl;

    //Сохранение
    db.saveData(filePath);

    return 0;
}