## Overview

In the role of client, the low-level details have already been dealt with, and you can focus your
attention on solving more pressing problems. Having a library of thoroughly tested and reviewed
types designed by field experts vastly broadens the kinds of tasks you can “easily” tackle. In this
first assignment, you’ll write the back-end for a program that heavily leverages the standard
library to do nifty things. It might sound a little daunting at first; but given the power tools in
your arsenal, you can focus on solving the problem at hand, instead of worrying about how to
implement everything from scratch. Let’s hear it for abstraction!

This assignment has several purposes:

1. To explore C++’s value-semantics.
2. To stress the notion of abstraction as a mechanism for managing data and providing functionality
   without revealing the implementation.
3. To become more familiar with C++'s type system.
4. To gain familiarity with the C++ standard library.

Leveraging `std::vector`, `std::queue`, and `std::unordered_set` are critical for writing a
word ladder builder. A word ladder is a connection from one word to another, formed by changing one
letter at a time, with the constraint that each transformation yields a new valid word. For example,
here is a word ladder connecting "code" to "data".


```txt
code -> cade -> cate -> date -> data
```

Many word ladders will have mutliple solutions, such as connecting "work" to "play".

```txt
work fork form foam flam flay play
work pork perk peak pean plan play
work pork perk peak peat plat play
work pork perk pert peat plat play
work pork porn pirn pian plan play
work pork port pert peat plat play
work word wood pood plod ploy play
work worm form foam flam flay play
work worn porn pirn pian plan play
work wort bort boat blat plat play
work wort port pert peat plat play
work wort wert pert peat plat play
```

The back-end that you will write takes a start word, a destination word, and a lexicon, which
returns valid word ladders. By using a [breadth-first search][bfs], you’re guaranteed to find the
shortest such sequence. Here are some more examples.

```cpp
::word_ladder::generate("awake", "sleep", english_words);
// returns {
//   {"awake","aware","sware","share","sharn","shawn","shewn","sheen","sheep","sleep"}
//   {"awake","aware","sware","share","shire","shirr","shier","sheer","sheep","sleep"}
// }
::word_ladder::generate("airplane", "tricycle", engish_words);
// returns {}
```

## Understanding a word ladder implementation

Finding a word ladder is a specific instance of a [shortest-path problem][ssp], where the challenge
is to find the shortest path from a starting position to a goal. Shortest-path problems come up in a
variety of situations, such as packet routing, robot motion planning, social networks, and studying
gene mutations. One approach for finding a shortest path is the classic breadth-first search
algorithm. A breadth-first search searches outward from the start in a radial fashion, until it hits
the goal. For our word ladder, this means examining those ladders that represent one hop from the
start. A “hop” is a change in letter. One “hop” from the start means one changed letter, two “hops”
means two changes in letters, and so on. It’s possible for the same position to change letters
across multiple non-adjacent hops. If any of these reach the destination, we’re done. If not, the
search now examines all the ladders that add one more hop. By expanding the search at each step, all
one-hop ladders are examined before two-hop ladders, and three-hop ladders are only taken into
consideration if none of the one-hop or two-hop ladders work out; thus the algorithm is guaranteed
to find the shortest successful ladder.

Breadth-first searches are typically implemented using a queue. The queue stores partial ladders
that represent possibilities to explore. The ladders are enqueued in order of increasing length. The
first elements enqueued are all the one-hop ladders, followed by the two-hop ladders, and so on. Due
to FIFO handling, ladders will be dequeued in order of increasing length. The algorithm operates by
dequeuing the front ladder from the queue and determining if it reaches the goal. If it does, then
you have a complete ladder, and it is the shortest. If not, you take that partial ladder and extend
it to reach words that are one more hop away, and enqueue those ladders onto the queue, to be
examined later. If you exhaust the queue of possibilities without having found a completed ladder,
you can conclude that no ladder exists.

A few of these tasks deserve a bit more explanation. For example, you’ll need to find all the words
that differ by one letter from a given word. You might reach for a raw loop to change each letter to
all the other letters in the alphabet. Repeat this for each letter position in the
word and you will have discovered all the words that are one letter away.

Another, more subtle issue, is the restriction that you shouldn’t reuse words that have been
included in a previous ladder. This is an optimisation that avoids exploring redundant paths. For
example, if you previously tried the ladder `cat->cot->cog` and are now processing `cat->cot->con`,
you would find that the word `cog` is one letter away from `con`, which looks like a potential
candidate to extend this ladder. However, `cog` has already been reached in an earlier (and thus
shorter) ladder, so there is no point in reconsidering it in a longer ladder. The simplest way to
ensure this is to keep track of the words that have been used in any ladder, and ignore them when
they resurface. This is also necessary to avoid getting trapped in circular, non-terminal ladders
such as `cat->cot->cog->bog->bat->cat`. Since you need linear access to all of the items in a
word ladder when time comes to return it, it makes sense to model an individual word ladder using
`std::vector<std::string>`. Remember that because C++ has value semantics, you’re able to copy
vectors via copy initialisation (e.g. `auto word_ladder_clone = word_ladder;`) and copy assignment
(e.g. `word_ladder_clone = word_ladder`).

**If there are multiple shortest paths, your implementation must return all the solutions, sorted in
lexicographical order. Thus, the return type for your word ladder generator will be
`std::vector<std::vector<std::string>>`.**


## The Task

