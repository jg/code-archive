final(s3).

trans(s1, a, s1).
trans(s1, a, s2).
trans(s1, b, s1).
trans(s2, b, s3).
trans(s3, b, s4).

silent(s2, s4).
silent(s3, s1).

accepts(S, []) :-
  final(S).

accepts(S, [X|Rest]) :-
  trans(S, X, S1),
  accepts(S1, Rest).

accepts(S, String) :-
  silent(S, S1),
  accepts(S1, String).
