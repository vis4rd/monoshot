#!/bin/bash

# Usage: ./ensure_all_commits_start_with_gitmoji.sh <commit_messages_file> <gitmoji_list_file>
# Working directory should be the root of the repository.

commit_messages_file=$1
gitmoji_list_file=$2

# Check each commit message
is_ok=true
while IFS= read -r commit; do
    scripts/ensure_commit_starts_with_gitmoji.sh "$commit" "$gitmoji_list_file"
    if [[ $? -eq 1 ]]; then
        is_ok=false
        break
    fi
done < "$commit_messages_file"

if [[ $is_ok == false ]]; then
    echo "Commit message does not start with a Gitmoji: '$commit'"
    exit 1
fi
