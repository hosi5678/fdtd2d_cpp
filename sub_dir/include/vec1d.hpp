#ifndef B3F1C6A7_2F38_475D_88BE_9ED36031DF20
#define B3F1C6A7_2F38_475D_88BE_9ED36031DF20

// 1次元のvectorの操作

// #include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream> // fileの書き込みに使用する
#include <iomanip>  // fileの書き込みに使用する
#include <string>
#include <algorithm>

// #include "../../matplotlib-cpp/matplotlibcpp.h"
// namespace plt=matplotlibcpp;

class vec1d{

   protected :


   public:

      std::vector <double> vec;


      // default coonstructor 
      vec1d():vec(static_cast<int>(0)) {
         // std::cout << "called default constructor.(1 dim),vec.size():"<< vec.size() << std::endl;
      }

      // constructor with argument
      vec1d(int _length):vec(static_cast<int>(_length)){
         // std::cout << "called constructor with argument(1 dim),vec.size():" << vec.size() << std::endl;
      };

      // copy constructor
      vec1d(const vec1d& obj):vec(obj.vec) {
         // std::cout << "called copy constructor(1 dim)." << std::endl;
      }

      // constructor with vector
      vec1d(std::vector<double> v):vec(v.size()) {
         for(size_t i=0; i<v.size(); i++) {
            (*this).vec[i]=v[i];
         }
      }

      // コピー代入演算子の明示的な定義(A=B)
      virtual vec1d& operator=(const vec1d& obj) ;

   // vectorの代入
   // virtual vec1d& operator=(const std::vector<double>& obj) ;

   // vectorの代入
   virtual vec1d& operator=(const std::vector<double> obj);

   // vec[i]=d
   virtual vec1d& operator=(const double& d) ;

   // <vector> 同士の加算
   // 参照ではなく、実体を返すとスコープを抜けても消失しない。
   // 参照ならば、staticとする。
   virtual vec1d operator+(const vec1d& obj) const ;

   // c=A+d
   vec1d operator+(const double param) const ;

   // c=d+A
   friend vec1d operator+(const double param, const vec1d& other) {
      // obj+paramなのでオーバロード先もそのようにする。
      return other+param;
   }

// vec同士の減算
   // <vector> 同士の加算
   // 参照ではなく、実体を返すとスコープを抜けても消失しない。
  virtual vec1d operator-(const vec1d& obj) const;

   // vec同士の乗算 // C=A*B
   virtual vec1d operator*(const vec1d& obj) const;

   // vector 同士の除算 // C=A/B
   virtual vec1d operator/(const vec1d& obj) const;

   // C=A*_param
   virtual vec1d operator*(const double param) const;

   // C=_param*A
   friend vec1d operator*(const double _param,const vec1d& other) {
      return other*_param;
   }

   // differential of vector(vectorの差分を返す)(元のvectorの長さに影響を与えない)
   virtual vec1d differential(const vec1d& obj) const ;

   // vectorとvectorを結合する。
   virtual vec1d merge(const vec1d& obj) const ;

   // vector の反転
   virtual vec1d invert(const vec1d& obj) const;

   // vector の補間を返却する
   virtual vec1d interpolation(const vec1d& obj) const;

   // vectorの値を出力する
   void show(const int precisition) const;

   // vector同士のsize errorを返す
   virtual void show_throw_size_error(const vec1d& obj1,const vec1d& obj2) const;

   // vector同士のaddress errorを返す
   virtual void show_throw_address_error(const vec1d& arg1,const vec1d& arg2) const;

   // vecのpositionにvalをsetする(setter)
   virtual void setValue(const size_t position,const double val);

   // vec のpoint番目の値を返却する
   virtual double getValue(double point) const;

   // double を引数にとり、vecの末尾にdoubleをpushする。
   virtual vec1d& push_front(const double d) ;

   // double を引数にとり、vecの先頭にdoubleをpushする。
   virtual vec1d& push_back(const double d) ;

   // vectorの両端に値を挿入する。(sizeは2大きくなる)
   virtual vec1d& push_both_sides(const double d) ;

   // vectorの両端の値を削除する。(sizeは2小さくなる)
   virtual vec1d pop_both_sides(const vec1d& obj) ;

   // 引数objの最初の要素を抜いてvectorの参照を返却する。
   virtual vec1d& pop_front(const vec1d& obj);

    // 引数objの最初の要素を抜いてvectorの参照を返却する。
   virtual vec1d& pop_back(const vec1d& obj);
  
