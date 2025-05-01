#!/bin/bash

LOGFILE="$1"
TIMESTEP=100    # milliseconds per timeline column
MAXTIME=0       # will be detected from log

if [ -z "$LOGFILE" ] || [ ! -f "$LOGFILE" ]; then
    echo "Usage: $0 philo_log.txt"
    exit 1
fi

# Use awk to parse and build timeline
awk -v step="$TIMESTEP" '
BEGIN {
    state_char["eating"] = "█"
    state_char["sleeping"] = "▓"
    state_char["thinking"] = "░"
    max_time = 0
}
# Match lines like: "640    1 is eating"
{
    if ($0 ~ /^[0-9]+\s+[0-9]+\s+is\s+(eating|sleeping|thinking)/) {
        time = $1
        id = $2
        state = $4
        times[id][time] = state
        if (time > max_time)
            max_time = time
    }
}
END {
    max_step = int(max_time / step) + 5
    for (id in times) {
        printf("Philo %d: ", id)
        curr_state = "thinking"
        time_index = 0
        for (i = 0; i < max_step; i++) {
            t = i * step
            # Check if a state change happened at or before this time
            if (t in times[id])
                curr_state = times[id][t]
            else {
                # search earlier states
                for (pt in times[id]) {
                    if (pt <= t && pt > time_index) {
                        curr_state = times[id][pt]
                        time_index = pt
                    }
                }
            }
            printf("%s", state_char[curr_state])
        }
        print ""
    }
}
' "$LOGFILE"
