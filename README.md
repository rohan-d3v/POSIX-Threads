# POSIX-Threads
The Baboon Crossing (Single Lane Bridge Problem) solved using Threads

Baboon Crossing Problem (semaphore-based process synchronization). There is a deep canyon 
somewhere in the Katavi National Park, Tanzania, and a single rope that spans the canyon from point 
A to point B. Baboons can cross the canyon by swinging hand-over-hand on the rope, but if two 
baboons going in opposite directions meet in the middle, they fight until one or both drop to their 
deaths. Furthermore, the rope is only strong enough to hold 5 baboons. If there are more baboons on 
the rope at the same time, it breaks. 
In our environment, baboons (i.e., baboon processes) have become smarter and now use semaphores 
for peacefully using the crossing, instead of fighting and killing each other. 
Assuming that the baboons can use semaphores, we would like to design a rope crossing 
synchronization scheme with the following properties:
1) Ordered crossing. Baboons arriving point A (B) line up, and cross the rope to B (A) in the 
order they arrive.
2) Rope load. There cannot be more than five baboons on the rope: the rope cannot handle the load.
3) Crossing guarantee. Once a baboon process begins to cross the rope, it is guaranteed to get to 
the other side without running into another baboon going the other way. (Remember, baboons 
are smart now).
4) Streaming guarantee when only one side has baboons waiting to cross. If multiple baboons 
arrive to point A, and, there are no baboons at point B, they can all cross in sequence, subject to 
the rope load and fairness restrictions. 
5) Fairness (no starvation). A continuing stream of baboons crossing in one direction should not 
bar baboons from crossing the other way indefinitely. So, after every 10 baboons complete their 
crossing from A to B (or vice versa), if baboons are waiting to cross from B to A, the crossing 
direction must switch into a B-to-A crossin
