# Stanford106X
Practice problems provided by Stanford from various online sources

I 'took' this course over approximately twoish months 2018 in order to brush up on some of my mechanics, it proved to be a decently challenging endeavour which strengthed my foundations and provided confidence.

It may be worth the while to come back to this after some time and improve on some implementations, or at least the core bits of some implementations. Ideally this would function sort of like a cheat sheet for some foundational things like file parsing in c++ or a pathfinding algorithm implementation.

<ul>
<li> Assignment 1: </li>
  <ul>
    <li> <i>Fauxtoshop</i>: simple image editor, changed the difference portion to show an overlay of the two images being compared and highlighting the different pixels in red, as I thought that would be more useful than simple saying "there are 20,000 pixels different".</li>
  </ul>
<li> Assignment 2: </li>
  <ul>
    <li> <i>NGrams</i>: [File Parsing] A simple text creation program which generates text based off of already existing text, optionally generating complete sentences. </li>
    <li> <i>Transpositional cipher implementation</i> </li>
    <li> <i>WordLadder</i>: [File Parsing, Data Structures] Attempts to find words that "bridge" two other words, via changing one letter at a time. Optionally has the ability to add or remove a letter at each step. </li>
  </ul>
  <li> Assignment 3:</li>
  	<ul>
      <li> <i>20Questions</i>: [File Parsing, Recursion] Reads from a text file of questions/answers and then prompts the user to play a game where the computer attempts to guess the correct answer based on a series of yes/no answers provided from the user. Utilizes recursion to traverse the file to provide questions and answers to the user.</li>
      <li> <i>Fractals</i>: [Recursion] The traditional recursive fractal assignment. Draws varying types of fractals based on user input. Also implemented flood fill. </li>
	  <li> <i>GrammarSolver</i>: [File Parsing, Recursion, Data Structures] Takes in an input file in BNF format representing a language's grammar rules, then recursively generates words from a users input based on those rules. An input of [s] generates a full sentence ([np], [vp]), an input of [adj] generates a list of adjectives. etc.</li>
    </ul>
	<li> Assignment 4: </li>
  	<ul>
		<li><i>Anagrams</i>: <Recursion, Recursive Backtracking> An anagram generator. Takes a phrase and then generates the phrases that can be created using each letter from that phrase only once. Clint Eastwood -> old west action.</li>
		<li><i>Boggle</i>: [Recursion, Recursive Backtracking] Simple boggle implementation. User is prompted to input the size of an NxN board, then is tasked to find valid word combinations on that board of at least size 4. After the user has completed, the computer plays and finds every valid word on the board based on a passed in dictionary. </li>
		<li> <i>Brute Force Decryption</i>: [Recursion, Recursive Backtracking, Data Structures] Expands on the Transposition Cipher assignment and implements the option to decrypt an encrypted string without the use of a key.  </li>
 	</ul>
	<li>Assignment 5: </li>
		<ul>
			<li><i>Patient Queue with Heaps</i>: [Data Structures, Min Binary Heap] Minimum binary heap with patient upgrade functionality. </li>
			<li> <i>Patient Queue with Linked List</i>: [Data Structures, Singly Linked List, Priority Queue] Patient queue as above, except with a singly linked list rather than a minimum binary heap</li>
			<li> <i>Tiles</i>: [Data Structures, Doubly Linked List] Graphical tile app which implements a simple z-ordering scheme in order to display tiles ontop/below each other. </li>
		</ul>
		<li>Assignment 6: </li>
		<ul>
			<li><i>Huffman Encoding</i>: [Data Structures, Binary Tree, Algorithm] Huffman compression algorithm implementation.  </li>
			<li><i>TwentyOne Questions</i>: [Data Structures, Binary Tree ] 20Questions re-implementation, utilizing binary tree for yes/no answers and the ability to correct itself by prompting the user for input after a defeat.</li>
		</ul>
	<li>Assignment 7: </li>
		<ul>
			<li><i>Pathfinder</i>: [Data Structures, Graph, A*, Depth First Search, Dijkstra's] Utilizes a graph to find the fastest path from points on provided maps. Can perform a search using three different algorithms: A*, Depth First Search and Dijkstra's. Also has another route option which calculates the best route in addition to a second-best route that differs from the optimal route by a value specified. </li>
		</ul>
	<li>Assignment 8: </li>
		<ul>
			<li><i>Stanford123</i>: [MVC, Data Structures, Graph] TODO. </li>
</ul>
