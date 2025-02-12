#!/bin/bash

# Usage:
# ensure_commit_starts_with_gitmoji.sh <commit_message> <gitmoji_list_file>

# Example:
# ensure_commit_starts_with_gitmoji.sh < dev/commit_msg dev/gitmojis.txt

commit_msg="$1"
gitmoji_list_file="$2"

mapfile -t gitmojis < <(sed 's/"//g' "$gitmoji_list_file")

for gitmoji in "${gitmojis[@]}"; do
    if [[ $commit_msg == "$gitmoji "* ]]; then
        exit 0
    fi
done

exit 1
