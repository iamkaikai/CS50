#!/bin/bash

# Some initial arguments
echo "Argument 1: $1"
echo "Argument 2: $2"
echo "Argument 3: $3"

# Shift the positional parameters to the left
shift

# Now the first argument is shifted out and the second one becomes the first one and so on.
echo "After shift, Argument 1: $1"
echo "After shift, Argument 2: $2"
echo "After shift, Argument 3: $3"
