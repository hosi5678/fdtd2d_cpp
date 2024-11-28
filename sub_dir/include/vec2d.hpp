#ifndef EE24465A_0090_4BA1_8A3B_5530A95BF437
#define EE24465A_0090_4BA1_8A3B_5530A95BF437

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream> // fileの書き込みに使用する
#include <iomanip>  // fileの書き込みに使用する
#include <string>
#include <algorithm>

#include <sys/stat.h>
#include <sys/types.h>

#include "parameter.hpp"

/*
   =のときは*thisを使う
   その他のoperatorのときはtempを使う
*/

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
               this->vec[j][i]=obj[j][i];
            }
         }

         (*this).ylength=obj.size();
         (*this).xlength=obj[0].size();

      }

      // copy constructor
      vec2d(const vec2d& obj):vec(obj.vec){
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

      virtual vec2d& getthis(){return *this;}

      // x方向の差分
      // virtual vec2d diff_x(const vec2d& obj) const;

      // y方向の差分
      // virtual vec2d diff_y(const vec2d& obj) const;

      // xyの軸成分の反転
      virtual vec2d invxy(const vec2d& obj) const;


   // setter,getter
      // 行の長さを返却する
      size_t getylength(void) {
         return vec.size();
      }

      // 列の長さを返却する
      int getxlength(void) {
         return vec[0].size();
      }

      // 2d vecの値を返却する
      double getvalue(int y, int x) const;

      // vectorの内容をfile出力する
      void createFile(int precision, const std::string& dir,const std::string& filename) const;

      // vectorの値を出力する
      void show(const int precisition) const;
      void show() const;

      std::vector<std::vector<double>> getvector() const {
         return (*this).vec;
      }

      // 平面の周囲をdで囲う
      vec2d wrap_around(double d);

      // y軸の中心行を削除する
      vec2d dropcenter_y(const vec2d& obj) const ;

      // x軸の中心行を削除する
      vec2d dropcenter_x(const vec2d& obj) const ;

      // 2次元のcsvファイル(軸ラベルあり)を読み込む 引数:string& 返り値:vec2d
      vec2d readCSV(const std::string& filename) const;


};  // class vec2d

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

   // 自己代入を防ぐ
   if (this != &obj) {
      this->vec=obj.vec;
   }

   return *this;
}

// vectorの代入 (A=vector) (実体を渡す)
vec2d& vec2d::operator=(const std::vector<std::vector<double>> obj) {

   // もしobjの内容が空ならエラーを出す
   if(obj.empty()){
      throw runtime_error("in operator, obj is empty.");
   }

   // 引数objのサイズに*thisのサイズをresizeする
   this->vec.resize(obj.size(),std::vector<double>(obj[0].size()));


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

   for(size_t j=0; j<ylength; j++) {
      for(size_t i=0; i<xlength; i++) {
         (*this).vec[j][i]=d;
      }
   }
   return *this;
}

     // 2次元の加算C=A+B
vec2d vec2d::operator+(const vec2d& obj) {

    // 入力ベクトルが空かどうかを確認
    if (obj.vec.empty() || obj.vec[0].empty()) {
        throw std::runtime_error("Input vector is empty or malformed.");
    }

    // 呼び出し元とobjのサイズの違いを確認
    if (obj.vec.size()!=(*this).vec.size()) {
      throw std::runtime_error("Input vector size(y) is different with *this.");
    }

    if (obj.vec[0].size()!=(*this).vec[0].size()) {
      throw std::runtime_error("Input vector size(x) is different with *this.");
    }

   size_t ylength=getylength();
   size_t xlength=getxlength();

   vec2d temp(ylength,xlength);   // vec.resize(obj.vec.size(),std::vector<double>(obj.vec[0].size()));

   for(size_t j=0; j<ylength; j++) {
      for(size_t i=0; i<xlength; i++) {
         temp.vec[j][i]=(*this).vec[j][i]+obj.vec[j][i];
      }
   }

   return temp;

};

// 2次元の要素同士の引き算C=A-B
vec2d vec2d::operator-(const vec2d& obj) {

    // 入力ベクトルが空かどうかを確認
    if (obj.vec.empty() || obj.vec[0].empty()) {
        throw std::runtime_error("Input vector is empty or malformed.");
    }

    // 呼び出し元とobjのサイズの違いを確認
    if (obj.vec.size()!=(*this).vec.size()) {
      throw std::runtime_error("Input vector size(y) is different with *this.");
    }

    if (obj.vec[0].size()!=(*this).vec[0].size()) {
      throw std::runtime_error("Input vector size(x) is different with *this.");
    }


   size_t ylength=getylength();
   size_t xlength=getxlength();

   vec2d temp(ylength,xlength) ;

   for (size_t j=0; j<ylength; j++) {
      for (size_t i=0; i<xlength; i++) {
         temp.vec[j][i]=(*this).vec[j][i]-obj.vec[j][i];
      }
   }

   // 実体を返す
   return temp;
}


