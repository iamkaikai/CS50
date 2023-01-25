#!/bin/bash                                                                                                      
file=$(find . -name 'test*[0-9]')                                                                                
                                                                                                                 
valgrind --leak-check=full $file
