#include "comp6771/word_ladder.hpp"
#include <iostream>

// Please note: it's not good practice to test your code via a main function that does
//  printing. Instead, you should be using your test folder. This file should only really
//  be used for more "primitive" debugging as we know that working solely with test
//  frameworks might be overwhelming for some.

auto main() -> int {
	auto const english_lexicon = word_ladder::read_lexicon("./test/word_ladder/english.txt");
	// auto const ladders = word_ladder::generate("atlases", "talons", english_lexicon);
	// auto const ladders = word_ladder::generate("abbreviating", "woodshedding", english_lexicon);
	// auto const ladders = word_ladder::generate("at", "it", english_lexicon);
	// auto const ladders1 = word_ladder::generate("hat", "him", english_lexicon);
	// auto const ladders2 = word_ladder::generate("dog", "mug", english_lexicon);
	// auto const ladders3 = word_ladder::generate("fly", "sky", english_lexicon);
	// auto const ladders4 = word_ladder::generate("code", "data", english_lexicon);
	// auto const ladders5 = word_ladder::generate("work", "play", english_lexicon);
	// auto const ladders6 = word_ladder::generate("code", "good", english_lexicon);
	// auto const ladders = word_ladder::generate("atlases", "cabaret", english_lexicon);
	// auto const ladders = word_ladder::generate("charge", "comedo", english_lexicon);
}