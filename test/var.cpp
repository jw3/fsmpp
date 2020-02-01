#include <iostream>
#include <variant>
#include <chrono>
#include <optional>

struct Fan
{
   int pin;

   struct Off {
      std::chrono::system_clock::time_point offTime;
   };

   struct Blowing {
      std::chrono::system_clock::time_point onTime;
      std::optional<std::chrono::milliseconds> runFor;
   };

   std::variant<Off, Blowing> state;
};


int main(int c, char** v) {
   std::cout << "entering main" << std::endl;

   Fan f;

   while(true) {

   }



   return 0;
}
