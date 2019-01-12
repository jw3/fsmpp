#include "fsm.hpp"

#include <iostream>

using B = Behavior<>;
using OptB = std::shared_ptr<B>;

struct Start : public B
{
   OptB operator()(State&) override {
      std::cout << "one and done" << std::endl;
      return End;
   }
};

int main(int c, char** v) {
   std::cout << "entering main" << std::endl;

   State state;
   OptB current = std::make_shared<Start>();

   while(current) {
      auto& f = *current;
      current = f(state);
   }

   std::cout << "leaving main" << std::endl;

   return 0;
}
