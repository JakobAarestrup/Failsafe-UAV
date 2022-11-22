#include "SimpleState.hpp"
#include "Normal.hpp"

class HyperCritical : public ValidateState {
public:
  void AxisControl() {
    std::cout << typeid(this).name() << " Yo yocalled AxisControl()\n";
    RDS_->TransitionTo(new Normal);
  }
  void RouteControl() {
    std::cout << typeid(this).name() << " called RouteControl()\n";
  }
  void HeightControl() {
    std::cout << typeid(this).name() << " called HeightControl()\n";
    //
  }

};
