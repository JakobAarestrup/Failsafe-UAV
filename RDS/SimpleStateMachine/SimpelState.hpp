#pragma once
#include <iostream>
#include <typeinfo>
typeid(this).name()

class RDS;

class ValidateState {

public:
  virtual ~State() {}

  void setRDS_(RDS* RDS) {
    RDS_ = RDS;
  }

  virtual void DoWorkA() = 0;
  virtual void DoWorkB() = 0;

protected:
  RDS* RDS_;
};

class RDS {

public:
  RDS(ValidateState* state) : state_(nullptr) {
    TransitionTo(state);
  }

  ~RDS() { delete state_; }

  void TransitionTo(ValidateState* state) {
    if (state_) delete state_;
    state_ = state;
    state_->setRDS_(this);
  }

  void RequestA() { state_->DoWorkA(); }
  void RequestB() { state_->DoWorkB(); }

private:
  State* state_;

};

class Normal : public ValidateState {
public:
  void DoWorkA() override {
    std::cout << typeid(this).name() << " called DoWorkA()\n" << thing;
  }
  void DoWorkB() override {
    std::cout << typeid(this).name() << " called DoWorkB()\n";
  }
};

class Critical : public ValidateState {
public:
  void DoWorkA() override {
    std::cout << typeid(this).name()<< " called DoWorkA()\n";
    RDS_->TransitionTo(new Normal);
  }
  void DoWorkB() override {
    std::cout << typeid(this).name() << " called DoWorkB()\n";
  }
};

class HyperCritical : public ValidateState {
public:
  void DoWorkA() override {
    std::cout << typeid(this).name()<< " called DoWorkA()\n";
    RDS_->TransitionTo(new Normal);
  }
  void DoWorkB() override {
    std::cout << typeid(this).name() << " called DoWorkB()\n";
  }
};