minsert(X, L, [X | L]). 

mdel(X, [X|L], L).
mdel(X, [Y|L1], [Y|L2]) :-
  mdel(X, L1, L2).


mpermutation([], []).

mpermutation([X | L], P) :-
  mpermutation(L, L1),
  minsert(X, L1, P).

mpermutation2([],[]).
mpermutation2(L, [X|P]) :-
  mdel(X, L, L1),
  mpermutation2(L1, P).

oddlength([_]).
oddlength([_|L]) :-
  evenlength(L).

evenlength([_, _]).
evenlength([_|L]) :-
  oddlength(L).


mreverse2([], X, X).
mreverse2([X|Y], Z, W) :-
  mreverse2(Y, [X|Z], W).

mreverse([], []).
mreverse([X|L1], L2) :-
  mreverse(L1, T), 
  append(T, [X], L2).

