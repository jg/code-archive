gcd(X, X, X).
gcd(X, Y, D) :-
  X < Y,
  Y1 is Y - X,
  gcd(X, Y1, D).
gcd(X, Y, D) :-
  Y < X,
  gcd(Y, X, D).

mlength([], 0).
mlength([_|Tail], N) :-
  mlength(Tail, N1),
  N is 1 + N1.

mlength2([], 0).
mlength2([_|Tail], 1 + N) :-
  mlength2(Tail, N).

max(X, Y, Max) :- X < Y, Max is Y.
max(X, Y, Max) :- X > Y, Max is X.

maxList([X], X).
maxList([X|Tail], Max) :-
  maxList(Tail, M),
  (M>=X, Max=M; M<X; Max=M).

