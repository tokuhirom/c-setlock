setlock: setlock.c
	$(CC) -o setlock setlock.c

clean:
	$(RM) setlock

.PHONY: clean
