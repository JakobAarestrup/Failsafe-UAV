#include "SimpleState.hpp"
#include "Normal.hpp"
class Critical{
public:
  void AxisControl(){
    std::cout << typeid(this).name() << " called AxisControl()\n";
    //RDS_->TransitionTo(new Normal);
  }
  void RouteControl() {
    std::cout << typeid(this).name() << " called RouteControl()\n";
  }
  void HeightControl(){
    std::cout << typeid(this).name() << " called HeightControl()\n";
    RDS_->TransitionTo(new Normal);
  }
}; 