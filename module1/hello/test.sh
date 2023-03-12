output=$(find . -name "*.b")

if [ -z "$output" ]; then
  echo "empty"
else
  echo "$output"
fi
