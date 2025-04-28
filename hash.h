#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        constexpr int GROUP = 5;
        constexpr int gsize = 6;

        const int n = static_cast<int>(k.size());
        unsigned long long w[GROUP] = {0,0,0,0,0};

        //for each 5 base 36 dig, substrings
        for(int g = 0; g < GROUP; ++g){
          int posend = n - (GROUP - 1- g) * gsize;

          if(posend <= 0){
            continue;
          }
          
          int posstart = posend - gsize;

          if(posend < 0){
            posstart = 0;
          }
          
          unsigned long long gval = 0;
          //  build base-36 num
          for(int i = posstart; i < posend; ++i){
            unsigned long long dig = letterDigitToNumber(k[i]);

            int exp = (posend - 1) - i;

            //  36^exp
            unsigned long long place = 1;

            for(int e = 0; e < exp; ++e){
              place *= 36ULL;
            }
            gval += dig*place;
          }
          w[g] = gval;
        }
        //  combiens with r vals
        unsigned long long h = 0;

        for(int i = 0; i < GROUP; ++i){
          h += static_cast<unsigned long long>(rValues[i]) * w[i];
        }
        return static_cast<HASH_INDEX_T>(h);
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        if(std::isupper(static_cast<unsigned char>(letter))){
          letter = static_cast<char>(std::tolower(static_cast<unsigned char>(letter)));

        }
        if(letter >= 'a' && letter <= 'z'){
          return letter - 'a';
        }
        else if(letter >= '0' && letter <= '9'){
          return (letter - '0') + 26;
        }

      return 0; //  only if invalid input
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
