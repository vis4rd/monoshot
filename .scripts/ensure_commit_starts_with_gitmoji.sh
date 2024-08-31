#!/bin/bash

# Usage:
# ensure_commit_starts_with_gitmoji.sh <commit_message_file> <gitmojis_file>

commit_msg="$1"
gitmoji_list="$2"

mapfile -t gitmojis < <(sed 's/"//g' "$gitmoji_list")

for gitmoji in "${gitmojis[@]}"; do
    if [[ $commit_msg == "$gitmoji "* ]]; then
        exit 0
    fi
done

exit 1
