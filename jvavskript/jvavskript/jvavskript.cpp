#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <cstdlib>
using std::string;

class token;

typedef std::vector<string> args;
typedef void(*TokenFunc)(token*,args);

std::istream* in;

class token
{
public:
    string usage;
    string describe;
    string name;
    TokenFunc todo;
    token(string usage, string describe, string name, TokenFunc f)
    {
        this->usage = usage;
        this->describe = describe;
        this->name = name;
        this->todo = f;
    }
};

std::map<string, token*>tokenList;
std::vector<string>tokenNames;

void registryToken(token* t) {
    tokenList[t->name] = t;
    tokenNames.push_back(t->name);
}

void freeToken() {
    for (int i = 0; i < tokenNames.size(); i++) {
        delete tokenList[tokenNames[i]];
    }
}

void funcHelp(token* ttoken,args arg) {
    using std::cout;
    cout << "\n";
    cout << "------------------------------\n";
    cout << "|      jvavskript 1.2.0      |\n";
    cout << "|       madeby 飞翔之歌      |\n";
    cout << "------------------------------\n";
    for (int i = 0; i < tokenNames.size(); i++) {
        cout << tokenList[tokenNames[i]]->name << ": " << tokenList[tokenNames[i]]->describe<<"\nusage:"<<tokenList[tokenNames[i]]->usage<< "\n\n";
    }
}

void funcOutput(token* ttoken, args arg)
{
    for (int i = 0; i < arg.size() - 1; i++) {
        std::cout << arg[i] << " ";
    }
    if (arg[arg.size()-1] == "true") {
        std::cout << "\n";
    }
    else if (arg[arg.size()-1] != "false") {
        std::cout << "Wrong Boolean Value " << arg[1] << "!\n";
    }
}

void funcExit(token* ttoken, args arg)
{
    freeToken();
    exit(0);
}

std::map<string, string>varList;

void funcInput(token*, args arg)
{
    string word;
    std::cin >> word;
    varList[arg[0]] = word;
}

void funcOutputVar(token*, args arg) 
{
    if (varList.count(arg[0]) == 0) {
        std::cout << "Doesn't has variable" << arg[0] << "!\n";
        return;
    }
    char a;
    if (arg[arg.size() - 1] == "enter") {
        a = '\n';
    }
    else if (arg[arg.size() - 1] == "comma") {
        a = ',';
    }
    else if (arg[arg.size() - 1] == "space") {
        a = ' ';
    }
    else if (arg[arg.size() - 1] == "null") {
        a = '\0';
    }
    else {
        std::cout << "Unknow prefab split" << arg[arg.size() - 1] << "!\n";
    }
    for (int i = 0; i < arg.size() - 2; i++) {
        std::cout << varList[arg[i]];
        if (i != arg.size() - 3) {
            std::cout << a;
        }
    }
    if (arg[arg.size() - 2] == "true") {
        std::cout << "\n";
    }
    else if (arg[arg.size() - 2] != "false") {
        std::cout << "Wrong Boolean Value " << arg[arg.size() - 2] << "!\n";
    }
}

void funcSleep(token*, args arg) {
    Sleep(atoi(arg[0].c_str()));
}

void initTokens() {
    registryToken(new token("help","获得jvavskript的帮助","help",funcHelp));
    registryToken(new token("output string(可以有空格) ... boolean(为true则在行尾输出回车，为false不输出)", "输出字符串", "output", funcOutput));
    registryToken(new token("input name", "输入字符串到变量name中", "input", funcInput));
    registryToken(new token("outputVar name1 name2 name3 ... boolean(为true则在行尾输出回车，为false不输出) prefabSplit(字符串,在name1,name2之间输出,comma是逗号,space是空格,enter是回车,null是无", "输出变量name1,name2", "outputVar", funcOutputVar));
    registryToken(new token("exit", "退出", "exit", funcExit));
    registryToken(new token("sleep time", "使程序休息time毫秒", "sleep", funcSleep));
}

std::ifstream fin;
std::ofstream fout;

int main()
{
    in = &std::cin;
    std::cout << "输入jvavskript文件路径:";
    string way;
    *in >> way;
    if (way != "STD"){
        fin.open(way.c_str());
        in = &fin;
    }
    initTokens();
    while (true) {
        if (in->eof()) {
            break;
        }
        if (in == &std::cin) {
            std::cout << ">>>";
        }
        string to;
        *in >> to;
        std::vector<string> Iargs;
        if (!tokenList.count(to)==1) {
            std::cout << "错误的命令\n";
            continue;
        }
        else {
            string str;
            char ch;
            while (true)
            {
                if (in->eof()) {
                    tokenList[to]->todo(tokenList[to], Iargs);
                    freeToken();
                    return 0;
                }
                ch = in->get();
                if (!(ch == ' ')) {
                    break;
                }                
            }
            if (!(ch == '\n' || ch == '\r')) {
                while (true) {
                    if (in->eof()) {
                        Iargs.push_back(str);
                        tokenList[to]->todo(tokenList[to],Iargs);
                        freeToken();
                        return 0;
                    }
                    if (ch == ' ' || ch == '\n' || ch == '\r'){
                        Iargs.push_back(str);
                        str = "";
                        if (ch == '\n'||ch=='\r') {
                            break;
                        }
                    }
                    else {
                        str += ch;
                    }
                    ch = in->get();
                }
            }
            tokenList[to]->todo(tokenList[to], Iargs);
        }
        
        
    }
    freeToken();
}