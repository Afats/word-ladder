//
//  Copyright UNSW Sydney School of Computer Science and Engineering
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "comp6771/word_ladder.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "catch2/catch.hpp"

// I'm assuming that for these test cases the lexicon provided in this assignment will be used

// [NonExistent] test cases: Start and End words
//       This is to test cases that have no connecting words and thus no paths
// [Small] test cases: Start and End words that have <=2 paths and path lengths.
//	     This is to test for paths that are likely to have only unique words not seen in other
//ladders
// [Medium] test cases: Start and End words that have >=3 paths and path lengths
//	     This is to test for paths that are likely to have common words between ladders
// [Large] test cases: Start and End words that have >= 20 paths or path lengths
//       This is to test for paths that are extemely likely to have more than one common word
//       between ladders These tests can be used as benchmarks

// helper function to check path len
auto pathlen_check(auto const& ladders, auto const& length) {
	for (size_t i = 0; i < ladders.size(); i++) {
		if (ladders[i].size() != static_cast<size_t>(length))
			return false;
	}

	return true;
}

// case of unequal word lengths
TEST_CASE("atlases -> talons", "[NonExistent]") {
	auto const start = std::string("atlases");
	auto const dest = std::string("talons");

	CHECK(start != dest);
	CHECK(std::size(start) != std::size(dest));

	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const ladders = word_ladder::generate(start, dest, english_lexicon);

	CHECK(std::size(ladders) == 0);
	CHECK(std::count(ladders.begin(), ladders.end(), std::vector<std::string>{start, dest}) == 0);
}

// no path between two words
TEST_CASE("yttric -> talons", "[NonExistent]") {
	auto const start = std::string("yttric");
	auto const dest = std::string("talons");

	// checks to fulfill assumptions mentioned in spec
	CHECK(start != dest);
	CHECK(std::size(start) == std::size(dest));

	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const ladders = word_ladder::generate(start, dest, english_lexicon);

	CHECK(std::size(ladders) == 0);
	CHECK(std::count(ladders.begin(), ladders.end(), std::vector<std::string>{start, dest}) == 0);
}

// no path between two words
TEST_CASE("abbreviating -> woodshedding", "[NonExistent]") {
	auto const start = std::string("abbreviating");
	auto const dest = std::string("woodshedding");

	CHECK(start != dest);
	CHECK(std::size(start) == std::size(dest));

	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const ladders = word_ladder::generate(start, dest, english_lexicon);

	CHECK(std::size(ladders) == 0);
	// checking if the outputs are right
	CHECK(std::count(ladders.begin(), ladders.end(), std::vector<std::string>{start, dest}) == 0);
}

// small case with unique path
TEST_CASE("at -> it", "[Small]") {
	auto const start = std::string("at");
	auto const dest = std::string("it");

	CHECK(start != dest);
	CHECK(std::size(start) == std::size(dest));

	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const ladders = word_ladder::generate(start, dest, english_lexicon);

	auto const length = 2;

	CHECK(std::size(ladders) == 1);
	CHECK(std::is_sorted(ladders.begin(), ladders.end()));

	// checking if the outputs are right
	CHECK(std::count(ladders.begin(), ladders.end(), std::vector<std::string>{start, dest}) == 1);

	// check if the number of words for each path is the same and of the correct length
	REQUIRE(pathlen_check(ladders, length));
}

// small case with unique paths
TEST_CASE("hat -> him", "[Small]") {
	auto const start = std::string("hat");
	auto const dest = std::string("him");

	CHECK(start != dest);
	CHECK(std::size(start) == std::size(dest));

	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const ladders = word_ladder::generate(start, dest, english_lexicon);

	auto const length = 3;

	CHECK(std::size(ladders) == 2);
	CHECK(std::is_sorted(ladders.begin(), ladders.end()));

	// checking if the outputs are right
	CHECK(std::count(ladders.begin(), ladders.end(), std::vector<std::string>{start, "ham", dest})
	      == 1);
	CHECK(std::count(ladders.begin(), ladders.end(), std::vector<std::string>{start, "hit", dest})
	      == 1);

	// check if the number of words for each path is the same and of the correct length
	REQUIRE(pathlen_check(ladders, length));
}