// 2次元の掛け算C=A*B
vec2d vec2d::operator*(const vec2d& obj) {

    // 入力ベクトルが空かどうかを確認
    if (obj.vec.empty() || obj.vec[0].empty()) {
        throw std::runtime_error("Input vector is empty or malformed.");
    }

    // 呼び出し元とobjのサイズの違いを確認
    if (obj.vec.size()!=(*this).vec.size()) {
      throw std::runtime_error("Input vector size(y) is different with *this.");
    }

    if (obj.vec[0].size()!=(*this).vec[0].size()) {
      throw std::runtime_error("Input vector size(x) is different with *this.");
    }

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

// 2次元の要素同士の割り算
vec2d vec2d::operator/(const vec2d& obj) {

    // 入力ベクトルが空かどうかを確認
    if (obj.vec.empty() || obj.vec[0].empty()) {
        throw std::runtime_error("Input vector is empty or malformed.");
    }

    // 呼び出し元とobjのサイズの違いを確認
    if (obj.vec.size()!=(*this).vec.size()) {
      throw std::runtime_error("Input vector size(y) is different with *this.");
    }

    if (obj.vec[0].size()!=(*this).vec[0].size()) {
      throw std::runtime_error("Input vector size(x) is different with *this.");
    }

   // size_t ylength=getylength();
   // size_t xlength=getxlength();

   vec2d temp(obj.vec.size(),obj.vec[0].size());

    for (size_t j = 0; j < obj.vec.size(); j++) {
        for (size_t i = 0; i < obj.vec[0].size(); i++) {

            // もし引数objが0ならerrorを出す。
            if (obj.vec[j][i] == 0) {
                throw std::runtime_error("Division by zero in vec2d::operator/.");
            }
            // 除算する
            temp.vec[j][i] = this->vec[j][i] / obj.vec[j][i];
        }
    }

    return temp;
}

// 係数による割り算
vec2d vec2d::operator/(const double d) {

   if (d==0.0) {
      throw std::runtime_error("Division by zero in vec2d::operator/(const d)");
   }

   size_t ylength=getylength();
   size_t xlength=getxlength();

   for (size_t j=0; j<ylength; j++) {
      for (size_t i=0; i<xlength; i++) {
         (*this).vec[j][i]=(*this).vec[j][i]/d;
      }
   }

   return *this;

}

// xy軸成分の反転
vec2d vec2d::invxy(const vec2d& obj) const {

   if(this==&obj) {
      throw std::runtime_error("Input vector(obj) is same with *this .");
   }

    // 入力ベクトルが空かどうかを確認
    if (obj.vec.empty() || obj.vec[0].empty()) {
        throw std::runtime_error("Input vector is empty or malformed.");
    }

   // objectの作成
   vec2d temp(obj.vec[0].size(),obj.vec.size());

   for (size_t j=0; j<obj.vec.size(); j++) {
      for (size_t i=0; i<obj.vec[0].size(); i++) {
         temp.vec[i][j]=obj.vec[j][i];
      }
   }

   // objectの返却
   return temp;

}

vec2d vec2d::dropcenter_y(const vec2d& obj) const {

    // 入力ベクトルが空かどうかを確認
    if (obj.vec.empty() || obj.vec[0].empty()) {
        throw std::runtime_error("Input vector is empty or malformed");
    }

    // 中心行の削除を行えない場合のエラーチェック
    if (obj.vec.size() <= 1) {
        throw std::runtime_error("Cannot drop center row from a vector with only one row");
    }

   // 中心の行の位置を求める
    int center_y=(obj.vec.size()-1)/2;


    // 中心行を除いた新しいベクトルの作成
    vec2d temp(obj.vec.size() - 1, obj.vec[0].size());

    size_t temp_row = 0; // temp の行インデックス

    for (size_t j = 0; j < obj.vec.size(); j++) {

        if ((int)j == center_y) {
            // 中心行はスキップ
            continue;
        }
        for (size_t i = 0; i < obj.vec[0].size(); i++) {
            temp.vec[temp_row][i] = obj.vec[j][i];
        }
        temp_row++; // temp の次の行に進む
    }

    return temp;
}

vec2d vec2d::dropcenter_x(const vec2d& obj) const {

    // 入力ベクトルが空かどうかを確認
    if (obj.vec.empty() || obj.vec[0].empty()) {
        throw std::runtime_error("Input vector is empty or malformed");
    }

    // 中心列の削除を行えない場合のエラーチェック
    if (obj.vec[0].size() <= 1) {
        throw std::runtime_error("Cannot drop center column from a vector with only one column");
    }

  // 中心列の位置を求める
    int center_x=(obj.vec[0].size()-1)/2;

    // 中心列を除いた新しいベクトルの作成
    vec2d temp(obj.vec.size(), obj.vec[0].size()-1);

    for (size_t j = 0; j < obj.vec.size(); j++) {

         // 削除後の列インデックスは毎行リセット
         int temp_column=0;

        for (size_t i = 0; i < obj.vec[0].size(); i++) {
            
               // 中心列をスキップ
            if ((int)i== center_x) continue;

            temp.vec[j][temp_column] = obj.vec[j][i];
            temp_column++;
        }
    }

   // vec2dの実体を返却する
    return temp;
}

// directoryが無ければ作成する
void createDirectory(const std::string& dir) {

   struct stat info;

   // もしdirectoryが空なら新規作成する
   if(stat(dir.c_str(),&info)!=0) {
      mkdir(dir.c_str(),0755);
   }

}

void vec2d::createFile(const int precision, const std::string& directory, const std::string& filename) const {

   // もしdirectoryがなければ新規作成する
    createDirectory(directory);

    std::ofstream file(directory + "/" + filename);

   // もしファイルが開けなかったらエラーを表示する
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << directory + "/" + filename << std::endl;
        return;
    }

   // 1行1列目はタブを書き込む
   file << '\t';

   for (size_t i=0; i<(*this).vec[0].size(); i++){
      if(i!=(*this).vec[0].size()-1){
         file << i << ',';
      } else {
         file << i << '\n';
      }
   }

   for (size_t j=0; j<(*this).vec.size(); j++) {
      file << j << ',';
      for (size_t i=0; i<(*this).vec[0].size(); i++){
         file << std::fixed << std::setprecision(precision) << (*this).vec[j][i] << ",";
      }
         file << "\n";

   }

   //  for (const auto& row : vec) {
   //      for (const auto& val : row) {
   //          file << std::fixed << std::setprecision(precision) << val << ",";
   //      }
   //      file << "\n";
   //  }

   //  std::cout << "file written successfully: " << directory + "/" + filename << std::endl;
}


