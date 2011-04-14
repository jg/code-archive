
family(
  person(tom, fox, date(7, may, 1950), works(bbc, 15200)),
  person(ann, fox, date(9, may, 1951), unemployed),
  [ person(pat, fox, date(5, may, 1973), unemployed),
    person(jim, fox, date(5, may, 1973), unemployed) ]).


husband(X) :-
  family(X, _, _).

wife(X) :-
  family(_, X, _).

child(X) :-
  family(_, _, Children),
  member(X, Children).

member(X, [X|L]).

member(X, [Y|L]) :-
  member(X, L).

exists(Person) :-
  husband(Person);
  wife(Person);
  child(Person).

dateofbirth(person(_, _, Date, _), Date).

total([], 0).

total([Person|List], Sum) :-
  salary(Person, S),
  total(List, Rest),
  Sum is S + Rest.
