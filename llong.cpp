#include "llong.h"

llong :: llong (string s){
        int k = 0;
        if(s[0] == '-'){
            k = 1;
            this->sign_num_ = '-';
        }
        else this->sign_num_ = '+';
        if(s[0] == '+') k = 1;
       for(int i =s.size() - 1; i >= k; i-=4){
          int cnt = 0,m = 1;
          for(int j = i; j > i - 4 && j >= k; --j){
              cnt += (s[j]-'0')*m;
              m *= 10;
          }
          val_.push_back(cnt);
       }
}
 vector <int> llong ::get_vec(){return val_;}
llong :: llong(vector <int> &val,char sign){
        this->val_ = val;
        this->sign_num_ = sign;
    }
llong :: llong(const llong &value){
        val_ = value.val_;
        sign_num_ = value.sign_num_;
    }
int llong :: size(){
        return val_.size();
    }
 int llong ::operator[](int i){
        return val_[i];
    }
int llong :: back(){return val_.back();}
void llong ::  pop_back(){val_.pop_back();}
void llong ::  push_back(int i){val_.push_back(i);}
char llong :: get_sign(){return sign_num_;}
void llong :: set_sign(char sign){sign_num_ = sign;}
std::vector<int>::iterator llong :: begin(){return val_.begin();}
std::vector<int>::iterator llong :: end(){return val_.end();}
vector <int> llong :: naive_mul(vector <int> &first_num,vector <int> &second_num){
        int len = first_num.size();
        vector <int> res(2*len);
        for(int i = 0; i < len; i++){
            for(int j = 0; j < len; j++){
                res[i+j] += first_num[i]*second_num[j];
            }
        }
        return res;
    }
void llong :: norm(vector <int> &res) {
        for(int i = 0; i < res.size(); i++) {
            res[i+1] += res[i]/base;
            res[i] %= base;
        }
    }
string llong :: print(){
        ostringstream ss;
        string s;
        for(int i = val_.size() - 1; i >= 0; --i){
            //cout << val[i] <<" ";
            ss << val_[i];
            string cnt = ss.str();
            ss.str("");
            for(int i = cnt.size(); i < 4; i++){
                cnt = '0'+cnt;
            }
            s += cnt;
        }
        int k = 1;
        string ans;
        ans += sign_num_;
        for(int i =0; i < s.size(); i++){
            if(k && s[i] == '0') continue;
            else{
                k = 0;
                ans += s[i];
            }
        }
        if(k) ans += '0';
        return ans;
    }
vector <int> llong :: karatsuba_mul(vector <int> &first, vector <int> &second)
    {
        int len = first.size();
        vector <int> res(2 * len);
        if(len <= LEN) {
            return naive_mul(first,second);
        }
        int k = len/2;
        vector <int> first_left {first.begin(),first.begin() + k};
        vector <int> first_right {first.begin() + k,first.end()};
        vector <int> second_left {second.begin(),second.begin() + k};
        vector <int> second_right {second.begin() + k,second.end()};
        vector <int> left_mul = karatsuba_mul(first_left,second_left);
        vector <int> right_mul = karatsuba_mul(first_right,second_right);
        vector <int> first_left_right(k);
        vector <int> second_left_right(k);
        for(int i =0; i < k; i++){
            first_left_right[i] = first_left[i] + first_right[i];
            second_left_right[i] = second_left[i] + second_right[i];
        }
        vector <int>  mul_all = karatsuba_mul(first_left_right,second_left_right);
        for(int i = 0; i < len; i++) mul_all[i] -= right_mul[i] + left_mul[i];
        for(int i = 0; i < len; i++) res[i] = right_mul[i];
        for(int i = len; i < 2*len; i++) res[i] = right_mul[i - len];
        for(int i = k; i < len + k; i++) res[i] += mul_all[i-k];
        return res;
    }
