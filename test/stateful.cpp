#include "fsm.hpp"

#include <iostream>

/**
 * Produces:
 *
 */

struct Payload
{
   int v[128] = {};
};

struct S : public State
{
   S() = default;
   S(const S&) = delete;
   Payload p;
};

using StateRef = S&;
using B = Behavior<Config, S>;
using OptB = std::shared_ptr<B>;


/**
 * implement states
 */

struct One;

struct Two : public B
{
   Two(StateRef s) : s(s) {}
   OptB operator()(AnyE) override {
      std::cout << "2: " << s.p.v[0] << std::endl;
      if(s.p.v[0] < 127)
         return become<One>(s);
      return End;
   };
private:
   StateRef s;
};

struct One : public B
{
   One(StateRef s) : s(s) {}
   OptB operator()(AnyE) override {
      std::cout << "1" << std::endl;
      s.p.v[++s.p.v[0]] = s.p.v[0];
      return become<Two>(s);
   };
private:
   StateRef s;
};

struct Start : public B
{
   explicit Start(StateRef s, const Config& c) : B(c), s(s) {}
   OptB operator()(AnyE) override {
      std::cout << "starting" << std::endl;
      return become<One>(s);
   }
private:
   StateRef s;
};


int main(int c, char** v) {
   std::cout << "entering main" << std::endl;

   S s;
   Config cfg;

   OptB current = std::make_shared<Start>(s, cfg);

   while(current) {
      auto& f = *current;
      current = f();
   }

   std::cout << "leaving main, looped " << s.p.v[0] << "x" << std::endl;

   return 0;
}
