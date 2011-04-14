gt(X, Y) :- X > Y.

bubblesort(List, Sorted) :-
  swap(List, List1), !,
  sort(List1, Sorted).

bubblesort(Sorted, Sorted).

swap([X, Y | Rest], [Y, X | Rest]) :-
  gt(X, Y).

swap([Z | Rest], [Z | Rest1]) :-
  swap(Rest, Rest1).


insertsort([], []).

insertsort([X | Tail], Sorted) :-
  insertsort(Tail, SortedTail),
  insert(X, SortedTail, Sorted).

insert(X, [Y | Sorted], [Y | Sorted1]) :-
  gt(X,Y), !,
  insert(X, Sorted, Sorted1).

insert(X, Sorted, [X | Sorted]).

