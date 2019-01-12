#pragma once

#include <memory>
#include <cassert>

struct State {};
struct Config {};

constexpr auto End = nullptr;

template<typename C = Config, typename S = State>
struct Behavior : std::enable_shared_from_this<Behavior<C, S>>
{
   using OptBehavior = std::shared_ptr<Behavior<C, S>>;

   virtual OptBehavior operator()(S&) = 0;

   virtual void enter() {}
   virtual void exit() {}

   template<class T, class... A>
   OptBehavior become(A&& ... args) {
      OptBehavior t = std::make_shared<T>(std::forward<A>(args)...);
      t->cfg(*config);
      exit();
      t->enter();
      return t;
   }

   OptBehavior remain() {
      return this->shared_from_this();
   }

protected:
   Behavior() = default;
   const C& cfg() {
      assert(config);
      return *config;
   }
   void cfg(const C& c) { config = &c; }

private:
   const C* config = nullptr;
};