   // vector自身を返す
   virtual const std::vector<double>& getvector() const;

   // 1dのvectorの内容を文字列出力する
   virtual string oss_str() const;
   
   // 精度とファイル名を与えて1d vectorの内容をファイルに書き込む
   virtual void createFile(const int _precision,const char* file_path,const char* file_name) const;
   virtual void createFile(const int _precision,const char* file_path,const char* file_name,const double d) const;

   // fileを読み込んで配列に格納する
   virtual vec1d readfile(const char* file_path,const char* file_name);

   // 与えられたオブジェクトの配列の要素のpeakを返す
   virtual vec1d get_peak(const vec1d& obj,int limit);

   // 与えられたオブジェクトをグラフにして、png形式で保存する。
   // virtual void create_png(const std::vector<double>& obj, const char* directory, const char* file_name,const char* _suptitle);

   // destructor
   virtual ~vec1d()=default;

}; // class declaration

// コピー代入演算子の明示的な定義(A=B)
 vec1d& vec1d::operator=(const vec1d& obj)  {
      
   // 自己代入の場合、自身を返却する
   if(this==&obj) return *this;

   vec.resize(static_cast<int>(0));

   for (size_t i=0; i<obj.vec.size(); i++) {
      vec.push_back(obj.vec[i]);
   }

   return *this;

}

// vectorの代入 (A=vector) (実体を渡す)
vec1d& vec1d::operator=(const vector<double> obj)  {

   vec.resize(static_cast<int>(0));

   // (*this).vec=obj;

   for(size_t i=0;i<obj.size(); i++) {
      vec.push_back(obj[i]);
   }

   return *this;

}

// A=d
vec1d& vec1d::operator=(const double& d) {
   for (size_t i=0; i<vec.size(); i++) {
      vec[i]=d;
   }
   return *this;
}

// vector同士の加算(C=A+B)
vec1d vec1d::operator+(const vec1d& obj) const {

      show_throw_size_error(vec,obj);
 
      vec1d result(obj.vec.size());

      // vec.resize(obj.vec.size());

      for(size_t i=0; i<obj.vec.size() ; i++ ) {
         result.vec[i]=vec[i]+obj.vec[i]; //=();
      }


   return result;
      // vec=result.vec;

      // return *this;
}

// C=A-B
vec1d vec1d::operator-(const vec1d& obj) const {

   show_throw_size_error(vec,obj);

   vec1d result(obj.vec.size());

   // vec.resize(static_cast<double>(0));

   for(size_t i=0; i<obj.vec.size() ; i++ ) {
      result.vec[i]=vec[i]-obj.vec[i];
   }

   return result;
   // return *this;
}

// C=A*B
vec1d vec1d::operator*(const vec1d& obj) const {

   show_throw_size_error(vec,obj);

   vec1d result(obj.vec.size());

   // vec.resize(static_cast<int>(0));

   for(size_t i=0;i<obj.vec.size();i++){
      result.vec[i]=vec[i]*obj.vec[i];
   }

   // vec=result.vec;

   return result;

   // return *this;
}

// vector C= A* double d
vec1d vec1d::operator*(const double param) const {

   vec1d result(static_cast<int>(vec.size()));

   for(size_t i=0; i<vec.size(); i++) {
      result.vec[i]=param*vec[i];
   }


   // return *this;
   return result;
}

// vector C=vector A /vector B
vec1d vec1d::operator/(const vec1d& obj) const {

   show_throw_size_error(vec,obj);

   // vec.resize(static_cast<int>(0));

   vec1d result(obj.vec.size());

   for (size_t i=0; i<obj.vec.size(); i++) {
      result.vec[i]=vec[i]/obj.vec[i];
   }

   return result;
   // return *this;

}

// あるpointにおけるvecの値を返す
double vec1d::getValue(double point) const {return vec[point];}

// A+param
vec1d vec1d::operator+(const double param) const {
   
   vec1d result(static_cast<double>(0));

   for(size_t i=0; i<vec.size(); i++) {
      result.vec.push_back(vec[i]+param);
   }

   return result;
}

// vectorの差分を返す
vec1d vec1d::differential(const vec1d& obj) const {

    vec1d result(static_cast<int>(0)); 

      for(size_t i=0;i< obj.vec.size()-1; i++) {
            result.vec.push_back(obj.vec[i+1]-obj.vec[i]);
      }

      // vec=result.vec;
   
   // return *this;

   return result;

}

// 自身のvectorを返却する。
const std::vector<double>& vec1d::getvector() const {return vec;}

