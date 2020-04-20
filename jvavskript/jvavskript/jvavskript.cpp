#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <cstdlib>
#include <sstream>
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
    cout << "|      jvavskript 1.3.3      |\n";
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

std::map<string, string>varStringList;

void funcInput(token* ttoken, args arg)
{
    string word;
    std::cin >> word;
    varStringList[arg[0]] = word;
    std::cin.get();
}

void funcOutputVar(token* ttoken, args arg) 
{
    if (varStringList.count(arg[0]) == 0) {
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
        std::cout << varStringList[arg[i]];
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

void funcSleep(token* ttoken, args arg) {
    Sleep(atoi(arg[0].c_str()));
}

void funcRCIVS(token* ttoken, args arg) {
    std::istream* inBuckup = in;
    std::stringstream ss;
    if (varStringList.count(arg[0]) == 0) {
        std::cout << "Doesn't has variable" << arg[0] << "!\n";
    }
    ss << varStringList[arg[0]];
    in = &ss;
    while (true) {
        if (in->eof()) {
            in = inBuckup;
            break;
        }
        if (in == &std::cin) {
            std::cout << ">>>";
        }
        string to;
        *in >> to;
        if (to == "") {
            continue;
        }
        std::vector<string> Iargs;
        if (!tokenList.count(to) == 1) {
            std::cout << "错误的命令\n";
            continue;
        }
        else {
            string str;
            char ch;
            while (true)
            {
                if (in->eof()) {
                    in = inBuckup;
                    tokenList[to]->todo(tokenList[to], Iargs);
                    return;
                }
                ch = in->get();
                if (!(ch == ' ')) {
                    break;
                }
            }
            if (!(ch == '\n' || ch == '\r')) {
                while (true) {
                    if (in->eof()) {
                        in = inBuckup;
                        Iargs.push_back(str);
                        tokenList[to]->todo(tokenList[to], Iargs);
                        return;
                    }
                    if (ch == ' ' || ch == '\n' || ch == '\r') {
                        Iargs.push_back(str);
                        str = "";
                        if (ch == '\n' || ch == '\r') {
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
    in = inBuckup;
}

void funcInputGetLine(token* ttoken, args arg)
{
    string word;
    std::getline(std::cin, word);
    varStringList[arg[0]] = word;
    if (arg[1] == "true") {
        varStringList[arg[0]] += "\n";
    }
    else if (arg[1] != "false") {
        std::cout << "Wrong Boolean Value " << arg[1] << "!\n";
    }
}

void funcAppendString(token* ttoken, args arg){
    string n1=arg[0], n2=arg[1];
    if(arg[0][0]=='$'&&arg[0][1]!='$')
    {
        string str = arg[0];
        n1 = "";
        for (int i = 1; i < str.length(); i++) {
            n1 += str[i];
        }
        if (varStringList.count(n1) == 0) {
            std::cout << "Doesn't has variable" << n1 << "!\n";
            return;
        }
        n1 = varStringList[n1];
    }
    if (arg[1][0] == '$' && arg[1][1] != '$')
    {
        string str = arg[1];
        n2 = "";
        for (int i = 1; i < str.length(); i++) {
            n2 += str[i];
        }
        if (varStringList.count(n2) == 0) {
            std::cout << "Doesn't has variable" << n2 << "!\n";
            return;
        }
        n2 = varStringList[n2];
    }
    varStringList[arg[2]] = n1 + n2;
}

void initTokens() {

    registryToken(new token("help","获得jvavskript的帮助","help",funcHelp));
    registryToken(new token("output string(可以有空格) ... boolean(为true则在行尾输出回车，为false不输出)", "输出字符串", "output", funcOutput));
    registryToken(new token("input name", "输入字符串到变量name中", "input", funcInput));
    registryToken(new token("outputVar name1 name2 name3 ... boolean(为true则在行尾输出回车，为false不输出) prefabSplit(字符串,在name1,name2之间输出,comma是逗号,space是空格,enter是回车,null是无", "输出变量name1,name2", "outputVar", funcOutputVar));
    registryToken(new token("exit", "退出", "exit", funcExit));
    registryToken(new token("sleep time", "使程序休息time毫秒", "sleep", funcSleep));
    registryToken(new token("RCIVS name", "运行name里的代码。RCIVS全称是RunCodeInVariableString", "RCIVS", funcRCIVS));
    registryToken(new token("getline name boolean(为true则在字符串末尾加上回车，为false则不加)", "获取一整行的字符串到name中", "getline", funcInputGetLine));
    registryToken(new token("append string1(name1) string2(name2) name3", "将string1与string2进行拼接的结果赋给name3,如果要标明这个string1或string2是一个变量，请在名字前加上一个$,如果你想在开头使用$,请输入$$", "append", funcAppendString));
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
        if (to == "") {
            continue;
        }
        if (!tokenList.count(to)==1) {
            std::cout << "错误的命令"<<to<<"\n";
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