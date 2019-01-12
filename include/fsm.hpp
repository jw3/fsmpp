#pragma once

#include <cassert>
#include <experimental/optional>

struct State {};
struct Config {};

constexpr auto End = std::experimental::nullopt;

template<typename C = Config, typename S = State>
struct Behavior
{
   using OptBehavior = std::experimental::optional<Behavior<C, S>>;

   virtual OptBehavior operator()(const S&) {
      return End;
   };

   virtual void enter() {}
   virtual void exit() {}

   template<class T, class... A>
   OptBehavior become(A&& ... args) {
      T t(std::forward<A>(args)...);
      t.cfg(*config);
      exit();
      t.enter();
      return std::move(t);
   }

   OptBehavior remain() {
      return std::experimental::make_optional(this);
   }

protected:
   Behavior() = default;
   const C& cfg() {
      assert(config);
      return *config;
   }
   void cfg(const C&) {}

private:
   C* config = nullptr;
};
