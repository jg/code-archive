conc([], L, L).

conc([X|L1], L2, [X|L3]) :-
  conc(L1, L2, L3).

concat(A1-Z1, Z1-Z2, A1-Z2).
