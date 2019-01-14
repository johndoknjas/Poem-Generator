# Poem-Generator
This program reads in the "sentences.txt" file and generates rhymes based off pairs of sentences.

I previously uploaded contents from my own word documents into sentences.txt. However, as I am uploading this publicly, I have instead copied the entire contents of Hamlet into sentences.txt.
I downloaded the Hamlet play from an MIT webpage: http://shakespeare.mit.edu/hamlet/full.html

You can replace the contents of sentences.txt with your own choice of sentences. Make sure at least some of them rhyme though! From Hamlet my program is able to generate 553 rhymes.

To come up with rhymes, each sentence in sentences.txt is put into a vector and examined individually. Trying to find a rhyme for each sentence by running through the entire vector is O(n^2), which takes a considerable amount of time. 

Therefore, I've instead grouped similarly-ending sentences together so that the program doesn't have to do as much work to find the rhymes for each sentence. This allows the program to finish very quickly.
