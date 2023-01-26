#!/bin/bash                                                                                                      
#file=$(find . -name 'test*[0-9]')                                                                                
                                                                                                                 
#valgrind --leak-check=full $file
valgrind --leak-check=full test1
valgrind --leak-check=full test2
valgrind --leak-check=full test3
