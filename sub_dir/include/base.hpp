#ifndef DBAE4210_98FF_40CD_9B54_E5208CBD3D49
#define DBAE4210_98FF_40CD_9B54_E5208CBD3D49

#include <iostream>
using namespace std;

class base1d{

   protected :
      double x;

   public:
      base1d() : x(0.0) {}
      base1d(double _x) : x(_x) {}

      virtual void show(){
         cout << "called 1d constructor." << endl;
         cout << "x:" << x << endl;
      }

      // 仮想デストラクタ
      virtual ~base1d()=default ;
};

class base2d : public base1d {

   protected:
      double y;

   public :
      base2d() :base1d(), y(0.0) {}

      base2d(double _x, double _y) : base1d(_x), y(_y) {} 

      void calc(){
         cout << "x+y=" << x+y << endl;
      }

      void calc(double _x, double _y){
         cout << "x+y="<< _x+_y << endl;
      }

   virtual void show() override {
      cout << "called 2d constructor." << endl;
      cout << "x:" << x << " " << "y:" << y << endl;
   }

};

class base3d : public base2d {

   protected :
      double z;
   
   public :
      base3d():base2d(),z(0.0){}
      base3d(double _x, double _y, double _z) : base2d(_x,_y),z(_z) {}

      void calc(){
         cout << "x+y+z=" << x+y+z << endl;
      }

      void calc(double _x, double _y, double _z){
         cout << "x+y+z=" << _x+_y+_z << endl;
      }

      virtual void show() override {
               cout << "called 3d constructor." << endl;
      cout << "x:" << x << " " << "y:" << y << " " << "z:" << z << endl;

      }

};

#endif /* DBAE4210_98FF_40CD_9B54_E5208CBD3D49 */

/*
sudo mkdir /usr/share/fonts/meiryo
sudo mv ~/Downloads/meiryo.ttc ~/Downloads/meiryob.ttc /usr/share/fonts/meiryo
sudo chmod 755 /usr/share/fonts/meiryo -R

fontチャッシュを更新

sudo fc-cache -fv

*/