This generator might seem like it’s a panacea, but it still benefits from a step-by-step development
plan to keep things moving along.

* **Task 1** --- *Familiarise yourself with the libraries available.* You don’t need to deep-dive,
but it would be a smart idea to read up on `std::vector`, `std::queue`, and `std::unordered_set`,
from the standard library; and to read up on the range adaptors we’ve listed in the appendix. You
shouldn’t worry about their implementation details: focus on the interfaces, and how you use them in
practice.
* **Task 2** --- *Test design.* We’ve provided you with a very simple test case to show you how to
use the test library. You should add more `TEST_CASE`s underneath, so you have a suite of checks to
make sure you catch any logic errors in your generator. We adopt Google’s Beyoncé rule in this
class: “if you liked it, you should have put a test on it”. Test words that are short (one or two
letters) and test words that are longer.
* **Task 3** --- *Design your algorithm.* Be sure you understand the breadth-first search algorithm
on paper, and what types you will need to use.
* **Task 4** --- *Lexicon handling.* Set up an `std::unordered_set` object with the large lexicon,
read from our data file. There’s a utility function called `word_ladder::read_lexicon` that will
read it in from file for you. Please don't modify this function.

### Assumptions

* You can assume that the start word and end word have the same length (i.e. number of characters).
* You can assume that both the start and the end word are in the lexicon.
* You can assume the start word and the end word will not be the same word

### Implementation hints

Again, it’s all about leveraging the libraries at your disposal --- you’ll find your job is just to
coordinate the activities of your objects to do the search.

* [`std::vector`][vector] maintains a contiguous sequence of elements, and has random access.
  Benchmarks have shown that its contiguous storage makes it the fastest and smallest container
  in many situations (even when computer science theory tells us that a linked list would be
  better!).
* [`std::queue`][queue] offers a FIFO interface over a container, which is probably what you’ll
  want to use for tracking your partial ladders. Ladders are enqueued (and thus dequeued) in
  order of length, so as to find the shortest option first.
* [`std::unordered_set`][us] (note 1) is the hash-set that we use for the input lexicon, and is
  recommended for any internal lexicons you might use.
* [Algorithms] and ranges are powerful tools for describing what you want your program to do.
* As a minor detail, it doesn’t matter if the start and destination words are contained in the
  lexicon or not. You can eliminate non-words from the get-go or just allow them to fall through and
  be searched anyway. During marking, the start and destination words will always be taken from the
  lexicon.


### Running the tests

Similar to the first tutorial, you simply to have to run `Ctrl+Shift+P` and then type `Run Test` and
hit enter. VS Code will compile and run all of your tests and produce an output.

### Adding more tests

Part of your assignment mark will come from the quality and extensiveness of tests that you write.

You can add more test files to the `test/word_ladder/` directory. Simply copy `test/word_ladder/word_ladder_test1.cpp`
into another file in that directory.

Note, everytime that you add a new file to the `test/word_ladder/` directory you will need to add
another few lines to `test/CMakeLists.txt`. You can once again, simply copy the test reference for
`word_ladder_test1.cpp` and rename the appropriate parts. Every time you update `CMakeLists.txt` in
any repository, in VSCode you should press `Ctrl+Shift+P` and run `Reload Window` for the changes to
take effect.

### Measuring your performance

While you aren't assessed on the performance of your code, there is a **15 second time limit** per
test that we will enforce when marking your assignment. For the average student, this means that
some of the hardest test cases may not run fast enough with your algorithm and you may fail the latter
tests due to timeout.

We have provided the hardest test case we will test against in `test/word_ladder/word_ladder_test_benchmark.cpp`.
If you can successfully pass this test within 15 seconds **ON THE CSE MACHINE** then we are providing
you a guarantee that you will not fail those tests from a timeout. If you exceed 15 seconds for that
test, you may still get the marks, but it's just something we can't guarantee.

Please note: This benchmark test is VERY difficult. If your code takes an hour to run this test that's
not something that should make you panic. It's quite easy to fall in the time limit for most of the
tests. Don't stress and just make sure earlier tests fall in the 15 second time limit on CSE
machines.

To measure your performance:

1. In VSCode, down the very bottom of the window, change your Cmake from `[Debug]` to `[Release]`.
   This will remove debug symbols and other things, which will mean your code runs faster, but is
   near impossible to debug for mere mortals. This is usually what is done when you're finished
   developing and ready for release. We will discuss this more in depth later.

![Debug to release picture](debug.png)

2. In VSCode, press `Ctrl+Shift+P` and select `Build Taret`. Once the next textbox comes up, type
   `benchmark` and select that test to build

3. In your project directory (the one that has the README.md file in it) run `bash benchmark`. It
   will output something like this below. The time you should measure yourself against is the "user"
   time. If this is under 15 seconds then you're all good.

```txt
real    1m45.089s
user    1m44.497s
sys     0m0.560s
```

4. In VSCode, down the very bottom of the window, change your Cmake from `[Release]` to `[Debug]`.
   Now that you're done doing a sanity check benchmark, leave debug symbols on so that you can more
   effectively debug your code.

### Compiling with debugging_main.cpp

Two important notes here are:
 * You will have to add another file to the "LINK" part in `CMakeLists.txt` to have this compile with `lexicon.cpp`
 * When loading the lexicon file, use the path `./test/word_ladder/english.txt`. This is because this is the path that the executable wants if you run it as `build/source/debugging_main` etc.

