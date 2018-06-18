/**
 * @file node abstract class tests
 * @author Rafael Kallis <rk@rafaelkallis.com>
 */

#include "art.hpp"
#include "doctest.h"
#include <algorithm>
#include <array>
#include <memory>
#include <random>
#include <utility>
#include <vector>
#include <iostream>

using namespace ::art;

using std::array;
using std::make_pair;
using std::make_shared;
using std::mt19937;
using std::pair;
using std::random_device;
using std::shared_ptr;
using std::shuffle;

TEST_SUITE("node") {

  TEST_CASE("check_prefix") {
    node_0<int> node;
    key_type key = {0, 0, 0, 1, 0, 0, 0, 0, 1};

    node.set_prefix(key_type{0, 0, 0, 0});

    CHECK_EQ(3, node.check_prefix(key, 0));
    CHECK_EQ(2, node.check_prefix(key, 1));
    CHECK_EQ(1, node.check_prefix(key, 2));
    CHECK_EQ(0, node.check_prefix(key, 3));
    CHECK_EQ(4, node.check_prefix(key, 4));
    CHECK_EQ(3, node.check_prefix(key, 5));
    CHECK_EQ(2, node.check_prefix(key, 6));
    CHECK_EQ(1, node.check_prefix(key, 7));
    CHECK_EQ(0, node.check_prefix(key, 8));
  }

  TEST_CASE("iteration") {
    node_4<void> subject;

    node_0<void> n0;
    node_0<void> n1;
    node_0<void> n2;
    node_0<void> n3;

    subject.set_child(0, &n0);
    subject.set_child(5, &n1);
    subject.set_child(6, &n2);
    subject.set_child(255, &n3);

    auto it = subject.begin();
    auto it_end = subject.end();
    REQUIRE_EQ(nullptr, *it_end);

    REQUIRE(it < it_end);
    REQUIRE(it <= it_end);
    REQUIRE(it_end > it);
    REQUIRE(it_end >= it);
    REQUIRE(it != it_end);
    REQUIRE_EQ(0, it.get_partial_key());
    REQUIRE_EQ(&n0, *it);
    REQUIRE_EQ(&n0, it.get_node());

    ++it;

    REQUIRE(it < it_end);
    REQUIRE_EQ(5, it.get_partial_key());
    REQUIRE_EQ(&n1, *it);
    REQUIRE_EQ(&n1, it.get_node());

    ++it;

    REQUIRE(it < it_end);
    REQUIRE_EQ(6, it.get_partial_key());
    REQUIRE_EQ(&n2, *it);
    REQUIRE_EQ(&n2, it.get_node());

    ++it;

    REQUIRE(it < it_end);
    REQUIRE_EQ(255, it.get_partial_key());
    REQUIRE_EQ(&n3, *it);
    REQUIRE_EQ(&n3, it.get_node());

    ++it;

    REQUIRE(it == it_end);
    REQUIRE(it <= it_end);
    REQUIRE(it >= it_end);
    REQUIRE_EQ(nullptr, *it);
    REQUIRE_EQ(nullptr, it.get_node());
  }
}
