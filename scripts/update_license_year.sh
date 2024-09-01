#!/bin/bash

LICENSE_FILE="LICENSE.md"
CURRENT_YEAR=$(date +'%Y')
LICENSE_YEAR=$(grep -oP '(?<=Copyright © `)\d{4}(?=`)' "$LICENSE_FILE")

if [ "$LICENSE_YEAR" != "$CURRENT_YEAR" ]; then
  # Update the year in the LICENSE.md file
  sed -i "s/Copyright © \`$LICENSE_YEAR\`/Copyright © \`$CURRENT_YEAR\`/" "$LICENSE_FILE"
  echo "License year has been updated from $LICENSE_YEAR to $CURRENT_YEAR"
  exit 0
fi

exit 1
