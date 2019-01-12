#include "fsm.hpp"

#include <iostream>

/**
 * define your state object
 */
struct S : public State
{
   int val;
};

/**
 * define your config object
 */
struct C : public Config
{
   bool enabled;
};

/**
 * define your types
 */
using StateRef = const S&;
using ConfigRef = const C&;
using B = Behavior<C, S>;
using OptB = std::experimental::optional<B>;


/**
 * define your states
 */

struct Stop : public B
{
   OptB operator()(StateRef s) override {
      return End;
   }
};

struct Three : public B
{
   OptB operator()(StateRef s) override {
      std::cout << "in Three" << std::endl;
      return Stop();
   };
};

struct Two : public B
{
   OptB operator()(StateRef s) override {
      std::cout << "in Two" << std::endl;
      return Three();
   };
};

struct One : public B
{
   OptB operator()(StateRef s) override {
      std::cout << "in One" << std::endl;
      return Two();
   };
};

/**
 * create the entrypoint to the state machine
 */
struct Start : public B
{
   explicit Start(const C& c) { cfg(c); }
   OptB operator()(StateRef) override {
      std::cout << "in Start" << std::endl;
      return One();
   }
};


int main(int c, char** v) {
   std::cout << "in main" << std::endl;

   C cfg;
   S state;
   OptB current = std::experimental::make_optional(Start(cfg));

   while(current) {
      auto f = *current;
      current = f(state);
   }

   return 0;
}
