//
//  transform.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 13/9/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

namespace Grid {
  template <template <Coord, Coord> typename... Trans>
  struct Combine;

  template <
    template <Coord, Coord> typename First,
    template <Coord, Coord> typename Second
  >
  struct CombinePair {
    template <Coord Width, Coord Height>
    struct type {
    private:
      using SizedFirst = First<Width, Height>;
      using SizedSecond = Second<SizedFirst::new_width, SizedFirst::new_height>;
      
    public:
      static constexpr Coord new_width = SizedSecond::new_width;
      static constexpr Coord new_height = SizedSecond::new_height;
      static constexpr Pos apply(const Pos pos) {
        return SizedSecond::apply(SizedFirst::apply(pos));
      }
    };
  };
  
  template <template <Coord, Coord> typename Trans>
  struct Combine<Trans> {
    template <Coord Width, Coord Height>
    struct type : Trans<Width, Height> {};
  };
  
  template <
    template <Coord, Coord> typename First,
    template <Coord, Coord> typename Second,
    template <Coord, Coord> typename... Rest
  >
  struct Combine<First, Second, Rest...> {
  private:
    template <Coord Width, Coord Height>
    struct Pair : CombinePair<First, Second>::template type<Width, Height> {};
  public:
    template <Coord Width, Coord Height>
    struct type : Combine<Pair, Rest...>::template type<Width, Height> {};
  };
}
