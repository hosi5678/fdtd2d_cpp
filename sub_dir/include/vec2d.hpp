#ifndef EE24465A_0090_4BA1_8A3B_5530A95BF437
#define EE24465A_0090_4BA1_8A3B_5530A95BF437

// #include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream> // fileの書き込みに使用する
#include <iomanip>  // fileの書き込みに使用する
#include <string>
#include <algorithm>
#include <iostream>

#include "parameter.hpp"

class vec2d {

   protected:

   public:
      int ylength ;
      int xlength ;

      std::vector<std::vector<double>> vec;

      // default constructor
      vec2d(){}

      // constructor with argument
      vec2d(int _ylength, int _xlength): ylength(_ylength),xlength(_xlength),vec(_ylength,std::vector<double>(_xlength,0.0)) {}

      // constructor with vector
      vec2d(std::vector<std::vector<double>> obj):vec2d(obj.size(),obj[0].size()){
         for(size_t j=0; j<obj.size(); j++) {
            for (size_t i=0; i<obj[0].size(); i++) {
               vec[j][i]=obj[j][i];
            }
         }
      }

      // copy constructor
      vec2d(const vec2d& obj){

         if(this!=&obj){
            this->vec=obj.vec;
         }
      }

      // 代入演算子A=B
      virtual vec2d& operator=(const vec2d& obj);

      // vectorの代入A=vector
      virtual vec2d& operator=(const std::vector<std::vector<double>> obj);

      // 値の代入A=d
     virtual vec2d operator=(const double d);

     // 2次元の加算C=A+B
     virtual vec2d operator+(const vec2d& obj);

     // 2次元の要素同士の掛け算C=A*B
     virtual vec2d operator*(const vec2d& obj);

     // 2次元の要素同士の引き算C=A-B
     virtual vec2d operator-(const vec2d& obj);

     // 係数の掛け算C=A*d
     virtual vec2d operator*(const double d);

     // 係数の掛け算C=d*A
      friend vec2d operator*(const double d,vec2d& obj) {
         return obj*d;
      }

     // 2次元の要素同士の割り算
      virtual vec2d operator/(const vec2d& obj);

      // 係数による割り算
      virtual vec2d operator/(const double d);

      // 係数による割り算
      friend vec2d operator/(const double d, vec2d& obj){
         return obj/d;
      }

      // x方向の差分
      virtual vec2d diff_x(const vec2d& obj) const;

      // y方向の差分
      virtual vec2d diff_y(const vec2d& obj) const;

      // xyの軸成分の反転
      virtual vec2d invxy(const vec2d& obj) const;


   // setter,getter
      // 行の長さをsetする
      void setylength(int _ylength) {
         ylength=_ylength;
      }

      // 列の長さをsetする
      void setxlength(int _xlength) {
         xlength=_xlength;
      }

      // 行の長さを返却する
      size_t getylength(void) {
         return vec.size();
      }

      // 列の長さを返却する
      int getxlength(void) {
         return vec[0].size();
      }

      double getvalue(int y, int x) const;

      // vectorの内容をfile出力する
      void createFile(const int _precision,const char *file_path,const char* file_name);

      // vectorの値を出力する
      void show(const int precisition) const;
      void show() const;

      vec2d getthis(){
         return *this;
      }

      std::vector<std::vector<double>> getvector() const {
        
         // std::vector<std::vector<double>> temp=(*this).vec;

         return (*this).vec;

      } 



};  // class plane, vec2d

/*
   =のときは*thisを使う
   その他のoperatorのときはtempを使う
*/

// vector2dの内容を出力する
void vec2d::show() const {
   for(const auto& row:vec) {
      for(const auto& elem:row) {
         std::cout << elem << " ," ;
      }
      std::cout << std::endl;
   }
}

// 受け取った精度でvectorの要素を出力する
void vec2d::show(const int precision) const {

   for (const auto& row : vec) {
      for (const auto& elem : row) {
         std::cout << std::fixed << std::setprecision(precision) << elem << " " ;
      }
         std::cout << std::endl;
   }

}

