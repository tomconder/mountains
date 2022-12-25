#!/bin/sh

cd "$(dirname $0)/../src"

echo "Running clang-format in $(pwd)"

find . -regex '.*\.[chm]p*' -exec clang-format -i {} \;

echo "clang-format complete!"
