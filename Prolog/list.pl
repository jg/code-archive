
mmember(X, [X | _]) !.
mmember(X, [_ | Tail]) :-
  mmember(X, Tail).

mconc([], L, L).
mconc([X|L1], L2, [X|L3]) :-
  mconc(L1, L2, L3).


mmember2(X, List) :-
  mconc(_, [X|_], List).

% del3last([], [], [X,Y,Z]).
del3last(L) :-
  append(_, [_,_,_], L).


% conc( Before, [may | After],
% [jan,feb,mar,apr,may,jun,jul,aug,sep,oct,nov,dec]).

% mdel(X, [X|L], L).
% mdel(X, [Y|L1], [Y|L2]) :-
  % mdel(X, L1, L2).

mdel(X, [X|Tail], Tail).
mdel(X, [Y|Tail], [Y|Tail1]) :- 
  mdel(X, Tail, Tail1).

msublist(S, L) :-
  mconc(_, L2, L),
  mconc(S, _, L2).

mpushfront(X, L, [X|L]).

minsert2(X, L, [X|L]). 

minsert(X, List, BiggerList) :-
  mdel(X, BiggerList, List).

mpermutation([], []).
mpermutation([X|L], P) :-
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


mlast(X, [X]).
mlast(X, [_|List]) :-
  mlast(X, List).

mpalindrome(List) :-
  mreverse(List, List).

mshift([Head1|Tail1], List2) :-
  mappend(Tail1, [Head1], List2).

mequal([_|Tail1], [_|Tail2]) :-
  mequal(Tail1, Tail2).

subset([], _).
subset([X|Tail], Set) :-
  mmember(X, Set), 
  subset(Tail, Set).

