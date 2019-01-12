#include "fsm.hpp"

#include <iostream>

/**
 * define your state object
 */
struct S : public State
{
   int val = 0;
};

/**
 * define your config object
 */
struct C : public Config
{
   int count = 5;
};

/**
 * define your types
 */
using StateRef = S&;
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
   void enter() override {
      std::cout << "in Two, I loop" << std::endl;
   }

   OptB operator()(StateRef s) override {
      auto remaining = cfg().count - ++s.val;
      std::cout << "\tloop: " << s.val << std::endl;
      return remaining ? remain() : become<Three>();
   };

   void exit() override {
      std::cout << "leaving Two, done looping" << std::endl;
   }
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

/*
entering main
starting
in One
in Two, I loop
	loop: 1
	loop: 2
	loop: 3
	loop: 4
	loop: 5
leaving Two, done looping
in Three
stopping
leaving main
 */
