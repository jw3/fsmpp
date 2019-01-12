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
using OptB = std::shared_ptr<B>;


/**
 * define your states
 */

struct Stop : public B
{
   OptB operator()(StateRef s) override {
      std::cout << "stopping" << std::endl;
      return End;
   }
};

struct Three : public B
{
   OptB operator()(StateRef s) override {
      std::cout << "in Three" << std::endl;
      return become<Stop>();
   };
};

struct Two : public B
{
   OptB operator()(StateRef s) override {
      std::cout << "in Two" << std::endl;
      return become<Three>();
   };
};

struct One : public B
{
   OptB operator()(StateRef s) override {
      std::cout << "in One" << std::endl;
      return become<Two>();
   };
};

/**
 * create the entrypoint to the state machine
 */
struct Start : public B
{
   explicit Start(const C& c) { cfg(c); }
   OptB operator()(StateRef) override {
      std::cout << "starting" << std::endl;
      return become<One>();
   }
};


int main(int c, char** v) {
   std::cout << "entering main" << std::endl;

   C cfg;
   S state;

   OptB current = std::make_shared<Start>(cfg);

   while(current) {
      auto& f = *current;
      current = f(state);
   }

   std::cout << "leaving main" << std::endl;

   return 0;
}