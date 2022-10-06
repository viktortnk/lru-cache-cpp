#include <gtest/gtest.h>
#include "lru_cache.h"

// Demonstrate some basic assertions.
TEST(BasicCacheTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);

  lru_cache<std::string, std::string> cache(4);

  cache.insert("key1", "v1");
  cache.insert("key2", "v2");

  auto value = cache.get("key1");

  EXPECT_TRUE(value.has_value());
  EXPECT_EQ(value.value(), "v1");

  cache.insert("key3", "v3");
  cache.remove("key2");

  EXPECT_EQ(cache.count(), 2);

  cache.insert("key4", "v4");
  cache.insert("key5", "v5");

  cache.insert("key1", "v11");

  cache.insert("key6", "v6");

  EXPECT_EQ(cache.count(), 4);
  ASSERT_FALSE(cache.get("key3").has_value()); //key3 evicted
  ASSERT_EQ(cache.stats().evictions, 1);

  ASSERT_EQ(cache.get("key1"), "v11");

  std::cout << "..." << std::endl;
}
