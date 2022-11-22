#include "SimpleState.hpp"

class HyperCritical : public ValidateState {
public:
  void AxisControl() override {
    std::cout << typeid(this).name() << " Yo yocalled AxisControl()\n";
    //RDS_->TransitionTo(new Normal);
  }
  void RouteControl() override {
    std::cout << typeid(this).name() << " called RouteControl()\n";
  }
  void HeightControl() override {
    std::cout << typeid(this).name() << " called HeightControl()\n";
    //
  }

};
