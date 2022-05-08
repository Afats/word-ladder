#include "comp6771/word_ladder.hpp"
#include <bits/types/struct_tm.h>
#include <unordered_map>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <queue>
#include <string>
#include <vector>

// z5232937

namespace word_ladder {

	// helper function
	auto print_results(std::vector<std::vector<std::string>> const& results) {
		for (std::size_t i = 0; i < results.size(); i++) {
			for (std::size_t j = 0; j < results[i].size(); j++)
				std::cout << "\"" << results[i][j] << "\", "
				          << " ";
			std::cout << std::endl;
		}
	}

	// helper function
	auto print_currpath(std::vector<std::string> const& curr_path) {
		std::cout << "Current path: ";
		for (std::vector<std::string>::const_iterator word = curr_path.begin(); word != curr_path.end();
		     word++)
		{
			std::cout << *word << " -> ";
		}
	}

	// helper function
	auto print_set(std::unordered_map<std::string, std::vector<std::string>> const& word_map,
	               std::string const& key) {
		auto l = word_map.find(key);
		std::cout << "Set of word " << key << ":  ";
		if (l != word_map.end()) {
			for (std::vector<std::string>::const_iterator word = l->second.begin();
			     word != l->second.end();
			     word++)
			{
				std::cout << *word << " ";
			}

			std::cout << std::endl;
		}
	}

	// helper function
	auto print_level(std::unordered_map<std::string, int> const& hop_level, int const& level) {
		for (auto key = hop_level.begin(); key != hop_level.end(); key++) {
			if (key->second == level)
				std::cout << "key: " << key->first << " - level: " << key->second << std::endl;
		}
	}

	// helper function
	auto print_levels(std::unordered_map<std::string, int> const& hop_level) {
		for (auto key = hop_level.begin(); key != hop_level.end(); key++) {
			std::cout << "key: " << key->first << " - level: " << key->second << std::endl;
		}
	}

	// check if word is in lexicon
	auto in_lexicon(std::string const& word, std::unordered_set<std::string> const& lex) {
		std::unordered_set<std::string>::const_iterator iter = lex.find(word);
		// word not in lex
		if (iter == lex.end())
			return false;
		else
			return true;
	}

	// check if a word has been seen
	auto seen(std::string const& word, std::unordered_set<std::string> const& explored_words) {
		auto found = explored_words.find(word);
		// word has been seen
		if (found != explored_words.end()) {
			return true;
		}
		else {
			return false;
		}
	}

	// add to word map where words differ from start by one letter (one hop)
	auto one_hop(std::unordered_map<std::string, std::vector<std::string>>& word_map,
	             std::unordered_set<std::string> const& lex,
	             std::string const& start) {
		auto adjacent_words = std::vector<std::string>();

		// get all legal words one letter away
		for (std::size_t i = 0; i < start.size(); i++) {
			for (char alph = 'a'; alph <= 'z'; alph++) {
				if (alph == start[i])
					continue;
				else {
					std::string word = start;
					word[i] = alph;

					// add one-hop word to set
					if (in_lexicon(word, lex))
						adjacent_words.emplace_back(word);
				}
			}
		}

		// sort set in lexographical order
		std::sort(adjacent_words.begin(), adjacent_words.end());
		word_map[start] = adjacent_words;
	}

	// check if a seen word has a valid depth to be considered in a ladder
	auto valid_depth(std::string const& curr_word,
	                 int const& depth,
	                 std::unordered_map<std::string, int> const& hop_level) {
		auto iter = hop_level.find(curr_word);

		if (iter != hop_level.end()) {
			if (iter->second == depth) {
				return true;
			}
			else {
				return false;
			}
		}

		return false;
	}

	// enqueue words one hop away that are only one level below it
	auto enqueue(std::unordered_map<std::string, std::vector<std::string>>& word_map,
	             std::unordered_map<std::string, int>& hop_level,
	             std::string const& curr_word,
	             std::string const& to,
	             std::queue<std::string>& buckets,
	             std::unordered_set<std::string>& seen_words) {
		auto path_len = 0;
		auto word_key = word_map.find(curr_word);

		// curr_word is in word map
		if (word_key != word_map.end()) {
			auto& set = word_key->second;

			// to word found, wrap up bfs
			if (std::find(set.begin(), set.end(), to) != set.end()) {
				path_len = hop_level[curr_word];

				// remove words that have already been seen at a higher level
				for (auto word = set.begin(); word != set.end();) {
					if (seen(*word, seen_words)) {
						// if word has already been seen at a higher level, remove from curr_word map
						if (!valid_depth(*word, hop_level[curr_word] + 1, hop_level)) {
							word = set.erase(word);
						}

						else {
							++word;
						}
					}

					else {
						++word;
					}
				}
				return path_len;
			}

			else {
				// enqueue the words one hop away to be bfs'ed and record depth
				// dont enqueue words already seen that are of lower depth
				for (auto word = set.begin(); word != set.end();) {
					if (seen(*word, seen_words)) {
						if (!valid_depth(*word, hop_level[curr_word] + 1, hop_level)) {
							word = set.erase(word);
						}

						else {
							++word;
						}
					}

					// unseen word, queue for exploration
					else {
						hop_level[*word] = hop_level[curr_word] + 1;
						buckets.push(*word);
						seen_words.emplace(*word);
						++word;
					}
				}
			}
		}
		return path_len;
	}