double vec2d::getvalue(int y, int x) const {
   return (*this).vec[y][x];
}

vec2d vec2d::wrap_around(double d) {

   vec2d temp((*this).vec.size()+2, (*this).vec[0].size()+2);

   // temp を d で初期化
   temp=d;

   for (size_t j=1; j<temp.vec.size()-1; j++) {
      for (size_t i=1; i<temp.vec[0].size()-1; i++) {
         temp.vec[j][i]=(*this).vec[j-1][i-1];
      }
   }

   // objectの返却
   return temp;

}      
// 2次元のcsvファイル(軸ラベルあり)を読み込む 引数:string& 返り値:vec2d
vec2d vec2d::readCSV(const std::string& filename) const{
      
   std::ifstream file(filename);

      // もしファイルが開けなかったらエラーを出す。
      if (!file.is_open()) {
         throw std::runtime_error("ファイルを開けませんでした: " + filename);
      }

      vec2d result;

      std::vector<std::vector<double>> csv_data;

      std::vector<int> y_labels;
      std::vector<int> x_labels;

      std::string line;
      bool is_first_line = true;

      while (std::getline(file, line)) {

        std::stringstream ss(line);
        std::string cell;

        if (is_first_line) {
            // 1行目の処理
            is_first_line = false;

            // 最初のタブ文字をスキップ
            std::getline(ss, cell, '\t');

            // x_labels を取得
            while (std::getline(ss, cell, ',')) {
                x_labels.push_back(std::stoi(cell));
            }

        } else {
            // 2行目以降の処理
            std::vector<double> row_data;

            // 1列目 (y_labels) を取得
            // カンマは読み飛ばす
            std::getline(ss, cell, ',');
            
            y_labels.push_back(std::stoi(cell));

            // 残りのデータを取得
            while (std::getline(ss, cell, ',')) {
                row_data.push_back(std::stod(cell));
            }

            // csvデータを2次元vectorにpushする。
            csv_data.push_back(row_data);
        }
    }

   // fileを閉じる
    file.close();

   // vec2dにしてresultを返却する
   result=csv_data;

   return result;
}


/*
gdb ./build/main
(gdb) break vec2d::invxy
(gdb) run
(gdb) print other
(gdb) print *this
*/
#endif /* EE24465A_0090_4BA1_8A3B_5530A95BF437 */