// vector同士をマージする。
vec1d vec1d::merge(const vec1d& obj) const {

   // 自己代入の場合、自身を返す
   // if(this==&obj) return *this;

// 長さ0のvectorを用意する。
   vec1d result(static_cast<int>(0));

   // 自分の要素を代入する。
   for(size_t i=0; i<vec.size();i++) {
      result.vec.push_back(vec[i]);
   }

   // 引数のオブジェクトの要素を代入する。
   for(size_t i=0;i<obj.vec.size();i++){
      result.vec.push_back(obj.vec[i]);
   }

   // vec=result.vec;

   // 自身を返却する。
   // return *this;
   return result;
}

// vectorの反転を返す
vec1d vec1d::invert(const vec1d& obj) const {

   vec1d result(static_cast<double>(0));

   for(size_t i=0; i<vec.size(); i++){
      result.vec.push_back(obj.vec[obj.vec.size()-1-i]);
   }

   // vec=result.vec;

   // return *this;
   return result;

}

// vectorの補間を返却する。
vec1d vec1d::interpolation(const vec1d& obj) const {

   // objより長さが1つ少ないオブジェクトを用意する
   vec1d result(static_cast<int>(0));

   for(size_t i=0; i<obj.vec.size()-1; i++) {
      result.vec.push_back((obj.vec[i+1]+obj.vec[i])*0.5);
   }

   // vec=result.vec;

   // return *this;
   return result;
}

// 受け取った精度でvectorの要素を出力する
void vec1d::show(const int precision) const {

   std::cout << "(in show) vec.size=" << vec.size() << std::endl;

   for (size_t i=0;i<vec.size();i++){
      std::cout << std::fixed << std::setprecision(precision) << "vec["  << i << "]=" << vec[i] << std::endl; 
   }
}


void vec1d::show_throw_address_error(const vec1d& arg1,const vec1d& arg2) const {
   if(&arg1==&arg2){
      throw std::invalid_argument("address error.");
   }
}


void vec1d::show_throw_size_error(const vec1d& obj1,const vec1d& obj2) const {
      if (obj1.vec.size() != obj2.vec.size()){
         throw std::invalid_argument("size error:: obj1.size:"+to_string(obj1.vec.size())+" obj2.size:"+to_string(obj2.vec.size()));
      }
}

void vec1d::setValue(const size_t position,const double val)  {
      if(position>vec.size()) {
         throw std::invalid_argument("insert position error."+to_string(position));
      }
      vec[position]=val;
}

// 1dのvectorの内容を文字列出力する
string vec1d::oss_str() const {

      std::ostringstream oss;

      for(size_t i=0; i<vec.size();++i) {
         oss << vec[i];
         if (i!=vec.size()-1){
            oss  << ",";
         }
      }
      
      return oss.str();

}

// vectorの先頭に値を挿入する
 vec1d& vec1d::push_front(const double d) {

   vec.insert(vec.begin(),d);

   return *this;
 }

    // double を引数にとり、vecの先頭にdoubleをpushする。
vec1d& vec1d::push_back(const double d) {
   vec.insert(vec.end(),d);
   return *this;
}

   // vectorの両端に値を挿入する。(sizeは2大きくなる)
vec1d& vec1d::push_both_sides(const double d) {

   (*this).push_back(d);
   (*this).push_front(d);

   return *this;

}

// vectorの両端の値を削除する。(sizeは2小さくなる)
vec1d vec1d::pop_both_sides(const vec1d& obj) {

   (*this).vec=obj.vec;

   (*this).pop_back(obj);
   (*this).pop_front(obj);

   return *this;

}

// vectorを引数に取り、最後の要素を除去したvectorの参照を返却する。
vec1d& vec1d::pop_back(const vec1d& obj) {

   (*this).vec=obj.vec;

   vec.pop_back();

   // 自身を返却する。
   return *this;
}

// vectorを引数に取り、最後の要素を除去したvectorの参照を返却する。
vec1d& vec1d::pop_front(const vec1d& obj) {

   (*this).vec=obj.vec;
   vec.erase(vec.begin());

   // 自身を返却する。
   return *this;
}


void vec1d::createFile(const int _precision,const char *file_path,const char* file_name) const {

      std::string path= std::string(file_path)+"/"+std::string(file_name);

      std::ofstream file(path);
      if (!file.is_open()) {
        std::cerr << "ファイルを開けませんでした。" << std::endl;
        exit(1);
      }

    // 受け取った精度を設定する
    file << std::fixed << std::setprecision(_precision);

    // データをファイルに書き込む
    for (size_t i = 0; i < vec.size(); ++i) {
      file << vec[i] << std::endl;
    }

    // ファイルを閉じる
    file.close();

   //  std::cout << path <<"に書き込みが完了しました。" << std::endl;

}