// small case with unique paths but some common words that are not in path
TEST_CASE("dog -> mug", "[Small]") {
	auto const start = std::string("dog");
	auto const dest = std::string("mug");

	CHECK(start != dest);
	CHECK(std::size(start) == std::size(dest));

	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const ladders = word_ladder::generate(start, dest, english_lexicon);

	auto const length = 3;

	CHECK(std::size(ladders) == 2);
	CHECK(std::is_sorted(ladders.begin(), ladders.end()));

	// checking if the outputs are right
	CHECK(std::count(ladders.begin(), ladders.end(), std::vector<std::string>{start, "dug", dest})
	      == 1);
	CHECK(std::count(ladders.begin(), ladders.end(), std::vector<std::string>{start, "mog", dest})
	      == 1);

	REQUIRE(pathlen_check(ladders, length));
}

// small case with unique paths
TEST_CASE("fly -> sky", "[Small]") {
	auto const start = std::string("fly");
	auto const dest = std::string("sky");

	CHECK(start != dest);
	CHECK(std::size(start) == std::size(dest));

	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const ladders = word_ladder::generate(start, dest, english_lexicon);

	auto const length = 3;

	CHECK(std::size(ladders) == 1);
	CHECK(std::is_sorted(ladders.begin(), ladders.end()));

	// checking if the outputs are right
	CHECK(std::count(ladders.begin(), ladders.end(), std::vector<std::string>{start, "sly", dest})
	      == 1);
	REQUIRE(pathlen_check(ladders, length));
}

// medium case with common words across paths
TEST_CASE("code -> data", "[Medium]") {
	auto const start = std::string("code");
	auto const dest = std::string("data");

	CHECK(start != dest);
	CHECK(std::size(start) == std::size(dest));

	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const ladders = word_ladder::generate(start, dest, english_lexicon);

	auto const length = 5;

	CHECK(std::size(ladders) == 3);
	CHECK(std::is_sorted(ladders.begin(), ladders.end()));

	// checking if the outputs are right
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{start, "cade", "cate", "date", dest})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{start, "cote", "cate", "date", dest})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{start, "cote", "dote", "date", dest})
	      == 1);

	REQUIRE(pathlen_check(ladders, length));
}

// medium case with common words across paths
TEST_CASE("code -> good", "[Medium]") {
	auto const start = std::string("code");
	auto const dest = std::string("good");

	CHECK(start != dest);
	CHECK(std::size(start) == std::size(dest));

	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const ladders = word_ladder::generate(start, dest, english_lexicon);

	auto const length = 5;

	CHECK(std::size(ladders) == 3);
	CHECK(std::is_sorted(ladders.begin(), ladders.end()));

	// checking if the outputs are right
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{start, "cods", "coos", "goos", dest})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{start, "cods", "gods", "goos", dest})
	      == 1);
	CHECK(std::count(ladders.begin(),
	                 ladders.end(),
	                 std::vector<std::string>{start, "cole", "cold", "gold", dest})
	      == 1);

	REQUIRE(pathlen_check(ladders, length));
}

// medium case with common words across paths
TEST_CASE("work -> play", "[Medium]") {
	auto const start = std::string("work");
	auto const dest = std::string("play");

	CHECK(start != dest);
	CHECK(std::size(start) == std::size(dest));

	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const ladders = word_ladder::generate(start, dest, english_lexicon);

	auto const length = 7;

	CHECK(std::size(ladders) == 12);
	CHECK(std::is_sorted(ladders.begin(), ladders.end()));

	REQUIRE(pathlen_check(ladders, length));
}

// large test case
TEST_CASE("charge -> comedo", "[Large]") {
	auto const start = std::string("charge");
	auto const dest = std::string("comedo");

	CHECK(start != dest);
	CHECK(std::size(start) == std::size(dest));

	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const ladders = word_ladder::generate(start, dest, english_lexicon);

	auto const length = 23;

	CHECK(std::size(ladders) == 4);
	CHECK(std::is_sorted(ladders.begin(), ladders.end()));

	REQUIRE(pathlen_check(ladders, length));
}

// large test case -- benchmark
TEST_CASE("atlases -> cabaret", "[Large]") {
	auto const start = std::string("atlases");
	auto const dest = std::string("cabaret");

	CHECK(start != dest);
	CHECK(std::size(start) == std::size(dest));

	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const ladders = word_ladder::generate(start, dest, english_lexicon);

	auto const length = 58;

	CHECK(std::size(ladders) == 840);
	CHECK(std::is_sorted(ladders.begin(), ladders.end()));

	REQUIRE(pathlen_check(ladders, length));
}
//./test/word_ladder/english.txt
