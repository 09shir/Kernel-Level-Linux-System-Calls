# Define the compiler
CC = gcc

# Define the compiler flags
CFLAGS = -std=c99 -D _GNU_SOURCE -static

# Define the targets
TARGETS = array_stats_test process_ancestors_test cs300_testapp

# Default target
all: $(TARGETS)

# Rule for array_stats_test
array_stats_test: array_stats_test.c
	$(CC) $(CFLAGS) $< -o $@

# Rule for process_ancestors_test
process_ancestors_test: process_ancestors_test.c
	$(CC) $(CFLAGS) $< -o $@

# Rule for cs300_testapp
cs300_testapp: cs300_testapp.c
	$(CC) $(CFLAGS) $< -o $@

# Clean up build files
clean:
	rm -f $(TARGETS)

# Transfer target
.PHONY: transfer
transfer: $(TARGETS)
	scp -P 10022 cs300_testapp array_stats_test process_ancestors_test ubuntu@localhost:

# Clean up build files
.PHONY: clean
clean:
	rm -f $(TARGETS)
