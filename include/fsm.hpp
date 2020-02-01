#pragma once

#include <memory>
#include <cassert>

struct Event {};
struct State {};
struct Config {};

constexpr auto End = nullptr;
using AnyE = const Event&;
constexpr Event EmptyE;

template<typename C = Config, typename S = State, typename E = Event>
struct Behavior : std::enable_shared_from_this<Behavior<C, S, E>>
{
   using OptBehavior = std::shared_ptr<Behavior<C, S, E>>;

   virtual OptBehavior operator()(const E&) = 0;

   OptBehavior operator()() {
      return operator()(EmptyE);
   }

   virtual void enter() {}
   virtual void exit() {}

   template<typename T, typename ... A>
   OptBehavior become(A&& ... args) {
      OptBehavior next = std::make_shared<T>(std::forward<A>(args)...);
      next->config = config;
      this->exit();
      next->enter();
      return next;
   }

   OptBehavior remain() {
      return this->shared_from_this();
   }

protected:
   Behavior() = default;
   Behavior(const C& c) : config(&c) {}
   const C* cfg() { return *config; }

private:
   const C* config = nullptr;
};
