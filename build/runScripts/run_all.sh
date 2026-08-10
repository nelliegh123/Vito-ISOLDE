#!/bin/bash

thickness=(
    0.5
    1.0
    2.0
)
 for t in "${thickness[@]}"; do
        echo "Running thickness: $t mm"

        ./run.sh "$t" 

        if [ $? -ne 0 ]; then
            echo "Run failed for thickness $t detector $detector"
            exit 1
        fi
done
echo "All runs completed."

