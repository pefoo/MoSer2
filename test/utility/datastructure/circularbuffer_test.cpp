#include "utility/datastructure/circularbuffer.hpp"
#include "catch2/catch.hpp"

TEST_CASE("Circular buffer", "[Utility]") {
  utility::datastructure::CircularBuffer<int> cb{10};

  // Buffer is empty and not full
  REQUIRE(cb.is_empty());
  REQUIRE(!cb.is_full());

  cb.Put(5);
  REQUIRE(cb.size() == 1);

  // Fill the buffer
  for (int i = 0; i < 10; ++i) {
    cb.Put(i);
  }

  // buffer is now full and not empty
  REQUIRE(!cb.is_empty());
  REQUIRE(cb.is_full());
  REQUIRE(cb.size() == 10);

  // Reset the buffer
  cb.Reset();
  REQUIRE(cb.is_empty());
  REQUIRE(!cb.is_full());

  // Fill it once more
  for (int i = 0; i < 20; ++i) {
    cb.Put(i);
    REQUIRE(cb.size() == (i < 10 ? i + 1 : 10));
  }

  // Buffer returns the values in the correct order
  for (int i = 0; i < 10; ++i) {
    int v;
    REQUIRE(cb.Get(&v));
    REQUIRE(v == i + 10);
  }

  // Buffer is empty after getting all items
  int v;
  REQUIRE(cb.is_empty());
  REQUIRE(!cb.is_full());
  REQUIRE(cb.Get(&v) == false);

  // Buffer size is correct when head wrapped around
  for (int i = 0; i < 15; ++i) {
    cb.Put(i);
  }
  cb.Get(&v);
  REQUIRE(cb.size() == 9);
}