// 代入演算子 A = B
vec2d& vec2d::operator=(const vec2d& obj) {

   if (this != &obj) {
      this->vec=obj.vec;
   }

   return *this;
}

// vectorの代入 (A=vector) (実体を渡す)
vec2d& vec2d::operator=(const std::vector<std::vector<double>> obj) {

   this->vec.resize(obj.size(),std::vector<double>(obj[0].size()));

   // vec2d temp=obj; 

   if((this)->vec!=obj) {
      if (obj.empty()) {
         this->vec.clear();
         return *this;
      }

      this->vec=obj;
   }

   return *this;
}

// 値の代入A=d
vec2d vec2d::operator=(const double d){

   size_t ylength=getylength();
   size_t xlength=getxlength();

   // std::cout << "vec.size():" <<(*this).vec.size() << std::endl;
   // std::cout << "vec.[0].size():" << (*this).vec[0].size() << std::endl;

   // vec2d temp;
   // (*this).vec.clear();
   // (*this).vec.resize((*this).vec.size(),std::vector<double>((*this).vec[0].size()));

   for(size_t j=0; j<ylength; j++) {
      for(size_t i=0; i<xlength; i++) {
         (*this).vec[j][i]=d;
      }
   }
   return *this;
   // return temp;
}

     // 2次元の加算C=A+B
vec2d vec2d::operator+(const vec2d& obj) {

   size_t ylength=getylength();
   size_t xlength=getxlength();

   vec2d temp(ylength,xlength);   // vec.resize(obj.vec.size(),std::vector<double>(obj.vec[0].size()));

   for(size_t j=0; j<ylength; j++) {
      for(size_t i=0; i<xlength; i++) {
         temp.vec[j][i]=(*this).vec[j][i]+obj.vec[j][i];
      }
   }

   // return *this;
   return temp;

};

// 2次元の要素同士の引き算C=A-B
vec2d vec2d::operator-(const vec2d& obj) {

   size_t ylength=getylength();
   size_t xlength=getxlength();

   vec2d temp(ylength,xlength) ;

   for (size_t j=0; j<ylength; j++) {
      for (size_t i=0; i<xlength; i++) {
         temp.vec[j][i]=(*this).vec[j][i]-obj.vec[j][i];
      }
   }

   // return *this;

   return temp;
}


// 2次元の掛け算C=A*B
vec2d vec2d::operator*(const vec2d& obj) {
   // vec.resize(obj.vec.size(),std::vector<double>(obj.vec[0].size()));

   size_t ylength=getylength();
   size_t xlength=getxlength();

   vec2d temp(ylength,xlength);

   for(size_t j=0; j<ylength; j++) {
      for(size_t i=0; i<xlength; i++) {
         temp.vec[j][i]=(*this).vec[j][i]*obj.vec[j][i];
      }
   }

   // return *this;
   return temp;
}

// 係数の掛け算C=A*d

vec2d vec2d::operator*(double scalar) {

   size_t ylength=getylength();
   size_t xlength=getxlength();

    vec2d temp(ylength,xlength);

    for (size_t j = 0; j < ylength; ++j) {
        for (size_t i = 0; i < xlength; ++i) {
            temp.vec[j][i] = this->vec[j][i] * scalar;
        }
    }
    return temp;
}


// vec2d vec2d::operator*(const double d) const {

//    vec2d temp;

//    temp.vec.resize((*this).vec.size(),std::vector<double>((*this).vec[0].size()));


//    for (size_t j=0; j<(*this).vec.size(); j++) {
//       for(size_t i=0; i<(*this).vec[0].size(); i++) {
//          temp.vec[j][i]=( (*this).vec[j][i] ) *d;
//       }
//    }

//    // return *this;
//    return temp;
// }

// 2次元の要素同士の割り算
vec2d vec2d::operator/(const vec2d& obj) {
    // サイズが一致するか確認
    if (this->vec.size() != obj.vec.size() || this->vec[0].size() != obj.vec[0].size()) {
        throw std::invalid_argument("Dimensions of vec2d objects do not match for division.");
    }

   size_t ylength=getylength();
   size_t xlength=getxlength();

   vec2d temp(ylength,xlength);


    for (size_t j = 0; j < ylength; j++) {
        for (size_t i = 0; i < xlength; i++) {
            if (obj.vec[j][i] == 0) {
                throw std::runtime_error("Division by zero in vec2d::operator/.");
            }
            temp.vec[j][i] = this->vec[j][i] / obj.vec[j][i];
        }
    }

    return temp;
}


