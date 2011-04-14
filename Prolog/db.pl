parent(pam, bob).
parent(tom, bob).
parent(tom, liz).
parent(bob, ann).
parent(bob, pat).
parent(pat, jim).

female(pam).
female(liz).
female(pat).
female(ann).
male(tom).
male(bob).
male(jim).

offspring(X, Y) :- parent(X, Y).

mother(X, Y) :- 
  parent(X, Y), female(X).

father(X, Y) :- 
  parent(X, Y), male(X).

grandparent(X, Z) :- 
  parent(X,Y), parent(Y,Z).

sister(X, Y) :-
  parent(Z, X), parent(Z, Y), 
  female(X). 

hasachild(X) :-
  parent(X, Y).

happy(X) :-
  hasachild(X).

predecessor(X, Z) :-
  parent(X, Y),
  predecessor(Y, Z).

predecessor(X, Z) :-
  parent(X, Z).
  
