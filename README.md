# Dartmouth CS50 - Tiny Search Engine
Team: YENKAI HUANG, HONGKE (LUIS) LU, ERIN

Tiny Search Engine (TSE) design is a group project for CS50 at dartmouth inspired by the material in the paper Searching the Web, by Arvind Arasu, Junghoo Cho, Hector Garcia-Molina, Andreas Paepcke, and Sriram Raghavan (Stanford University); ACM Transactions on Internet Technology (TOIT), Volume 1, Issue 1 (August 2001).

The Tiny Search Engine (TSE) shall consist of three subsystems:
 - crawler, which crawls the web from a given seed to a given maxDepth and caches the content of the pages it finds, one page per file, in a given directory.
- indexer, which reads files from the given directory, builds an index that maps from words to pages (URLs), and writes that index to a given file.
- querier, which reads the index from a given file, and a query expressed as a set of words optionally combined by (AND, OR), and outputs a ranked list of pages (URLs) in which the given combination of words appear.