// vec2d vec2d::operator/(vec2d& obj) const {

//    vec2d temp;

//    temp.vec.resize(obj.vec.size(),std::vector<double>(obj.vec[0].size()));

//    for (size_t j=0; j<obj.vec.size(); j++) {
//       for (size_t i=0; i< obj.vec[0].size(); i++) {
//          temp.vec[j][i]=(*this).vec[j][i]/obj.vec[j][i];
//       }
//    }
//    // return *this;
//    return temp;
// }

// 係数による割り算
vec2d vec2d::operator/(const double d) {

   size_t ylength=getylength();
   size_t xlength=getxlength();

   vec2d temp(ylength,xlength);

   vec2d obj=getthis();

   for (size_t j=0; j<ylength; j++) {
      for (size_t i=0; i<xlength; i++) {
         temp.vec[j][i]=obj.vec[j][i]/d;
      }
   }

   // return *this;
   return temp;
}

// x方向の差分
vec2d vec2d::diff_x(const vec2d& obj) const {

   vec2d temp(obj.vec.size(),obj.vec[0].size()-1);

   for (size_t j=0; j<obj.vec.size(); j++) {
      for (size_t i=0; i<obj.vec[0].size()-1; i++){
         temp.vec[j][i]=obj.vec[j][i+1]-obj.vec[j][i];
      }
   }

   return temp;

}

// y方向の差分
vec2d vec2d::diff_y(const vec2d& obj) const {

   vec2d temp(obj.vec.size()-1,obj.vec[0].size());

   for (size_t j=0; j<obj.vec.size()-1; j++) {
      for (size_t i=0; i<obj.vec[0].size(); i++){
         temp.vec[j][i]=obj.vec[j+1][i]-obj.vec[j][i];
      }
   }

   return temp;

}

// xy軸成分の反転
vec2d vec2d::invxy(const vec2d& obj) const {

   vec2d temp(obj.vec[0].size(),obj.vec.size());

   for (size_t j=0; j<obj.vec.size(); j++) {
      for (size_t i=0; i<obj.vec[0].size(); i++) {
         temp.vec[i][j]=obj.vec[j][i];
      }
   }

   return temp;

}


void vec2d::createFile(const int _precision, const char *file_path, const char* file_name) {

      std::string path= std::string(file_path)+"/"+std::string(file_name);

      std::ofstream file(path);
         if (!file.is_open()) {
            std::cerr << path<< ": " << "を開けませんでした。" << std::endl;
            exit(1);
         }

    // 受け取った精度を設定する
    file << std::fixed << std::setprecision(_precision);

    // データをファイルに書き込む
   size_t ylength=getylength(); //(*this).vec.size();
   size_t xlength=getxlength(); //(*this).vec[0].size();

   // std::cout << "ylength: " << ylength << std::endl;
   // std::cout << "xlength: " << xlength << std::endl;

   // x軸の書き込み
   for (size_t i=0; i<xlength; i++){
      if(i==0) file << '\t';
      if(i!=xlength-1) file << i << ',';
      if(i==(xlength-1)) file << i << std::endl;
   }

   // y軸と値の書き込み

    for (size_t j = 0; j < ylength; j++) {

      file << j << ',';

      for (size_t i=0; i< xlength;i++){

         if(i!=(xlength-1)){
            file << (*this).vec[j][i] << ',';
         } else {
            file << (*this).vec[j][i] << std::endl;

         }

      }
   }

    // ファイルを閉じる
   file.close();

   //  std::cout << path <<"に書き込みが完了しました。" << std::endl;

}

double vec2d::getvalue(int y, int x) const {
   return (*this).vec[y][x];
}

#endif /* EE24465A_0090_4BA1_8A3B_5530A95BF437 */