	// generate one hops for words at the right depth
	auto end_bfs(std::queue<std::string> buckets,
	             std::unordered_map<std::string, std::vector<std::string>>& word_map,
	             std::unordered_map<std::string, int>& hop_level,
	             int const& depth,
	             std::unordered_set<std::string> const& lex,
	             std::unordered_set<std::string>& seen_words) {
		while (buckets.size() > 0) {
			auto curr_word = buckets.front();
			buckets.pop();

			// check if words are to be removed from the set
			if (valid_depth(curr_word, depth, hop_level)) {
				one_hop(word_map, lex, curr_word);
				auto word_key = word_map.find(curr_word);
				auto& set = word_key->second;

				for (auto word = set.begin(); word != set.end();) {
					if (seen(*word, seen_words)) {
						if (!valid_depth(*word, hop_level[curr_word] + 1, hop_level)) {
							word = set.erase(word);
						}

						else {
							++word;
						}
					}

					else {
						++word;
					}
				}
			}
		}
	}

	// queue ladders until every key explored or dest word found
	auto bfs(std::unordered_map<std::string, std::vector<std::string>>& word_map,
	         std::unordered_map<std::string, int>& hop_level,
	         std::unordered_set<std::string> const& lex,
	         std::string const& from,
	         std::string const& to) {
		auto buckets = std::queue<std::string>();
		auto seen_words = std::unordered_set<std::string>();
		seen_words.emplace(from);

		hop_level[from] = 0;
		auto path_len = 0;
		auto prev_word = std::string();
		// push starting word onto queue
		buckets.push(from);

		while (buckets.size() > 0) {
			auto curr_word = buckets.front();
			buckets.pop();

			// if shortest path has been found, end bfs at this level
			if (path_len) {
				end_bfs(buckets, word_map, hop_level, path_len, lex, seen_words);
				break;
			}

			else {
				// enqueue words one hop away
				one_hop(word_map, lex, curr_word);
				path_len = enqueue(word_map, hop_level, curr_word, to, buckets, seen_words);
			}
		}
		return path_len + 1;
	}

	// dfs through word_map and add all valid paths to results
	auto dfs(std::unordered_map<std::string, std::vector<std::string>> const& word_map,
	         std::string const& from,
	         std::string const& to,
	         std::vector<std::string>& curr_path,
	         std::vector<std::vector<std::string>>& results) {
		curr_path.push_back(from);
		auto l = word_map.find(from);

		// to word found, add path to results path and return
		if (from == to) {
			results.push_back(curr_path);
			curr_path.pop_back();
			return;
		}

		// base case
		// word is not in word map: no adjacent words -- backtrack
		if (l == word_map.end()) {
			curr_path.pop_back();
			return;
		}

		// dfs the adjacent words
		else {
			for (std::vector<std::string>::const_iterator word = l->second.begin();
			     word != l->second.end();
			     word++)
			{
				dfs(word_map, *word, to, curr_path, results);
			}
			curr_path.pop_back();
		}
		return;
	}

	auto generate(std::string const& from,
	              std::string const& to,
	              std::unordered_set<std::string> const& lexicon)
	   -> std::vector<std::vector<std::string>> {
		auto const& start_len = from.length();
		auto lex = std::unordered_set<std::string>();
		auto word_map = std::unordered_map<std::string, std::vector<std::string>>();
		auto hop_level = std::unordered_map<std::string, int>();
		auto results = std::vector<std::vector<std::string>>();

		// filter lexicon by word length
		for (auto const& word : lexicon) {
			if (word.length() == start_len) {
				lex.emplace(word);
				// std::cout <<  word << "\n";
			}
		}

		auto path_len = bfs(word_map, hop_level, lex, from, to);
		// std::cout << "Length of shortest path: " << path_len<< std::endl;
		// std::cout << "Number of words in path: " << path_len+1 << std::endl;
		(void)path_len;
		std::vector<std::string> curr_path;
		dfs(word_map, from, to, curr_path, results);
		// print_results(results);
		return results;
	}
} // namespace word_ladder