void vec1d::createFile(const int _precision,const char *file_path,const char* file_name, const double d) const {

      std::string path= std::string(file_path)+"/"+std::string(file_name);

      std::ofstream file(path);

      if (!file.is_open()) {
        std::cerr << "ファイルを開けませんでした。" << std::endl;
        exit(1);
      }

    // 受け取った精度を設定する
      file << std::fixed << std::setprecision(_precision);

    // データをCSV形式で書き込む
   //  for (size_t i = 0; i < vec.size(); ++i) {
      file << d << std::endl;
   //  }

    // ファイルを閉じる
    file.close();

   //  std::cout << path <<"に書き込みが完了しました。" << std::endl;

}

   // 与えられたオブジェクトの配列の要素のpeak成分を返す
vec1d vec1d::get_peak(const vec1d& obj,int limit) {

   int count=0;

   std::vector <double> peak(0);

	for(size_t i=0;i<obj.vec.size()-2;i++){

		if(obj.vec[i+1]>obj.vec[i] && obj.vec[i+1]>obj.vec[i+2]){

			if(count<limit){
				// std::cout << i+1 << " : " << obj.vec[i+1] << std::endl;
            peak.push_back(double(i+1));

            count++;
         }
		}
   }

   (*this).vec=peak;

   return *this;

}


// fileを読み込んで配列に格納する
vec1d vec1d::readfile(const char* file_path,const char* file_name) {

   std::string path= std::string(file_path)+"/"+std::string(file_name);

   ifstream ifs(path);

   if(ifs.fail()){
      std::cerr << "cannot open the file" << std::endl;
      exit(0);
   }

   string str;

   double x_temp;

   while(getline(ifs,str)) {
      stringstream ss(str);
      ss >> x_temp ;
      (*this).push_back(x_temp);
   }

   return *this;
}

   // 与えられたオブジェクトをグラフにして、png形式で保存する。
// void vec1d::create_png(const std::vector<double>& obj, const char* directory, const char* file_name,const char* _suptitle) {
 
//    std::vector<double> x(0);
//    std::vector<double> y(0);

//    for(size_t i=0;i<obj.size();i++) {
//       x.push_back(i);
//    }

//    std::string path= std::string(directory)+"/"+std::string(file_name)+".png";

//       plt::suptitle(_suptitle);
//       plt::plot(x,obj);
//       plt::save(path);

//       plt::clf();
//       plt::close();

// }


#endif /* B3F1C6A7_2F38_475D_88BE_9ED36031DF20 */


// vec1d result(static_cast<int>(other.vec.size()));
// std::transform(other.vec.begin(),other.vec.end(),result.vec.begin(),
// [_param](double v){return _param+v;})
// return result;

//   file << vec[i];
//   if (i != vec.size() - 1) {
//       file << ",";  // CSVフォーマットとしてカンマを追加
//   }

// for (const auto& elem : vec ) {
//    std::cout << elem << " ";
// }
//    std::cout << std::endl;

// vec1d result(static_cast<int>(other.vec.size()))
// std::transform(other.vec.begin(),other.vec.end(),result.vec.begin(),
// [_param](double v){return _param*v;})
// return result;

// vec1 と vec2 の対応する要素を加算して result に保存
// std::transform(vec.begin(), vec.end(), obj.begin(), result.begin(),
// [](double a, double b) { return a + b; })
// vec=temp.vec;

// std::transform(vec.begin(),vec.end(),result.vec.begin(),
//    [_param](double a){return _param*a;}
// );

// vec1d result(static_cast<int>(vec.size()));
// std::transform(vec.begin(),vec.end(),result.vec.begin(),
// [_param](double v){return _param+v;});

// localのresultの参照を返すと関数の抜けた後、中身が消滅する
// 参照を返したいときは*thisを返却する。
// ただし、vec[i]=vec[i]+obj[i]はできない。
// ＊thisの中身を変えるときはconstは使えない。
// vec1dのローカルオブジェクトを作成し、thisに格納して、
// 参照を返却しても、中身は消えている。ので、参照ではなく、実体を返却する。
// ただし、例外としてresize関数を用いて、vecの長さを変えて、そこにpush_backしていくのであれば、
// ＊thisを参照として返却してよい。
