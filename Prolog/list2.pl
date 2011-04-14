mmember(X, [X|L]).
mmember(X, [Y|L]) :-
  Y \= X,
  mmember(X, L).

mmember2(X, [X|L]).
mmember2(X, [Y|L]) :-
  mmember2(X, L).
