//#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <stack>
#include <cmath>
const int base = 10000;
const int LEN = 512;

using namespace std;
class llong{
public:
    llong(string s);
    vector <int> get_vec();
    llong(vector <int> &val,char sign);
    llong(const llong &value);
    int size();
    int operator[](int i);
    int back();
    void pop_back();
    void push_back(int i);
    char get_sign();
    void set_sign(char sign);
    std::vector<int>::iterator begin();
    std::vector<int>::iterator end();
    vector <int> naive_mul(vector <int> &first_num,vector <int> &second_num);
    void norm(vector <int> &res);
    string print();
    vector <int> karatsuba_mul(vector <int> &first, vector <int> &second);
    llong& mulitary(llong &first, llong &second);
    bool compare(llong &first,llong &second);
   void swap(llong &first,llong &second);
   llong add(llong &first,llong &second);
   llong operator-();
   llong sub(llong &first,llong &second);
   llong stepen(llong &value,int b);
  double delenie(llong &value,int b);
private:
    vector <int> val_;
    char sign_num_;
};
