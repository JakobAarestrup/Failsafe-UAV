#pragma once
#include "SimpleState.hpp"
#include "HyperCritical.hpp"
class Normal {
public:
  void AxisControl(){
    std::cout << typeid(this).name() << " called AxisControl()\n";
    RDS_->TransitionTo(new HyperCritical);
  }
  void RouteControl() {
    std::cout << typeid(this).name() << " called RouteControl()\n";
  }
  void HeightControl() {
    std::cout << typeid(this).name() << " called HeightControl()\n";
  }
};