llong& llong :: mulitary(llong &first, llong &second){
        vector <int> v1 = first.get_vec();
        vector <int> v2 = second.get_vec();
        int v1_size = v1.size();
        int v2_size = v2.size();
        if(v1_size > v2_size){
            for(int i = v2_size; i < v1_size; i++){
                v2.push_back(0);
            }
        }
        else if(v2_size > v1_size){
          for(int i = v1_size; i < v2_size; i++){
                v1.push_back(0);
            }
        }
        //for(int i =0; i < v1.size();i++) cout << v1[i] <<" ";
        //cout <<'\n';
        //for(int i =0; i < v2.size();i++) cout << v2[i] <<" ";
        vector <int> ans = karatsuba_mul(v1,v2);
        norm(ans);
        char ans_char;
        if(first.get_sign() == second.get_sign()) ans_char = '+';
        else ans_char = '-';
        llong* ans_end = new llong(ans, ans_char);
        return *(ans_end);
    }
bool llong :: compare(llong &first,llong &second){
    if(first.size() > second.size()) return 1;
    if(first.size() < second.size()) return 0;
    for(int i =first.size()-1; i >= 0; --i){
        if(first[i] > second[i]) return 1;
        if(first[i] < second[i]) return 0;
    }
    return 1;
   }
void llong :: swap(llong &first,llong &second){
       llong cnt(first);
       first = second;
       second = cnt;
   }
llong llong ::add(llong &first,llong &second){
    if(first.get_sign() == second.get_sign()){
     if(second.size() > first.size()) swap(first,second);
     int m = 0;
     vector <int> res(first.size());
     for(int i =0; i < first.size() || m; i++){
        if(i < second.size()){
            res[i] = first[i] + m + second[i];
        }
        else res[i] = m + first[i];
        if(res[i] > base) m = 1;
        else m = 0;
        if(m) res[i] -=base;
    }
     if(first.get_sign() == '-') return {res,'-'};
     else return {res,'+'};
     }
     else{
        bool comp_res = compare(first,second);
       if(!comp_res) swap(first,second);
       vector <int> res(first.size());
       int m = 0;
       for(int i = 0; i < second.size() || m; i++){
        if(i < second.size()){
            res[i] = first[i] - m - second[i];
        }
        else res[i] = first[i] - m;
        //cout << res[i] <<'\n';
        if(res[i] < 0) m = 1;
        else m = 0;
        if(m) res[i] = res[i] + base;
        //cout << res[i] <<'\n';
        //cout << first[i] <<" " << second[i] <<'\n';
    }
      while (res.size() > 1 && res.back() == 0)
	  res.pop_back();
       if(first.get_sign() == '-') return {res,'-'};
       else return {res,'+'};
     }
   }
llong llong :: operator-() {
       llong cnt (*this);
       if(get_sign() == '-') cnt.set_sign('+');
       else cnt.set_sign('-');
       return cnt;
   }
llong llong :: sub(llong &first,llong &second){
      llong t = -second;
     return add(first,t);
   }
double llong :: delenie(llong &value,int b){
    ostringstream ss;
    string s;
    int m = 0,k = 0;
    if(b < 0) k = 1;
    b = abs(b);
    vector <int> res(value.size());
    for(int i = value.size() - 1; i >= 0; --i){
        int cnt = value[i] + m*base;
        res[i] = cnt/b;
        m = cnt%b;
    }
    while (res.size() > 1 && res.back() == 0)
	res.pop_back();
	string ans;
	for(int i = 0; i < res.size(); i++){
        ss << res[i];
        s = ss.str();
        ss.str("");
        ans += s;
	}
	double mm = double(m)/double(b);
	ss << mm;
	s = ss.str();
	s.erase(0,1);
	ans += s;
    if(value.get_sign() == '-' && k == 0 || value.get_sign() == '+' && k == 1) ans = '-' + ans;
	return atof(ans.c_str());
}
llong llong :: stepen(llong &value,int b){
    if(b == 1) return value;
    if(b%2) {
        llong cnt = stepen(value,b-1);
        return mulitary(value,cnt);
    }
    else {
        llong tmp = stepen(value,b/2);
        return mulitary(tmp,tmp);
    }
}
