#include "proj/proj.hpp"
#include <UnitTest++/UnitTest++.h>
#include <unordered_set>

namespace proj
{

  TEST(SKIPLIST_small)
  {
	skiplist<int, int> sl;

	CHECK(sl.size() == 0);

	CHECK(!sl.find(1));
	sl.insert(1, -1);
	CHECK(sl.find(1));
	CHECK(sl.get(1) == -1);

	CHECK(sl.size() == 1);

	CHECK(!sl.find(2));
	sl.insert(2, -2);
	CHECK(sl.find(2));
	CHECK(sl.get(2) == -2);

	CHECK(sl.size() == 2);

	CHECK(!sl.find(3));
	sl.insert(3, -3);
	CHECK(sl.find(3));
	CHECK(sl.get(3) == -3);

	CHECK(sl.size() == 3);

	CHECK_THROW(sl.get(4), std::invalid_argument);

	CHECK(sl.find(3));
	sl.remove(3);
	CHECK(!sl.find(3));
	CHECK_THROW(sl.get(3), std::invalid_argument);

	CHECK(sl.size() == 2);

	CHECK(sl.find(2));
	sl.print();
	sl.remove(2);
	sl.print();
	CHECK(!sl.find(2));
	CHECK_THROW(sl.get(2), std::invalid_argument);

	CHECK(sl.size() == 1);

  }

  TEST(SKIPLIST_large)
  {
	skiplist<int, int> sl;

	std::unordered_set<int> st;

	size_t sz = 32000;
	size_t total = sz;

	CHECK(sl.size() == 0);

	for(int i = 0; i < sz; i++) {
		CHECK(sl.size() == i);
		CHECK(!sl.find(i));
		CHECK_THROW(sl.get(i), std::invalid_argument);
		sl.insert(i, -i);
		st.insert(i);
		CHECK(sl.find(i));
		CHECK(sl.get(i) == -i);
	}

	for(int i = 0; i < sz * 3; i++) {
		int key = rand() % (2 * sz);
		CHECK(sl.size() == total);
		std::cout << key << std::endl;
		if(st.find(key) == st.end()) {
			CHECK(!sl.find(key));
			CHECK_THROW(sl.get(key), std::invalid_argument);
			sl.remove(key);
		} else {
			CHECK(sl.find(key));
			CHECK(sl.get(key) == -key);
			sl.remove(key);
			st.erase(key);
			total--;
			CHECK(!sl.find(key));
			CHECK_THROW(sl.get(key), std::invalid_argument);
		}
	}

  }


}  // namespace proj

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
