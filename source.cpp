#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "llong.h"
#include <QInputDialog>
#include <map>
#include <QMessageBox>
using namespace std;
std:: string s,val,cur_num="",cur_str="",cnt_num ="",cnt_str="",poliz = "";
int LOG = 0;
stack <char> st;
stack <char> brace;
map <string,string> mp_str;
void reset(){
    LOG = 0;
     mp_str.clear();
     s.clear();
     val.clear();
     cur_num.clear();
     cur_str.clear();
     cnt_num.clear();
     cnt_str.clear();
     poliz.clear();
     while(!st.empty()) st.pop();
     while(!brace.empty()) brace.pop();
}
long long power(int a,int b){
    if(b == 1) return a;
    if(b%2) return a*power(a,b-1);
    long long tmp = power(a,b/2);
    return tmp*tmp;
}
bool if_float(string str){
    for(size_t i = 0; i < str.size(); i++){
            if(str[i] == '.' || str[i] == 'E' || str[i] == 'e'){
                return 1;
            }
    }
    return 0;
}
double logarithm(double first,double second){
    return log(second)/log(first);
}
void bracket_balance(char c){
        if(brace.empty()){
                throw "не соблюдён баланс скобок";
        }
        else if((brace.top() == '[' && c == ']') || (brace.top() == '(' && c == ')')) brace.pop();
        else {
            while(!brace.empty()) brace.pop();
            throw "неправильно расставлены скобки";
        }
}
enum States{
    start,
    bracket_close,
    bracket_open,
    number,
    un_op,
    /*trig_braket,
    log_braket,
    trig_arg_str,
    trig_arg_num,
    log_str_first,
    log_num_first,
    log_str_second,
    log_num_second,
    log_second*/
    operation,
    str,
    real,
    real_order,
    comma
};
bool Number(char c){
    if(c >= 48 && c <= 57) return 1;
    return 0;
}
bool letter(char c){
    if((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) return 1;
    return 0;
}
int unary(const string &s,size_t i){
    if(i == 0 && (s[i] == '+' || s[i] == '-')) {
        if(letter(s[i+1])) return 1;
        else if(Number(s[i+1])) return 2;
    }
    if(i > 0 && s[i-1] == '(' && (s[i] == '+' || s[i] == '-')&& (i < s.size() - 1)){
        if(letter(s[i+1])) return 1;
        else if(Number(s[i+1])) return 2;
    }
    return 0;
}
bool oper(char c){
    if(c == '+' || c == '-' || c == '/' || c == '*' || c == '^') return 1;
    return 0;
}
bool br_open(char c){
    if(c == '[' || c == '(') return 1;
    return 0;
}
bool br_close(char c){
    if(c == ']' || c == ')') return 1;
    return 0;
}
bool isnumber(string s){
    size_t k = 0,flag = 0;
    if(s[0] == '-' || s[0] == '+') k = 1;
    for(size_t i = k; i < s.size(); i++)
    {
        if(s[i] == '.') flag++;
        if(flag > 1) return 0;
        if ((s[i] >= 48 && s[i] <= 57 || (i != s.size() - 1 && i > k && (s[i] == '.' || s[i] == 'E'||s[i] == 'e') && s[i-1] >= 48 && s[i-1] <= 57)) || (i > k && i != s.size() - 1 && (s[i-1] == 'E'|| s[i-1] == 'e') && (s[i] == '+' || s[i] == '-'))) continue;
        else return 0;
    }
    return 1;
}
bool func(string s){
    if(s != "sin" && s != "cos" && s != "log") return 1;
    return 0;
}
int prior(char c){
    switch(c){
    case '~':
        return 6;
    case 's':
    case 'c':
    case 'l':
        return 5;
    case '^':
        return 4;
    case '*':
    case '/':
        return 3;
    case '-' :
    case '+' :
        return 2;
    case '(' :
    case '[' :
    case ',' :
        return 1;
    default:
        return 0;
    }
}
void cur_empty( string &cur_str,string &cur_num){
    if(cur_str != "") {
            string cur;
            /*if(cur_str[0] == '+' || cur_str[0] == '-'){
                if(cur_str[0] == '-') st.push('~');
                cur += string(cur_str,1,cur_str.size()-1);
            }
            else*/ cur = cur_str;
            if(cur == "s" || cur == "c" || cur == "l" || cur =="sin" || cur == "cos" || cur == "log"){
                throw "переменная не может иметь имён s/c/l/sin/cos/log";
            }
            if(mp_str.find(cur) == mp_str.end()){
               string str;
              if(cur == "pi") str = "3.14159265358979323846";
              else if(cur == "e") str = "2.718281828";
              else{
               bool ok;
               str = QInputDialog::getText(0,QString::fromUtf8("Input"),QString::fromStdString(cur),QLineEdit::Normal,"",&ok).toStdString();
                while(!(ok && str.size() && isnumber(str))){
                 if (!ok) throw "Значение выражения не введено";
                 QMessageBox msg;
                 msg.setText("Неверно введено число.Введите ещё раз.");
                 msg.exec();
                 str = QInputDialog::getText(0,QString::fromUtf8("Input"),QString::fromStdString(cur),QLineEdit::Normal,"",&ok).toStdString();
                 }
              }
             mp_str.insert({cur,str});
            }
            poliz += cur_str;
            poliz += " ";
            cur_str ="";
        }
       if(cur_num != ""){
            poliz += cur_num;
            poliz += " ";
            cur_num ="";
       }
}
string poliz_str(string s){
    for(size_t i = 0; i < s.size();i++){
        if(s[i] == ' ') continue;
        else if((s[i] == '-' || s[i] == '+' )&& (i == 0 || s[i-1] == '(' || s[i-1] == '[')){
            if(s[i] == '-') st.push('~');
        }
        else if(Number(s[i]) || s[i] =='.' || (i > 0 && (s[i] == 'E' ||s[i] == 'e') && Number(s[i-1])) || (i > 1 && Number(cur_num[cur_num.size()-2]) && (s[i-1] == 'E' || s[i-1] == 'e') &&(s[i] =='+' || s[i] =='-'))){
            cur_num += s[i];
        }
        else if(letter(s[i])){
            cur_str += s[i];
        }
        else {
          if(cur_str == "sin" || cur_str == "cos" || cur_str == "log"){
              st.push(cur_str[0]);
              cur_str ="";
          }
          else cur_empty(cur_str,cur_num);
          if(st.empty() || s[i] == '(' || s[i] == '[') st.push(s[i]);
          else if(s[i] == ')' || s[i] == ','){
            while(st.top() != '('){
                 poliz += st.top();
                 poliz += " ";
                 st.pop();
            }
            if(s[i] == ')') st.pop();
           }
          else if(s[i] == ']'){
            while(st.top() != '['){
                 poliz += st.top();
                 poliz += " ";
                 st.pop();
            }
            st.pop();
           }
          else {
            while(!st.empty() && prior(st.top()) >= prior(s[i])){
                poliz += st.top();
                poliz += " ";
                st.pop();
            }
            st.push(s[i]);
           }
        }
    }
    cur_empty(cur_str,cur_num);
    while(!st.empty()){
        poliz += st.top();
        poliz += " ";
        st.pop();
    }
    return poliz;
}
void is_correct(string s){
    States state = start;
     for(size_t i = 0; i < s.size() ; i++ ){
            if(s[i] ==' ') continue;
            switch(state){
            case start:
                if(Number(s[i])){
                    state = number;
                    cnt_num += s[i];
                }
                else if(letter(s[i])){
                    state = str;
                    cnt_str += s[i];
                }
                else if(s[i] == '+' || s[i] == '-'){
                    state = un_op;
                    //cnt_num += s[i];
                    //cnt_str += s[i];
                }
                else if(s[i] =='(' || s[i] == '['){
                    brace.push(s[i]);
                    state = bracket_open;
                }
                else{
                    throw "неверно введён первый символ";
                }
                break;
             case number:
                if(s[i] == 'E' || s[i] == 'e'){
                    cnt_num += s[i];
                    state = real;
                }
                else if(Number(s[i]) || s[i] =='.'){
                    cnt_num += s[i];
                    if(s[i] == '.') state = real_order;
                }
                else if(letter(s[i]) || br_open(s[i])){
                    throw "число состоит только из цифр";
                }
                else if(oper(s[i])){
                    if(!isnumber(cnt_num)) throw "неправильно введено число";
                    state = operation;
                    cnt_num = "";
                }
                else if(br_close(s[i])){
                    if(!isnumber(cnt_num)) throw "неправильно введено число";
                     bracket_balance(s[i]);
                    state = bracket_close;
                    cnt_num = "";
                }
                else if(s[i] == ','){
                    if(!isnumber(cnt_num)) throw "неправильно введено число";
                    state = comma;
                }
                else{
                    throw s[i];
                }
                break;
            case str:
                if(Number(s[i])){
                    throw "переменная не может содержать цифр";
                }
                else if(letter(s[i])){
                    cnt_str += s[i];
                }
                else if(oper(s[i]) || br_close(s[i])){
                    if(!func(cnt_str)){
                        throw "после функции ожидается открывающая скобка";
                    }
                    else{
                       if(oper(s[i])){
                          state = operation;
                        }
                        else{
                            state = bracket_close;
                            bracket_balance(s[i]);
                        }
                            cnt_str ="";
                    }
                }
                else if (br_open(s[i])){
                    if(cnt_str == "sin" || cnt_str == "cos" ){
                        brace.push(s[i]);
                        //state = trig_braket;
                        state = bracket_open;
                        cnt_str = "";
                    }
                    else if(cnt_str == "log" ){
                        brace.push(s[i]);
                        //state = log_braket;
                        state = bracket_open;
                        LOG++;
                        cnt_str = "";
                    }
                    else{
                        throw "После переменной не может идти открывающая скобка";
                    }
                }
                else if(s[i] == ','){
                    state = comma;
                }
                else{
                    throw s[i];
                }
                break;
            case bracket_open:
                if(s[i] == ']' || s[i] == ')' || s[i] == '*' || s[i] == '/' || s[i] == '^'){
                    throw "после открывающей скобки не может быть такого символа";
                }
                else if(Number(s[i])){
                    state = number;
                    cnt_num += s[i];
                }
                else if(letter(s[i])){
                    state = str;
                    cnt_str += s[i];
                }
                else if(s[i] == '-' || s[i] == '+'){
                    state = un_op;
                    //cnt_str += s[i];
                    //cnt_num += s[i];
                }
                else if(br_open(s[i])){
                    brace.push(s[i]);
                }
                else{
                    throw s[i];
                }
                break;
            case bracket_close:
                if(br_open(s[i]) || letter(s[i]) || Number(s[i])){
                    throw "после закрывающей скобки не может быть такого символа";
                }
                else if(oper(s[i])){
                    state = operation;
                }
                else if(br_close(s[i])){
                    bracket_balance(s[i]);
                }
                else if(s[i] == ','){
                    state = comma;
                }
                else{
                    throw "После закрывающей скобки ожидается +,-,*,^,/";
                }
                break;
             case operation:
                if(letter(s[i])){
                    state = str;
                    cnt_str += s[i];
                }
                else if(Number(s[i])){
                    state = number;
                    cnt_num += s[i];
                }
                else if(br_open(s[i]))
                {
                    state = bracket_open;
                    brace.push(s[i]);
                }
                else{
                    throw "После арифметической операции ожидается число/переменная/(/[";
                }
                break;
             case un_op:
                if(letter(s[i])){
                    cnt_str += s[i];
                    //cnt_num = "";
                    state = str;
                }
                else if(Number(s[i])){
                    //cnt_str ="";
                    cnt_num += s[i];
                    state = number;
                }
                else if(br_open(s[i])){
                   // cnt_str = "";
                   // cnt_num = "";
                    brace.push(s[i]);
                    state = bracket_open;
                }
                else{
                    throw "После унарного + и - ожидается число или переменная";
                }
                break;
            case real:
                if(s[i] == '+' || s[i] == '-'){
                    cnt_num += s[i];
                    state = real_order;
                }
                else if(Number(s[i])){
                    i--;
                    state = real_order;
                }
                else{
                    throw "неправильно введено вещественное число";
                }
             break;
            case real_order:
                if(Number(s[i])){
                    cnt_num+= s[i];
                    state = number;
                }
                else{
                    throw "неправильно введено вещественное число";
                }
            break;
            case comma:
                if(!LOG || s[i] == ',') throw "лишняя запятая";
                else if(Number(s[i])){
                    state = number;
                    cnt_num += s[i];
                }
                else if(letter(s[i])){
                    state = str;
                    cnt_str += s[i];
                }
                else if(s[i] == '+'){
                    state = un_op;
                    //cnt_str += s[i];
                   // cnt_num += s[i];
                }
                else if(br_open(s[i])){
                    state = bracket_open;
                    brace.push(s[i]);
                }
                else{
                    throw "неверно введён 2 аргумент функции log";
                }
                LOG--;
                break;

        }
        //cout << state <<" ";
      }
      if(!Number(s[s.size()-1]) && !letter(s[s.size()-1]) && !(br_close(s[s.size()-1]))){
        throw "неверно введён последний символ";
      }
      if(cnt_num != "" && !isnumber(cnt_num)) throw "неверно введено число";
      if(!brace.empty()){
        throw "не соблюдён баланс скобок";
      }
}
bool action(char c){
    if(c != '+' && c != '-' && c != '^' && c != '*' && c != '/' && c != 'l' && c != 's' && c != 'c' && c != '~'){
        return 1;
    }
    return 0;
}
string computing(string poliz){
    string value;
    stack <string> banan;
    for(size_t i = 0; i < poliz.size();){
        if(poliz[i] ==' ') {i++;continue;}
        else if(action(poliz[i]) || (poliz[i] == 's' || poliz[i] == 'c' || poliz[i] == 'l')&& (i != poliz.size()-1)&&letter(poliz[i+1])){
         while(i < poliz.size() && poliz[i] != ' '){
            value += poliz[i];
            i++;
         }
         if(!(Number(value[value.size()-1]))){
             banan.push((mp_str.find(value))->second);
         }
         else banan.push(value);
         value="";
       }
       else{
        long double ans,sec_double,fir_double;
        string second = banan.top();
        banan.pop();
        ostringstream ss;
        string s;
        sec_double = atof(second.c_str());
        if(poliz[i] == '~'){
           if(second[0] != '-') second = '-' + second;
           else if(second[0] == '-') second = string(second,1,second.size() - 1);
           banan.push(second);
        }
        else if(poliz[i] == 's' || poliz[i] == 'c'){
            if(poliz[i] == 's') ans = sin(sec_double);
            else ans = cos(sec_double);
           ss << ans;
           s = ss.str();
           ss.str("");
           banan.push(s);
        }
        else{
        string first = banan.top();
        banan.pop();
        fir_double = atof(first.c_str());
        if(poliz[i] == 'l'){
            if(sec_double <= 0) {throw "2 аргумент логарифма должен быть > 0";}
            if(fir_double < 0 || fir_double == 1) throw "1 аргумент логарифма > 0 и != 1";
            ans = logarithm(fir_double,sec_double);
        }
        else if(!if_float(second) && !if_float(first)){
            string cnt;
            llong mem_one(first);
            llong mem_two(second);
            if(poliz[i] == '+') {
                llong res(res.add(mem_one,mem_two));
                cnt = res.print();
            }
           else if(poliz[i] == '-') {
                llong res(res.sub(mem_one,mem_two));
                cnt = res.print();
           }
           else if(poliz[i] == '*') {
                llong res(res.mulitary(mem_one,mem_two));
                cnt = res.print();
           }
           else if(poliz[i] == '/') {
                int tmp = 0,m = 1;
                for(int i = second.size() - 1; i >= 0; --i){
                  if(i == 0 && (second[i] == '-' || second[i] == '+')) continue;
                  tmp += (second[i] -'0') *m;
                  m *= 10;
                }
                if(second[0] == '-') tmp *= (-1);
                if(tmp == 0){
                    throw "я вам запрещаю делить на 0";
                }
                ans = mem_one.delenie(mem_one,tmp);
           }
           else if(poliz[i] == '^') {
                int tmp = 0,m = 1;
                if(second[0] == '-'){throw "нельзя возводить в отрицательную степень";}
                for(int i = second.size() - 1; i >= 0; --i){
                  if(i == 0 && second[i] == '+')continue;
                  tmp += (second[i] -'0') *m;
                  m *= 10;
                }
                llong res(res.stepen(mem_one,tmp));
                cnt = res.print();
           }
           if(poliz[i] != '/') {
                banan.push(cnt);
           }
        }
        else{
           if(poliz[i] == '+') ans = sec_double + fir_double;
           else if(poliz[i] == '-') ans = fir_double - sec_double;
           else if(poliz[i] == '*') ans = sec_double * fir_double;
           else if(poliz[i] == '/') {
               if(sec_double == 0){
                   throw "я вам запрещаю делить на 0";
               }
               ans = fir_double/sec_double;
           }
           else if(poliz[i] == '^') {
               if(sec_double < 0) {throw "нельзя возводить в отрицательную степень";}
              ans = pow(fir_double,sec_double);
           }
          }
          if(!(!if_float(second) && !if_float(first)) || poliz[i] == 's' || poliz[i] == 'c' || poliz[i] == '/' || poliz[i] == 'l'){
            ss << ans;
            s = ss.str();
            ss.str("");
            banan.push(s);
          }
        }
        i++;
      }
    }
    return banan.top();
}


